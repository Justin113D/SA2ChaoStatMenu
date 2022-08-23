#pragma once

#include "SA2ModLoader.h"
#include <string>

enum ChaoEmotionball : __int8
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
	char SwimFraction; //0x20
	char FlyFraction;
	char RunFraction;
	char PowerFraction;
	char StaminaFraction;
	char LuckFraction;
	char IntelligenceFraction;
	char UnknownFraction;
	ChaoGrade SwimGrade; //0x28
	ChaoGrade FlyGrade;
	ChaoGrade RunGrade;
	ChaoGrade PowerGrade;
	ChaoGrade StaminaGrade;
	ChaoGrade LuckGrade;
	ChaoGrade IntelligenceGrade;
	ChaoGrade UnknownGrade;
	char SwimLevel;	//0x30
	char FlyLevel;
	char RunLevel;
	char PowerLevel;
	char StaminaLevel;
	char LuckLevel;
	char IntelligenceLevel;
	char UnknownLevel;
	__int16 SwimStat; //0x38
	__int16 FlyStat;
	__int16 RunStat;
	__int16 PowerStat;
	__int16 StaminaStat;
	__int16 LuckStat; 
	__int16 IntelligenceStat; //0x44
	__int16 UnknownStat;
	char field_46[56];
	ChaoType Type;	//0x80
	char Garden;
	__int16 Happiness;
	__int16 field_84;
	__int16 ClockRollovers;
	__int16 field_88;
	__int16 LifeSpan;
	__int16 Lifespan2;
	__int16 Reincarnations;
	char field_90[24];
	float PowerRun;	//0xA8
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
	SA2BBodyType BodyType;
	SA2BAnimal BodyTypeAnimal;
	char field_DF[57];
	int SA2AnimalBehaviour;
	SA2BAnimal SA2BArmType;
	SA2BAnimal SA2BEarType;
	SA2BAnimal SA2BForeheadType;
	SA2BAnimal SA2BHornType;
	SA2BAnimal SA2BLegType;
	SA2BAnimal SA2BTailType;
	SA2BAnimal SA2BWingType;
	SA2BAnimal SA2BFaceType;
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
	char Naive_Normal;
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
	char field_166[6];
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

std::wstring ChaoNameDecoderSimple[256]
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
	L"?",
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
	L"?",
	L"?",
	L"È",
	L"É",
	L"Ê",
	L"Ë",
	L"Ì",
	L"Í",
	L"Î",
	L"Ï",
	L"?",
	L"?",
	L"Ò",
	L"Ó",
	L"Ô",
	L"Õ",
	L"Ö",
	L"?",
	L"?",
	L"Ù",
	L"Ú",
	L"Û",
	L"Ü",
	L"?",
	L"?",
	L"ß",
	L"à",
	L"á",
	L"â",
	L"ã",
	L"ä",
	L"å",
	L"?",
	L"?",
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
	L"?",
	L"ù",
	L"ú",
	L"û",
	L"ü",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
	L"?",
};


std::wstring DecodeChaoName(char name[])
{
	int i = 0;
	std::wstring result = L"";
	while (name[i] != 0 && i < 7)
	{
		if(name[i] < 160) result.append(ChaoNameDecoderSimple[name[i]]);
		else result.append(L"_");
		i++;
	}
	return result;
}

std::string FavFruit[8] = {
	"Round",
	"Round",
	"Triangle",
	"Triangle",
	"Square",
	"Square",
	"None",
	"None",
};

std::string Grade[7] = {
	"E",
	"D",
	"C",
	"B",
	"A",
	"S",
	"X",
};

std::string Color[14] = {
	"Normal",
	"Yellow",
	"White",
	"Brown",
	"Sky Blue",
	"Pink",
	"Blue",
	"Grey",
	"Green",
	"Red",
	"Lime Green",
	"Purple",
	"Orange",
	"Black"
};

std::string Texture[17] = {
	"None",
	"Gold",
	"Silver",
	"Ruby",
	"Sapphire",
	"Emerald",
	"Amethyst",
	"Aquamarine",
	"Garnet",
	"Onyx",
	"Peridot",
	"Topaz",
	"Pearl",
	"Metal1",
	"Metal2",
	"Glass",
	"Moon",
};

std::string EggColor[55] = {
	"Normal",
	"Yellow Mono-Tone",
	"White Mono-Tone",
	"Brown Mono-Tone",
	"Sky Blue Mono-Tone",
	"Pink Mono-Tone",
	"Blue Mono-Tone",
	"Grey Mono-Tone",
	"Green Mono-Tone",
	"Red Mono-Tone",
	"Lime Green Mono-Tone",
	"Purple Mono-Tone",
	"Orange Mono-Tone",
	"Black Mono-Tone",
	"Yellow Two-Tone",
	"White Two-Tone",
	"Brown Two-Tone",
	"Sky Blue Two-Tone",
	"Pink Two-Tone",
	"Blue Two-Tone",
	"Grey Two-Tone",
	"Green Two-Tone",
	"Red Two-Tone",
	"Lime Green Two-Tone",
	"Purple Two-Tone",
	"Orange Two-Tone",
	"Black Two-Tone",
	"Normal Shiny",
	"Yellow Shiny Mono-Tone",
	"White Shiny Mono-Tone",
	"Brown Shiny Mono-Tone",
	"Sky Shiny Blue Mono-Tone",
	"Pink Shiny Mono-Tone",
	"Blue Shiny Mono-Tone",
	"Grey Shiny Mono-Tone",
	"Green Shiny Mono-Tone",
	"Red Shiny Mono-Tone",
	"Lime Shiny Green Mono-Tone",
	"Purple Shiny Mono-Tone",
	"Orange Shiny Mono-Tone",
	"Black Shiny Mono-Tone",
	"Yellow Shiny Two-Tone",
	"White Shiny Two-Tone",
	"Brown Shiny Two-Tone",
	"Sky Shiny Blue Two-Tone",
	"Pink Shiny Two-Tone",
	"Blue Shiny Two-Tone",
	"Grey Shiny Two-Tone",
	"Green Shiny Two-Tone",
	"Red Shiny Two-Tone",
	"Lime Shiny Green Two-Tone",
	"Purple Shiny Two-Tone",
	"Orange Shiny Two-Tone",
	"Black Shiny Two-Tone",
	"Glitchy Normal"
};

std::string Type[26] = {
	"Empty",
	"Egg",
	"Child",
	"Good",
	"Bad",
	"Neutral Normal",
	"Hero Normal",
	"Dark Normal",
	"Neutral Swim",
	"Hero Swim",
	"Dark Swim",
	"Neutral Fly",
	"Hero Fly",
	"Dark Fly",
	"Neutral Run",
	"Hero Run",
	"Dark Run",
	"Neutral Power",
	"Hero Power",
	"Dark Power",
	"Neutral Chaos",
	"Hero Chaos",
	"Dark Chaos",
	"Tails",
	"Knuckles",
	"Amy",
};

std::string Garden[4] = {
	"None",
	"Neutral Garden",
	"Hero Garden",
	"Dark Garden"
};

std::string Eyes[14] = {
	"Normal",
	"Painful",
	"Closed straight",
	"Closed happy",
	"Closed up",
	"Tiny",
	"Circles",
	"Closed down",
	"Tiny",
	"Half closed",
	"Mean",
	"Green Chaos",
	"Blue Chaos",
	"Yellow Chaos"
};

std::string Medal[16] = {
	"None",
	"Aquamarine",
	"Topaz",
	"Peridot",
	"Garnet",
	"Onyx",
	"Diamond",
	"Beginner",
	"Challenge",
	"Hero",
	"Dark",
	"Pearl",
	"Amethyst",
	"Emerald",
	"Ruby",
	"Sapphire"
};

std::string Mouth[16] = {
	"None",
	"Toothy Smile",
	"Open",
	"Closed Smile",
	"Closed Frown",
	"Open Smile",
	"Open Frown",
	"Closed Smile",
	"Squiggly",
	"Toothy Frown",
	"Closed Frown",
	"Wide Open",
	"Closed Frown",
	"Straight Mustache",
	"Squiggly Mustache 1",
	"Squiggly Mustache 2",
};

std::string EmotionBall[3] ={
	"Normal",
	"Flame",
	"None"
};

std::string Hat[71] = {
	"None",
	"Pumpkin",
	"Skull",
	"Apple",
	"Bucket",
	"Empty Can",
	"Cardboard Box",
	"Flower Pot",
	"Paper Bag",
	"Pan",
	"Stump",
	"Watermelon",
	"Red Wool Beanie",
	"Blue Wool Beanie",
	"Black Wool Beanie",
	"Pacifier",
	"Normal Egg Shell",
	"Yellow Mono-Tone Egg Shell",
	"White Mono-Tone Egg Shell",
	"Brown Mono-Tone Egg Shell",
	"Sky Blue Mono-Tone Egg Shell",
	"Pink Mono-Tone Egg Shell",
	"Blue Mono-Tone Egg Shell",
	"Grey Mono-Tone Egg Shell",
	"Green Mono-Tone Egg Shell",
	"Red Mono-Tone Egg Shell",
	"Lime Green Mono-Tone Egg Shell",
	"Purple Mono-Tone Egg Shell",
	"Orange Mono-Tone Egg Shell",
	"Black Mono-Tone Egg Shell",
	"Yellow Two-Tone Egg Shell",
	"White Two-Tone Egg Shell",
	"Brown Two-Tone Egg Shell",
	"Sky Blue Two-Tone Egg Shell",
	"Pink Two-Tone Egg Shell",
	"Blue Two-Tone Egg Shell",
	"Grey Two-Tone Egg Shell",
	"Green Two-Tone Egg Shell",
	"Red Two-Tone Egg Shell",
	"Lime Green Two-Tone Egg Shell",
	"Purple Two-Tone Egg Shell",
	"Orange Two-Tone Egg Shell",
	"Black Two-Tone Egg Shell",
	"Normal Shiny Egg Shell",
	"Yellow Shiny Mono-Tone Egg Shell",
	"White Shiny Mono-Tone Egg Shell",
	"Brown Shiny Mono-Tone Egg Shell",
	"Sky Blue Shiny Mono-Tone Egg Shell",
	"Pink Shiny Mono-Tone Egg Shell",
	"Blue Shiny Mono-Tone Egg Shell",
	"Grey Shiny Mono-Tone Egg Shell",
	"Green Shiny Mono-Tone Egg Shell",
	"Red Shiny Mono-Tone Egg Shell",
	"Lime Green Shiny Mono-Tone Egg Shell",
	"Purple Shiny Mono-Tone Egg Shell",
	"Orange Shiny Mono-Tone Egg Shell",
	"Black Shiny Mono-Tone Egg Shell",
	"Yellow Shiny Two-Tone Egg Shell",
	"White Shiny Two-Tone Egg Shell",
	"Brown Shiny Two-Tone Egg Shell",
	"Sky Blue Shiny Two-Tone Egg Shell",
	"Pink Shiny Two-Tone Egg Shell",
	"Blue Shiny Two-Tone Egg Shell",
	"Grey Shiny Two-Tone Egg Shell",
	"Green Shiny Two-Tone Egg Shell",
	"Red Shiny Two-Tone Egg Shell",
	"Lime Green Shiny Two-Tone Egg Shell",
	"Purple Shiny Two-Tone Egg Shell",
	"Orange Shiny Two-Tone Egg Shell",
	"Black Shiny Two-Tone Egg Shell",
	"Glitchy Normal Egg Shell"
};

std::string BodyType[6] = {
	"Normal",
	"Egg Chao",
	"Omochao",
	"Animal",
	"???",
	"None",
};

std::string Animal[25] = {
	"Penguin",
	"Seal",
	"Otter",
	"Rabbit",
	"Cheetah",
	"Warthog",
	"Bear",
	"Tiger",
	"Gorilla",
	"Peacock",
	"Parrot",
	"Condor",
	"Skunk",
	"Sheep",
	"Raccoon",
	"Half Fish",
	"Skeleton Dog",
	"Bat",
	"Dragon",
	"Unicorn",
	"Phoenix",
	"Yellow Chaos Drive",
	"Green Chaos Drive",
	"Red Chaos Drive",
	"Purple Chaos Drive",
};

std::string SetZero(int number, int count)
{
	int l = std::to_string(number).length();
	std::string result;
	if(l < count)
		for (int i = count - l; i > 0; i--)
		{
			result.append("0");
		}
	result.append(std::to_string(number));
	return result;
}
