#include "stdafx.h"
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include "SA2ModLoader.h"
#include "ChaoData.h"
#include "Trampoline.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "magic.h"

#pragma region Variable declaration
	#pragma region Function Variables
	char ChaoMenuString[] = { 07, 'C', 'H', 'A', 'O', ' ', 'D', 'A', 'T', 'A', 0 };
	DataPointer(int, PauseMenuID, 0x017472BC);
	FunctionPointer(void, DisplayPauseMenu, (), 0x00440AD0);
	NChaoData *displayChao;


	std::string ResourcesPath;
	Trampoline *RenderFunc;
	Trampoline *ResetFunc;
	#pragma endregion

	#pragma region general render variables

	const float& width = HorizontalResolution;
	const float& height = VerticalResolution;

	const D3DCOLOR Colors[8] = {
		0x50FF8080,
		0x5080FF80,
		0x508080FF,
		0x50FFFF80,

		0xFFDD2323,
		0xFF19B75E,
		0xFF2975D8,
		0xFFFFCB3D,
	};

	ID3DXFont *font = nullptr;
	LPD3DXSPRITE Sprite;
	LPD3DXLINE Line;

	RECT MenuBackgroundRect;
	RECT GradientRects[5];
	LPDIRECT3DTEXTURE9 MenuFillingTextures[9];
	LPDIRECT3DTEXTURE9 MenuBorderTextures[9];
	LPDIRECT3DTEXTURE9 Gradients[4];
	#pragma endregion

	#pragma region menu base
	D3DXMATRIX MenuBackgroundTransforms[9];
	D3DXMATRIX MenuTabTransforms[24];
	D3DXMATRIX InvalidMenuTransforms[9];

	RECT invalidTextRect[2];
	RECT TabTextRects[8];

	const std::string TabTextStrings[4] = {
		"Stats",
		"Visual",
		"Behaviour",
		"DNA"
	};
	#pragma endregion

	#pragma region Stat menu
	RECT Stat_Texts[15];
	RECT Stat_TextShadows[15];
	RECT Stat_StatTexts[28];
	RECT Stat_StatTextShadows[28];

	const std::string Stat_Descr[17] = {
		"Type:",
		"Garden:",
		"Age:",
		"Time remaining:",
		"Reincarnations:",
		"Swim/Fly:",
		"Run/Power:",
		"Alignment:",
		"Magnitude:",
		"Swim:",
		"Fly:",
		"Run:",
		"Power:",
		"Stamina:",
		"Luck:",
		"Intel.:",
		"Lv.:"
	};

	std::string Stat_Strings[26];
	std::wstring ChaoName;
	D3DXVECTOR2 LineStarts[4];
	float BarWidth;

	D3DXMATRIX Stat_NameBG[6];
	D3DXMATRIX Stat_Gradients[10];
	float ProgressValues[4];
	#pragma endregion

	#pragma region visual menu
	RECT Vis_Texts[41];
	RECT Vis_TextShadows[41];

	const std::string Vis_Descr[21] = {
		"EggColor:",
		"Color:",
		"Monotone:",
		"Shiny:",
		"Texture:",
		"Eyes:",
		"Mouth:",
		"Emot. Ball:",
		"Hat:",
		"Medal:",
		"BodyType:",
		"Animal parts",
		"Body:",
		"Arms:",
		"Ears:",
		"Forehead:",
		"Horns:",
		"Legs:",
		"Tail:",
		"Wings:",
		"Face:"
	};

	std::string Vis_Strings[21];
	#pragma endregion

	#pragma region behaviour menu
	bool behavMenu2;

	const std::string behav_descr1[19] = {
		"Bonds",
		"Sonic:",
		"Tails:",
		"Knuckles:",
		"Shadow:",
		"Eggman:",
		"Rouge:",
		"Personality",
		"Normal",
		"Curious",
		"Crybaby",
		"Energetic",
		"Naive",
		"Normal",
		"Normal",
		"Bigeater",
		"Normal",
		"CareFree",
		"Favourite Fruit:",
	};

	const std::string behav_descr2[19] = {
		"Emotions",
		"Joy:",
		"Energy:",
		"Hunger:",
		"Boredom:",
		"Urge to cry:",
		"Fear:",
		"Dizziness:",
		"Tiredness:",
		"Sleepiness:",
		"Mating desire:",
		"Health",
		"Coughing",
		"Cold",
		"Rash",
		"Runny Nose",
		"Hiccups",
		"Stomache ache",
		"Healthy"
	};

	float behav_values[27];
	RECT behav_LeftBox1[13];
	RECT behav_LeftBoxShadows1[13];
	RECT behav_LeftBox2[11];
	RECT behav_LeftBoxShadows2[11];
	RECT behav_RightBox[7];
	RECT behav_RightBoxShadows[7];
	RECT behav_Info[5];

	float behav_BarWidth[4];
	D3DXVECTOR2 behav_lineStarts1[12];
	D3DXMATRIX behav_Bars1[12];
	D3DXVECTOR2 behav_lineStarts2[16];
	D3DXMATRIX behav_Bars2[16];
	#pragma endregion

	#pragma region dna menu
	D3DXMATRIX DNA_LeftBox[9];
	D3DXMATRIX DNA_RightBox[9];
	RECT DNA_Texts[54];
	RECT DNA_TextShadows[54];

	const std::string DNA_Descr[13] = {
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
	#pragma endregion
#pragma endregion

#pragma region functions
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

static const void *const PlaySoundPtr = (void*)0x00437260;
static inline void PlaySound(int a1, int a2, char a3, char a4)
{
	__asm
	{
		movzx eax, [a4]
		push eax
		movzx eax, [a3]
		push eax
		push [a2]
		mov esi, [a1]
		call PlaySoundPtr
	}
}

void ActivateChaoMenu()
{
	if (IsNotPauseHide >= 0 && PauseMenuID != 3)
	{
		DisplayPauseMenu();
	}
	if (PauseMenuID == 3)
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
		if(PauseSelection == 2)
			if (MenuButtons_Pressed[0] & Buttons_X)
			{
				behavMenu2 = !behavMenu2;
				PlaySound(0x8000, 0, 0, 0);
			}
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

float map(float value, float istart, float istop, float ostart, float ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
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
				#pragma region stat strings
				Stat_Strings[0] = Type[displayChao->Type];
				if ((unsigned char)displayChao->Garden != 255)
					Stat_Strings[1] = displayChao->Garden < 4 ? Garden[displayChao->Garden] : "???";
				else Stat_Strings[1] = "None";

				Stat_Strings[2] = std::to_string((float)displayChao->ClockRollovers / 60.0f); // Age
				
				// Limit to 1 digit after decimal point
				auto it = Stat_Strings[2].find('.');
				if (it != std::string::npos)
				{
					Stat_Strings[2].resize(it + 2);
				}

				Stat_Strings[3] = std::to_string(displayChao->Lifespan2);
				Stat_Strings[4] = std::to_string(displayChao->Reincarnations);

				Stat_Strings[5] = displayChao->SwimGrade < 6? Grade[displayChao->SwimGrade] : "?";
				Stat_Strings[6] = displayChao->FlyGrade < 6 ? Grade[displayChao->FlyGrade] : "?";
				Stat_Strings[7] = displayChao->RunGrade < 6 ? Grade[displayChao->RunGrade] : "?";
				Stat_Strings[8] = displayChao->PowerGrade < 6 ? Grade[displayChao->PowerGrade] : "?";
				Stat_Strings[9] = displayChao->StaminaGrade < 6 ? Grade[displayChao->StaminaGrade] : "?";
				Stat_Strings[10] = std::to_string((unsigned char)displayChao->LuckGrade);
				Stat_Strings[11] = std::to_string((unsigned char)displayChao->IntelligenceGrade);

				int values[21];

				values[0] = displayChao->SwimLevel;
				values[1] = displayChao->SwimFraction;
				values[2] = (unsigned short)displayChao->SwimStat;
				values[3] = displayChao->FlyLevel;
				values[4] = displayChao->FlyFraction;
				values[5] = (unsigned short)displayChao->FlyStat;
				values[6] = displayChao->RunLevel;
				values[7] = displayChao->RunFraction;
				values[8] = (unsigned short)displayChao->RunStat;
				values[9] = displayChao->PowerLevel;
				values[10] = displayChao->PowerFraction;
				values[11] = (unsigned short)displayChao->PowerStat;
				values[12] = displayChao->StaminaLevel;
				values[13] = displayChao->StaminaFraction;
				values[14] = (unsigned short)displayChao->StaminaStat;
				values[15] = displayChao->LuckLevel;
				values[16] = displayChao->LuckFraction;
				values[17] = (unsigned short)displayChao->LuckStat;
				values[18] = displayChao->IntelligenceLevel;
				values[19] = displayChao->IntelligenceFraction;
				values[20] = (unsigned short)displayChao->IntelligenceStat;

				for (int i = 0; i < 7; i++)
				{
					Stat_Strings[i+12] = "Lv.: ";
					Stat_Strings[i+12] += std::to_string(values[i*3]);
					if(Stat_Strings[i+12].length() == 6) Stat_Strings[11] += ' ';
					Stat_Strings[i+12] += " + ";
					Stat_Strings[i+12] += std::to_string(values[i * 3 + 1]);
					Stat_Strings[i+12] += "%";
				}

				Stat_Strings[19] = SetZero(values[2], 4);
				Stat_Strings[20] = SetZero(values[5], 4);
				Stat_Strings[21] = SetZero(values[8], 4);
				Stat_Strings[22] = SetZero(values[11], 4);
				Stat_Strings[23] = SetZero(values[14], 4);
				Stat_Strings[24] = SetZero(values[17], 4);
				Stat_Strings[25] = SetZero(values[20], 4);

				ProgressValues[0] = displayChao->FlySwim;
				ProgressValues[1] = displayChao->PowerRun;
				ProgressValues[2] = displayChao->Alignment;
				ProgressValues[3] = displayChao->EvolutionProgress;
				#pragma endregion
				#pragma region behaviour values
				for (int i = 0; i < 3; i++)
				{
					behav_values[i] = map((float)(signed)displayChao->SA2BCharacterBonds[i*2].a, -100, 100, 0, 1);
					behav_values[i+3] = map((float)(signed)displayChao->SA2BCharacterBonds[i*2+1].a, -100, 100, 0, 1);
				}
				behav_values[6] = map((float)(signed)displayChao->Normal_Curiosity, -100, 100, 0, 1);
				behav_values[7] = map((float)(signed)displayChao->Crybaby_energetic, -100, 100, 0, 1);
				behav_values[8] = map((float)(signed)displayChao->Naive_Normal, -100, 100, 0, 1);
				behav_values[9] = map((float)(signed)displayChao->glutton, -100, 100, 0, 1);
				behav_values[10] = map((float)(signed)displayChao->carefree, -100, 100, 0, 1);
				behav_values[11] = map((float)(signed)displayChao->Joy, 0, 200, 0, 1);
				behav_values[12] = map((float)(signed)displayChao->energy, 0, 10000, 0, 1);
				behav_values[13] = map((float)(signed)displayChao->hunger, 0, 10000, 0, 1);
				behav_values[14] = map((float)(signed)displayChao->boredom, 0, 10000, 0, 1);
				behav_values[15] = map((float)(signed)displayChao->Urgetocry, 0, 200, 0, 1);
				behav_values[16] = map((float)(signed)displayChao->fear, 0, 200, 0, 1);
				behav_values[17] = map((float)(signed)displayChao->dizziness, 0, 200, 0, 1);
				behav_values[18] = map((float)(signed)displayChao->slpdpth, 0, 10000, 0, 1);
				behav_values[19] = map((float)(signed)displayChao->sleepiness, 0, 10000, 0, 1);
				behav_values[20] = map((float)(signed)displayChao->breed, 0, 10000, 0, 1);
				behav_values[21] = map((float)(signed)displayChao->CoughLevel, -100, 100, 1, 0);
				behav_values[22] = map((float)(signed)displayChao->ColdLevel, -100, 100, 1, 0);
				behav_values[23] = map((float)(signed)displayChao->RashLevel, -100, 100, 1, 0);
				behav_values[24] = map((float)(signed)displayChao->RunnyNoseLevel, -100, 100, 1, 0);
				behav_values[25] = map((float)(signed)displayChao->HiccupsLevel, -100, 100, 1, 0);
				behav_values[26] = map((float)(signed)displayChao->StomachAcheLevel, -100, 100, 1, 0);
				#pragma endregion

				
				#pragma region DNA strings
				ChaoDNA dna = displayChao->DNA;
				DNA_DATA[0] = (unsigned char)dna.EggColor1 < 55 ? EggColor[dna.EggColor1] : std::to_string((unsigned char)dna.EggColor1);
				DNA_DATA[1] = (unsigned char)dna.Color1 < 14 ? Color[dna.Color1] : std::to_string((unsigned char)dna.Color1);
				DNA_DATA[2] = dna.MonotoneFlag1 ? "Yes" : "No";
				DNA_DATA[3] = dna.ShinyFlag1 ? "Yes" : "No";
				DNA_DATA[4] = (unsigned char)dna.Texture1 < 17 ? Texture[dna.Texture1] : std::to_string((unsigned char)dna.Texture1);
				DNA_DATA[5] = dna.SwimStatGrade1 < 6 ? Grade[dna.SwimStatGrade1] : "???";
				DNA_DATA[6] = dna.FlyStatGrade1 < 6 ? Grade[dna.FlyStatGrade1] : "???";
				DNA_DATA[7] = dna.RunStatGrade1 < 6 ? Grade[dna.RunStatGrade1] : "???";
				DNA_DATA[8] = dna.PowerStatGrade1 < 6 ? Grade[dna.PowerStatGrade1] : "???";
				DNA_DATA[9] = dna.StaminaStatGrade1 < 6 ? Grade[dna.StaminaStatGrade1] : "???";
				DNA_DATA[10] = std::to_string((unsigned char)dna.LuckStatGrade1);
				DNA_DATA[11] = std::to_string((unsigned char)dna.IntelligenceStatGrade1);
				DNA_DATA[12] = dna.FavoriteFruit1 < 8 ? FavFruit[dna.FavoriteFruit1] : "???";

				DNA_DATA[13] = (unsigned char)dna.EggColor2 < 55 ? EggColor[dna.EggColor2] : std::to_string((unsigned char)dna.EggColor2);
				DNA_DATA[14] = (unsigned char)dna.Color2 < 14 ? Color[dna.Color2] : std::to_string((unsigned char)dna.Color2);
				DNA_DATA[15] = dna.MonotoneFlag2 ? "Yes" : "No";
				DNA_DATA[16] = dna.ShinyFlag2 ? "Yes" : "No";
				DNA_DATA[17] = (unsigned char)dna.Texture2 < 17 ? Texture[dna.Texture2] : std::to_string((unsigned char)dna.Texture2);
				DNA_DATA[18] = dna.SwimStatGrade2 < 6 ? Grade[dna.SwimStatGrade2] : "???";
				DNA_DATA[19] = dna.FlyStatGrade2 < 6 ? Grade[dna.FlyStatGrade2] : "???";
				DNA_DATA[20] = dna.RunStatGrade2 < 6 ? Grade[dna.RunStatGrade2] : "???";
				DNA_DATA[21] = dna.PowerStatGrade2 < 6 ? Grade[dna.PowerStatGrade2] : "???";
				DNA_DATA[22] = dna.StaminaStatGrade2 < 6 ? Grade[dna.StaminaStatGrade2] : "???";
				DNA_DATA[23] = std::to_string((unsigned char)dna.LuckStatGrade2);
				DNA_DATA[24] = std::to_string((unsigned char)dna.IntelligenceStatGrade2);
				DNA_DATA[25] = dna.FavoriteFruit2 < 8 ? FavFruit[dna.FavoriteFruit2] : "???";
				#pragma endregion		
				

				#pragma region Vis Strings
				
				Vis_Strings[0] = (unsigned char)displayChao->EggColor < 55 ? EggColor[displayChao->EggColor] : std::to_string((unsigned char)displayChao->EggColor);
				Vis_Strings[1] = (unsigned char)displayChao->Color < 14 ? Color[displayChao->Color] : std::to_string((unsigned char)displayChao->Color);
				Vis_Strings[2] = displayChao->MonotoneHighlights ? "Yes" : "No";
				Vis_Strings[3] = displayChao->Shiny ? "Yes" : "No";
				Vis_Strings[4] = (unsigned char)displayChao->Texture < 17 ? Texture[displayChao->Texture] : std::to_string((unsigned char)displayChao->Texture);
				Vis_Strings[5] = (unsigned char)displayChao->EyeType < 14 ? Eyes[displayChao->EyeType] : std::to_string((unsigned char)displayChao->EyeType);
				Vis_Strings[6] = (unsigned char)displayChao->MouthType < 16 ? Mouth[displayChao->MouthType] : std::to_string((unsigned char)displayChao->MouthType);
				Vis_Strings[7] = (unsigned char)displayChao->BallType < 3 ? EmotionBall[displayChao->BallType] : std::to_string((unsigned char)displayChao->BallType);
				Vis_Strings[8] = (unsigned char)displayChao->Headgear < 71 ? Hat[displayChao->Headgear] : std::to_string((unsigned char)displayChao->Headgear);
				Vis_Strings[9] = (unsigned char)displayChao->Medal < 16 ? Medal[displayChao->Medal] : std::to_string((unsigned char)displayChao->Medal);

				Vis_Strings[10] = displayChao->BodyType < 6 ? BodyType[displayChao->BodyType] : "???";
				Vis_Strings[11] = displayChao->BodyType != 4 ? "None" : displayChao->BodyTypeAnimal == -1 ? "None" : Animal[displayChao->BodyTypeAnimal];
				Vis_Strings[12] = displayChao->SA2BArmType == -1 ? "None" : Animal[displayChao->SA2BArmType];
				Vis_Strings[13] = displayChao->SA2BEarType == -1 ? "None" : Animal[displayChao->SA2BEarType];
				Vis_Strings[14] = displayChao->SA2BForeheadType == -1 ? "None" : Animal[displayChao->SA2BForeheadType];
				Vis_Strings[15] = displayChao->SA2BHornType == -1 ? "None" : Animal[displayChao->SA2BHornType];
				Vis_Strings[16] = displayChao->HideFeet ? "Invisible" : displayChao->SA2BLegType == -1 ? "None" : Animal[displayChao->SA2BLegType];
				Vis_Strings[17] = displayChao->SA2BTailType == -1 ? "None" : Animal[displayChao->SA2BTailType];
				Vis_Strings[18] = displayChao->SA2BWingType == -1 ? "None" : Animal[displayChao->SA2BWingType];
				Vis_Strings[19] = displayChao->SA2BFaceType == -1 ? "None" : Animal[displayChao->SA2BFaceType];
				#pragma endregion 
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

void DrawRectangle(RECT rectangle, int Color)
{
	int rwidth = rectangle.right - rectangle.left;
	Line->Begin();
	for (int x = 0; x < rwidth; x++)
	{
		D3DXVECTOR2 vlist[2] = { D3DXVECTOR2((float)rectangle.left + x, (float)rectangle.top), D3DXVECTOR2((float)(rectangle.left + x), (float)rectangle.bottom) };
		Line->Draw(&vlist[0], 2, Color);
	}
	Line->End();
}

void ChaoMenu_Stats(LPDIRECT3DDEVICE9 device)
{
	//drawing the progress bar gradients
	Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	for (int i = 0; i < 3; i++)
	{
		Sprite->SetTransform(&Stat_NameBG[i]);
		Sprite->Draw(MenuFillingTextures[i], &MenuBackgroundRect, NULL, NULL, 0xFF2655ff);
		Sprite->SetTransform(&Stat_NameBG[i+3]);
		Sprite->Draw(MenuFillingTextures[i+6], &MenuBackgroundRect, NULL, NULL, 0xFF2655ff);
	}

	for (int i = 0; i < 2; i++)
	{
		if (ProgressValues[i] > 1)
		{
			Sprite->SetTransform(&Stat_Gradients[i * 4 + 2]);
			Sprite->Draw(Gradients[i], &GradientRects[0], NULL, NULL, 0xFFFFFFFF);
			Sprite->SetTransform(&Stat_Gradients[i * 4 + 1]);
			Sprite->Draw(Gradients[i], &GradientRects[3], NULL, NULL, 0xFFFFFFFF);
		}
		else if (ProgressValues[i] < -1)
		{
			Sprite->SetTransform(&Stat_Gradients[i * 4 + 3]);
			Sprite->Draw(Gradients[i], &GradientRects[2], NULL, NULL, 0xFFFFFFFF);
			Sprite->SetTransform(&Stat_Gradients[i*4]);
			Sprite->Draw(Gradients[i], &GradientRects[1], NULL, NULL, 0xFFFFFFFF);
		}
		else
		{
			Sprite->SetTransform(&Stat_Gradients[i * 4 + 2]);
			Sprite->Draw(Gradients[i], &GradientRects[0], NULL, NULL, 0xFFFFFFFF);
			Sprite->SetTransform(&Stat_Gradients[i * 4]);
			Sprite->Draw(Gradients[i], &GradientRects[1], NULL, NULL, 0xFFFFFFFF);
		}
	}

	Sprite->SetTransform(&Stat_Gradients[8]);
	Sprite->Draw(Gradients[2], &GradientRects[4], NULL, NULL, 0xFFFFFFFF);
	Sprite->SetTransform(&Stat_Gradients[9]);
	Sprite->Draw(Gradients[3], &GradientRects[4], NULL, NULL, 0xFFFFFFFF);
	Sprite->End();

	int textColor = 0xFF99002d;

	//drawing the bar indicator lines
	Line->Begin();

	for (int i = 0; i < 4; i++)
	{
		float progr;
		if(i == 3) progr = map(ProgressValues[i], 0.0f, 1.2f, 0.0f, 1.0f);
		else if (ProgressValues[i] < -1.0f || ProgressValues[i] > 1.0f) progr = map(ProgressValues[i], -2.0f, 2.0f, 0.0f, 1.0f);
		else progr = map(ProgressValues[i], -1.0f, 1.0f, 0.0f, 1.0f);


		D3DXVECTOR2 Pos[2];
		progr *= BarWidth;
		Pos[0] = D3DXVECTOR2(LineStarts[i].x + progr, LineStarts[i].y);
		Pos[1] = D3DXVECTOR2(Pos[0].x, LineStarts[i].y + 38);
		Line->Draw(&Pos[0], 2, 0xFF000000);
		Pos[0].x++;
		Pos[1].x++;
		Line->Draw(&Pos[0], 2, 0xFF000000);
		Pos[0].x -= 2;
		Pos[1].x -= 2;
		Line->Draw(&Pos[0], 2, 0xFF000000);
	}

	D3DXVECTOR2 pos[6] = {	D3DXVECTOR2(width / 2.0f - 20.0f, 200.0f),			D3DXVECTOR2(width / 2.0f - 20.0f, height - 64.0f),
							D3DXVECTOR2(width / 2.0f + width * 0.16f, 136.0f),  D3DXVECTOR2(width / 2.0f + width * 0.16f, height - 64.0f),
							D3DXVECTOR2(width / 2.0f + width * 0.35f, 136.0f),  D3DXVECTOR2(width / 2.0f + width * 0.35f, height - 64.0f), };
	
	for (int i = 0; i < 3; i++)
	{
		Line->Draw(&pos[i*2], 2, textColor);
		pos[i * 2].x--;
		pos[i * 2 + 1].x--;
		Line->Draw(&pos[i*2], 2, textColor);
	}

	Line->End();

	font->DrawTextW(NULL, ChaoName.c_str(), -1, &Stat_TextShadows[0], DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xFF000000);
	font->DrawTextW(NULL, ChaoName.c_str(), -1, &Stat_Texts[0], DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xFFFFFFFF);

	for (int i = 0; i < 9; i++)
	{
		font->DrawTextA(NULL, Stat_Descr[i].c_str(), -1, &Stat_TextShadows[i+1], DT_LEFT | DT_VCENTER | DT_NOCLIP, 0xFF000000);
		font->DrawTextA(NULL, Stat_Descr[i].c_str(), -1, &Stat_Texts[i+1], DT_LEFT | DT_VCENTER | DT_NOCLIP, textColor);
	}

	for (int i = 0; i < 5; i++)
	{
		font->DrawTextA(NULL, Stat_Strings[i].c_str(), -1, &Stat_TextShadows[i+10], DT_CENTER | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, Stat_Strings[i].c_str(), -1, &Stat_Texts[i+10], DT_CENTER | DT_VCENTER, textColor);
	}

	for (int i = 0; i < 7; i++)
	{
		font->DrawTextA(NULL, Stat_Descr[i + 9].c_str(), -1, &Stat_StatTextShadows[i], DT_LEFT | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, Stat_Descr[i + 9].c_str(), -1, &Stat_StatTexts[i], DT_LEFT | DT_VCENTER, textColor);

		font->DrawTextA(NULL, Stat_Strings[i + 5].c_str(), -1, &Stat_StatTextShadows[i +7], DT_LEFT | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, Stat_Strings[i + 5].c_str(), -1, &Stat_StatTexts[i +7], DT_LEFT | DT_VCENTER, textColor);

		font->DrawTextA(NULL, Stat_Strings[i + 12].c_str(), -1, &Stat_StatTextShadows[i+14], DT_LEFT | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, Stat_Strings[i + 12].c_str(), -1, &Stat_StatTexts[i+14], DT_LEFT | DT_VCENTER, textColor);

		font->DrawTextA(NULL, Stat_Strings[i + 19].c_str(), -1, &Stat_StatTextShadows[i + 21], DT_CENTER | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, Stat_Strings[i + 19].c_str(), -1, &Stat_StatTexts[i + 21], DT_CENTER | DT_VCENTER, textColor);
	}
}

void ChaoMenu_Visual(LPDIRECT3DDEVICE9 device)
{
	D3DXVECTOR2 pos[2] = { D3DXVECTOR2(width/2 -20, 136), D3DXVECTOR2(width / 2 - 20, height - 64) };
	Line->Begin();
	Line->Draw(&pos[0], 2, 0xFF008c2c);
	pos[0].x--;
	pos[1].x--;
	Line->Draw(&pos[0], 2, 0xFF008c2c);
	Line->End();

	for (int i = 0; i < 11; i++)
	{
		font->DrawTextA(NULL, Vis_Descr[i].c_str(), -1, &Vis_TextShadows[i], DT_LEFT | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, Vis_Descr[i].c_str(), -1, &Vis_Texts[i], DT_LEFT | DT_VCENTER, 0xFF008c2c);
	}

	font->DrawTextA(NULL, Vis_Descr[11].c_str(), -1, &Vis_TextShadows[11], DT_CENTER | DT_VCENTER, 0xFF000000);
	font->DrawTextA(NULL, Vis_Descr[11].c_str(), -1, &Vis_Texts[11], DT_CENTER | DT_VCENTER, 0xFF008c2c);

	for (int i = 0; i < 9; i++)
	{
		font->DrawTextA(NULL, Vis_Descr[i +12].c_str(), -1, &Vis_TextShadows[i+12], DT_LEFT | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, Vis_Descr[i +12].c_str(), -1, &Vis_Texts[i+12], DT_LEFT | DT_VCENTER, 0xFF008c2c);
	}

	for (int i = 0; i < 20; i++)
	{
		font->DrawTextA(NULL, Vis_Strings[i].c_str(), -1, &Vis_TextShadows[i + 21], DT_LEFT | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, Vis_Strings[i].c_str(), -1, &Vis_Texts[i + 21], DT_LEFT | DT_VCENTER, 0xFF008c2c);
	}
}

void ChaoMenu_Personality(LPDIRECT3DDEVICE9 device)
{
	D3DXVECTOR2 pos[2] = { D3DXVECTOR2(width / 2 - 20, 136), D3DXVECTOR2(width / 2 - 20, height - 64) };
	Line->Begin();
	Line->Draw(&pos[0], 2, 0xFF0040a8);
	pos[0].x--;
	pos[1].x--;
	Line->Draw(&pos[0], 2, 0xFF0040a8);
	Line->End();

	for (int i = 0; i < 4; i++)
	{
		font->DrawTextA(NULL, "Press X For Second Menu", -1, &behav_Info[i+1], DT_CENTER | DT_BOTTOM, 0xFF000000);
	}
	font->DrawTextA(NULL, "Press X For Second Menu", -1, &behav_Info[0], DT_CENTER | DT_BOTTOM, 0xFFFFFFFF);

	if (!behavMenu2) // bonds and personality
	{
		Sprite->Begin(D3DXSPRITE_ALPHABLEND);
		for (int i = 0; i < 6; i++)
		{
			Sprite->SetTransform(&behav_Bars1[i]);
			Sprite->Draw(Gradients[3], &GradientRects[4], NULL, NULL, 0xFFFFFFFF);

		}
		for (int i = 0; i < 5; i++)
		{
			Sprite->SetTransform(&behav_Bars1[i + 6]);
			Sprite->Draw(Gradients[3], &GradientRects[4], NULL, NULL, 0xFFFFFFFF);
		}
		Sprite->End();

		Line->Begin();
		for (int i = 0; i < 6; i++)
		{
			D3DXVECTOR2 pos[2];
			pos[0] = D3DXVECTOR2(behav_lineStarts1[i].x + behav_BarWidth[0] * behav_values[i], behav_lineStarts1[i].y);
			pos[1] = D3DXVECTOR2(pos[0].x, pos[0].y + 38.0f);
			Line->Draw(&pos[0], 2, 0xFF000000);
			pos[0].x--;
			pos[1].x--;
			Line->Draw(&pos[0], 2, 0xFF000000);
		}
		for (int i = 0; i < 5; i++)
		{
			D3DXVECTOR2 pos[2];
			pos[0] = D3DXVECTOR2(behav_lineStarts1[i + 6].x + behav_BarWidth[1] * behav_values[i + 6], behav_lineStarts1[i + 6].y);
			pos[1] = D3DXVECTOR2(pos[0].x, pos[0].y + 38.0f);
			Line->Draw(&pos[0], 2, 0xFF000000);
			pos[0].x--;
			pos[1].x--;
			Line->Draw(&pos[0], 2, 0xFF000000);
		}
		Line->End();

		font->DrawTextA(NULL, behav_descr1[0].c_str(), -1, &behav_LeftBoxShadows1[0], DT_CENTER | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, behav_descr1[0].c_str(), -1, &behav_LeftBox1[0], DT_CENTER | DT_VCENTER, 0xFF0040a8);
		font->DrawTextA(NULL, behav_descr1[7].c_str(), -1, &behav_RightBoxShadows[0], DT_CENTER | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, behav_descr1[7].c_str(), -1, &behav_RightBox[0], DT_CENTER | DT_VCENTER, 0xFF0040a8);

		font->DrawTextA(NULL, behav_descr1[18].c_str(), -1, &behav_RightBoxShadows[6], DT_LEFT | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, behav_descr1[18].c_str(), -1, &behav_RightBox[6], DT_LEFT | DT_VCENTER, 0xFF0040a8);
		font->DrawTextA(NULL, FavFruit[displayChao->FavoriteFruit].c_str(), -1, &behav_RightBoxShadows[6], DT_RIGHT | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, FavFruit[displayChao->FavoriteFruit].c_str(), -1, &behav_RightBox[6], DT_RIGHT | DT_VCENTER, 0xFF0040a8);
		
		for (int i = 1; i < 7; i++)
		{
			font->DrawTextA(NULL, behav_descr1[i].c_str(), -1, &behav_LeftBoxShadows1[i], DT_LEFT | DT_VCENTER, 0xFF000000);
			font->DrawTextA(NULL, behav_descr1[i].c_str(), -1, &behav_LeftBox1[i], DT_LEFT | DT_VCENTER, 0xFF0040a8);

			font->DrawTextA(NULL, "-", -1, &behav_LeftBoxShadows1[i+6], DT_LEFT | DT_VCENTER, 0xFF000000);
			font->DrawTextA(NULL, "-", -1, &behav_LeftBox1[i+6], DT_LEFT | DT_VCENTER, 0xFF0040a8);
			font->DrawTextA(NULL, "+", -1, &behav_LeftBoxShadows1[i+6], DT_RIGHT | DT_VCENTER, 0xFF000000);
			font->DrawTextA(NULL, "+", -1, &behav_LeftBox1[i+6], DT_RIGHT | DT_VCENTER, 0xFF0040a8);
		}

		for (int i = 0; i < 5; i++)
		{
			font->DrawTextA(NULL, behav_descr1[i*2 + 8].c_str(), -1, &behav_RightBoxShadows[i+1], DT_LEFT | DT_VCENTER, 0xFF000000);
			font->DrawTextA(NULL, behav_descr1[i*2 + 8].c_str(), -1, &behav_RightBox[i+1], DT_LEFT | DT_VCENTER, 0xFF0040a8);
			font->DrawTextA(NULL, behav_descr1[i*2 + 9].c_str(), -1, &behav_RightBoxShadows[i+1], DT_RIGHT | DT_VCENTER, 0xFF000000);
			font->DrawTextA(NULL, behav_descr1[i*2 + 9].c_str(), -1, &behav_RightBox[i+1], DT_RIGHT | DT_VCENTER, 0xFF0040a8);
		}

	}
	else // emotions and health
	{
		Sprite->Begin(D3DXSPRITE_ALPHABLEND);
		for (int i = 0; i < 10; i++)
		{
			Sprite->SetTransform(&behav_Bars2[i]);
			Sprite->Draw(Gradients[3], &GradientRects[4], NULL, NULL, 0xFFFFFFFF);
		}
		for (int i = 0; i < 6; i++)
		{
			Sprite->SetTransform(&behav_Bars2[i+10]);
			Sprite->Draw(Gradients[3], &GradientRects[4], NULL, NULL, 0xFFFFFFFF);
		}
		Sprite->End();

		Line->Begin();
		for (int i = 0; i < 10; i++)
		{
			D3DXVECTOR2 pos[2];
			pos[0] = D3DXVECTOR2(behav_lineStarts2[i].x + behav_BarWidth[2] * behav_values[i + 11], behav_lineStarts2[i].y);
			pos[1] = D3DXVECTOR2(pos[0].x, pos[0].y + 38.0f);
			Line->Draw(&pos[0], 2, 0xFF000000);
			pos[0].x--;
			pos[1].x--;
			Line->Draw(&pos[0], 2, 0xFF000000);
		}
		for (int i = 0; i < 6; i++)
		{
			D3DXVECTOR2 pos[2];
			pos[0] = D3DXVECTOR2(behav_lineStarts2[i + 10].x + behav_BarWidth[3] * behav_values[i + 21], behav_lineStarts2[i + 10].y);
			pos[1] = D3DXVECTOR2(pos[0].x, pos[0].y + 38.0f);
			Line->Draw(&pos[0], 2, 0xFF000000);
			pos[0].x--;
			pos[1].x--;
			Line->Draw(&pos[0], 2, 0xFF000000);
		}
		Line->End();

		font->DrawTextA(NULL, behav_descr2[0].c_str(), -1, &behav_LeftBoxShadows2[0], DT_CENTER | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, behav_descr2[0].c_str(), -1, &behav_LeftBox2[0], DT_CENTER | DT_VCENTER, 0xFF0040a8);
		font->DrawTextA(NULL, behav_descr2[11].c_str(), -1, &behav_RightBoxShadows[0], DT_CENTER | DT_VCENTER, 0xFF000000);
		font->DrawTextA(NULL, behav_descr2[11].c_str(), -1, &behav_RightBox[0], DT_CENTER | DT_VCENTER, 0xFF0040a8);

		for (int i = 1; i < 11; i++)
		{
			font->DrawTextA(NULL, behav_descr2[i].c_str(), -1, &behav_LeftBoxShadows2[i], DT_LEFT | DT_VCENTER, 0xFF000000);
			font->DrawTextA(NULL, behav_descr2[i].c_str(), -1, &behav_LeftBox2[i], DT_LEFT | DT_VCENTER, 0xFF0040a8);
		}

		for (int i = 1; i < 7; i++)
		{
			font->DrawTextA(NULL, behav_descr2[i + 11].c_str(), -1, &behav_RightBoxShadows[i], DT_LEFT | DT_VCENTER, 0xFF000000);
			font->DrawTextA(NULL, behav_descr2[i + 11].c_str(), -1, &behav_RightBox[i], DT_LEFT | DT_VCENTER, 0xFF0040a8);
		}
	}
}

void ChaoMenu_DNA(LPDIRECT3DDEVICE9 device)
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

void SetRectangle(LPRECT lprc, float x, float y, float w, float h)
{
	SetRect(lprc, (int)x, (int)y, (int)w, (int)h);
}

void SetRectangle(LPRECT lprc, LONG x, LONG y, LONG w, LONG h)
{
	SetRect(lprc, (int)x, (int)y, (int)w, (int)h);
}

void SetRectangle(LPRECT lprc, int x, int y, int w, int h)
{
	SetRect(lprc, x, y, w, h);
}

void SetTransform(D3DXMATRIX* m, float sx, float sy, float px, float py)
{
	D3DXVECTOR3 scale = { sx, sy, 0.0f };
	D3DXVECTOR3 pos = { px, py, 0.0f };
	D3DXMatrixTransformation(m, NULL, NULL, &scale, NULL, NULL, &pos);
}

void SetTransform(D3DXMATRIX* m, float cx, float cy, float sx, float sy, float px, float py)
{
	D3DXVECTOR3 center = { cx, cy, 0.0f };
	D3DXVECTOR3 scale = { sx, sy, 0.0f };
	D3DXVECTOR3 pos = { px, py, 0.0f };
	D3DXMatrixTransformation(m, &center, NULL, &scale, NULL, NULL, &pos);
}

static bool initialized = false;

void FreeMenu()
{
	if (!initialized)
	{
		return;
	}

	for (auto& i : MenuFillingTextures)
	{
		if (i)
		{
			i->Release();
			i = nullptr;
		}
	}

	for (auto& i : MenuBorderTextures)
	{
		if (i)
		{
			i->Release();
			i = nullptr;
		}
	}

	for (auto& i : Gradients)
	{
		if (i)
		{
			i->Release();
			i = nullptr;
		}
	}

	if (font)
	{
		font->Release();
		font = nullptr;
	}

	if (Line)
	{
		Line->Release();
		Line = nullptr;
	}

	if (Sprite)
	{
		Sprite->Release();
		Sprite = nullptr;
	}

	initialized = false;
}

void InitializeMenu(LPDIRECT3DDEVICE9 device)
{
	if (initialized)
	{
		return;
	}

	float BoxWidth;
	float BoxHeight;
	float Xpos;
	float Xpos2;
	float Ypos;
	float LineHeight;

	#pragma region general menu
	//initializing the font
	std::string loc;
	loc.assign(ResourcesPath);
	loc.append("Typo_DodamM.ttf");
	AddFontResourceExA(loc.c_str(), FR_PRIVATE, NULL);
	D3DXCreateFontA(device, (INT)(height * 0.05f), 0, FW_THIN, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Typo_dodam M", &font);
	
	D3DXCreateLine(device, &Line);

	//gettint the textures and initializing the sprite
	D3DXCreateSprite(device, &Sprite);
	SetRectangle(&MenuBackgroundRect, 0, 0, 32, 32);
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
	for (int i = 0; i < 4; i++)
	{
		loc.assign(ResourcesPath);
		std::string t = "";
		switch (i)
		{
		case 0:
			t = "SFGradient.png";
			break;
		case 1:
			t = "RPGradient.png";
			break;
		case 2:
			t = "AlignmentGradient.png";
			break;
		case 3:
			t = "EvoProgressGradient.png";
			break;
		}
		loc.append(t);
		D3DXCreateTextureFromFileA(device, loc.c_str(), &Gradients[i]);
	}

	#pragma region setting backgroundtransforms
	SetTransform(&MenuBackgroundTransforms[0], 1.0f, 1.0f, 40.0f, 104.0f);
	SetTransform(&MenuBackgroundTransforms[2], 1.0f, 1.0f, width - 72.0f, 104.0f);
	SetTransform(&MenuBackgroundTransforms[6], 1.0f, 1.0f, 40.0f, height - 72.0f);
	SetTransform(&MenuBackgroundTransforms[8], 1.0f, 1.0f, width - 72.0f, height - 72.0f);

	float widthTA = (width - 144.0f) / 32.0f;
	float heightTA = (height - 208.0f) / 32.0f;

	SetTransform(&MenuBackgroundTransforms[1], widthTA, 1.0f, 72.0f, 104.0f);
	SetTransform(&MenuBackgroundTransforms[3], 1.0f, heightTA, 40.0f, 136.0f);
	SetTransform(&MenuBackgroundTransforms[5], 1.0f, heightTA, width - 72.0f, 136.0f);
	SetTransform(&MenuBackgroundTransforms[7], widthTA, 1.0f, 72.0f, height - 72.0f);

	SetTransform(&MenuBackgroundTransforms[4], widthTA, heightTA, 72.0f, 136.0f);
	#pragma endregion

	widthTA = (width - 80) / 4;
	float ScaleWidthTA = (widthTA - 64) / 32;

	//tab positions
	for (int i = 0; i < 4; i++)
	{
		int it = i * 6;
		SetTransform(&MenuTabTransforms[it + 0], 1.0f, 1.0f,         (widthTA * (float)i) + 40.0f, 40.0f);
		SetTransform(&MenuTabTransforms[it + 1], ScaleWidthTA, 1.0f, (widthTA * (float)i) + 72.0f, 40.0f);
		SetTransform(&MenuTabTransforms[it + 2], 1.0f, 1.0f,         widthTA * (float)(i + 1) + 8, 40.0f);
		SetTransform(&MenuTabTransforms[it + 3], 1.0f, 2.0f,         (widthTA * (float)i) + 40.0f, 72.0f);
		SetTransform(&MenuTabTransforms[it + 4], ScaleWidthTA, 2.0f, (widthTA * (float)i) + 72.0f, 72.0f);
		SetTransform(&MenuTabTransforms[it + 5], 1.0f, 2.0f,         widthTA * ((float)i + 1) + 8.0f, 72.0f);
	}

	//tab text positions
	for (int i = 0; i < 4; i++)
	{
		SetRectangle(&TabTextRects[i], (widthTA * (float)i) + 72.0f, 58.0f, (widthTA * (float)(i + 1)), 104.0f);
		SetRectangle(&TabTextRects[i+4], (widthTA * (float)i) + 74.0f, 60.0f, (widthTA * (float)(i + 1)) + 2.0f, 106.0f);
	}

	#pragma region setting invalid menu transforms
	SetTransform(&InvalidMenuTransforms[0], 1.0f, 1.0f, width / 2.0f - 224.0f, height / 2.0f - 160.0f);
	SetTransform(&InvalidMenuTransforms[2], 1.0f, 1.0f, width / 2.0f + 192.0f, height / 2.0f - 160.0f);
	SetTransform(&InvalidMenuTransforms[6], 1.0f, 1.0f, width / 2.0f - 224.0f, height / 2.0f + 128.0f);
	SetTransform(&InvalidMenuTransforms[8], 1.0f, 1.0f, width / 2.0f + 192.0f, height / 2.0f + 128.0f);

	SetTransform(&InvalidMenuTransforms[1], 16.0f, 16.0f, 12.0f, 1.0f, width / 2.0f - 16.0f,  height / 2.0f - 160.0f);
	SetTransform(&InvalidMenuTransforms[3], 16.0f, 16.0f, 1.0f, 8.0f,  width / 2.0f - 224.0f, height / 2.0f - 16.0f);
	SetTransform(&InvalidMenuTransforms[5], 16.0f, 16.0f, 1.0f, 8.0f,  width / 2.0f + 192.0f, height / 2.0f - 16.0f);
	SetTransform(&InvalidMenuTransforms[7], 16.0f, 16.0f, 12.0f, 1.0f, width / 2.0f - 16.0f,  height / 2.0f + 128.0f);

	SetTransform(&InvalidMenuTransforms[4], 16.0f, 16.0f, 12.0f, 8.0f, width / 2.0f - 16.0f, height / 2.0f - 16.0f);

	SetRectangle(&invalidTextRect[0], width / 2.0f - 200.0f, height / 2.0f - 80.0f, width / 2.0f + 200.0f, height / 2.0f + 80.0f);
	SetRectangle(&invalidTextRect[1], width / 2.0f - 196.0f, height / 2.0f - 76.0f, width / 2.0f + 204.0f, height / 2.0f + 84.0f);
	#pragma endregion
	#pragma endregion

	#pragma region Stat Menu
	BoxWidth = (width - 144.0f) * 0.47f;
	BoxHeight = (height - 180.0f);
	Xpos = 72.0f;
	Ypos = 136.0f;
	LineHeight = BoxHeight / 10.0f;

	//setting text positions
	SetRectangle(&Stat_Texts[0], Xpos, Ypos, Xpos + BoxWidth, Ypos + 64.0f);
	for (int i = 0; i < 9; i++)
	{
		float j = LineHeight * (float)(i + 1);
		SetRectangle(&Stat_Texts[i+1], Xpos + 20.0f, Ypos + j, Xpos + 20.0f + BoxWidth * 0.4f, Ypos + (float)j + LineHeight);
	}
	for (int i = 0; i < 5; i++)
	{
		float j = LineHeight * (float)(i + 1);
		SetRectangle(&Stat_Texts[i+10], Xpos + BoxWidth * 0.4f + 20.0f, Ypos + j, Xpos + 20.0f + BoxWidth, Ypos + (float)j + LineHeight);
	}
	for (int i = 0; i < 15; i++)
	{
		SetRectangle(&Stat_TextShadows[i], Stat_Texts[i].left + 2, Stat_Texts[i].top + 2, Stat_Texts[i].right + 2, Stat_Texts[i].bottom + 2);
	}

	#pragma region setting name background positions
	SetTransform(&Stat_NameBG[0], 1.0f, 1.0f, Xpos, Ypos);
	SetTransform(&Stat_NameBG[2], 1.0f, 1.0f, Xpos + BoxWidth, Ypos);
	SetTransform(&Stat_NameBG[3], 1.0f, 1.0f, Xpos, Ypos + 32.0f);
	SetTransform(&Stat_NameBG[5], 1.0f, 1.0f, Xpos + BoxWidth, Ypos + 32.0f);

	SetTransform(&Stat_NameBG[1], BoxWidth / 32.0f - 1.0f, 1.0f, Xpos + 32.0f, Ypos);
	SetTransform(&Stat_NameBG[4], BoxWidth / 32.0f - 1.0f, 1.0f, Xpos + 32.0f, Ypos + 32.0f);
	#pragma endregion

	#pragma region setting gradient positions and sizes
	SetRectangle(&GradientRects[0], 128, 0, 256, 1);
	SetRectangle(&GradientRects[1], 256, 0, 384, 1);
	SetRectangle(&GradientRects[2], 0, 0, 256, 1);
	SetRectangle(&GradientRects[3], 256, 0, 512, 1);
	SetRectangle(&GradientRects[4], 0, 0, 256, 1);
	
	float halfHeight = (LineHeight - 32.0f) / 2.0f;
	BarWidth = BoxWidth * 0.25f + 16.0f;
	SetTransform(&Stat_Gradients[0], BarWidth / 128.0f, 32.0f, Xpos + BoxWidth * 0.75f - 16.0f, Ypos + LineHeight * 6.0f + halfHeight);
	SetTransform(&Stat_Gradients[1], BarWidth / 256.0f, 32.0f, Xpos + BoxWidth * 0.75f - 16.0f, Ypos + LineHeight * 6.0f + halfHeight);

	SetTransform(&Stat_Gradients[2], BarWidth / 128.0f, 32.0f, Xpos + BoxWidth * 0.5f - 32.0f, Ypos + LineHeight * 6.0f + halfHeight);
	SetTransform(&Stat_Gradients[3], BarWidth / 256.0f, 32.0f, Xpos + BoxWidth * 0.5f - 32.0f, Ypos + LineHeight * 6.0f + halfHeight);
	
	SetTransform(&Stat_Gradients[4], BarWidth / 128.0f, 32.0f, Xpos + BoxWidth * 0.75f - 16.0f, Ypos + LineHeight * 7.0f + halfHeight);
	SetTransform(&Stat_Gradients[5], BarWidth / 256.0f, 32.0f, Xpos + BoxWidth * 0.75f - 16.0f, Ypos + LineHeight * 7.0f + halfHeight);

	SetTransform(&Stat_Gradients[6], BarWidth / 128.0f, 32.0f, Xpos + BoxWidth * 0.5f - 32.0f, Ypos + LineHeight * 7.0f + halfHeight);
	SetTransform(&Stat_Gradients[7], BarWidth / 256.0f, 32.0f, Xpos + BoxWidth * 0.5f - 32.0f, Ypos + LineHeight * 7.0f + halfHeight);

	BarWidth *= 2;

	SetTransform(&Stat_Gradients[8], BarWidth / 256.0f, 32.0f, Xpos + BoxWidth * 0.5f - 32.0f, Ypos + LineHeight * 8.0f + halfHeight);
	SetTransform(&Stat_Gradients[9], BarWidth / 256.0f, 32.0f, Xpos + BoxWidth * 0.5f - 32.0f, Ypos + LineHeight * 9.0f + halfHeight);

	LineStarts[0] = D3DXVECTOR2(Xpos + BoxWidth * 0.5f - 32.0f, Ypos + LineHeight * 6.0f + halfHeight - 3.0f);
	LineStarts[1] = D3DXVECTOR2(LineStarts[0].x, Ypos + LineHeight * 7.0f + halfHeight - 3.0f);
	LineStarts[2] = D3DXVECTOR2(LineStarts[0].x, Ypos + LineHeight * 8.0f + halfHeight - 3.0f);
	LineStarts[3] = D3DXVECTOR2(LineStarts[0].x, Ypos + LineHeight * 9.0f + halfHeight - 3.0f);
	#pragma endregion

	LineHeight = BoxHeight / 7;

	for (int i = 0; i < 7; i++)
	{
		float j = LineHeight * (float)i;
		SetRectangle(&Stat_StatTexts[i], width / 2.0f, Ypos + j, width - 72.0f, Ypos + j + LineHeight);
		SetRectangle(&Stat_StatTexts[i + 7], width / 2.0f + BoxWidth * 0.3f, Ypos + j, width - 72.0f, Ypos + j + LineHeight);
		SetRectangle(&Stat_StatTexts[i + 14], width / 2.0f + BoxWidth * 0.4f, Ypos + j, width - 72.0f, Ypos + j + LineHeight);
		SetRectangle(&Stat_StatTexts[i + 21], width / 2.0f + BoxWidth * 0.85f, Ypos + j, width - 72.0f, Ypos + j + LineHeight);
	}
	Stat_StatTexts[12].left -= (LONG)(BoxWidth * 0.05f);
	Stat_StatTexts[13].left -= (LONG)(BoxWidth * 0.05f);
	for (int i = 0; i < 28; i++)
	{
		SetRectangle(&Stat_StatTextShadows[i], Stat_StatTexts[i].left + 2, Stat_StatTexts[i].top + 2, Stat_StatTexts[i].right + 2, Stat_StatTexts[i].bottom + 2);
	}
	#pragma endregion

	#pragma region visual menu
	BoxWidth = (width - 144.0f) * 0.47f;
	BoxHeight = (height - 180.0f);
	Xpos = 92.0f;
	Ypos = 128.0f;
	LineHeight = BoxHeight / 11.0f;
	for (int i = 0; i < 11; i++)
	{
		float j = LineHeight * (float)i;
		SetRectangle(&Vis_Texts[i], Xpos, Ypos + j, Xpos + BoxWidth, Ypos + j + LineHeight);
	}
	LineHeight = BoxHeight / 10;
	for (int i = 0; i < 10; i++)
	{
		float j = LineHeight * (float)i;
		SetRectangle(&Vis_Texts[i+11], width / 2.0f, Ypos + j, width / 2.0f + BoxWidth, Ypos + j + LineHeight);
	}
	for (int i = 0; i < 11; i++)
	{
		SetRectangle(&Vis_Texts[i+21], Vis_Texts[i].left + (LONG)(BoxWidth * 0.4f), Vis_Texts[i].top, Vis_Texts[i].right, Vis_Texts[i].bottom);
	}
	for (int i = 0; i < 9; i++)
	{
		SetRectangle(&Vis_Texts[i + 32], Vis_Texts[i+12].left + (LONG)(BoxWidth * 0.4f), Vis_Texts[i+12].top, Vis_Texts[i+12].right, Vis_Texts[i+12].bottom);
	}
	for (int i = 0; i < 41; i++)
	{
		SetRectangle(&Vis_TextShadows[i], Vis_Texts[i].left + 2, Vis_Texts[i].top + 2, Vis_Texts[i].right + 2, Vis_Texts[i].bottom + 2);
	}
	#pragma endregion

	#pragma region behaviour menu
	BoxWidth = (width - 144.0f) * 0.47f;
	BoxHeight = (height - 180.0f);
	Xpos = 92.0f;
	Ypos = 128.0f;
	LineHeight = BoxHeight / 7.0f;

	for (int i = 0; i < 7; i++)
	{
		float j = LineHeight * (float)i;
		SetRectangle(&behav_LeftBox1[i], Xpos, Ypos + j, Xpos + BoxWidth, Ypos + j + LineHeight);
		SetRectangle(&behav_RightBox[i], width / 2.0f, Ypos + j, width / 2.0f + BoxWidth, Ypos + j + LineHeight);

		SetRectangle(&behav_LeftBoxShadows1[i], behav_LeftBox1[i].left + 2, behav_LeftBox1[i].top + 2, behav_LeftBox1[i].right + 2, behav_LeftBox1[i].bottom + 2);
		SetRectangle(&behav_RightBoxShadows[i], behav_RightBox[i].left + 2, behav_RightBox[i].top + 2, behav_RightBox[i].right + 2, behav_RightBox[i].bottom + 2);
	}
	for (int i = 1; i < 7; i++)
	{
		SetRectangle(&behav_LeftBox1[i + 6], behav_LeftBox1[i].left + (int)(BoxWidth * 0.35f), behav_LeftBox1[i].top, behav_LeftBox1[i].right - (int)(BoxWidth * 0.04f), behav_LeftBox1[i].bottom);
		SetRectangle(&behav_LeftBoxShadows1[i + 6], behav_LeftBoxShadows1[i].left + (int)(BoxWidth * 0.35f), behav_LeftBoxShadows1[i].top, behav_LeftBoxShadows1[i].right - (int)(BoxWidth * 0.04f), behav_LeftBoxShadows1[i].bottom);
	}
	LineHeight = BoxHeight / 11;
	for (int i = 0; i < 11; i++)
	{
		float j = LineHeight * (float)i;
		SetRectangle(&behav_LeftBox2[i], Xpos, Ypos + j, Xpos + BoxWidth, Ypos + j + LineHeight);
		SetRectangle(&behav_LeftBoxShadows2[i], behav_LeftBox2[i].left + 2, behav_LeftBox2[i].top + 2, behav_LeftBox2[i].right + 2, behav_LeftBox2[i].bottom + 2);
	}
	SetRectangle(&behav_Info[0], 0.0f, height - 64.0f, width, height + 6.0f);
	SetRectangle(&behav_Info[1], behav_Info[0].left + 1, behav_Info[0].top, behav_Info[0].right + 1, behav_Info[0].bottom);
	SetRectangle(&behav_Info[2], behav_Info[0].left, behav_Info[0].top + 1, behav_Info[0].right, behav_Info[0].bottom + 1);
	SetRectangle(&behav_Info[3], behav_Info[0].left - 1, behav_Info[0].top, behav_Info[0].right - 1, behav_Info[0].bottom);
	SetRectangle(&behav_Info[4], behav_Info[0].left, behav_Info[0].top - 1, behav_Info[0].right, behav_Info[0].bottom - 1);

	LineHeight = BoxHeight / 7.0f;
	halfHeight = (LineHeight - 32.0f) / 2.0f;
	behav_BarWidth[0] = BoxWidth * 0.5f;
	behav_BarWidth[1] = BoxWidth * 0.4f;
	for (int i = 0; i < 6; i++)
	{
		float j = LineHeight * (float)(i + 1);
		behav_lineStarts1[i] = D3DXVECTOR2(Xpos + BoxWidth * 0.4f, Ypos + j + halfHeight - 3);
		SetTransform(&behav_Bars1[i], (BoxWidth * 0.5f) / 256.0f, 32.0f, Xpos + BoxWidth * 0.4f, Ypos + j + halfHeight);
		behav_lineStarts1[i + 6] = D3DXVECTOR2(width / 2.0f + BoxWidth * 0.28f, Ypos + j + halfHeight - 3);
		SetTransform(&behav_Bars1[i + 6], (BoxWidth * 0.4f) / 256.0f, 32.0f, width / 2.0f + BoxWidth * 0.28f, Ypos + j + halfHeight);
		behav_lineStarts2[i + 10] = D3DXVECTOR2(width / 2.0f + BoxWidth * 0.5f, Ypos + j + halfHeight - 3);
		SetTransform(&behav_Bars2[i + 10], (BoxWidth * 0.5f) / 256.0f, 32.0f, width / 2.0f + BoxWidth * 0.5f, Ypos + j + halfHeight);
	}
	behav_BarWidth[2] = (BoxWidth * 0.5f);
	behav_BarWidth[3] = (BoxWidth * 0.5f);
	LineHeight = BoxHeight / 11.0f;
	halfHeight = (LineHeight - 32.0f) / 2.0f;
	for (int i = 0; i < 10; i++)
	{
		float j = LineHeight * (float)(i + 1);
		behav_lineStarts2[i] = D3DXVECTOR2(Xpos + BoxWidth * 0.46f, Ypos + j + halfHeight - 3.0f);
		SetTransform(&behav_Bars2[i], (BoxWidth * 0.5f) / 256.0f, 32.0f, Xpos + BoxWidth * 0.46f, Ypos + j + halfHeight);
	}
	#pragma endregion

	#pragma region DNA Menu
	BoxWidth = (width - 144.0f) * 0.47f;
	BoxHeight = height - 288.0f;
	Xpos = 72.0f;
	Xpos2 = width - 104.0f - BoxWidth;
	Ypos = 184.0f;

	SetTransform(&DNA_LeftBox[0], 1.0f, 1.0f, Xpos,            Ypos);
	SetTransform(&DNA_LeftBox[2], 1.0f, 1.0f, Xpos + BoxWidth, Ypos);
	SetTransform(&DNA_LeftBox[6], 1.0f, 1.0f, Xpos,            Ypos + BoxHeight);
	SetTransform(&DNA_LeftBox[8], 1.0f, 1.0f, Xpos + BoxWidth, Ypos + BoxHeight);

	SetTransform(&DNA_LeftBox[1], BoxWidth / 32.0f - 1.0f, 1.0f,  Xpos + 32.0f,    Ypos);
	SetTransform(&DNA_LeftBox[3], 1.0f, BoxHeight / 32.0f - 1.0f, Xpos,            Ypos + 32.0f);
	SetTransform(&DNA_LeftBox[5], 1.0f, BoxHeight / 32.0f - 1.0f, Xpos + BoxWidth, Ypos + 32.0f);
	SetTransform(&DNA_LeftBox[7], BoxWidth / 32.0f - 1.0f, 1.0f,  Xpos + 32.0f,    Ypos + BoxHeight);

	SetTransform(&DNA_LeftBox[4], BoxWidth / 32.0f - 1.0f, BoxHeight / 32.0f - 1.0f, Xpos + 32.0f, Ypos + 32.0f);
	
	SetTransform(&DNA_RightBox[0], 1.0f, 1.0f, Xpos2,            Ypos);
	SetTransform(&DNA_RightBox[2], 1.0f, 1.0f, Xpos2 + BoxWidth, Ypos);
	SetTransform(&DNA_RightBox[6], 1.0f, 1.0f, Xpos2,            Ypos + BoxHeight);
	SetTransform(&DNA_RightBox[8], 1.0f, 1.0f, Xpos2 + BoxWidth, Ypos + BoxHeight);

	SetTransform(&DNA_RightBox[1], BoxWidth / 32.0f - 1.0f, 1.0f,  Xpos2 + 32.0f,    Ypos);
	SetTransform(&DNA_RightBox[3], 1.0f, BoxHeight / 32.0f - 1.0f, Xpos2,            Ypos + 32.0f);
	SetTransform(&DNA_RightBox[5], 1.0f, BoxHeight / 32.0f - 1.0f, Xpos2 + BoxWidth, Ypos + 32.0f);
	SetTransform(&DNA_RightBox[7], BoxWidth / 32.0f - 1.0f, 1.0f,  Xpos2 + 32.0f,    Ypos + BoxHeight);

	SetTransform(&DNA_RightBox[4], BoxWidth / 32.0f - 1.0f, BoxHeight / 32.0f - 1.0f, Xpos2 + 32.0f, Ypos + 32.0f);

	LineHeight = BoxHeight / 13.0f;
	SetRectangle(&DNA_Texts[0], Xpos, 136.0f, Xpos + BoxWidth, 168.0f);
	SetRectangle(&DNA_Texts[27], Xpos2, 136.0f, Xpos2 + BoxWidth, 168.0f);
	for (int i = 0; i < 13; i++)
	{
		float j = LineHeight * (float)i;
		SetRectangle(&DNA_Texts[i + 1], Xpos + 20.0f, 208.0f + (float)j, Xpos + 20.0f + BoxWidth * 0.4f, 208.0f + (float)j + LineHeight);
		SetRectangle(&DNA_Texts[i + 14], Xpos + 20.0f + BoxWidth * 0.4f, 208.0f + (float)j, Xpos + 20.0f + BoxWidth, 208.0f + (float)j + LineHeight);

		SetRectangle(&DNA_Texts[i + 28], Xpos2 + 20.0f, 208.0f + (float)j, Xpos2 + 20 + BoxWidth * 0.4f, 208.0f + (float)j + LineHeight);
		SetRectangle(&DNA_Texts[i + 41], Xpos2 + 20.0f + BoxWidth * 0.4f, 208.0f + (float)j, Xpos2 + 20.0f + BoxWidth, 208.0f + (float)j + LineHeight);
	}
	for (int i = 0; i < 54; i++)
	{
		SetRectangle(&DNA_TextShadows[i], DNA_Texts[i].left + 2, DNA_Texts[i].top + 2, DNA_Texts[i].right + 2, DNA_Texts[i].bottom + 2);
	}
	#pragma endregion

	initialized = true;
}

void DisplayChaoMenu(LPDIRECT3DDEVICE9 pDevice)
{
	if (PauseMenuID == 3)
	{
		InitializeMenu(pDevice);

		if (displayChao == nullptr)
		{
			Sprite->Begin(D3DXSPRITE_ALPHABLEND);
			for (int i = 0; i < 9; i++)
			{
				Sprite->SetTransform(&InvalidMenuTransforms[i]);
				Sprite->Draw(MenuFillingTextures[i], &MenuBackgroundRect, NULL, NULL, 0x503963CE);
				Sprite->Draw(MenuBorderTextures[i], &MenuBackgroundRect, NULL, NULL, 0xFF4E97FF);
			}
			Sprite->End();

			font->DrawTextW(NULL, L"Please hold a Chao in your hands", -1, &invalidTextRect[1], DT_CENTER | DT_WORDBREAK | DT_NOCLIP | DT_VCENTER, 0xFF000000);
			font->DrawTextW(NULL, L"Please hold a Chao in your hands", -1, &invalidTextRect[0], DT_CENTER | DT_WORDBREAK | DT_NOCLIP | DT_VCENTER, 0xFFFFFFFF);
		}
		else
		{
			int n = 0;
			int c = 0;

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
				font->DrawTextA(NULL, TabTextStrings[i].c_str(), -1, &TabTextRects[i + 4], DT_CENTER | DT_NOCLIP, 0xFF000000);
				font->DrawTextA(NULL, TabTextStrings[i].c_str(), -1, &TabTextRects[i], DT_CENTER | DT_NOCLIP, 0xFFFFFFFF);
			}

			switch (PauseSelection)
			{
			case 0:
				ChaoMenu_Stats(pDevice);
				break;
			case 1:
				ChaoMenu_Visual(pDevice);
				break;
			case 2:
				ChaoMenu_Personality(pDevice);
				break;
			case 3:
				ChaoMenu_DNA(pDevice);
				break;
			}
		}
	}
}

extern "C"
{
	__declspec(dllexport) void OnRenderSceneEnd()
	{
		DisplayChaoMenu(g_pRenderDevice->m_pD3DDevice);
	}

	__declspec(dllexport) void OnRenderDeviceLost()
	{
		FreeMenu();
	}

	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		char buf[256];
		
		if (_getcwd(buf, 256) == nullptr)
		{
			MessageBoxA(NULL, "Could not get absolute path to mod.", "ChaoStatMenu error", MB_ICONERROR);
			exit(EXIT_FAILURE);
		}

		ResourcesPath = buf;
		ResourcesPath.append("\\");
		ResourcesPath.append(path);
		ResourcesPath.append("\\Resources\\");

		WriteData((int*)0x0174A830, (int)&ChaoMenuString);
		WriteData((int*)0x0174A990, (int)&ChaoMenuString);
		WriteData((int*)0x0174A938, (int)&ChaoMenuString);
		WriteData((int*)0x0174A8E0, (int)&ChaoMenuString);
		WriteData((int*)0x0174A888, (int)&ChaoMenuString);
		WriteData((int*)0x0174A7D8, (int)&ChaoMenuString);

		WriteJump((int*)0x0043B119, RedirectToChaoMenuInput);
		WriteJump((int*)0x0043B1EE, SelectChaoMenuWrapper);
		WriteData((char*)0x0043B037, (char)0x8C);
		WriteJump((int*)0x0043D49B, ActivateChaoMenu_Wrapper);
	}

	__declspec(dllexport) void OnExit()
	{
		FreeMenu();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
