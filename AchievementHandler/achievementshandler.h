#ifndef _ACHIEVEMENTS_HANDLER_H_
#define _ACHIEVEMENTS_HANDLER_H_
#include <unordered_map>
#include <string>

#pragma comment(lib, "steam_api64.lib")
#include <steam/steam_api.h>

#include <eos_achievements.h>
#include <eos_achievements_types.h>
#include <eos_init.h>
#include <eos_sdk.h>

class AchievementsHandler {
public:
	AchievementsHandler();

	std::string get_steam_ach_id_from_epic(std::string_view epic);
	void UnlockSteamAchievement(std::string epic_id);
	void GetAllAchievementsForEpicAndUnlockThem(EOS_HPlatform platform, EOS_ProductUserId user_id);
	STEAM_CALLBACK(AchievementsHandler, OnAchievementsStored, UserAchievementStored_t, CallbackAchievementStored);
private:
	std::unordered_map<std::string_view, std::string> epic_to_steam_ach_bindings;

	// Stored in steam achievement ids. Ex. ACH_1...ACH_83
	std::vector<std::string> unlocked_steam_achievements;
};
void GetAllAchievements(EOS_HPlatform platform, EOS_ProductUserId user_id);


extern AchievementsHandler g_achievements_handler;

#endif