#include "stdafx.h"
#include "SA2ModLoader.h"

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
	}
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}

