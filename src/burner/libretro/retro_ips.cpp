//#include <wdef.h>
#include "burner.h"
#include "retro_common.h"

#define NUM_LANGUAGES		12
#define MAX_ACTIVE_PATCHES	1024

#ifndef MAX_PATH
#define MAX_PATH			260
#endif

#if W_NOT_PRINTF
#define WERROR_A(...)
#else
#define WERROR_A printf
#endif

// Game patching

#define UTF8_SIGNATURE	"\xef\xbb\xbf"
#define IPS_SIGNATURE	"PATCH"
#define IPS_TAG_EOF		"EOF"
#define IPS_EXT			".ips"

#define BYTE3_TO_UINT(bp) \
     (((UINT32)(bp)[0] << 16) & 0x00FF0000) | \
     (((UINT32)(bp)[1] <<  8) & 0x0000FF00) | \
     (( UINT32)(bp)[2]        & 0x000000FF)

#define BYTE2_TO_UINT(bp) \
    (((UINT32)(bp)[0] << 8) & 0xFF00) | \
    (( UINT32)(bp)[1]       & 0x00FF)

bool bDoIpsPatch = false;
UINT32 nIpsDrvDefine = 0, nIpsMemExpLen[SND2_ROM + 1] = { 0 };
TCHAR szAppIpsPath[MAX_PATH];

static INT32 nRomOffset = 0;

static void PatchFile(const char* ips_path, UINT8* base, bool readonly)
{
	char buf[6];
	FILE* f = NULL;
	INT32 Offset = 0, Size = 0;
	UINT8* mem8 = NULL;

	if (NULL == (f = fopen(ips_path, "rb")))
	{
		WERROR_A("IPS - Can't open file %s!  Aborting.\n", ips_path);
		return;
	}

	WERROR_A("PatchFile=%s",ips_path);

	memset(buf, 0, sizeof(buf));
	fread(buf, 1, 5, f);

	if (strcmp(buf, IPS_SIGNATURE))
	{
		WERROR_A("IPS - Bad IPS-Signature in: %s.\n", ips_path);

		if (f)
			fclose(f);

		return;
	}
	else
	{
		WERROR_A("IPS - Patching with: %s. (%s)\n", ips_path, (readonly) ? "Read-Only" : "Write");

		UINT8 ch = 0;
		INT32 bRLE = 0;

		while (!feof(f)) {
			// read patch address offset
			fread(buf, 1, 3, f);
			buf[3] = 0;

			if (0 == strcmp(buf, IPS_TAG_EOF))
				break;

			Offset = BYTE3_TO_UINT(buf);

			// read patch length
			fread(buf, 1, 2, f);
			Size = BYTE2_TO_UINT(buf);

			bRLE = (0 == Size);
			if (bRLE) {
				fread(buf, 1, 2, f);
				Size = BYTE2_TO_UINT(buf);
				ch = fgetc(f);
			}

			while (Size--)
			{
				if (!readonly)
					mem8 = base + Offset + nRomOffset;
				
				Offset++;

                if (readonly)
				{
                    if (!bRLE)
						fgetc(f);
                }
				else
                    *mem8 = bRLE ? ch : fgetc(f);
			}
		}
	}

	// Avoid memory out-of-bounds due to ips offset greater than rom length.
	if (readonly && (0 == nIpsMemExpLen[EXP_FLAG]))	// Unspecified length.
		nIpsMemExpLen[LOAD_ROM] = Offset;

	fclose(f);
	WERROR_A("patch ok.");
}

static char* stristr_int(const char* str1, const char* str2)
{
    const char* p1 = str1;
    const char* p2 = str2;
    const char* r = (!*p2) ? str1 : NULL;

	while (*p1 && *p2)
	{
        if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2)) {
            if (!r)
				r = p1;

            p2++;
        }
		else
		{
			p2 = str2;

            if (r)
				p1 = r + 1;

            if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
			{
                r = p1; p2++;
            }
			else
                r = NULL;
        }

        p1++;
    }

	return (*p2) ? NULL : (char*)r;
}

static UINT32 hexto32(const char *s)
{
	UINT32 val = 0;
	char c;

	while ((c = *s++))
	{
		UINT8 v = ((c & 0xf) + (c >> 6)) | ((c >> 3) & 0x08);
		val = (val << 4) | (UINT32)v;
    }

	return val;
}

// strqtoken() - functionally identicle to strtok() w/ special treatment for
// quoted strings.  -dink april 2023
char *strqtoken(char *s, const char *delims)
{
	static char* prev_str = NULL;
	char* token = NULL;

	if (!s)
		s = prev_str;

	s += strspn(s, delims);

	if (s[0] == '\0')
	{
		prev_str = s;
		return NULL;
	}

	if (s[0] == '\"') { // time to grab quoted string!
		token = ++s;

		if ((s = strpbrk(token, "\"")))
			*(s++) = '\0';
	}
	else
		token = s;

	if ((s = strpbrk(s, delims)))
	{
		*(s++) = '\0';
		prev_str = s;
	}
	else
		// we're at the end of the road
		prev_str = (char*)memchr((void *)token, '\0', MAX_PATH);

	return token;
}

static void DoPatchGame(const char* patch_name, char* game_name, UINT32 crc, UINT8* base, bool readonly)
{
	char s[MAX_PATH];
	char* p = NULL, * rom_name = NULL, * ips_name = NULL, * ips_offs = NULL, * ips_crc = NULL;
	UINT32 nIps_crc = 0;
	FILE* fp = NULL;

	//bprintf(0, _T("DoPatchGame [%S][%S]\n"), patch_name, game_name);

	if (NULL != (fp = fopen(patch_name, "rb")))
	{
		bool bTarget = false;

        while (!feof(fp))
		{
			if (NULL != fgets(s, sizeof(s), fp))
			{
				p = s;

				// skip UTF-8 sig
				if (strncmp(p, UTF8_SIGNATURE, strlen(UTF8_SIGNATURE)) == 0)
					p += strlen(UTF8_SIGNATURE);

				if (p[0] == '[')	// reached info-section of .dat file, time to leave.
					break;

                // Can support linetypes: (space or tab)
                // "rom name.bin" "patch file.ips" CRC(abcd1234)
                // romname.bin patchfile CRC(abcd1234)
				#define DELIM_TOKENS_NAME	" \t\r\n"
				#define DELIM_TOKENS		" \t\r\n()"

				rom_name = strqtoken(p, DELIM_TOKENS_NAME);

				if (!rom_name)
					continue;
				if (*rom_name == '#')
					continue;

				ips_name = strqtoken(NULL, DELIM_TOKENS_NAME);
				if (!ips_name) {
					continue;
				}

				nIps_crc = nRomOffset = 0; // Reset to 0

				if (NULL != (ips_offs = strqtoken(NULL, DELIM_TOKENS)))
				{	// Parameters of the offset increment
					if (     0 == strcmp(ips_offs, "IPS_OFFSET_016")) nRomOffset = 0x1000000;
					else if (0 == strcmp(ips_offs, "IPS_OFFSET_032")) nRomOffset = 0x2000000;
					else if (0 == strcmp(ips_offs, "IPS_OFFSET_048")) nRomOffset = 0x3000000;
					else if (0 == strcmp(ips_offs, "IPS_OFFSET_064")) nRomOffset = 0x4000000;
					else if (0 == strcmp(ips_offs, "IPS_OFFSET_080")) nRomOffset = 0x5000000;
					else if (0 == strcmp(ips_offs, "IPS_OFFSET_096")) nRomOffset = 0x6000000;
					else if (0 == strcmp(ips_offs, "IPS_OFFSET_112")) nRomOffset = 0x7000000;
					else if (0 == strcmp(ips_offs, "IPS_OFFSET_128")) nRomOffset = 0x8000000;
					else if (0 == strcmp(ips_offs, "IPS_OFFSET_144")) nRomOffset = 0x9000000;

					if (nRomOffset != 0)	// better get next token (crc)
						ips_offs = strqtoken(NULL, DELIM_TOKENS);
				}

				if (ips_offs != NULL && stristr_int(ips_offs, "crc"))
				{
					ips_crc = strqtoken(NULL, DELIM_TOKENS);

					if (ips_crc)
						nIps_crc = hexto32(ips_crc);

                }

                char *has_ext = stristr_int(ips_name, ".ips");

				if (_stricmp(rom_name, game_name))	// name don't match?
					if (nIps_crc != crc)			// crc don't match?
						continue;					// not our file. next!

				bTarget = true;

				if (!readonly) {
					WERROR_A("ips name:[%s]\n", ips_name);
					WERROR_A("rom name:[%s]\n", rom_name);
					WERROR_A("rom crc :[%x]\n", nIps_crc);
				}

				char ips_path[MAX_PATH * 2];
				char ips_dir[MAX_PATH];
#if 0
				TCHARToANSI(szAppIpsPath, ips_dir, sizeof(ips_dir));
#else

				char szIps_path[MAX_PATH] = { 0 };
				strcpy(szIps_path, szAppIpsPath);

				for (INT32 i = 0; i <2; i++)
				{
					char* pszTmp = strrchr(szIps_path, '\\');
					if (NULL == pszTmp)
						pszTmp = strrchr(szIps_path, '/');
					if (NULL != pszTmp)
						pszTmp[0] = '\0';
				}
				strncpy(ips_dir, szIps_path, sizeof(ips_dir));
#endif
#if defined(_WIN32)
				sprintf(ips_path, "%s\\%s\\%s%s", ips_dir, ((0 != strchr(ips_name, '\\')) || (0 != strchr(ips_name, '/'))) ? BurnDrvGetTextA(DRV_NAME) : BurnDrvGetTextA(DRV_NAME), ips_name, (has_ext) ? "" : IPS_EXT);
				FILE* f = fopen("debug.txt", "wt");
				fwrite(ips_path, MAX_PATH, 1, f);
				fclose(f);
#else
				if (strchr(ips_name, '\\')) {
					// ips in parent's folder
                    sprintf(ips_path, "%s/%s%s", ips_dir, ips_name, (has_ext) ? "" : IPS_EXT);
				} else {
					sprintf(ips_path, "%s%s/%s%s", ips_dir, BurnDrvGetTextA(DRV_NAME), ips_name, (has_ext) ? "" : IPS_EXT);
				}
#endif
				WERROR_A("ips_path=%s game_name=%s\n", ips_path, game_name);
				PatchFile(ips_path, base, readonly);
			}
		}

		fclose(fp);

		if (!bTarget && (0 == nIpsMemExpLen[EXP_FLAG]))
			nIpsMemExpLen[LOAD_ROM] = 0;	// Must be reset to 0!
	}
}

static UINT32 GetIpsDefineExpValue(char* szTmp)
{
	if (NULL == (szTmp = strtok(NULL, " \t\r\n")))
		return 0U;

	INT32 nRet = 0;

	if      (0 == strcmp(szTmp, "EXP_VALUE_001")) nRet = 0x0010000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_002")) nRet = 0x0020000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_003")) nRet = 0x0030000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_004")) nRet = 0x0040000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_005")) nRet = 0x0050000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_006")) nRet = 0x0060000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_007")) nRet = 0x0070000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_008")) nRet = 0x0080000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_010")) nRet = 0x0100000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_020")) nRet = 0x0200000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_030")) nRet = 0x0300000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_040")) nRet = 0x0400000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_050")) nRet = 0x0500000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_060")) nRet = 0x0600000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_070")) nRet = 0x0700000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_080")) nRet = 0x0800000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_100")) nRet = 0x1000000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_200")) nRet = 0x2000000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_300")) nRet = 0x3000000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_400")) nRet = 0x4000000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_500")) nRet = 0x5000000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_600")) nRet = 0x6000000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_700")) nRet = 0x7000000;
	else if (0 == strcmp(szTmp, "EXP_VALUE_800")) nRet = 0x8000000;
	else if (EOF != (sscanf(szTmp, "%x", &nRet))) return nRet;

	return nRet;
}

// Run once to get the definition & definition values of the DAT files.
// Suppress CPU usage caused by multiple runs.
// Two entry points: cmdline Launch & SelOkay.
void GetIpsDrvDefine()
{
	if (!bDoIpsPatch)
		return;

	nIpsDrvDefine = 0;
	memset(nIpsMemExpLen, 0, sizeof(nIpsMemExpLen));

	char ips_data[MAX_PATH];

	memset(ips_data, 0, MAX_PATH);
	strcpy(ips_data, szAppIpsPath);

	char str[MAX_PATH] = { 0 }, * ptr = NULL, * tmp = NULL;
	FILE* fp = NULL;

	if (NULL != (fp = fopen(ips_data, "rb"))) {
		while (!feof(fp)) {
			if (NULL != fgets(str, sizeof(str), fp)) {
				ptr = str;

				// skip UTF-8 sig
				if (0 == strncmp(ptr, UTF8_SIGNATURE, strlen(UTF8_SIGNATURE)))
					ptr += strlen(UTF8_SIGNATURE);

				if (NULL == (tmp = strtok(ptr, " \t\r\n")))
					continue;
				if (0 != strcmp(tmp, "#define"))
					break;
				if (NULL == (tmp = strtok(NULL, " \t\r\n")))
					break;

				UINT32 nNewValue = 0;

				if (0 == strcmp(tmp, "IPS_NOT_PROTECT")) {
					nIpsDrvDefine |= IPS_NOT_PROTECT;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_PGM_SPRHACK")) {
					nIpsDrvDefine |= IPS_PGM_SPRHACK;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_PGM_MAPHACK")) {
					nIpsDrvDefine |= IPS_PGM_MAPHACK;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_PGM_SNDOFFS")) {
					nIpsDrvDefine |= IPS_PGM_SNDOFFS;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_LOAD_EXPAND")) {
					nIpsDrvDefine |= IPS_LOAD_EXPAND;
					nIpsMemExpLen[EXP_FLAG] = 1;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[LOAD_ROM])
						nIpsMemExpLen[LOAD_ROM] = nNewValue;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_EXTROM_INCL")) {
					nIpsDrvDefine |= IPS_EXTROM_INCL;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[EXTR_ROM])
						nIpsMemExpLen[EXTR_ROM] = nNewValue;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_PRG1_EXPAND")) {
					nIpsDrvDefine |= IPS_PRG1_EXPAND;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[PRG1_ROM])
						nIpsMemExpLen[PRG1_ROM] = nNewValue;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_PRG2_EXPAND")) {
					nIpsDrvDefine |= IPS_PRG2_EXPAND;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[PRG2_ROM])
						nIpsMemExpLen[PRG2_ROM] = nNewValue;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_GRA1_EXPAND")) {
					nIpsDrvDefine |= IPS_GRA1_EXPAND;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[GRA1_ROM])
						nIpsMemExpLen[GRA1_ROM] = nNewValue;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_GRA2_EXPAND")) {
					nIpsDrvDefine |= IPS_GRA2_EXPAND;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[GRA2_ROM])
						nIpsMemExpLen[GRA2_ROM] = nNewValue;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_GRA3_EXPAND")) {
					nIpsDrvDefine |= IPS_GRA3_EXPAND;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[GRA3_ROM])
						nIpsMemExpLen[GRA3_ROM] = nNewValue;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_ACPU_EXPAND")) {
					nIpsDrvDefine |= IPS_ACPU_EXPAND;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[ACPU_ROM])
						nIpsMemExpLen[ACPU_ROM] = nNewValue;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_SND1_EXPAND")) {
					nIpsDrvDefine |= IPS_SND1_EXPAND;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[SND1_ROM])
						nIpsMemExpLen[SND1_ROM] = nNewValue;
					continue;
				}
				if (0 == strcmp(tmp, "IPS_SND2_EXPAND")) {
					nIpsDrvDefine |= IPS_SND2_EXPAND;
					if ((nNewValue = GetIpsDefineExpValue(tmp)) > nIpsMemExpLen[SND2_ROM])
						nIpsMemExpLen[SND2_ROM] = nNewValue;
					continue;
				}
			}
		}

		fclose(fp);
	}
}

void IpsApplyPatches(UINT8* base, char* rom_name, UINT32 crc, bool readonly)
{
	if (!bDoIpsPatch)
		return;

	char ips_data[MAX_PATH];
	memset(ips_data, 0, MAX_PATH);
	strcpy(ips_data, szAppIpsPath);

	WERROR_A("ips_data=%s\n", ips_data);
	DoPatchGame(ips_data, rom_name, crc, base, readonly);
}

void IpsPatchExit()
{
	memset(nIpsMemExpLen, 0, sizeof(nIpsMemExpLen));

	nIpsDrvDefine	= 0;
	bDoIpsPatch		= false;
}
