#pragma region Includes
	#include "stdafx.h"
	#include "SA2ModLoader.h"
	#include "ChaoData.h"
	#include "Trampoline.cpp"
	#include <direct.h>
	#include <stdlib.h>
	#include <stdio.h>

	#include <d3d9.h>
	#include <d3dx9.h>
	#pragma comment(lib, "d3d9.lib")
	#pragma comment(lib, "d3dx9.lib")
#pragma endregion

#pragma region Variable declaration
	#pragma region Function Variables
	char ChaoMenuString[] = { 07, 'C', 'H', 'A', 'O', ' ', 'D', 'A', 'T', 'A', 0 };
	DataPointer(int, PauseMenuID, 0x017472BC);
	FunctionPointer(void, DisplayPauseMenu, (), 0x00440AD0);
	NChaoData *displayChao;
	std::wstring ChaoName;

	IDirect3D9 *d3d;
	IDirect3DDevice9 *d3ddev;
	void **DirectXFunctions;
	void *endSceneTableEntry;
	void *resetTableEntry;

	std::string ResourcesPath;
	Trampoline *RenderFunc;
	#pragma endregion

float width = 1920;
float height = 1080;
ID3DXFont *font = nullptr;

LPD3DXSPRITE Sprite;
RECT MenuBackgroundRect;
LPDIRECT3DTEXTURE9 MenuFillingTextures[9];
LPDIRECT3DTEXTURE9 MenuBorderTextures[9];

D3DXMATRIX MenuBackgroundTransforms[9];
D3DXMATRIX MenuTabTransforms[24];
D3DXMATRIX InvalidMenuTransforms[9];

RECT invalidTextRect[2];
RECT TabTextRects[8];
std::string TabTextStrings[4] = {
	"Stats",
	"Visual",
	"Behaviour",
	"DNA"
};

//DNA Menu
D3DXMATRIX DNA_LeftBox[9];
D3DXMATRIX DNA_RightBox[9];
RECT DNA_Texts[54];
RECT DNA_TextShadows[54];
std::string DNA_Descr[13] = {
	"Egg color:",
	"Color:",
	"Monotone:",
	"Shiny:",
	"Texture:",
	"Swim Rank:",
	"Fly Rank:",
	"Run Rank:",
	"Power Rank:",
	"Stamina Rank:",
	"Luck Rank:",
	"Intel. Rank:",
	"Fav. Fruit:"
};
std::string DNA_DATA[26];

// general things
int Colors[12] = {
	0x50FF8080,
	0x5080FF80,
	0x508080FF,
	0x50FFFF80,

	0xFFDD2323,
	0xFF19B75E,
	0xFF2975D8,
	0xFFFFCB3D,

	0xFFCC1212,
	0xFF08A64D,
	0xFF1864C7,
	0xFFEEBA2C
};
int TextColors[4] = {
	0xFF4E0052,
	0xFF005597,
	0xFFFFb24A,
	0xFFFF554A,
};

bool initialized = false;
bool noChao;
#pragma endregion
#pragma region functions
void initD3D()
{
	d3d = Direct3DCreate9(32);

	HWND hWnd = find_main_window(GetCurrentProcessId());

	RECT rect;
	GetWindowRect(hWnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	width -= 16;
	height -= 39;
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = 1;
	d3dpp.BackBufferHeight = 1;
	d3dpp.hDeviceWindow = hWnd;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, nullptr, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	DirectXFunctions = (void**)(*(void**)d3ddev);

	endSceneTableEntry = DirectXFunctions[D3D9Funcs_EndScene];
	resetTableEntry = DirectXFunctions[D3D9Funcs_Reset];
};

static const void *const PlaySoundPtr = (void*)0x00437260;
static inline void PlaySound(int a1, int a2, char a3, char a4)
{
	__asm
	{
		push [a4]
		push [a3]
		push [a2]
		mov esi, [a1]
		call PlaySoundPtr
	}
}

void MenuInput()
{
	if (MenuButtons_Pressed[0] & Buttons_Right)
	{
		PauseSelection++;
		PlaySound(0x8000, 0, 0, 0);
	}
	if (MenuButtons_Pressed[0] & Buttons_Left)
	{
		PauseSelection--;
		PlaySound(0x8000, 0, 0, 0);
	}
}

static const int returnToFunction = 0x0043C0B1;
static const int GoOn = 0x0043B11F;
__declspec(naked) void RedirectToChaoMenuInput()
{
	__asm
	{
		je continueL
		jmp returnToFunction
	continueL:
		jmp GoOn;

	}
}

void ActivateChaoMenu()
{
	if (IsNotPauseHide >= 0 && PauseMenuID < 3)
	{
		DisplayPauseMenu();
	}
	if (PauseMenuID == 3)
	{
		MenuInput();
	}
}
static const int Return1 = 0x0043D4A9;
__declspec(naked) void ActivateChaoMenu_Wrapper()
{
	__asm
	{
		call ActivateChaoMenu
		jmp Return1;
	}
}

void SelectChaoMenu()
{
	if (CurrentLevel == LevelIDs_ChaoWorld)
	{
		PauseMenuID = 3;
		PauseOptionCount = 4;
		PauseSelection = 0;
		if ((**MainCharObj2).HeldObject != NULL)
			if ((**MainCharObj2).HeldObject->MainSub == Chao_Main)
			{
				displayChao = (NChaoData*)(*(**MainCharObj2).HeldObject->Data1.Chao).ChaoDataBase_ptr;
				ChaoName = DecodeChaoName(displayChao->Name);
				#pragma region creating strings for menu
				ChaoDNA dna = displayChao->DNA;
				DNA_DATA[0] = EggColor[dna.EggColor1];
				DNA_DATA[1] = Color[dna.Color1];
				DNA_DATA[2] = dna.MonotoneFlag1 ? "Yes" : "No";
				DNA_DATA[3] = dna.ShinyFlag1 ? "Yes" : "No";
				DNA_DATA[4] = Texture[dna.Texture1];
				DNA_DATA[5] = Grade[dna.SwimStatGrade1];
				DNA_DATA[6] = Grade[dna.FlyStatGrade1];
				DNA_DATA[7] = Grade[dna.RunStatGrade1];
				DNA_DATA[8] = Grade[dna.PowerStatGrade1];
				DNA_DATA[9] = Grade[dna.StaminaStatGrade1];
				DNA_DATA[10] = std::to_string(dna.LuckStatGrade1);
				DNA_DATA[11] = std::to_string(dna.IntelligenceStatGrade1);
				DNA_DATA[12] = FavFruit[dna.FavoriteFruit1];

				DNA_DATA[13] = EggColor[dna.EggColor2];
				DNA_DATA[14] = Color[dna.Color2];
				DNA_DATA[15] = dna.MonotoneFlag2 ? "Yes" : "No";
				DNA_DATA[16] = dna.ShinyFlag2 ? "Yes" : "No";
				DNA_DATA[17] = Texture[dna.Texture2];
				DNA_DATA[18] = Grade[dna.SwimStatGrade2];
				DNA_DATA[19] = Grade[dna.FlyStatGrade2];
				DNA_DATA[20] = Grade[dna.RunStatGrade2];
				DNA_DATA[21] = Grade[dna.PowerStatGrade2];
				DNA_DATA[22] = Grade[dna.StaminaStatGrade2];
				DNA_DATA[23] = std::to_string(dna.LuckStatGrade2);
				DNA_DATA[24] = std::to_string(dna.IntelligenceStatGrade2);
				DNA_DATA[25] = FavFruit[dna.FavoriteFruit2];
				#pragma endregion
			}
			else displayChao = nullptr;
		else displayChao = nullptr;
	}
}
__declspec(naked) void SelectChaoMenuWrapper()
{
	__asm
	{
		call SelectChaoMenu
		jmp returnToFunction
	}
}
#pragma endregion

void ChaoMenu_Stats(IDirect3DDevice9 *device)
{
	RECT fontRect;
	SetRect(&fontRect, 72, 144, 1000, 1000);
	font->DrawTextW(NULL, ChaoName.c_str(), -1, &fontRect, DT_LEFT | DT_NOCLIP, 0xFF000000);
}
void ChaoMenu_Visual(IDirect3DDevice9 *device)
{

}
void ChaoMenu_Personality(IDirect3DDevice9 *device)
{

}
void ChaoMenu_DNA(IDirect3DDevice9 *device)
{
	int TextCol = 0xFFD95D00;
	Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	for (int i = 0; i < 9; i++)
	{
		Sprite->SetTransform(&DNA_LeftBox[i]);
		Sprite->Draw(MenuFillingTextures[i], &MenuBackgroundRect, NULL, NULL, 0xFFFDC501);
		Sprite->Draw(MenuBorderTextures[i], &MenuBackgroundRect, NULL, NULL, 0xFFFDAA01);
		Sprite->SetTransform(&DNA_RightBox[i]);
		Sprite->Draw(MenuFillingTextures[i], &MenuBackgroundRect, NULL, NULL, 0xFFFDC501);
		Sprite->Draw(MenuBorderTextures[i], &MenuBackgroundRect, NULL, NULL, 0xFFFDAA01);
	}
	Sprite->End();

	font->DrawTextA(NULL, "DNA 1", -1, &DNA_TextShadows[0], DT_CENTER | DT_NOCLIP, 0xFF000000);
	font->DrawTextA(NULL, "DNA 1", -1, &DNA_Texts[0], DT_CENTER | DT_NOCLIP, TextCol);
	font->DrawTextA(NULL, "DNA 2", -1, &DNA_TextShadows[27], DT_CENTER | DT_NOCLIP, 0xFF000000);
	font->DrawTextA(NULL, "DNA 2", -1, &DNA_Texts[27], DT_CENTER | DT_NOCLIP, TextCol);
	for (int i = 0; i < 13; i++)
	{
		font->DrawTextA(NULL, DNA_Descr[i].c_str(), -1, &DNA_TextShadows[i+1], DT_LEFT | DT_NOCLIP, 0xFF000000);
		font->DrawTextA(NULL, DNA_Descr[i].c_str(), -1, &DNA_Texts[i + 1], DT_LEFT | DT_NOCLIP, TextCol);
		font->DrawTextA(NULL, DNA_DATA[i].c_str(), -1, &DNA_TextShadows[i + 14], DT_CENTER | DT_NOCLIP, 0xFF000000);
		font->DrawTextA(NULL, DNA_DATA[i].c_str(), -1, &DNA_Texts[i + 14], DT_CENTER | DT_NOCLIP, TextCol);

		font->DrawTextA(NULL, DNA_Descr[i].c_str(), -1, &DNA_TextShadows[i + 28], DT_LEFT | DT_NOCLIP, 0xFF000000);
		font->DrawTextA(NULL, DNA_Descr[i].c_str(), -1, &DNA_Texts[i + 28], DT_LEFT | DT_NOCLIP, TextCol);
		font->DrawTextA(NULL, DNA_DATA[i + 13].c_str(), -1, &DNA_TextShadows[i + 41], DT_CENTER | DT_NOCLIP, 0xFF000000);
		font->DrawTextA(NULL, DNA_DATA[i + 13].c_str(), -1, &DNA_Texts[i + 41], DT_CENTER | DT_NOCLIP, TextCol);
	}
}
HRESULT __stdcall DisplayChaoMenu(IDirect3DDevice9 *device)
{
	if (!initialized)
	{
		#pragma region general menu
		//initializing the font
		std::string loc;
		loc.assign(ResourcesPath);
		loc.append("Typo_DodamM.ttf");
		AddFontResourceExA(loc.c_str(), FR_PRIVATE, NULL);
		D3DXCreateFontA(device, height * 0.05, 0, FW_THIN, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Typo_dodam M", &font);
		
		//gettint the textures and initializing the sprite
		D3DXCreateSprite(device, &Sprite);
		SetRect(&MenuBackgroundRect, 0, 0, 32, 32);
		for (int i = 0; i < 9; i++)
		{
			loc.assign(ResourcesPath);
			std::string t = "";
			switch (i)
			{
			case 0:
				t = "Corner_TL_Filling.png";
				break;
			case 1:
				t = "Edge_T_Filling.png";
				break;
			case 2:
				t = "Corner_TR_Filling.png";
				break;
			case 3:
				t = "Edge_L_Filling.png";
				break;
			case 4:
				t = "Body.png";
				break;
			case 5:
				t = "Edge_R_Filling.png";
				break;
			case 6:
				t = "Corner_BL_Filling.png";
				break;
			case 7:
				t = "Edge_B_Filling.png";
				break;
			case 8:
				t = "Corner_BR_Filling.png";
				break;
			}
			loc.append(t);
			D3DXCreateTextureFromFileA(device, loc.c_str(), &MenuFillingTextures[i]);
		}
		for (int i = 0; i < 9; i++)
		{
			loc.assign(ResourcesPath);
			std::string t = "";
			switch (i)
			{
			case 0:
				t = "Corner_TL_Border.png";
				break;
			case 1:
				t = "Edge_T_Border.png";
				break;
			case 2:
				t = "Corner_TR_Border.png";
				break;
			case 3:
				t = "Edge_L_Border.png";
				break;
			case 4:
				t = "Empty.png";
				break;
			case 5:
				t = "Edge_R_Border.png";
				break;
			case 6:
				t = "Corner_BL_Border.png";
				break;
			case 7:
				t = "Edge_B_Border.png";
				break;
			case 8:
				t = "Corner_BR_Border.png";
				break;
			}
			loc.append(t);
			D3DXCreateTextureFromFileA(device, loc.c_str(), &MenuBorderTextures[i]);
		}

		#pragma region setting backgroundtransforms
		D3DXMatrixTransformation(&MenuBackgroundTransforms[0], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3(40, 104, 0));
		D3DXMatrixTransformation(&MenuBackgroundTransforms[2], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3(width - 72, 104, 0));
		D3DXMatrixTransformation(&MenuBackgroundTransforms[6], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3(40, height - 72, 0));
		D3DXMatrixTransformation(&MenuBackgroundTransforms[8], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3(width - 72, height - 72, 0));

		float widthTA = (width - 144) / 32.0;
		float heightTA = (height - 208) / 32.0;

		D3DXMatrixTransformation(&MenuBackgroundTransforms[1], NULL, NULL, new D3DXVECTOR3(widthTA, 1, 0), NULL, NULL, new D3DXVECTOR3(72, 104, 0));
		D3DXMatrixTransformation(&MenuBackgroundTransforms[3], NULL, NULL, new D3DXVECTOR3(1, heightTA, 0), NULL, NULL, new D3DXVECTOR3(40, 136, 0));
		D3DXMatrixTransformation(&MenuBackgroundTransforms[5], NULL, NULL, new D3DXVECTOR3(1, heightTA, 0), NULL, NULL, new D3DXVECTOR3(width- 72, 136, 0));
		D3DXMatrixTransformation(&MenuBackgroundTransforms[7], NULL, NULL, new D3DXVECTOR3(widthTA, 1, 0), NULL, NULL, new D3DXVECTOR3(72, height - 72, 0));

		D3DXMatrixTransformation(&MenuBackgroundTransforms[4], NULL, NULL, new D3DXVECTOR3(widthTA, heightTA, 0), NULL, NULL, new D3DXVECTOR3(72, 136, 0));
		#pragma endregion

		widthTA = (width - 80) / 4;
		float ScaleWidthTA = (widthTA - 64) / 32;

		//tab positions
		for (int i = 0; i < 4; i++)
		{
			int it = i * 6;
			D3DXMatrixTransformation(&MenuTabTransforms[it + 0], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3((widthTA * i) + 40, 40, 0));
			D3DXMatrixTransformation(&MenuTabTransforms[it + 1], NULL, NULL, new D3DXVECTOR3(ScaleWidthTA, 1, 0), NULL, NULL, new D3DXVECTOR3((widthTA * i) + 72, 40, 0));
			D3DXMatrixTransformation(&MenuTabTransforms[it + 2], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3(widthTA * (i + 1) + 8, 40, 0));
			D3DXMatrixTransformation(&MenuTabTransforms[it + 3], NULL, NULL, new D3DXVECTOR3(1, 2, 0), NULL, NULL, new D3DXVECTOR3((widthTA * i) + 40, 72, 0));
			D3DXMatrixTransformation(&MenuTabTransforms[it + 4], NULL, NULL, new D3DXVECTOR3(ScaleWidthTA, 2, 0), NULL, NULL, new D3DXVECTOR3((widthTA * i) + 72, 72, 0));
			D3DXMatrixTransformation(&MenuTabTransforms[it + 5], NULL, NULL, new D3DXVECTOR3(1, 2, 0), NULL, NULL, new D3DXVECTOR3(widthTA * (i + 1) + 8, 72, 0));
		}

		//tab text positions
		for (int i = 0; i < 4; i++)
		{
			SetRect(&TabTextRects[i], (widthTA * i) + 72, 58, (widthTA * (i + 1)), 104);
			SetRect(&TabTextRects[i+4], (widthTA * i) + 74, 60, (widthTA * (i + 1)) + 2, 106);
		}

		#pragma region setting invalid menu transforms
		D3DXMatrixTransformation(&InvalidMenuTransforms[0], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3(width / 2 - 224, height / 2 - 160, 0));
		D3DXMatrixTransformation(&InvalidMenuTransforms[2], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3(width / 2 + 192, height / 2 - 160, 0));
		D3DXMatrixTransformation(&InvalidMenuTransforms[6], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3(width / 2 - 224, height / 2 + 128, 0));
		D3DXMatrixTransformation(&InvalidMenuTransforms[8], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL, new D3DXVECTOR3(width / 2 + 192, height / 2 + 128, 0));

		D3DXMatrixTransformation(&InvalidMenuTransforms[1], new D3DXVECTOR3(16, 16, 0), NULL, new D3DXVECTOR3(12, 1, 0), NULL, NULL, new D3DXVECTOR3(width / 2 - 16, height / 2 - 160, 0));
		D3DXMatrixTransformation(&InvalidMenuTransforms[3], new D3DXVECTOR3(16, 16, 0), NULL, new D3DXVECTOR3(1 , 8, 0), NULL, NULL, new D3DXVECTOR3(width / 2 - 224, height / 2 - 16, 0));
		D3DXMatrixTransformation(&InvalidMenuTransforms[5], new D3DXVECTOR3(16, 16, 0), NULL, new D3DXVECTOR3(1 , 8, 0), NULL, NULL, new D3DXVECTOR3(width / 2 + 192, height / 2 - 16, 0));
		D3DXMatrixTransformation(&InvalidMenuTransforms[7], new D3DXVECTOR3(16, 16, 0), NULL, new D3DXVECTOR3(12, 1, 0), NULL, NULL, new D3DXVECTOR3(width / 2 - 16, height / 2 + 128, 0));

		D3DXMatrixTransformation(&InvalidMenuTransforms[4], new D3DXVECTOR3(16, 16, 0), NULL, new D3DXVECTOR3(12, 8, 0), NULL, NULL, new D3DXVECTOR3(width / 2 - 16, height / 2 - 16, 0));

		SetRect(&invalidTextRect[0], width / 2 - 200, height / 2 - 50, width / 2 + 200, height / 2 + 50);
		SetRect(&invalidTextRect[1], width / 2 - 196, height / 2 - 46, width / 2 + 204, height / 2 + 54);
		#pragma endregion
		#pragma endregion

		#pragma region DNA Menu
			float BoxWidth = ((width - 144) * 0.47);
			float BoxHeight = (height - 288);
			int Xpos = 72;
			int Xpos2 = width - 104 - BoxWidth;;
			int Ypos = 184;

			D3DXMatrixTransformation(&DNA_LeftBox[0], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL,						new D3DXVECTOR3(Xpos,				Ypos, 0));
			D3DXMatrixTransformation(&DNA_LeftBox[2], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL,						new D3DXVECTOR3(Xpos + BoxWidth,	Ypos, 0));
			D3DXMatrixTransformation(&DNA_LeftBox[6], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL,						new D3DXVECTOR3(Xpos,				Ypos + BoxHeight, 0));
			D3DXMatrixTransformation(&DNA_LeftBox[8], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL,						new D3DXVECTOR3(Xpos + BoxWidth,	Ypos + BoxHeight, 0));

			D3DXMatrixTransformation(&DNA_LeftBox[1], NULL, NULL, new D3DXVECTOR3(BoxWidth / 32 - 1, 1, 0),	NULL, NULL,		new D3DXVECTOR3(Xpos + 32,			Ypos, 0));
			D3DXMatrixTransformation(&DNA_LeftBox[3], NULL, NULL, new D3DXVECTOR3(1, BoxHeight / 32 - 1, 0), NULL, NULL,	new D3DXVECTOR3(Xpos,				Ypos + 32, 0));
			D3DXMatrixTransformation(&DNA_LeftBox[5], NULL, NULL, new D3DXVECTOR3(1, BoxHeight / 32 - 1, 0), NULL, NULL,	new D3DXVECTOR3(Xpos + BoxWidth,	Ypos + 32, 0));
			D3DXMatrixTransformation(&DNA_LeftBox[7], NULL, NULL, new D3DXVECTOR3(BoxWidth / 32 - 1, 1, 0), NULL, NULL,		new D3DXVECTOR3(Xpos + 32,			Ypos + BoxHeight, 0));

			D3DXMatrixTransformation(&DNA_LeftBox[4], NULL, NULL, new D3DXVECTOR3(BoxWidth / 32 -1, BoxHeight / 32 -1, 0), NULL, NULL, new D3DXVECTOR3(Xpos + 32,		Ypos + 32, 0));


			D3DXMatrixTransformation(&DNA_RightBox[0], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL,						new D3DXVECTOR3(Xpos2,				Ypos, 0));
			D3DXMatrixTransformation(&DNA_RightBox[2], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL,						new D3DXVECTOR3(Xpos2 + BoxWidth,	Ypos, 0));
			D3DXMatrixTransformation(&DNA_RightBox[6], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL,						new D3DXVECTOR3(Xpos2,				Ypos + BoxHeight, 0));
			D3DXMatrixTransformation(&DNA_RightBox[8], NULL, NULL, new D3DXVECTOR3(1, 1, 0), NULL, NULL,						new D3DXVECTOR3(Xpos2 + BoxWidth,	Ypos + BoxHeight, 0));

			D3DXMatrixTransformation(&DNA_RightBox[1], NULL, NULL, new D3DXVECTOR3(BoxWidth / 32 - 1, 1, 0),NULL, NULL,			new D3DXVECTOR3(Xpos2 + 32,			Ypos, 0));
			D3DXMatrixTransformation(&DNA_RightBox[3], NULL, NULL, new D3DXVECTOR3(1, BoxHeight / 32 - 1, 0), NULL, NULL,		new D3DXVECTOR3(Xpos2,				Ypos + 32, 0));
			D3DXMatrixTransformation(&DNA_RightBox[5], NULL, NULL, new D3DXVECTOR3(1, BoxHeight / 32 - 1, 0), NULL, NULL,		new D3DXVECTOR3(Xpos2 + BoxWidth,	Ypos + 32, 0));
			D3DXMatrixTransformation(&DNA_RightBox[7], NULL, NULL, new D3DXVECTOR3(BoxWidth / 32 - 1, 1, 0), NULL, NULL,		new D3DXVECTOR3(Xpos2 + 32,			Ypos + BoxHeight, 0));

			D3DXMatrixTransformation(&DNA_RightBox[4], NULL, NULL, new D3DXVECTOR3(BoxWidth / 32 -1, BoxHeight / 32 -1, 0), NULL, NULL, new D3DXVECTOR3(Xpos2 + 32,		Ypos + 32, 0));

			int LineHeight = BoxHeight / 13;
			SetRect(&DNA_Texts[0], Xpos, 136, Xpos + BoxWidth, 168);
			SetRect(&DNA_Texts[27], Xpos2, 136, Xpos2 + BoxWidth, 168);
			for (int i = 0; i < 13; i++)
			{
				int j = LineHeight * i;
				SetRect(&DNA_Texts[i + 1], Xpos + 20, 208 + j, Xpos + 20 + BoxWidth * 0.4, 208 + j + LineHeight);
				SetRect(&DNA_Texts[i + 14], Xpos + 20 + BoxWidth * 0.4, 208 + j, Xpos + 20 + BoxWidth, 208 + j + LineHeight);

				SetRect(&DNA_Texts[i + 28], Xpos2 + 20, 208 + j, Xpos2 + 20 + BoxWidth * 0.4, 208 + j + LineHeight);
				SetRect(&DNA_Texts[i + 41], Xpos2 + 20 + BoxWidth * 0.4, 208 + j, Xpos2 + 20 + BoxWidth, 208 + j + LineHeight);
			}
			for (int i = 0; i < 54; i++)
			{
				SetRect(&DNA_TextShadows[i], DNA_Texts[i].left + 2, DNA_Texts[i].top + 2, DNA_Texts[i].right + 2, DNA_Texts[i].bottom + 2);
			}

		#pragma endregion

		initialized = true;
	}

	//ChaoMenu_DNA(device);

	if (PauseMenuID == 3)
	{
		int n = 0;
		int c = 0;

		if (displayChao == nullptr) goto NoChao;

		Sprite->Begin(D3DXSPRITE_ALPHABLEND);
		for (int i = 0; i < 24; i++)
		{
			Sprite->SetTransform(&MenuTabTransforms[i]);
			Sprite->Draw(MenuFillingTextures[n], &MenuBackgroundRect, NULL, NULL, Colors[c]);
			Sprite->Draw(MenuBorderTextures[n], &MenuBackgroundRect, NULL, NULL, Colors[c + 4]);
			if (n < 5) n++;
			else
			{
				n = 0;
				c++;
			}
		}
		for (int i = 0; i < 9; i++)
		{
			Sprite->SetTransform(&MenuBackgroundTransforms[i]);
			Sprite->Draw(MenuFillingTextures[i], &MenuBackgroundRect, NULL, NULL, Colors[PauseSelection] + 0xAF000000);
			Sprite->Draw(MenuBorderTextures[i], &MenuBackgroundRect, NULL, NULL, Colors[PauseSelection + 4]);
		}
		Sprite->End();
		for (int i = 0; i < 4; i++)
		{
			font->DrawTextA(NULL, TabTextStrings[i].c_str(), -1, &TabTextRects[i+4], DT_CENTER | DT_NOCLIP, 0xFF000000);
			font->DrawTextA(NULL, TabTextStrings[i].c_str(), -1, &TabTextRects[i], DT_CENTER | DT_NOCLIP, 0xFFFFFFFF);
		}

		switch (PauseSelection)
		{
		case 0:
			ChaoMenu_Stats(device);
			goto finish;
		case 1:
			ChaoMenu_Visual(device);
			goto finish;
		case 2:
			ChaoMenu_Personality(device);
			goto finish;
		case 3:
			ChaoMenu_DNA(device);
			goto finish;
		default:
			goto finish;
		}

		#pragma region no chao menu
	NoChao:

		Sprite->Begin(D3DXSPRITE_ALPHABLEND);
		for (int i = 0; i < 9; i++)
		{
			Sprite->SetTransform(&InvalidMenuTransforms[i]);
			Sprite->Draw(MenuFillingTextures[i], &MenuBackgroundRect, NULL, NULL, 0x503963CE);
			Sprite->Draw(MenuBorderTextures[i], &MenuBackgroundRect, NULL, NULL, 0xFF4E97FF);
		}
		Sprite->End();

		font->DrawTextW(NULL, L"Please hold a Chao \n in your hands", -1, &invalidTextRect[1], DT_CENTER, 0xFF000000);
		font->DrawTextW(NULL, L"Please hold a Chao \n in your hands", -1, &invalidTextRect[0], DT_CENTER, 0xFFFFFFFF);
		#pragma endregion
	}
finish:
	auto orig = (decltype(DisplayChaoMenu)*)RenderFunc->Target();
	return orig(device);
}

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		char buf[256];
		_getcwd(buf, 256);
		ResourcesPath = buf;
		ResourcesPath.append("\\");
		ResourcesPath.append(path);
		ResourcesPath.append("\\Resources\\");

		initD3D();
		RenderFunc = new Trampoline((int)endSceneTableEntry, ((int)endSceneTableEntry) + 7, &DisplayChaoMenu);

		WriteData((int*)0x0174A830, (int)&ChaoMenuString);
		WriteJump((int*)0x0043B119, RedirectToChaoMenuInput);
		WriteJump((int*)0x0043B1EE, SelectChaoMenuWrapper);
		WriteData((char*)0x0043B037, (char)0x8C);
		WriteJump((int*)0x0043D49B, ActivateChaoMenu_Wrapper);
	}
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}

