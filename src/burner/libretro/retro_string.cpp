#include "retro_common.h"

#define MAX_LANGUAGES	3

const char* pSelLangStr[NUM_STRING] = { NULL };

static const char* multi_language_strings[MAX_LANGUAGES][NUM_STRING] = {
	{
		"Use 32-bits color depth when available",
		"Change pixel format, some games require this to render properly, it could impact performances on some platforms",
		"Vertical mode",
		"Rotate display for vertical screens",
		"alternate",
		"TATE alternate",
		"Force 60Hz",
		"Ignore game's original refresh rate and try to run it at 60hz instead. It will cause incorrect game speed and frame pacing. It will try to use your monitor's correct refresh rate instead of 60hz if this refresh rate is between 59hz and 61hz.",
		"Fixed Frameskip",
		"When 'Frameskip' is set to 'Fixed', or if the frontend doesn't support the alternative 'Frameskip' mode, skip rendering at a fixed rate of X frames out of X+1",
		"No skipping",
		"Skip rendering of 1 frames out of 2",
		"Skip rendering of 2 frames out of 3",
		"Skip rendering of 3 frames out of 4",
		"Skip rendering of 4 frames out of 5",
		"Skip rendering of 5 frames out of 6",
		"Frameskip",
		"Skip frames to avoid audio buffer under-run (crackling). Improves performance at the expense of visual smoothness. 'Auto' skips frames when advised by the frontend. 'Manual' uses the 'Frameskip Threshold (%)' setting. 'Fixed' uses the 'Fixed Frameskip' setting.",
		"Fixed",
		"Auto",
		"Manual",
		"Frameskip Threshold(%)",
		"When 'Frameskip' is set to 'Manual', specifies the audio buffer occupancy threshold (percentage) below which frames will be skipped. Higher values reduce the risk of crackling by causing frames to be dropped more frequently.",
		"CPU clock",
		"Change emulated cpu frequency for various systems, by increasing you can fix native slowdowns in some games, by decreasing you can help performance on low-end devices",
		"Diagnostic Input",
		"Configure button combination to enter cabinet service menu",
		"None",
		"Hold Start",
		"Hold Start + A + B",
		"Hold Start + L + R",
		"Hold Select",
		"Hold Select + A + B",
		"Hold Select + L + R",
		"Hiscores",
		"Enable high scores support, you also need the file hiscore.dat in your system/fbneo/ folder",
		"Allow patched romsets",
		"Allow romsets from your system/fbneo/patched/ folder to override your romsets, crcs will be ignored but sizes and names must still match, you need to close content for this setting to take effect",
		"Samplerate",
		"Configure samplerate, it could impact performances, closing & starting game again is required",
		"Sample Interpolation",
		"Configure sample interpolation, it could impact performances",
		"2-point 1st order",
		"4-point 3rd order",
		"FM Interpolation",
		"Configure FM interpolation, it could impact performances",
		"LowPass Filter",
		"Enable LowPass Filter",
		"Analog Speed",
		"Mitigate analog controls speed, some games might require low values to be playable",
		"Crosshair emulation",
		"Change emulated crosshair behavior",
		"hide with lightgun device",
		"always hide",
		"always show",
		"Enable cyclone",
		"Use at your own risk, it could improve performance on some emulated systems for low-end devices, but there are known side effects : savestates won't be compatible with normal interpreter, and some systems won't work",
		"Neo-Geo mode",
		"Load appropriate bios depending on your choice, under the condition such a bios is compatible with the running game",
		"FBNeo Error",
		"Use bios set in BIOS dipswitch",
		"Memory card mode",
		"Change the behavior for the memory card",
		"shared",
		"per-game",
		"Debug Dip 1_1",
		"Enable Debug Dip 1_1",
		"Debug Dip 1_2",
		"Enable Debug Dip 1_2",
		"Debug Dip 1_3",
		"Enable Debug Dip 1_3",
		"Debug Dip 1_4",
		"Enable Debug Dip 1_4",
		"Debug Dip 1_5",
		"Enable Debug Dip 1_5",
		"Debug Dip 1_6",
		"Enable Debug Dip 1_6",
		"Debug Dip 1_7",
		"Enable Debug Dip 1_7",
		"Debug Dip 1_8",
		"Enable Debug Dip 1_8",
		"Debug Dip 2_1",
		"Enable Debug Dip 2_1",
		"Debug Dip 2_2",
		"Enable Debug Dip 2_2",
		"Debug Dip 2_3",
		"Enable Debug Dip 2_3",
		"Debug Dip 2_4",
		"Enable Debug Dip 2_4",
		"Debug Dip 2_5",
		"Enable Debug Dip 2_5",
		"Debug Dip 2_6",
		"Enable Debug Dip 2_6",
		"Debug Dip 2_7",
		"Enable Debug Dip 2_7",
		"Debug Dip 2_8",
		"Enable Debug Dip 2_8",
		"For debugging",
		"Layer 1",
		"Layer 2",
		"Layer 3",
		"Layer 4",
		"Sprite 1",
		"Sprite 2",
		"Sprite 3",
		"Sprite 4",
		"Sprite 5",
		"Sprite 6",
		"Sprite 7",
		"Sprite 8",
		"THIS NEOGEO GAME USES A DIFFERENT DEFAULT BIOS, CHANGE IT AT YOUR OWN RISK",
		"Dipswitch setting, setting is specific to the running romset. Some dipswitches require a restart to work properly.",
		"Specific to the running romset and your cheat database",
		"Neo-Geo Settings",
		"Configure Neo-Geo Settings",
		"Frameskip Settings",
		"Configure Frameskip Settings",
		"Audio Settings",
		"Configure Audio Settings",
		"DIP Switches",
		"Configure DIP Switches",
		"Cheat",
		"Enable Cheats",
		"Enable selected IPS Patches after Restart",
		"IPS Patch",
		"Specific to the running romset and your IPS Patch database",
		"RomData is a single selection, or random when multiple selections are made",
		"Enable a selected RomData after Restart",
		"Debug",
		"Configure Debug Features",
		"This romset is known but marked as not working\nOne of its clones might work so maybe give it a try",
		"Bioses aren't meant to be launched this way",
		"You need a disc image to launch neogeo CD\n",
		"This game is known but one of your romsets is missing files for THIS VERSION of FBNeo.\n",
		"Verify the following romsets : %s%s%s%s%s\n",
		"Note that 7z archive support is disabled for your platform.\n\n",
		"THIS IS NOT A BUG ! If you don't understand what this message means,\nthen you need to read the arcade and FBNeo documentations at https://docs.libretro.com/.\n",
		"Failed initializing driver\nThis is unexpected, you should probably report it.",
		"Romset is unknown.\n",
		"Note that your device's limitations prevent you from running a full FBNeo build.\nSo the support for this romset might have been removed.\n\n",
		"%s\nROM with name %s and CRC 0x%08x is missing"
	},
	{	// Simplified Chinese
		"\u5c3d\u53ef\u80fd\u4f7f\u7528 32 \u4f4d\u8272\u6df1",
		"\u67d0\u4e9b\u6e38\u620f\u9700\u8981\u66f4\u6539\u50cf\u7d20\u683c\u5f0f\u624d\u80fd\u6b63\u5e38\u6e32\u67d3, \u8fd9\u53ef\u80fd\u4f1a\u5f71\u54cd\u67d0\u4e9b\u5e73\u53f0\u7684\u6027\u80fd",
		"\u7eb5\u5411\u6a21\u5f0f",
		"\u65cb\u8f6c\u663e\u793a\u5c4f\u4ee5\u9002\u5e94\u7eb5\u5411\u5c4f\u5e55",
		"\u66ff\u6362",
		"TATE \u66ff\u6362",
		"\u5f3a\u5236 60Hz",
		"\u5ffd\u7565\u6e38\u620f\u7684\u539f\u59cb\u5237\u65b0\u7387, \u5c1d\u8bd5\u4ee5 60hz \u8fd0\u884c, \u8fd9\u4f1a\u5bfc\u81f4\u6e38\u620f\u901f\u5ea6\u548c\u5e27\u6570\u4e0d\u6b63\u786e, \u5982\u679c\u663e\u793a\u5668\u7684\u5237\u65b0\u7387\u5728 59hz \u548c 61hz \u4e4b\u95f4, \u5b83\u4f1a\u5c1d\u8bd5\u4f7f\u7528\u6b63\u786e\u7684\u5237\u65b0\u7387, \u800c\u4e0d\u662f 60hz",
		"\u56fa\u5b9a\u8df3\u5e27",
		"\u5f53\u8df3\u5e27\u8bbe\u7f6e\u4e3a\u56fa\u5b9a\u65f6, \u6216\u8005\u524d\u7aef\u4e0d\u652f\u6301\u66ff\u4ee3\u7684\u8df3\u5e27\u6a21\u5f0f, \u5219\u4ee5 X+1 \u5e27\u4e2d X \u5e27\u7684\u56fa\u5b9a\u901f\u7387\u8df3\u8fc7\u6e32\u67d3",
		"\u7981\u7528\u8df3\u5e27",
		"\u8df3\u8fc7 2 \u5e27\u4e2d 1 \u5e27\u7684\u6e32\u67d3",
		"\u8df3\u8fc7 3 \u5e27\u4e2d 2 \u5e27\u7684\u6e32\u67d3",
		"\u8df3\u8fc7 4 \u5e27\u4e2d 3 \u5e27\u7684\u6e32\u67d3",
		"\u8df3\u8fc7 5 \u5e27\u4e2d 4 \u5e27\u7684\u6e32\u67d3",
		"\u8df3\u8fc7 6 \u5e27\u4e2d 5 \u5e27\u7684\u6e32\u67d3",
		"\u8df3\u5e27",
		"\u907f\u514d\u56e0\u97f3\u9891\u7f13\u51b2\u4e0d\u8db3 (\u7206\u88c2\u58f0), \u4ee5\u8df3\u5e27\u727a\u7272\u89c6\u89c9\u5e73\u6ed1\u5ea6\u4e3a\u4ee3\u4ef7\u63d0\u9ad8\u6027\u80fd, \u524d\u7aef\u5efa\u8bae [\u81ea\u52a8],  [\u624b\u52a8] \u53ef\u8c03\u6574\u8df3\u5e27\u9608\u503c (%) \u8bbe\u7f6e, \u4ee5\u53ca [\u56fa\u5b9a] \u7684\u53ef\u9009\u8bbe\u7f6e",
		"\u56fa\u5b9a",
		"\u81ea\u52a8",
		"\u624b\u52a8",
		"\u8df3\u5e27\u9608\u503c (%)",
		"\u5f53\u8df3\u5e27\u8bbe\u7f6e\u4e3a [\u624b\u52a8] \u65f6, \u97f3\u9891\u7f13\u51b2\u9608\u503c\u4f4e\u4e8e\u8be5\u9608\u503c (\u767e\u5206\u6bd4) \u7684\u5e27\u5c06\u88ab\u8df3\u8fc7, \u8f83\u9ad8\u7684\u503c\u4f1a\u9891\u7e41\u5730\u8df3\u5e27, \u51cf\u5c11\u58f0\u97f3\u7206\u88c2\u7684\u53d1\u751f",
		"CPU \u8d85\u9891",
		"\u589e\u52a0\u6216\u964d\u4f4e\u5404\u7cfb\u7edf\u6a21\u62df CPU \u7684\u9891\u7387, \u53ef\u4ee5\u4fee\u590d\u67d0\u4e9b\u6e38\u620f\u901f\u5ea6\u4e0b\u964d\u95ee\u9898, \u540c\u65f6\u53ef\u4ee5\u5e2e\u52a9\u63d0\u9ad8\u4f4e\u7aef\u8bbe\u5907\u7684\u6027\u80fd",
		"\u8bca\u65ad\u8f93\u5165",
		"\u8bbe\u7f6e\u8fdb\u5165\u670d\u52a1\u83dc\u5355\u7684\u7ec4\u5408\u952e",
		"\u4e0d\u8bbe\u7f6e",
		"\u6309\u4f4f Start",
		"\u6309\u4f4f Start + A + B",
		"\u6309\u4f4f Start + L + R",
		"\u6309\u4f4f Select",
		"\u6309\u4f4f Select + A + B",
		"\u6309\u4f4f Select + L + R",
		"\u9ad8\u5206",
		"\u542f\u7528\u9ad8\u5206\u652f\u6301, \u60a8\u8fd8\u9700\u8981 system/fbneo/ \u76ee\u5f55\u4e2d\u7684 hiscore.dat \u6587\u4ef6",
		"\u5141\u8bb8\u4fee\u8865\u96c6\u7ec4",
		"\u5141\u8bb8 system/fbneo/patched/ \u76ee\u5f55\u4e2d\u7684\u96c6\u7ec4\u8986\u76d6\u60a8\u7684\u96c6\u7ec4, crc \u5c06\u88ab\u5ffd\u7565, \u4f46\u5927\u5c0f\u548c\u540d\u79f0\u4ecd\u5fc5\u987b\u5339\u914d, \u60a8\u9700\u8981\u5173\u95ed\u5185\u5bb9\u624d\u80fd\u4f7f\u6b64\u8bbe\u7f6e\u751f\u6548",
		"\u91c7\u6837\u7387",
		"\u8bbe\u7f6e\u91c7\u6837\u7387, \u53ef\u80fd\u4f1a\u5f71\u54cd\u6027\u80fd, \u9700\u8981\u5173\u95ed\u5e76\u91cd\u65b0\u5f00\u59cb\u6e38\u620f",
		"\u91c7\u6837\u63d2\u8865",
		"\u8bbe\u7f6e\u91c7\u6837\u63d2\u8865, \u53ef\u80fd\u4f1a\u5f71\u54cd\u6027\u80fd",
		"2 \u70b9\u91c7\u6837\u63d2\u8865",
		"4 \u70b9\u91c7\u6837\u63d2\u8865",
		"FM \u63d2\u8865",
		"\u914d\u7f6e FM \u63d2\u8865, \u53ef\u80fd\u4f1a\u5f71\u54cd\u6027\u80fd",
		"\u4f4e\u901a\u6ee4\u6ce2\u5668",
		"\u542f\u7528\u4f4e\u901a\u6ee4\u6ce2\u5668",
		"\u624b\u67c4\u7684\u6447\u6746\u7075\u654f\u5ea6",
		"\u964d\u4f4e\u624b\u67c4\u7684\u6447\u6746\u7075\u654f\u5ea6, \u67d0\u4e9b\u6e38\u620f\u53ef\u80fd\u9700\u8981\u8f83\u4f4e\u7684\u6570\u503c\u624d\u80fd\u8fd0\u884c",
		"\u5341\u5b57\u51c6\u5fc3",
		"\u66f4\u6539\u5341\u5b57\u51c6\u5fc3",
		"\u5149\u67aa\u8bbe\u5907\u65f6\u9690\u85cf",
		"\u59cb\u7ec8\u9690\u85cf",
		"\u59cb\u7ec8\u663e\u793a",
		"\u542f\u7528\u65cb\u98ce",
		"\u4f7f\u7528\u98ce\u9669\u81ea\u8d1f, \u53ef\u4ee5\u63d0\u9ad8\u67d0\u4e9b\u4f4e\u7aef\u8bbe\u5907\u4eff\u771f\u7cfb\u7edf\u7684\u6027\u80fd, \u4f46\u4e5f\u6709\u5df2\u77e5\u7684\u526f\u4f5c\u7528: \u4fdd\u5b58\u72b6\u6001\u4e0e\u6b63\u5e38\u89e3\u91ca\u5668\u4e0d\u517c\u5bb9, \u67d0\u4e9b\u7cfb\u7edf\u65e0\u6cd5\u5de5\u4f5c",
		"Neo-Geo \u6a21\u5f0f",
		"\u5728\u4e0e\u8fd0\u884c\u4e2d\u7684\u6e38\u620f\u517c\u5bb9\u7684\u60c5\u51b5\u4e0b, \u6839\u636e\u60a8\u7684\u9009\u62e9\u52a0\u8f7d\u9002\u5f53\u7684 BIOS",
		"FBNeo \u9519\u8bef",
		"\u4f7f\u7528\u8df3\u7ebf\u5f00\u5173\u4e2d\u7684 BIOS \u8bbe\u7f6e",
		"\u8bb0\u5fc6\u5361\u6a21\u5f0f",
		"\u66f4\u6539\u8bb0\u5fc6\u5361",
		"\u5171\u4eab",
		"\u6bcf\u4e2a\u6e38\u620f",
		"\u8c03\u8bd5\u5f00\u5173 1_1",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 1_1",
		"\u8c03\u8bd5\u5f00\u5173 1_2",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 1_2",
		"\u8c03\u8bd5\u5f00\u5173 1_3",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 1_3",
		"\u8c03\u8bd5\u5f00\u5173 1_4",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 1_4",
		"\u8c03\u8bd5\u5f00\u5173 1_5",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 1_5",
		"\u8c03\u8bd5\u5f00\u5173 1_6",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 1_6",
		"\u8c03\u8bd5\u5f00\u5173 1_7",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 1_7",
		"\u8c03\u8bd5\u5f00\u5173 1_8",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 1_8",
		"\u8c03\u8bd5\u5f00\u5173 2_1",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 2_1",
		"\u8c03\u8bd5\u5f00\u5173 2_2",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 2_2",
		"\u8c03\u8bd5\u5f00\u5173 2_3",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 2_3",
		"\u8c03\u8bd5\u5f00\u5173 2_4",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 2_4",
		"\u8c03\u8bd5\u5f00\u5173 2_5",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 2_5",
		"\u8c03\u8bd5\u5f00\u5173 2_6",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 2_6",
		"\u8c03\u8bd5\u5f00\u5173 2_7",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 2_7",
		"\u8c03\u8bd5\u5f00\u5173 2_8",
		"\u542f\u7528\u8c03\u8bd5\u5f00\u5173 2_8",
		"\u7528\u4e8e\u8c03\u8bd5",
		"\u7b2c\u4e00\u5c42",
		"\u7b2c\u4e8c\u5c42",
		"\u7b2c\u4e09\u5c42",
		"\u7b2c\u56db\u5c42",
		"\u7cbe\u7075 1",
		"\u7cbe\u7075 2",
		"\u7cbe\u7075 3",
		"\u7cbe\u7075 4",
		"\u7cbe\u7075 5",
		"\u7cbe\u7075 6",
		"\u7cbe\u7075 7",
		"\u7cbe\u7075 8",
		"\u6b64 NEOGEO \u6e38\u620f\u4f7f\u7528\u7279\u6b8a\u7684 BIOS, \u66f4\u6539\u5b83\u7684\u98ce\u9669\u7531\u60a8\u81ea\u884c\u627f\u62c5",
		"\u8df3\u7ebf\u5f00\u5173\u8bbe\u7f6e\u9488\u5bf9\u8fd0\u884c\u4e2d\u7684\u96c6\u7ec4, \u67d0\u4e9b\u9700\u8981\u91cd\u65b0\u542f\u52a8\u624d\u80fd\u6b63\u5e38\u5de5\u4f5c",
		"\u5173\u8054\u6b63\u5728\u8fd0\u884c\u7684\u96c6\u7ec4\u548c\u91d1\u624b\u6307\u6570\u636e\u5e93",
		"Neo-Geo \u8bbe\u7f6e",
		"\u914d\u7f6e Neo-Geo \u8bbe\u7f6e",
		"\u8df3\u5e27\u8bbe\u7f6e",
		"\u914d\u7f6e\u8df3\u5e27\u8bbe\u7f6e",
		"\u97f3\u9891\u8bbe\u7f6e",
		"\u914d\u7f6e\u97f3\u9891\u8bbe\u7f6e",
		"\u8df3\u7ebf\u5f00\u5173",
		"\u914d\u7f6e\u8df3\u7ebf\u5f00\u5173",
		"\u91d1\u624b\u6307",
		"\u542f\u7528\u91d1\u624b\u6307",
		"\u91cd\u542f\u4ee5\u542f\u7528\u9009\u5b9a\u7684 IPS \u8865\u4e01",
		"IPS \u8865\u4e01",
		"\u5173\u8054\u6b63\u5728\u8fd0\u884c\u7684\u96c6\u7ec4\u548c IPS \u8865\u4e01\u6570\u636e\u5e93",
		"RomData \u4ec5\u80fd\u5355\u9009, \u591a\u9009\u65f6\u5f3a\u5236\u968f\u673a\u9009\u62e9",
		"\u91cd\u542f\u4ee5\u542f\u7528\u9009\u5b9a\u7684 RomData",
		"\u8c03\u8bd5",
		"\u914d\u7f6e\u8c03\u8bd5\u529f\u80fd",
		"\u8fd9\u4e2a\u96c6\u7ec4\u88ab\u6807\u8bb0\u4e3a\u4e0d\u53ef\u7528\n\u8bf7\u5c1d\u8bd5\u5b83\u7684\u5176\u5b83\u514b\u9686\u7248\u672c",
		"BIOS \u4e0d\u80fd\u4ee5\u8fd9\u79cd\u65b9\u5f0f\u542f\u52a8",
		"\u542f\u52a8 neogeo CD \u9700\u8981\u5149\u76d8\u6620\u50cf\n",
		"\u8be5\u6e38\u620f\u5b58\u5728, \u4f46\u5728\u5f53\u524d\u7248\u672c\u7684 FBNeo \u68c0\u67e5\u4e2d\u6b64\u96c6\u7ec4\u7f3a\u5c11 ROMs\u3002\n",
		"\u6838\u5bf9\u4e0b\u5217\u96c6\u7ec4 : %s%s%s%s%s\n",
		"\u8bf7\u6ce8\u610f, \u5f53\u524d\u5e73\u53f0\u5df2\u7981\u7528 7z \u652f\u6301\u3002\n\n",
		"\u8fd9\u5e76\u975e\u9519\u8bef\uff01\u5982\u679c\u60a8\u9700\u8981\u8fdb\u4e00\u6b65\u4e86\u89e3,\n\u60a8\u53ef\u4ee5\u5728 https://docs.libretro.com/ \u4e0a\u627e\u5230\u5e76\u9605\u8bfb\u8857\u673a\u548c FBNeo \u7684\u76f8\u5173\u6587\u6863\u3002\n",
		"\u9a71\u52a8\u7a0b\u5e8f\u521d\u59cb\u5316\u5931\u8d25,\n\u60a8\u53ef\u4ee5\u5728\u5fc5\u8981\u7684\u65f6\u5019\u9009\u62e9\u53cd\u9988\u3002",
		"\u672a\u77e5\u96c6\u7ec4\u3002\n",
		"\u60a8\u7684\u8bbe\u5907\u663e\u793a, \u56e0\u4e3a\u6027\u80fd\u539f\u56e0, \u5b83\u4ec5\u80fd\u4f7f\u7528\u6b64\u7248\u672c FBNeo \u7684\u90e8\u5206\u529f\u80fd\u3002\n\u4e5f\u56e0\u6b64, \u4e0d\u88ab\u8bbe\u5907\u652f\u6301\u7684\u96c6\u7ec4\u5df2\u88ab\u5254\u9664\u3002\n\n",
		"%s\n\u540d\u79f0\u4e3a %s \u548c CRC \u4e3a 0x%08x \u7684 ROM \u6ca1\u6709\u627e\u5230"
	},
	{	// Traditional Chinese
		"\u76e1\u53ef\u80fd\u4f7f\u7528 32 \u4f4d\u8272\u6df1",
		"\u67d0\u4e9b\u904a\u6232\u9700\u8981\u66f4\u6539\u50cf\u7d20\u683c\u5f0f\u624d\u80fd\u6b63\u5e38\u6e32\u67d3, \u9019\u53ef\u80fd\u6703\u5f71\u97ff\u67d0\u4e9b\u5e73\u81fa\u7684\u6027\u80fd",
		"\u7e31\u5411\u6a21\u5f0f",
		"\u65cb\u8f49\u986f\u793a\u5c4f\u4ee5\u9069\u61c9\u7e31\u5411\u5c4f\u5e55",
		"\u66ff\u63db",
		"TATE \u66ff\u63db",
		"\u5f37\u88fd 60Hz",
		"\u5ffd\u7565\u904a\u6232\u7684\u539f\u59cb\u5237\u65b0\u7387, \u5617\u8a66\u4ee5 60hz \u904b\u884c, \u9019\u6703\u5c0e\u81f4\u904a\u6232\u901f\u5ea6\u548c\u5e40\u6578\u4e0d\u6b63\u78ba, \u5982\u679c\u986f\u793a\u5668\u7684\u5237\u65b0\u7387\u5728 59hz \u548c 61hz \u4e4b\u9593, \u5b83\u6703\u5617\u8a66\u4f7f\u7528\u6b63\u78ba\u7684\u5237\u65b0\u7387, \u800c\u4e0d\u662f 60hz",
		"\u56fa\u5b9a\u8df3\u5e40",
		"\u7576\u8df3\u5e40\u8a2d\u7f6e\u70ba\u56fa\u5b9a\u6642, \u6216\u8005\u524d\u7aef\u4e0d\u652f\u6301\u66ff\u4ee3\u7684\u8df3\u5e40\u6a21\u5f0f, \u5247\u4ee5 X+1 \u5e40\u4e2d X \u5e40\u7684\u56fa\u5b9a\u901f\u7387\u8df3\u904e\u6e32\u67d3",
		"\u7981\u7528\u8df3\u5e40",
		"\u8df3\u904e 2 \u5e40\u4e2d 1 \u5e40\u7684\u6e32\u67d3",
		"\u8df3\u904e 3 \u5e40\u4e2d 2 \u5e40\u7684\u6e32\u67d3",
		"\u8df3\u904e 4 \u5e40\u4e2d 3 \u5e40\u7684\u6e32\u67d3",
		"\u8df3\u904e 5 \u5e40\u4e2d 4 \u5e40\u7684\u6e32\u67d3",
		"\u8df3\u904e 6 \u5e40\u4e2d 5 \u5e40\u7684\u6e32\u67d3",
		"\u8df3\u5e40",
		"\u907f\u514d\u56e0\u97f3\u983b\u7de9\u6c96\u4e0d\u8db3 (\u7206\u88c2\u8072), \u4ee5\u8df3\u5e40\u72a7\u7272\u8996\u89ba\u5e73\u6ed1\u5ea6\u70ba\u4ee3\u50f9\u63d0\u9ad8\u6027\u80fd, \u524d\u7aef\u5efa\u8b70 [\u81ea\u52d5], [\u624b\u52d5] \u53ef\u8abf\u6574\u8df3\u5e40\u95be\u503c (%) \u8a2d\u7f6e, \u4ee5\u53ca [\u56fa\u5b9a] \u7684\u53ef\u9078\u8a2d\u7f6e",
		"\u56fa\u5b9a",
		"\u81ea\u52d5",
		"\u624b\u52d5",
		"\u8df3\u5e40\u95be\u503c (%)",
		"\u7576\u8df3\u5e40\u8a2d\u7f6e\u70ba [\u624b\u52d5] \u6642, \u97f3\u983b\u7de9\u6c96\u95be\u503c\u4f4e\u65bc\u8a72\u95be\u503c (\u767e\u5206\u6bd4) \u7684\u5e40\u5c07\u88ab\u8df3\u904e, \u8f03\u9ad8\u7684\u503c\u6703\u983b\u7e41\u5730\u8df3\u5e40, \u6e1b\u5c11\u8072\u97f3\u7206\u88c2\u7684\u767c\u751f",
		"CPU \u8d85\u983b",
		"\u589e\u52a0\u6216\u964d\u4f4e\u5404\u7cfb\u7d71\u6a21\u64ec CPU \u7684\u983b\u7387, \u53ef\u4ee5\u4fee\u5fa9\u67d0\u4e9b\u904a\u6232\u901f\u5ea6\u4e0b\u964d\u554f\u984c, \u540c\u6642\u53ef\u4ee5\u5e6b\u52a9\u63d0\u9ad8\u4f4e\u7aef\u8a2d\u5099\u7684\u6027\u80fd",
		"\u8a3a\u65b7\u8f38\u5165",
		"\u8a2d\u7f6e\u9032\u5165\u670d\u52d9\u83dc\u55ae\u7684\u7d44\u5408\u9375",
		"\u4e0d\u8a2d\u7f6e",
		"\u6309\u4f4f Start",
		"\u6309\u4f4f Start + A + B",
		"\u6309\u4f4f Start + L + R",
		"\u6309\u4f4f Select",
		"\u6309\u4f4f Select + A + B",
		"\u6309\u4f4f Select + L + R",
		"\u9ad8\u5206",
		"\u555f\u7528\u9ad8\u5206\u652f\u6301, \u60a8\u9084\u9700\u8981 system/fbneo/ \u76ee\u9304\u4e2d\u7684 hiscore.dat \u6587\u4ef6",
		"\u5141\u8a31\u4fee\u88dc\u96c6\u7d44",
		"\u5141\u8a31 system/fbneo/patched/ \u76ee\u9304\u4e2d\u7684\u96c6\u7d44\u8986\u84cb\u60a8\u7684\u96c6\u7d44, crc \u5c07\u88ab\u5ffd\u7565, \u4f46\u5927\u5c0f\u548c\u540d\u7a31\u4ecd\u5fc5\u9808\u5339\u914d, \u60a8\u9700\u8981\u95dc\u9589\u5167\u5bb9\u624d\u80fd\u4f7f\u6b64\u8a2d\u7f6e\u751f\u6548",
		"\u91c7\u6a23\u7387",
		"\u8a2d\u7f6e\u91c7\u6a23\u7387, \u53ef\u80fd\u6703\u5f71\u97ff\u6027\u80fd, \u9700\u8981\u95dc\u9589\u4e26\u91cd\u65b0\u958b\u59cb\u904a\u6232",
		"\u91c7\u6a23\u63d2\u88dc",
		"\u8a2d\u7f6e\u91c7\u6a23\u63d2\u88dc, \u53ef\u80fd\u6703\u5f71\u97ff\u6027\u80fd",
		"2 \u9ede\u91c7\u6a23\u63d2\u88dc",
		"4 \u9ede\u91c7\u6a23\u63d2\u88dc",
		"FM \u63d2\u88dc",
		"\u914d\u7f6e FM \u63d2\u88dc, \u53ef\u80fd\u6703\u5f71\u97ff\u6027\u80fd",
		"\u4f4e\u901a\u6ffe\u6ce2\u5668",
		"\u555f\u7528\u4f4e\u901a\u6ffe\u6ce2\u5668",
		"\u624b\u67c4\u7684\u6416\u687f\u9748\u654f\u5ea6",
		"\u964d\u4f4e\u624b\u67c4\u7684\u6416\u687f\u9748\u654f\u5ea6, \u67d0\u4e9b\u904a\u6232\u53ef\u80fd\u9700\u8981\u8f03\u4f4e\u7684\u6578\u503c\u624d\u80fd\u904b\u884c",
		"\u5341\u5b57\u6e96\u5fc3",
		"\u66f4\u6539\u5341\u5b57\u6e96\u5fc3",
		"\u5149\u69cd\u8a2d\u5099\u6642\u96b1\u85cf",
		"\u59cb\u7d42\u96b1\u85cf",
		"\u59cb\u7d42\u986f\u793a",
		"\u555f\u7528\u65cb\u98a8",
		"\u4f7f\u7528\u98a8\u96aa\u81ea\u8ca0, \u53ef\u4ee5\u63d0\u9ad8\u67d0\u4e9b\u4f4e\u7aef\u8a2d\u5099\u4eff\u771f\u7cfb\u7d71\u7684\u6027\u80fd, \u4f46\u4e5f\u6709\u5df2\u77e5\u7684\u526f\u4f5c\u7528: \u4fdd\u5b58\u72c0\u614b\u8207\u6b63\u5e38\u89e3\u91cb\u5668\u4e0d\u517c\u5bb9, \u67d0\u4e9b\u7cfb\u7d71\u7121\u6cd5\u5de5\u4f5c",
		"Neo-Geo \u6a21\u5f0f",
		"\u5728\u8207\u904b\u884c\u4e2d\u7684\u904a\u6232\u517c\u5bb9\u7684\u60c5\u6cc1\u4e0b, \u6839\u64da\u60a8\u7684\u9078\u64c7\u52a0\u8f09\u9069\u7576\u7684 BIOS",
		"FBNeo \u932f\u8aa4",
		"\u4f7f\u7528\u8df3\u7dda\u958b\u95dc\u4e2d\u7684 BIOS \u8a2d\u7f6e",
		"\u8a18\u61b6\u5361\u6a21\u5f0f",
		"\u66f4\u6539\u8a18\u61b6\u5361",
		"\u5171\u4eab",
		"\u6bcf\u500b\u904a\u6232",
		"\u8abf\u8a66\u958b\u95dc 1_1",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 1_1",
		"\u8abf\u8a66\u958b\u95dc 1_2",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 1_2",
		"\u8abf\u8a66\u958b\u95dc 1_3",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 1_3",
		"\u8abf\u8a66\u958b\u95dc 1_4",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 1_4",
		"\u8abf\u8a66\u958b\u95dc 1_5",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 1_5",
		"\u8abf\u8a66\u958b\u95dc 1_6",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 1_6",
		"\u8abf\u8a66\u958b\u95dc 1_7",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 1_7",
		"\u8abf\u8a66\u958b\u95dc 1_8",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 1_8",
		"\u8abf\u8a66\u958b\u95dc 2_1",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 2_1",
		"\u8abf\u8a66\u958b\u95dc 2_2",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 2_2",
		"\u8abf\u8a66\u958b\u95dc 2_3",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 2_3",
		"\u8abf\u8a66\u958b\u95dc 2_4",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 2_4",
		"\u8abf\u8a66\u958b\u95dc 2_5",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 2_5",
		"\u8abf\u8a66\u958b\u95dc 2_6",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 2_6",
		"\u8abf\u8a66\u958b\u95dc 2_7",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 2_7",
		"\u8abf\u8a66\u958b\u95dc 2_8",
		"\u555f\u7528\u8abf\u8a66\u958b\u95dc 2_8",
		"\u7528\u65bc\u8abf\u8a66",
		"\u7b2c\u4e00\u5c64",
		"\u7b2c\u4e8c\u5c64",
		"\u7b2c\u4e09\u5c64",
		"\u7b2c\u56db\u5c64",
		"\u7cbe\u9748 1",
		"\u7cbe\u9748 2",
		"\u7cbe\u9748 3",
		"\u7cbe\u9748 4",
		"\u7cbe\u9748 5",
		"\u7cbe\u9748 6",
		"\u7cbe\u9748 7",
		"\u7cbe\u9748 8",
		"\u6b64 NEOGEO \u904a\u6232\u4f7f\u7528\u7279\u6b8a\u7684 BIOS, \u66f4\u6539\u5b83\u7684\u98a8\u96aa\u7531\u60a8\u81ea\u884c\u627f\u64d4",
		"\u8df3\u7dda\u958b\u95dc\u8a2d\u7f6e\u91dd\u5c0d\u904b\u884c\u4e2d\u7684\u96c6\u7d44, \u67d0\u4e9b\u9700\u8981\u91cd\u65b0\u555f\u52d5\u624d\u80fd\u6b63\u5e38\u5de5\u4f5c",
		"\u95dc\u806f\u6b63\u5728\u904b\u884c\u7684\u96c6\u7d44\u548c\u91d1\u624b\u6307\u6578\u64da\u5eab",
		"Neo-Geo \u8a2d\u5b9a",
		"\u914d\u7f6e Neo-Geo \u8a2d\u7f6e",
		"\u8df3\u5e40\u8a2d\u7f6e",
		"\u914d\u7f6e\u8df3\u5e40\u8a2d\u7f6e",
		"\u97f3\u983b\u8a2d\u7f6e",
		"\u914d\u7f6e\u97f3\u983b\u8a2d\u7f6e",
		"\u8df3\u7dda\u958b\u95dc",
		"\u914d\u7f6e\u8df3\u7dda\u958b\u95dc",
		"\u91d1\u624b\u6307",
		"\u555f\u7528\u91d1\u624b\u6307",
		"\u91cd\u555f\u4ee5\u555f\u7528\u9078\u5b9a\u7684 IPS \u88dc\u4e01",
		"Neo-Geo \u8a2d\u7f6e",
		"\u95dc\u806f\u6b63\u5728\u904b\u884c\u7684\u96c6\u7d44\u548c IPS \u88dc\u4e01\u6578\u64da\u5eab",
		"RomData \u50c5\u80fd\u55ae\u9078, \u591a\u9078\u6642\u5f37\u88fd\u96a8\u6a5f\u9078\u64c7",
		"\u91cd\u555f\u4ee5\u555f\u7528\u9078\u5b9a\u7684 RomData",
		"\u8abf\u8a66",
		"\u914d\u7f6e\u8abf\u8a66\u529f\u80fd",
		"\u9019\u500b\u96c6\u7d44\u88ab\u6a19\u8a18\u70ba\u4e0d\u53ef\u7528\n\u8acb\u5617\u8a66\u5b83\u7684\u5176\u5b83\u514b\u9686\u7248\u672c",
		"BIOS \u4e0d\u80fd\u4ee5\u9019\u7a2e\u65b9\u5f0f\u555f\u52d5",
		"\u555f\u52d5 neogeo CD \u9700\u8981\u5149\u76e4\u6620\u50cf\n",
		"\u8a72\u904a\u6232\u5b58\u5728, \u4f46\u5728\u7576\u524d\u7248\u672c\u7684 FBNeo \u6aa2\u67e5\u4e2d\u6b64\u96c6\u7d44\u7f3a\u5c11 ROMs\u3002\n",
		"\u6838\u5c0d\u4e0b\u5217\u96c6\u7d44 : %s%s%s%s%s\n",
		"\u8acb\u8a3b\u610f, \u7576\u524d\u5e73\u81fa\u5df2\u7981\u7528 7z \u652f\u6301\u3002\n\n",
		"\u9019\u4e26\u975e\u932f\u8aa4\uff01\u5982\u679c\u60a8\u9700\u8981\u9032\u4e00\u6b65\u4e86\u89e3,\n\u60a8\u53ef\u4ee5\u5728 https://docs.libretro.com/ \u4e0a\u627e\u5230\u4e26\u95b1\u8b80\u8857\u6a5f\u548c FBNeo \u7684\u76f8\u95dc\u6587\u6a94\u3002\n",
		"\u9a45\u52d5\u7a0b\u5e8f\u521d\u59cb\u5316\u5931\u6557,\n\u60a8\u53ef\u4ee5\u5728\u5fc5\u8981\u7684\u6642\u5019\u9078\u64c7\u53cd\u994b\u3002",
		"\u672a\u77e5\u96c6\u7d44\u3002\n",
		"\u60a8\u7684\u8a2d\u5099\u986f\u793a, \u56e0\u70ba\u6027\u80fd\u539f\u56e0, \u5b83\u50c5\u80fd\u4f7f\u7528\u6b64\u7248\u672c FBNeo \u7684\u90e8\u5206\u529f\u80fd\u3002\n\u4e5f\u56e0\u6b64, \u4e0d\u88ab\u8a2d\u5099\u652f\u6301\u7684\u96c6\u7d44\u5df2\u88ab\u5254\u9664\u3002\n\n",
		"%s\n\u540d\u7a31\u70ba %s \u548c CRC \u70ba 0x%08x \u7684 ROM \u6c92\u6709\u627e\u5230"
	}
};

void set_multi_language_strings()
{
	INT32 nLangcode = 0;
	environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &nLangcode);

	switch (nLangcode)
	{
		case RETRO_LANGUAGE_CHINESE_SIMPLIFIED:
			nLangcode = 1; break;
		case RETRO_LANGUAGE_CHINESE_TRADITIONAL:
			nLangcode = 2; break;
#if 0
		case RETRO_LANGUAGE_JAPANESE:
			nLangcode = 3; break;
		case RETRO_LANGUAGE_KOREAN:
			nLangcode = 4; break;
		case RETRO_LANGUAGE_FRENCH:
			nLangcode = 5; break;
		case RETRO_LANGUAGE_SPANISH:
			nLangcode = 6; break;
		case RETRO_LANGUAGE_ITALIAN:
			nLangcode = 7; break;
		case RETRO_LANGUAGE_GERMAN:
			nLangcode = 8; break;
		case RETRO_LANGUAGE_PORTUGUESE_BRAZIL:
			nLangcode = 9; break;
		case RETRO_LANGUAGE_POLISH:
			nLangcode = 10; break;
		case RETRO_LANGUAGE_HUNGARIAN:
			nLangcode = 11; break;
#endif // 0
	default:
		break;
	}

	for (INT32 i = 0; i < NUM_STRING; i++)
		pSelLangStr[i] = multi_language_strings[nLangcode][i];
}

#undef NUM_STRING
#undef MAX_LANGUAGES
