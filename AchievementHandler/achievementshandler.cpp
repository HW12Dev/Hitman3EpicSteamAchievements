#ifndef _ACHIEVEMENTS_HANDLER_HPP_
#define _ACHIEVEMENTS_HANDLER_HPP_

#include "achievementshandler.h"
#include "log.hpp"

EOS_HPlatform EOSPlatform;

AchievementsHandler g_achievements_handler = AchievementsHandler();

AchievementsHandler::AchievementsHandler() : CallbackAchievementStored(this, &AchievementsHandler::OnAchievementsStored) {
	epic_to_steam_ach_bindings = {
        // The Result of Previous Training
{ "ACH_000", "ACH_1" },

// Cleared for Field Duty
{ "ACH_001", "ACH_2" },

// Seizing the Opportunity
{ "ACH_002", "ACH_3" },

// The Creative Assassin
{ "ACH_003", "ACH_4" },

// Silent Assassin
{ "ACH_004", "ACH_5" },

// Training Escalated
{ "ACH_005", "ACH_6" },

// Top of the Class
{ "ACH_006", "ACH_7" },

// A New Profile
{ "ACH_007", "ACH_8" },

// Tools of the Trade
{ "ACH_008", "ACH_9" },

// Unseen Assassin
{ "ACH_009", "ACH_10" },

// Shortcut Killer
{ "ACH_010", "ACH_11" },

// Stylish Assassin
{ "ACH_011", "ACH_12" },

// Death From Above
{ "ACH_012", "ACH_13" },

// Dune Raider
{ "ACH_013", "ACH_14" },

// Treacherous Architecture
{ "ACH_014", "ACH_15" },

// Keep Your Eyes Peeled
{ "ACH_015", "ACH_16" },

// Rise Up
{ "ACH_016", "ACH_17" },

// Stair Master
{ "ACH_017", "ACH_18" },

// Master of the Household
{ "ACH_018", "ACH_19" },

// No Stone Unturned
{ "ACH_019", "ACH_20" },

// Upstairs, Downstairs
{ "ACH_020", "ACH_21" },

// Family Feud
{ "ACH_021", "ACH_22" },

// Full House
{ "ACH_022", "ACH_23" },

// The Great Outdoors
{ "ACH_023", "ACH_24" },

// Death of the Party
{ "ACH_024", "ACH_25" },

// Followed the Trails
{ "ACH_025", "ACH_26" },

// Partied Out
{ "ACH_026", "ACH_27" },

// Bird Art
{ "ACH_027", "ACH_28" },

// Last Call
{ "ACH_028", "ACH_29" },

// Warehouse Veteran
{ "ACH_029", "ACH_30" },

// NEXUS-47
{ "ACH_030", "ACH_31" },

// Surveillance Master
{ "ACH_031", "ACH_32" },

// Future Shock
{ "ACH_032", "ACH_33" },

// Console Cowboy
{ "ACH_033", "ACH_34" },

// Icebreaker
{ "ACH_034", "ACH_35" },

// Hack the Planet
{ "ACH_035", "ACH_36" },

// The Last Tango
{ "ACH_036", "ACH_37" },

// Master the Terroir
{ "ACH_037", "ACH_38" },

// Ripe for the Picking
{ "ACH_038", "ACH_39" },

// Evil Wine Club
{ "ACH_039", "ACH_40" },

// Rich Harvest
{ "ACH_040", "ACH_41" },

// Vineyard Virtuoso
{ "ACH_041", "ACH_42" },

// Nightmare Fuel
{ "ACH_042", "ACH_43" },

// Bullet Train
{ "ACH_043", "ACH_44" },

// Count Down From 47
{ "ACH_044", "ACH_45" },

// Train Surfing
{ "ACH_045", "ACH_46" },

// Last Stop
{ "ACH_046", "ACH_47" },

// Seven Figures
{ "ACH_047", "ACH_48" },

// Hawkeye
{ "ACH_048", "ACH_49" },

// Pure Poetry
{ "ACH_049", "ACH_50" },

// Break the Bank
{ "ACH_050", "ACH_51" },

// Top of the Heap
{ "ACH_051", "ACH_52" },

// In a League of Their Own
{ "ACH_052", "ACH_53" },

// Never Knew What Hit Them
{ "ACH_053", "ACH_54" },

// Capital Punishment
{ "ACH_054", "ACH_55" },

// Island and Chill
{ "ACH_055", "ACH_56" },

// Null and Void
{ "ACH_056", "ACH_57" },

// Infiltrator
{ "ACH_057", "ACH_58" },

// Local Knowledge
{ "ACH_058", "ACH_59" },

// Damage Control
{ "ACH_059", "ACH_60" },

// Miami Wise
{ "ACH_060", "ACH_61" },

// Tactical Strike
{ "ACH_061", "ACH_62" },

// Dark Tourist
{ "ACH_062", "ACH_63" },

// Pirate Hunter
{ "ACH_063", "ACH_64" },

// Keys to the City
{ "ACH_064", "ACH_65" },

// Long Shot
{ "ACH_065", "ACH_66" },

// Pillar of the Community
{ "ACH_066", "ACH_67" },

// This is Maintenance
{ "ACH_067", "ACH_68" },

// Honorary Member
{ "ACH_068", "ACH_69" },

// Silent Sniper
{ "ACH_069", "ACH_70" },

// When No One Else Dares
{ "ACH_070", "ACH_71" },

// City of Light
{ "ACH_071", "ACH_72" },

// Die By the Sword
{ "ACH_072", "ACH_73" },

// Amalfi Pearl
{ "ACH_073", "ACH_74" },

// Too Big to Fail
{ "ACH_074", "ACH_75" },

// Ancient Marrakesh
{ "ACH_075", "ACH_76" },

// Perfectionist
{ "ACH_076", "ACH_77" },

// Shining Bright
{ "ACH_077", "ACH_78" },

// One Night in Bangkok
{ "ACH_078", "ACH_79" },

// Guerrilla Warfare
{ "ACH_079", "ACH_80" },

// Mission Complete
{ "ACH_080", "ACH_81" },

// A Long Time Coming
{ "ACH_081", "ACH_82" },

// Sayōnara
{ "ACH_082", "ACH_83" }


	};

	SteamUserStats()->RequestCurrentStats();
	for (auto& entry : epic_to_steam_ach_bindings) {
		bool is_unlocked;
		SteamUserStats()->GetAchievement(entry.second.c_str(), &is_unlocked);

		if (is_unlocked) {
			unlocked_steam_achievements.push_back(entry.second);
		}
	}
}

std::string AchievementsHandler::get_steam_ach_id_from_epic(std::string_view epic) {
	auto entry = epic_to_steam_ach_bindings.find(epic);
	auto to_return = entry == epic_to_steam_ach_bindings.end() ? "ERROR" : entry->second;
	return to_return;
}

void AchievementsHandler::UnlockSteamAchievement(std::string epic_id) {
	auto steam_ach_name = get_steam_ach_id_from_epic(epic_id);
	
	if (std::find(unlocked_steam_achievements.begin(), unlocked_steam_achievements.end(), steam_ach_name) != unlocked_steam_achievements.end()) {
		// Don't bother unlocked an achievement already unlocked
		return;
	}
	if (steam_ach_name != "ERROR") {
		log(std::string("Unlocking Epic Achievement: ") + epic_id + std::string(" on Steam: ") + steam_ach_name);
		SteamUserStats()->SetAchievement(steam_ach_name.c_str());
		SteamUserStats()->StoreStats();
	}
}

void AchievementsHandler::OnAchievementsStored(UserAchievementStored_t* pParam) {
	unlocked_steam_achievements.push_back(std::string(pParam->m_rgchAchievementName));
}

void AchievementsHandler::GetAllAchievementsForEpicAndUnlockThem(EOS_HPlatform platform, EOS_ProductUserId user_id) {
	GetAllAchievements(platform, user_id);
}

void PlayerAchievementStuff(EOS_HPlatform platform, EOS_ProductUserId user_id) {
	EOS_HAchievements Achievements = EOS_Platform_GetAchievementsInterface(platform);

	EOS_Achievements_GetPlayerAchievementCountOptions ACO = {};
	ACO.ApiVersion = EOS_ACHIEVEMENTS_GETPLAYERACHIEVEMENTCOUNT_API_LATEST;
	ACO.UserId = user_id;

	uint32_t AC = EOS_Achievements_GetPlayerAchievementCount(Achievements, &ACO);

	EOS_Achievements_CopyPlayerAchievementByIndexOptions CO = {};
	CO.ApiVersion = 1;
	CO.TargetUserId = user_id;
	CO.LocalUserId = user_id;

	for (CO.AchievementIndex = 0; CO.AchievementIndex < AC; ++CO.AchievementIndex) {
		EOS_Achievements_PlayerAchievement* Achievement = NULL;
		EOS_EResult CAR = EOS_Achievements_CopyPlayerAchievementByIndex(Achievements, &CO, &Achievement);
		if (CAR != EOS_EResult::EOS_Success) {
			log(std::string("ERROR: CPA FAILURE ") + EOS_EResult_ToString(CAR));
			break;
		}

		if (Achievement->UnlockTime != -1) {
			g_achievements_handler.UnlockSteamAchievement(std::string(Achievement->AchievementId));
		}
	}
}

void EOS_CALL QueryPlayerAchievementsReceivedCallbackFn(const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data) {
	if (Data->ResultCode != EOS_EResult::EOS_Success) {
		log(std::string("Error: Failed to get Player Achievements. Error: ") + std::string(EOS_EResult_ToString(Data->ResultCode)));
		return;
	}
	PlayerAchievementStuff(EOSPlatform, (EOS_ProductUserId)Data->ClientData);
}

void GetAllAchievements(EOS_HPlatform platform, EOS_ProductUserId user_id) {
	// HITMAN 3 uses ApiVersion 1, otherwise it errors as EOS_IncompatibleVersion
	EOS_Achievements_QueryPlayerAchievementsOptions QPAO = {};
	QPAO.ApiVersion = 1;
	QPAO.LocalUserId = user_id;
	QPAO.TargetUserId = user_id;
	EOSPlatform = platform;
	EOS_Achievements_QueryPlayerAchievements(EOS_Platform_GetAchievementsInterface(platform), &QPAO, user_id, QueryPlayerAchievementsReceivedCallbackFn);
}

#endif