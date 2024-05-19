#include "burner.h"
#include "retro_common.h"
#include "retro_dirent.h"

#include <time.h>

#ifndef UINT32_MAX
#define UINT32_MAX	(UINT32)4294967295U
#endif

static TCHAR szRomset[MAX_PATH] = _T("");
static struct RomDataInfo RDI = { 0 };
RomDataInfo* pRDI = &RDI;

struct BurnRomInfo* pDataRomDesc = NULL;

TCHAR szRomdataName[MAX_PATH] = _T("");
static TCHAR szFullName[MAX_PATH] = { 0 }, szAltName[MAX_PATH] = { 0 }, szAltDesc[MAX_PATH] = { 0 };

std::vector<romdata_core_option> romdata_core_options;

static INT32 IsUTF8Text(const void* pBuffer, long size)
{
	INT32 nCode = 0;
	unsigned char* start = (unsigned char*)pBuffer;
	unsigned char* end   = (unsigned char*)pBuffer + size;

	while (start < end) {
		if (*start < 0x80) {        // (10000000) ASCII
			if (0 == nCode) nCode = 1;

			start++;
		} else if (*start < 0xc0) { // (11000000) Invalid UTF-8
			return 0;
		} else if (*start < 0xe0) { // (11100000) 2-byte UTF-8
			if (nCode < 2) nCode = 2;
			if (start >= end - 1) break;
			if (0x80 != (start[1] & 0xc0)) return 0;

			start += 2;
		} else if (*start < 0xf0) { // (11110000) 3-byte UTF-8
			if (nCode < 3) nCode = 3;
			if (start >= end - 2) break;
			if ((0x80 != (start[1] & 0xc0)) || (0x80 != (start[2] & 0xc0))) return 0;

			start += 3;
		} else {
			return 0;
		}
	}

	return nCode;
}

static INT32 IsDatUTF8BOM()
{
	FILE* fp = _tfopen(szRomdataName, _T("rb"));
	if (NULL == fp) return -1;

	// get dat size
	fseek(fp, 0, SEEK_END);
	INT32 nDatSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	INT32 nRet = 0;

	char* pszTest = (char*)malloc(nDatSize + 1);

	if (NULL != pszTest) {
		memset(pszTest, 0, nDatSize + 1);
		fread(pszTest, nDatSize, 1, fp);
		nRet = IsUTF8Text(pszTest, nDatSize);
		free(pszTest);
		pszTest = NULL;
	}

	fclose(fp);

	return nRet;
}

#define DELIM_TOKENS_NAME	_T(" \t\r\n,%:|{}")
#define _strqtoken	strqtoken
#define _tcscmp		strcmp
#define _stscanf	sscanf

static INT32 LoadRomdata()
{
	INT32 nType = IsDatUTF8BOM();
	if (-1 == nType) return -1;

	RDI.nDescCount = -1;	// Failed

	const TCHAR* szReadMode = (3 == nType) ? _T("rt, ccs=UTF-8") : _T("rt");

	FILE* fp = _tfopen(szRomdataName, szReadMode);
	if (NULL == fp) return RDI.nDescCount;

	TCHAR szBuf[MAX_PATH] = { 0 }, * pszBuf = NULL, * pszLabel = NULL, * pszInfo = NULL;

	memset(RDI.szExtraRom, 0, sizeof(RDI.szExtraRom));
	memset(szFullName,     0, sizeof(szFullName));

	while (!feof(fp)) {
		if (_fgetts(szBuf, MAX_PATH, fp) != NULL) {
			pszBuf = szBuf;
			pszLabel = _strqtoken(pszBuf, DELIM_TOKENS_NAME);

			if (NULL == pszLabel) continue;
			if ((_T('#') == pszLabel[0]) || ((_T('/') == pszLabel[0]) && (_T('/') == pszLabel[1]))) continue;
			if (0 == _tcsicmp(_T("ZipName"), pszLabel)) {
				pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME);
				if (NULL == pszInfo) break;	// No romset specified
				if (NULL != pDataRomDesc)
					strcpy(RDI.szZipName, TCHARToANSI(pszInfo, NULL, 0));
			}
			if (0 == _tcsicmp(_T("DrvName"), pszLabel)) {
				pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME);
				if (NULL == pszInfo) break;	// No driver specified
				if (NULL != pDataRomDesc)
					strcpy(RDI.szDrvName, TCHARToANSI(pszInfo, NULL, 0));
			}
			if (0 == _tcsicmp(_T("ExtraRom"), pszLabel)) {
				pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME);
				if ((NULL != pszInfo) && (NULL != pDataRomDesc))
					strcpy(RDI.szExtraRom, TCHARToANSI(pszInfo, NULL, 0));
			}
			if (0 == _tcsicmp(_T("FullName"), pszLabel)) {
				pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME);
				if ((NULL != pszInfo) && (NULL != pDataRomDesc))
					strcpy(szFullName, TCHARToANSI(pszInfo, NULL, 0));
			}

			{
				// romname, len, crc, type
				struct BurnRomInfo ri = { 0 };
				ri.nLen = UINT32_MAX;
				ri.nCrc = UINT32_MAX;
				ri.nType = 0;

				pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME);
				if (NULL != pszInfo) {
					_stscanf(pszInfo, _T("%x"), &(ri.nLen));
					if (UINT32_MAX == ri.nLen) continue;

					pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME);
					if (NULL != pszInfo) {
						_stscanf(pszInfo, _T("%x"), &(ri.nCrc));
						if (UINT32_MAX == ri.nCrc) continue;

						while (NULL != (pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME))) {
							INT32 nValue = -1;

							if (0 == _tcscmp(pszInfo, _T("BRF_PRG"))) {
								ri.nType |= BRF_PRG;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("BRF_GRA"))) {
								ri.nType |= BRF_GRA;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("BRF_SND"))) {
								ri.nType |= BRF_SND;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("BRF_ESS"))) {
								ri.nType |= BRF_ESS;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("BRF_BIOS"))) {
								ri.nType |= BRF_BIOS;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("BRF_SELECT"))) {
								ri.nType |= BRF_SELECT;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("BRF_OPT"))) {
								ri.nType |= BRF_OPT;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("BRF_NODUMP"))) {
								ri.nType |= BRF_NODUMP;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_PRG_68K"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_68K_PROGRAM_BYTESWAP")))) {			//  1
								ri.nType |= 1;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_PRG_68K_SIMM"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_68K_PROGRAM_NO_BYTESWAP")))) {		//  2
								ri.nType |= 2;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_PRG_68K_XOR_TABLE"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_Z80_PROGRAM")))) {					//  3
								ri.nType |= 3;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("CPS1_TILES"))) {							//  4
								ri.nType |= 4;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_GFX"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_OKIM6295_SAMPLES")))) {				//  5
								ri.nType |= 5;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_GFX_SIMM"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_QSOUND_SAMPLES")))) {				//  6
								ri.nType |= 6;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_GFX_SPLIT4"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_PIC")))) {							//  7
								ri.nType |= 7;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_GFX_SPLIT8"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_EXTRA_TILES_SF2EBBL_400000")))) {	//  8
								ri.nType |= 8;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_GFX_19XXJ"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_EXTRA_TILES_400000")))) {			//  9
								ri.nType |= 9;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_PRG_Z80"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_EXTRA_TILES_SF2KORYU_400000")))) {	// 10
								ri.nType |= 10;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("CPS1_EXTRA_TILES_SF2B_400000"))) {		// 11
								ri.nType |= 11;
								continue;
							}
							if ((0 == _tcscmp(pszInfo, _T("CPS2_QSND"))) ||
								(0 == _tcscmp(pszInfo, _T("CPS1_EXTRA_TILES_SF2MKOT_400000")))) {	// 12
								ri.nType |= 12;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("CPS2_QSND_SIMM"))) {						// 13
								ri.nType |= 13;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("CPS2_QSND_SIMM_BYTESWAP"))) {				// 14
								ri.nType |= 14;
								continue;
							}
							if (0 == _tcscmp(pszInfo, _T("CPS2_ENCRYPTION_KEY"))) {					// 15
								ri.nType |= 15;
								continue;
							}
							_stscanf(pszInfo, _T("%x"), &nValue);
							if (-1 != nValue) {
								ri.nType |= (UINT32)nValue;
								continue;
							}
						}

						if (ri.nType > 0) {
							RDI.nDescCount++;

							if (NULL != pDataRomDesc) {
								strcpy(pDataRomDesc[RDI.nDescCount].szName, TCHARToANSI(pszLabel, NULL, 0));

								pDataRomDesc[RDI.nDescCount].nLen  = ri.nLen;
								pDataRomDesc[RDI.nDescCount].nCrc  = ri.nCrc;
								pDataRomDesc[RDI.nDescCount].nType = ri.nType;
							}
						}
					}
				}
			}
		}
	}

	fclose(fp);

	return RDI.nDescCount;
}

char* RomdataGetDrvName()
{
	INT32 nType = IsDatUTF8BOM();
	if (-1 == nType) return NULL;

	const TCHAR* szReadMode = (3 == nType) ? _T("rt, ccs=UTF-8") : _T("rt");

	FILE* fp = _tfopen(szRomdataName, szReadMode);
	if (NULL == fp) return NULL;

	memset(szRomset, 0, MAX_PATH * sizeof(TCHAR));

	TCHAR szBuf[MAX_PATH] = { 0 }, * pszBuf = NULL, * pszLabel = NULL, * pszInfo = NULL;

	while (!feof(fp)) {
		if (_fgetts(szBuf, MAX_PATH, fp) != NULL) {
			pszBuf = szBuf;

			pszLabel = _strqtoken(pszBuf, DELIM_TOKENS_NAME);
			if (NULL == pszLabel) continue;
			if ((_T('#') == pszLabel[0]) || ((_T('/') == pszLabel[0]) && (_T('/') == pszLabel[1]))) continue;

			if (0 == _tcsicmp(_T("DrvName"), pszLabel)) {
				pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME);
				if (NULL == pszInfo) break;	// No driver specified

				fclose(fp);

				_tcscpy(szRomset, TCHARToANSI(pszInfo, NULL, 0));

				return szRomset;
			}
		}
	}

	fclose(fp);

	return NULL;
}

INT32 create_variables_from_romdatas()
{
	romdata_core_options.clear();
	bool bRomdataMode = (NULL != pDataRomDesc);
	const char* pszDrvName = bRomdataMode ? RDI.szDrvName : BurnDrvGetTextA(DRV_NAME), * pszExt = ".dat";

	if (NULL == pszDrvName) return -2;

	struct RDIR* entry = retro_opendir_include_hidden(szAppRomdatasPath, true);

	if (!entry || retro_dirent_error(entry))
		return -1;

	INT32 nRet = 0; FILE* fp = NULL;

	while (retro_readdir(entry))
	{
		const char* name = retro_dirent_get_name(entry);

		if (
			retro_dirent_is_dir(entry, NULL) ||
			(0 != strcmp(pszExt, strrchr(name, '.'))) ||
			(0 == strcmp(name, ".")) || (0 == strcmp(name, ".."))
		)
			continue;

		TCHAR szFilePathSearch[MAX_PATH] = { 0 };
		_stprintf(szFilePathSearch, _T("%s%s"), szAppRomdatasPath, name);

		FILE* fp = _tfopen(szFilePathSearch, _T("rb"));
		if (NULL == fp) continue;

		// get dat size
		fseek(fp, 0, SEEK_END);
		INT32 nDatSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		char* pszTest = (char*)malloc(nDatSize + 1);
		if (NULL == pszTest) continue;

		memset(pszTest, 0, nDatSize + 1);
		fread(pszTest, nDatSize, 1, fp);
		INT32 nType = IsUTF8Text(pszTest, nDatSize);
		free(pszTest);
		pszTest = NULL;
		fclose(fp);

		const TCHAR* szReadMode = (3 == nType) ? _T("rt, ccs=UTF-8") : _T("rt");
		if (NULL == (fp = _tfopen(szFilePathSearch, szReadMode))) continue;

		TCHAR szBuf[MAX_PATH] = { 0 }, * pszBuf = NULL, * pszLabel = NULL, * pszInfo = NULL;
		bool bDrvOK = false, bDescOK = false;

		memset(szAltName, 0, MAX_PATH * sizeof(TCHAR));
		memset(szAltDesc, 0, MAX_PATH * sizeof(TCHAR));

		while (!feof(fp))
		{
			if (NULL != _fgetts(szBuf, MAX_PATH, fp));
			{
				pszBuf   = szBuf;
				pszLabel = _strqtoken(pszBuf, DELIM_TOKENS_NAME);
				if ((NULL == pszLabel) || (_T('#') == pszLabel[0]) || ((_T('/') == pszLabel[0]) && (_T('/') == pszLabel[1]))) continue;

				if (0 == _tcsicmp(_T("DrvName"), pszLabel))
				{
					pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME);
					if ((NULL == pszInfo) || (0 != _tcsicmp(pszDrvName, pszInfo))) break;	// The driver specified by romdata does not match the current driver.
					_tcscpy(szAltName, pszInfo); bDrvOK = true;
				}
				if (0 == _tcsicmp(_T("FullName"), pszLabel))
				{
					const TCHAR* pDesc = (NULL == (pszInfo = _strqtoken(NULL, DELIM_TOKENS_NAME))) ? szFilePathSearch : pszInfo;
					_tcscpy(szAltDesc, pDesc); bDescOK = true;
				}
				if (bDrvOK && bDescOK)
				{
					char szKey[100] = { 0 };
					sprintf(szKey, "fbneo-romdata-%s-%d", pszDrvName, nRet);

					romdata_core_options.push_back(romdata_core_option());
					romdata_core_option* romdata_option = &romdata_core_options.back();
					romdata_option->dat_path            = szFilePathSearch;
					romdata_option->option_name         = szKey;
					romdata_option->friendly_name       = szAltDesc;

					nRet++; break;
				}
			}
		}
		fclose(fp);
	}
	retro_closedir(entry);

	return nRet;
}

#undef _stscanf
#undef _tcscmp
#undef _strqtoken
#undef DELIM_TOKENS_NAME

INT32 reset_romdatas_from_variables()
{
	struct retro_variable var = { 0 };

	for (INT32 romdata_idx = 0; romdata_idx < romdata_core_options.size(); romdata_idx++)
	{
		romdata_core_option* romdata_option = &romdata_core_options[romdata_idx];

		var.key = romdata_option->option_name.c_str();
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) == false || !var.value)
			continue;

		var.value = "disabled";

		if (environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &var) == false)
			return -1;
	}

	return 0;
}

INT32 apply_romdatas_from_variables()
{
	INT32 nIndex = -1, nCount = 0, nList[100], nRandNum = -1;
	struct retro_variable var = { 0 };

	memset(nList, -1, 100);

	for (INT32 romdata_idx = 0; romdata_idx < romdata_core_options.size(); romdata_idx++)
	{
		romdata_core_option* romdata_option = &romdata_core_options[romdata_idx];

		var.key = romdata_option->option_name.c_str();
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) == false || !var.value)
			continue;

		if (0 == strcmp(var.value, "enabled"))
		{
			nList[nCount] = romdata_idx;
			nCount++;
		}
	}

	if (nCount > 1)			// multiple selections.
	{
		bool bLoop = true;
		srand((UINT32)time(NULL));

		do
		{
			nRandNum = rand() % romdata_core_options.size();

			for (INT32 i = 0; i < nCount; i++)
			{
				if (nRandNum == nList[i])
				{
					nRandNum = nList[i];
					bLoop = false;
					break;
				}
			}
		} while (bLoop);
	}

	for (INT32 romdata_idx = 0; romdata_idx < romdata_core_options.size(); romdata_idx++)
	{
		if ((nCount > 1) && (romdata_idx < nRandNum)) continue;

		romdata_core_option* romdata_option = &romdata_core_options[romdata_idx];

		var.key = romdata_option->option_name.c_str();
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) == false || !var.value)
			continue;

		if (0 == strcmp(var.value, "enabled"))
		{
			nIndex = romdata_idx;
			RomDataExit();	// Reset the parasitized drvname.
			_tcscpy(szRomdataName, romdata_option->dat_path.c_str());
			break;
		}
	}

	for (INT32 romdata_idx = 0; romdata_idx < romdata_core_options.size(); romdata_idx++)
	{
		if (0 == strcmp(var.value, "enabled"))
		{
			var.value = "disabled";

			if (environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &var) == false)
				return -1;
		}
	}

	return nIndex;
}

void RomDataInit()
{
	INT32 nLen = LoadRomdata();

	if ((-1 != nLen) && (NULL == pDataRomDesc)) {
		pDataRomDesc = (struct BurnRomInfo*)malloc((nLen + 1) * sizeof(BurnRomInfo));
		if (NULL != pDataRomDesc) {
			LoadRomdata();

			RDI.nDriverId = BurnDrvGetIndex(RDI.szDrvName);

			if (-1 != RDI.nDriverId) {
				BurnDrvSetZipName(RDI.szZipName, RDI.nDriverId);
			}
		}
	}
}

void RomDataSetFullName()
{
#if 0
	// At this point, the driver's default ZipName has been replaced with the ZipName from the rom data
	RDI.nDriverId = BurnDrvGetIndex(RDI.szZipName);

	if (-1 != RDI.nDriverId) {
		wchar_t* szOldName = BurnDrvGetFullNameW(RDI.nDriverId);
		memset(RDI.szOldName, '\0', sizeof(RDI.szOldName));

		if (0 != wcscmp(szOldName, RDI.szOldName)) {
			wcscpy(RDI.szOldName, szOldName);
		}

		BurnDrvSetFullNameW(RDI.szFullName, RDI.nDriverId);
	}
#endif
}

void RomDataExit()
{
	if (NULL != pDataRomDesc) {
		free(pDataRomDesc);
		pDataRomDesc = NULL;

		if (-1 != RDI.nDriverId) {
			BurnDrvSetZipName(RDI.szDrvName, RDI.nDriverId);
#if 0
			if (0 != wcscmp(BurnDrvGetFullNameW(RDI.nDriverId), RDI.szOldName)) {
				BurnDrvSetFullNameW(RDI.szOldName, RDI.nDriverId);
			}
#endif
			RDI.nDriverId = -1;
		}

		memset(&RDI, 0, sizeof(RomDataInfo));
		memset(szRomdataName, 0, sizeof(szRomdataName));

		RDI.nDescCount = -1;
	}
}
