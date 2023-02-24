#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <eos_init.h>
#include <eos_achievements.h>

#include <MinHook.h>

#include <vector>
#include <string>

#include "achievementshandler.h"

EOS_HPlatform platform;

#define FUNC_SIG_DEF(RETURN_TYPE, FUNCTIONNAME, ...) typedef RETURN_TYPE(*t##FUNCTIONNAME)(__VA_ARGS__);

#define FUNC_POINTER_DEF(TYPE, FUNCTION_NAME) TYPE fp##FUNCTION_NAME = NULL;

#define CREATE_HOOK(FUNCTIONNAME) MH_CreateHook(&##FUNCTIONNAME, &Hook_##FUNCTIONNAME, reinterpret_cast<LPVOID*>(&fp##FUNCTIONNAME))

typedef EOS_HPlatform(*tEOS_Platform_Create)(const EOS_Platform_Options*);

tEOS_Platform_Create fpEOS_Platform_Create = NULL;


EOS_HPlatform Hook_EOS_Platform_Create(const EOS_Platform_Options* Options) {
	platform = fpEOS_Platform_Create(Options);
	return platform;
}


typedef void (*tEOS_Achievements_UnlockAchievements)(EOS_HAchievements Handle, const EOS_Achievements_UnlockAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnUnlockAchievementsCompleteCallback CompletionDelegate);

tEOS_Achievements_UnlockAchievements fpEOS_Achievements_UnlockAchievements = NULL;
bool ran = false;
void Hook_EOS_Achievements_UnlockAchievements(EOS_HAchievements Handle, const EOS_Achievements_UnlockAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnUnlockAchievementsCompleteCallback CompletionDelegate) {
	if (!ran) {
		// For all achievements unlocked before using this mod
		g_achievements_handler.GetAllAchievementsForEpicAndUnlockThem(platform, Options->UserId);
		ran = true;
	}
	for (int i = 0; i < Options->AchievementsCount; i++) {
		g_achievements_handler.UnlockSteamAchievement(Options->AchievementIds[i]);
	}
	fpEOS_Achievements_UnlockAchievements(Handle, Options, ClientData, CompletionDelegate);
}


BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		if (MH_Initialize() != MH_OK) {
			MessageBox(NULL, "MH_Initialize failed", "", NULL);
			return true;
		}

		if (CREATE_HOOK(EOS_Platform_Create) != MH_OK ||

			CREATE_HOOK(EOS_Achievements_UnlockAchievements) != MH_OK) {
			MessageBox(NULL, "MH_CreateHook failed", "", NULL);
			return true;
		}

		if (MH_EnableHook(&EOS_Platform_Create) != MH_OK ||
			MH_EnableHook(&EOS_Achievements_UnlockAchievements) != MH_OK) {
			MessageBox(NULL, "MH_EnableHook failed", "", NULL);
			return true;
		}
	}
	else if (reason == DLL_PROCESS_DETACH) {
		if (MH_Uninitialize() != MH_OK) {
			MessageBox(NULL, "MH_Uninitialize failed", "", NULL);
		}
	}

	return TRUE;
}