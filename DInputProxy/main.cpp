/*
* Code here is modifed from https://github.com/OrfeasZ/ZHMModSDK/blob/master/DirectInputProxy/Src/DirectInputProxy.cpp
* All credit goes to it's original authors
*/

#include <thread>
#include <unordered_set>
#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <iostream>

#include <steam/steam_api.h>
#pragma comment(lib, "steam_api64.lib")

static HMODULE g_OriginalDirectInput = nullptr;
static HMODULE g_ZHMModSDK = nullptr;
static HMODULE g_AchievementHandler = nullptr;

typedef HRESULT(__stdcall* DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
static DirectInput8Create_t o_DirectInput8Create = nullptr;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	static wchar_t s_PathBuffer[8192];

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (std::filesystem::exists("./Retail/AchievementHandler.dll")) {
			SteamAPI_Init();
			g_AchievementHandler =LoadLibrary("AchievementHandler");
		}
		if (!GetSystemDirectoryW(s_PathBuffer, sizeof(s_PathBuffer) / sizeof(wchar_t)))
			return false;

		std::filesystem::path s_Dinput8Path = s_PathBuffer;
		s_Dinput8Path += "/dinput8.dll";

		g_OriginalDirectInput = LoadLibraryW(canonical(s_Dinput8Path).c_str());

		if (g_OriginalDirectInput == nullptr)
			return false;

		o_DirectInput8Create = reinterpret_cast<DirectInput8Create_t>(GetProcAddress(g_OriginalDirectInput, "DirectInput8Create"));

		if (o_DirectInput8Create == nullptr)
			return false;

		// If this isn't the HITMAN3 executable then don't load the SDK.
		if (!GetModuleFileNameW(nullptr, s_PathBuffer, sizeof(s_PathBuffer) / sizeof(wchar_t)))
			return false;

		std::filesystem::path s_ModuleFilePath = s_PathBuffer;
		std::string s_ExecutableName = s_ModuleFilePath.filename().string();
		std::transform(s_ExecutableName.begin(), s_ExecutableName.end(), s_ExecutableName.begin(), [](unsigned char c) { return std::tolower(c); });

		// We return true here because if we return false then whatever app is loading this will crash.
		if (s_ExecutableName != "hitman3.exe")
			return true;

		// Load ZHMModSDK if present
		if (std::filesystem::exists("./Retail/ZHMModSDK.dll"))
		{
			g_ZHMModSDK = LoadLibraryA("ZHMModSDK");

			if (g_ZHMModSDK == nullptr)
				return false;
		}
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		if (g_OriginalDirectInput != nullptr)
		{
			o_DirectInput8Create = nullptr;

			FreeLibrary(g_OriginalDirectInput);
			g_OriginalDirectInput = nullptr;
		}

		if (g_ZHMModSDK != nullptr)
		{
			FreeLibrary(g_ZHMModSDK);
			g_ZHMModSDK = nullptr;
		}
		if (g_AchievementHandler != nullptr)
		{
			FreeLibrary(g_AchievementHandler);
			g_AchievementHandler = nullptr;
		}
	}

	return true;
}

extern "C" __declspec(dllexport) HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter)
{
	if (o_DirectInput8Create == nullptr)
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_NOT_READY); // DIERR_NOTINITIALIZED

	return o_DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}
