#pragma once
#include "stdafx.h"
#include "SA2ModLoader.h"
#include <string>

enum ChaoEmotionball : char
{
	ChaoEmotionball_Normal = 0,
	ChaoEmotionball_Flame = 1,
	ChaoEmotionball_None = 2,
};

struct NChaoData
{
	char gap_0[18];
	char Name[7];
	char gap_19[7];
	char SwimFraction;
	char FlyFraction;
	char RunFraction;
	char PowerFraction;
	char StaminaFraction;
	char LuckFraction;
	char IntelligenceFraction;
	char UnknownFraction;
	ChaoGrade SwimGrade;
	ChaoGrade FlyGrade;
	ChaoGrade RunGrade;
	ChaoGrade PowerGrade;
	ChaoGrade StaminaGrade;
	ChaoGrade LuckGrade;
	ChaoGrade IntelligenceGrade;
	ChaoGrade UnknownGrade;
	char SwimLevel;
	char FlyLevel;
	char RunLevel;
	char PowerLevel;
	char StaminaLevel;
	char LuckLevel;
	char IntelligenceLevel;
	char UnknownLevel;
	__int16 SwimStat;
	__int16 FlyStat;
	__int16 RunStat;
	__int16 PowerStat;
	__int16 StaminaStat;
	__int16 LuckStat;
	char IntelligenceStat;
	char UnknownStat;
	char field_46[58];
	char Type;
	char Garden;
	__int16 Happiness;
	__int16 field_84;
	__int16 ClockRollovers;
	__int16 field_88;
	__int16 LifeSpan;
	__int16 Lifespan2;
	__int16 Reincarnations;
	char field_90[24];
	float PowerRun;
	float FlySwim;
	float Alignment;
	char field_B4[12];
	float EvolutionProgress;
	char field_C4[13];
	ChaoEyes EyeType;
	ChaoMouth MouthType;
	ChaoEmotionball BallType;
	char gap_D4[1];
	char Headgear;
	char HideFeet;
	ChaoMedal Medal;
	char Color;
	char MonotoneHighlights;
	char Texture;
	char Shiny;
	char EggColor;
	SADXBodyType BodyType;
	char BodyTypeAnimal;
	char field_DF[57];
	int SA2AnimalBehaviour;
	char SA2BArmType;
	char SA2BEarType;
	char SA2BForeheadType;
	char SA2BHornType;
	char SA2BLegType;
	char SA2BTailType;
	char SA2BWingType;
	char SA2BFaceType;
	char field_124[8];
	char Joy;
	char Anger;
	char Urgetocry;
	char fear;
	char surprise;
	char dizziness;
	char relax;
	char total;
	__int16 sleepiness;
	__int16 slpdpth;
	__int16 hunger;
	__int16 breed;
	__int16 boredom;
	__int16 lonely;
	__int16 tire;
	__int16 stress;
	__int16 nourish;
	__int16 conditn;
	__int16 energy;
	char Normal_Curiosity;
	char Kindness;
	char Crybaby_energetic;
	char naive;
	char solitude;
	char vitality;
	char glutton;
	char regain;
	char skillful;
	char charm;
	char chatty;
	char carefree;
	char fickle;
	char FavoriteFruit;
	char field_158[2];
	char CoughLevel;
	char ColdLevel;
	char RashLevel;
	char RunnyNoseLevel;
	char HiccupsLevel;
	char StomachAcheLevel;
	char field_160[4];
	__int16 SA2BToys;
	char field_166;
	ChaoCharacterBond SA2BCharacterBonds[6];
	char field_190[680];
	ChaoDNA DNA;
	int field_4DC;
	int SADXAnimalBehaviors;
	__int16 ArmType;
	__int16 EarType;
	__int16 EyebrowType;
	__int16 ForeheadType;
	__int16 LegType;
	__int16 TailType;
	__int16 WingType;
	__int16 UnknownType;
	ChaoCharacterBond SADXCharacterBonds[6];
	char field_520[736];
};

std::wstring ChaoNameDecoder[256]
{
	L"",
	L"!",
	L"\"",
	L"#",
	L"$",
	L"%",
	L"&",
	L"'",
	L"(",
	L")",
	L"*",
	L"+",
	L",",
	L"-",
	L".",
	L"/",
	L"0",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L":",
	L";",
	L"<",
	L"=",
	L">",
	L"?",
	L"@",
	L"A",
	L"B",
	L"C",
	L"D",
	L"E",
	L"F",
	L"G",
	L"H",
	L"I",
	L"J",
	L"K",
	L"L",
	L"M",
	L"N",
	L"O",
	L"P",
	L"Q",
	L"R",
	L"S",
	L"T",
	L"U",
	L"V",
	L"W",
	L"X",
	L"Y",
	L"Z",
	L"[",
	L"¥",
	L"]",
	L"^",
	L"_",
	L"‘",
	L"a",
	L"b",
	L"c",
	L"d",
	L"e",
	L"f",
	L"g",
	L"h",
	L"i",
	L"j",
	L"k",
	L"l",
	L"m",
	L"n",
	L"o",
	L"p",
	L"q",
	L"r",
	L"s",
	L"t",
	L"u",
	L"v",
	L"w",
	L"x",
	L"y",
	L"z",
	L"{",
	L"|",
	L"}",
	L"~",
	L" ",
	L"À",
	L"Á",
	L"Â",
	L"Ã",
	L"Ä",
	L"Å",
	L"Æ",
	L"Ç",
	L"È",
	L"É",
	L"Ê",
	L"Ë",
	L"Ì",
	L"Í",
	L"Î",
	L"Ï",
	L"Ð",
	L"Ñ",
	L"Ò",
	L"Ó",
	L"Ô",
	L"Õ",
	L"Ö",
	L"¿",
	L"Ø",
	L"Ù",
	L"Ú",
	L"Û",
	L"Ü",
	L"Ý",
	L"Þ",
	L"ß",
	L"à",
	L"á",
	L"â",
	L"ã",
	L"ä",
	L"å",
	L"æ",
	L"ç",
	L"è",
	L"é",
	L"ê",
	L"ë",
	L"ì",
	L"í",
	L"î",
	L"ï",
	L"ð",
	L"ñ",
	L"ò",
	L"ó",
	L"ô",
	L"õ",
	L"ö",
	L"¡",
	L"ø",
	L"ù",
	L"ú",
	L"û",
	L"ü",
	L"ý",
	L"þ",
	L"ÿ",
	L"ァ",
	L"ア",
	L"ィ",
	L"イ",
	L"ゥ",
	L"ウ",
	L"ェ",
	L"エ",
	L"ォ",
	L"オ",
	L"カ",
	L"ガ",
	L"キ",
	L"ギ",
	L"ク",
	L"グ",
	L"ケ",
	L"ゲ",
	L"コ",
	L"ゴ",
	L"サ",
	L"ザ",
	L"シ",
	L"ジ",
	L"ス",
	L"ズ",
	L"セ",
	L"ゼ",
	L"ソ",
	L"ゾ",
	L"タ",
	L"ダ",
	L"チ",
	L"ヂ",
	L"ツ",
	L"ッ",
	L"ヅ",
	L"テ",
	L"デ",
	L"ト",
	L"ド",
	L"ナ",
	L"ニ",
	L"ヌ",
	L"ネ",
	L"ノ",
	L"ハ",
	L"バ",
	L"パ",
	L"ヒ",
	L"ビ",
	L"ピ",
	L"フ",
	L"ブ",
	L"プ",
	L"ヘ",
	L"ベ",
	L"ベ",
	L"ホ",
	L"ボ",
	L"ポ",
	L"マ",
	L"ミ",
	L"ム",
	L"メ",
	L"モ",
	L"ャ",
	L"ヤ",
	L"ュ",
	L"ユ",
	L"ョ",
	L"ヨ",
	L"ラ",
	L"リ",
	L"ル",
	L"レ",
	L"ロ",
	L"ヮ",
	L"ワ",
	L"ﾞ",
	L"ﾟ",
	L"ヲ",
	L"ン",
	L"。",
	L"、",
	L"〒",
	L"・",
	L"★",
	L"♀",
	L"♂",
	L"♪",
	L"…",
	L"「",
	L"」",
	L"ヴ",
	L" "
};

std::wstring DecodeChaoName(char name[])
{
	int i = 0;
	std::wstring result = L"";
	while (name[i] != 0 && i < 7)
	{
		result.append(ChaoNameDecoder[name[i]]);
		i++;
	}
	return result;
}

#pragma region rendering stuff

enum D3D9Funcs
{
	D3D9Funcs_QueryInterface = 0,
	D3D9Funcs_AddRef = 1,
	D3D9Funcs_Release = 2,
	D3D9Funcs_TestCooperativeLevel = 3,
	D3D9Funcs_GetAvailableTextureMem = 4,
	D3D9Funcs_EvictManagedResources = 5,
	D3D9Funcs_GetDirect3D = 6,
	D3D9Funcs_GetDeviceCaps = 7,
	D3D9Funcs_GetDisplayMode = 8,
	D3D9Funcs_GetCreationParameters = 9,
	D3D9Funcs_SetCursorProperties = 10,
	D3D9Funcs_SetCursorPosition = 11,
	D3D9Funcs_ShowCursor = 12,
	D3D9Funcs_CreateAdditionalSwapChain = 13,
	D3D9Funcs_GetSwapChain = 14,
	D3D9Funcs_GetNumberOfSwapChains = 15,
	D3D9Funcs_Reset = 16,
	D3D9Funcs_Present = 17,
	D3D9Funcs_GetBackBuffer = 18,
	D3D9Funcs_GetRasterStatus = 19,
	D3D9Funcs_SetDialogBoxMode = 20,
	D3D9Funcs_SetGammaRamp = 21,
	D3D9Funcs_GetGammaRamp = 22,
	D3D9Funcs_CreateTexture = 23,
	D3D9Funcs_CreateVolumeTexture = 24,
	D3D9Funcs_CreateCubeTexture = 25,
	D3D9Funcs_CreateVertexBuffer = 26,
	D3D9Funcs_CreateIndexBuffer = 27,
	D3D9Funcs_CreateRenderTarget = 28,
	D3D9Funcs_CreateDepthStencilSurface = 29,
	D3D9Funcs_UpdateSurface = 30,
	D3D9Funcs_UpdateTexture = 31,
	D3D9Funcs_GetRenderTargetData = 32,
	D3D9Funcs_GetFrontBufferData = 33,
	D3D9Funcs_StretchRect = 34,
	D3D9Funcs_ColorFill = 35,
	D3D9Funcs_CreateOffscreenPlainSurface = 36,
	D3D9Funcs_SetRenderTarget = 37,
	D3D9Funcs_GetRenderTarget = 38,
	D3D9Funcs_SetDepthStencilSurface = 39,
	D3D9Funcs_GetDepthStencilSurface = 40,
	D3D9Funcs_BeginScene = 41,
	D3D9Funcs_EndScene = 42,
	D3D9Funcs_Clear = 43,
	D3D9Funcs_SetTransform = 44,
	D3D9Funcs_GetTransform = 45,
	D3D9Funcs_MultiplyTransform = 46,
	D3D9Funcs_SetViewport = 47,
	D3D9Funcs_GetViewport = 48,
	D3D9Funcs_SetMaterial = 49,
	D3D9Funcs_GetMaterial = 50,
	D3D9Funcs_SetLight = 51,
	D3D9Funcs_GetLight = 52,
	D3D9Funcs_LightEnable = 53,
	D3D9Funcs_GetLightEnable = 54,
	D3D9Funcs_SetClipPlane = 55,
	D3D9Funcs_GetClipPlane = 56,
	D3D9Funcs_SetRenderState = 57,
	D3D9Funcs_GetRenderState = 58,
	D3D9Funcs_CreateStateBlock = 59,
	D3D9Funcs_BeginStateBlock = 60,
	D3D9Funcs_EndStateBlock = 61,
	D3D9Funcs_SetClipStatus = 62,
	D3D9Funcs_GetClipStatus = 63,
	D3D9Funcs_GetTexture = 64,
	D3D9Funcs_SetTexture = 65,
	D3D9Funcs_GetTextureStageState = 66,
	D3D9Funcs_SetTextureStageState = 67,
	D3D9Funcs_GetSamplerState = 68,
	D3D9Funcs_SetSamplerState = 69,
	D3D9Funcs_ValidateDevice = 70,
	D3D9Funcs_SetPaletteEntries = 71,
	D3D9Funcs_GetPaletteEntries = 72,
	D3D9Funcs_SetCurrentTexturePalette = 73,
	D3D9Funcs_GetCurrentTexturePalette = 74,
	D3D9Funcs_SetScissorRect = 75,
	D3D9Funcs_GetScissorRect = 76,
	D3D9Funcs_SetSoftwareVertexProcessing = 77,
	D3D9Funcs_GetSoftwareVertexProcessing = 78,
	D3D9Funcs_SetNPatchMode = 79,
	D3D9Funcs_GetNPatchMode = 80,
	D3D9Funcs_DrawPrimitive = 81,
	D3D9Funcs_DrawIndexedPrimitive = 82,
	D3D9Funcs_DrawPrimitiveUP = 83,
	D3D9Funcs_DrawIndexedPrimitiveUP = 84,
	D3D9Funcs_ProcessVertices = 85,
	D3D9Funcs_CreateVertexDeclaration = 86,
	D3D9Funcs_SetVertexDeclaration = 87,
	D3D9Funcs_GetVertexDeclaration = 88,
	D3D9Funcs_SetFVF = 89,
	D3D9Funcs_GetFVF = 90,
	D3D9Funcs_CreateVertexShader = 91,
	D3D9Funcs_SetVertexShader = 92,
	D3D9Funcs_GetVertexShader = 93,
	D3D9Funcs_SetVertexShaderConstantF = 94,
	D3D9Funcs_GetVertexShaderConstantF = 95,
	D3D9Funcs_SetVertexShaderConstantI = 96,
	D3D9Funcs_GetVertexShaderConstantI = 97,
	D3D9Funcs_SetVertexShaderConstantB = 98,
	D3D9Funcs_GetVertexShaderConstantB = 99,
	D3D9Funcs_SetStreamSource = 100,
	D3D9Funcs_GetStreamSource = 101,
	D3D9Funcs_SetStreamSourceFreq = 102,
	D3D9Funcs_GetStreamSourceFreq = 103,
	D3D9Funcs_SetIndices = 104,
	D3D9Funcs_GetIndices = 105,
	D3D9Funcs_CreatePixelShader = 106,
	D3D9Funcs_SetPixelShader = 107,
	D3D9Funcs_GetPixelShader = 108,
	D3D9Funcs_SetPixelShaderConstantF = 109,
	D3D9Funcs_GetPixelShaderConstantF = 110,
	D3D9Funcs_SetPixelShaderConstantI = 111,
	D3D9Funcs_GetPixelShaderConstantI = 112,
	D3D9Funcs_SetPixelShaderConstantB = 113,
	D3D9Funcs_GetPixelShaderConstantB = 114,
	D3D9Funcs_DrawRectPatch = 115,
	D3D9Funcs_DrawTriPatch = 116,
	D3D9Funcs_DeletePatch = 117,
	D3D9Funcs_CreateQuery = 118,
};

struct handle_data {
	unsigned long process_id;
	HWND window_handle;
};

BOOL is_main_window(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !is_main_window(handle))
		return TRUE;
	data.window_handle = handle;
	return FALSE;
}

HWND find_main_window(unsigned long process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.window_handle = 0;
	EnumWindows(enum_windows_callback, (LPARAM)&data);
	return data.window_handle;
}

#pragma endregion
