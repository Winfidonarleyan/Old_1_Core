/*
Field * field = resultvip->Fetch();				
uint32 menu_id = field[0].GetUInt32();
std::string text = field[1].GetString();
*/

#include "ScriptPCH.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Common.h"
#include "Player.h"
#include "WorldSession.h"
#include "Language.h"
#include "SpellAuras.h"
#include "World.h"
#include "Config.h"
#include "SystemConfig.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "revision.h"
#include "GitRevision.h"
#include "TicketMgr.h"
#include "Unell_Script.h"

#define CONST_ARENA_RENAME 500
#define CONST_ARENA_CUSTOMIZE 500
#define CONST_ARENA_CHANGE_FACTION 1500
#define CONST_ARENA_CHANGE_RACE 1000
#define EMOTE_NO_VIP "Извините, этот нпц доступен только для VIP игроков..."
#define EMOTE_COOLDOWN "В этот раз не получится..."
#define EMOTE_NO_SICKENSS "На вас нету данной ауры!"
#define EMOTE_NO_DESERTER "На вас нету дезертира!"
#define EMOTE_COMBAT "Вы в бою!"
#define EMOTE_NO_ARENA_POINTS "Вам не хватает очков арены!"
#define EMOTE_ALREADY_ITEM "У вас уже есть эти сумки!"
#define EMOTE_NO_TOKEN "Вам не хватает VIP монет" 
#define MSG_RENAME_COMPLETE "Перезайдите в игру предварительно удалить папку Cache!"
#define MSG_CUSTOMIZE_COMPLETE "Перезайдите в игру предварительно удалить папку Cache!"
#define MSG_CHANGE_FACTION_COMPLETE "Перезайдите в игру предварительно удалить папку Cache!" 
#define MSG_CHANGE_RACE_COMPLETE "Перезайдите в игру предварительно удалить папку Cache!"
#define MSG_REMOVE_SICKNESS_COMPLETE "Удалена маска после смерти!"
#define MSG_REMOVE_DESERTER_COMPLETE "На Вас больше нету Дезертира!"
#define MSG_RESET_QUEST_STATUS_COMPLETE "Ваши Ежедневные и Еженедельные квесты были сброшены!"

//OnSpellCast
#define EMOTE_NO_VIP "Извините, этот нпц доступен только для VIP игроков..."
#define EMOTE_NO_DNTOKEN "Извините, вам не хватает DN монет"

// Етернал вов
class npc_premium_master_eternal : public CreatureScript
{
public:
    npc_premium_master_eternal() : CreatureScript("npc_premium_master_eternal") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		if (!player->GetSession()->IsPremium() && !player->isGameMaster())
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper(EMOTE_NO_VIP, player->GetGUID());
			return true;
		}
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Spell_Holy_DivineSpirit:30:28:-28|t Персонаж", GOSSIP_SENDER_MAIN, 1205);
        player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Spell_Holy_ElunesGrace:30:28:-28|t Баффы", GOSSIP_SENDER_MAIN, 1206);
        player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Spell_Shaman_Hex:30:28:-28|t Морфы", GOSSIP_SENDER_MAIN, 1203);
        player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/spell_shadow_deathscream:30:28:-28|t Удалить маску после смерти", GOSSIP_SENDER_MAIN, 1209);
        player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/ability_druid_cower:30:28:-28|t Удалить дизертира", GOSSIP_SENDER_MAIN, 1210);
        player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/achievement_quests_completed_01:30:28:-28|t Сбросить КД Квестов", GOSSIP_SENDER_MAIN, 1215);
        player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Spell_Holy_Absolution:30:28:-28|t VIP Магазин", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
		return true;
	}
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		if (!player->getAttackers().empty())
		{
			creature->MonsterWhisper(EMOTE_COMBAT, player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			return false;
		}

		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{

		case 100:
		OnGossipHello(player, creature);
		break;
		
            case 1203: // Morphs
				player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(5, "Star Craft Murlock", GOSSIP_SENDER_MAIN, 499);
                player->ADD_GOSSIP_ITEM(5, "Ambrose Boltspark", GOSSIP_SENDER_MAIN, 500);
                player->ADD_GOSSIP_ITEM(5, "Jonathan The Revelator", GOSSIP_SENDER_MAIN, 501);
                player->ADD_GOSSIP_ITEM(5, "Magi", GOSSIP_SENDER_MAIN, 502);
				player->ADD_GOSSIP_ITEM(5, "Worgen", GOSSIP_SENDER_MAIN, 503);
				player->ADD_GOSSIP_ITEM(5, "Pandaren Monk", GOSSIP_SENDER_MAIN, 504);
				player->ADD_GOSSIP_ITEM(5, "Crok Scourgebane", GOSSIP_SENDER_MAIN, 505);
				player->ADD_GOSSIP_ITEM(5, "Iron Mender", GOSSIP_SENDER_MAIN, 506);
				player->ADD_GOSSIP_ITEM(5, "Druid", GOSSIP_SENDER_MAIN, 507);
				player->ADD_GOSSIP_ITEM(5, "Priest", GOSSIP_SENDER_MAIN, 508);
				player->ADD_GOSSIP_ITEM(5, "Paladin", GOSSIP_SENDER_MAIN, 509);
				player->ADD_GOSSIP_ITEM(5, "Rogue", GOSSIP_SENDER_MAIN, 510);
				player->ADD_GOSSIP_ITEM(5, "Death Knight", GOSSIP_SENDER_MAIN, 511);
				player->ADD_GOSSIP_ITEM(5, "Warlock", GOSSIP_SENDER_MAIN, 512);
				player->ADD_GOSSIP_ITEM(5, "Warrior", GOSSIP_SENDER_MAIN, 513);
				player->ADD_GOSSIP_ITEM(5, "Mage", GOSSIP_SENDER_MAIN, 514);
				player->ADD_GOSSIP_ITEM(5, "Shaman", GOSSIP_SENDER_MAIN, 515);
				player->ADD_GOSSIP_ITEM(5, "Hunter", GOSSIP_SENDER_MAIN, 516);
				player->ADD_GOSSIP_ITEM(5, "Draenei Girl White", GOSSIP_SENDER_MAIN, 517);
				player->ADD_GOSSIP_ITEM(5, "Draenei Girl Black", GOSSIP_SENDER_MAIN, 518);
				player->ADD_GOSSIP_ITEM(0, "Demorph", GOSSIP_SENDER_MAIN, 519);
				player->ADD_GOSSIP_ITEM(4, "|TInterface/ICONS/Achievement_BG_returnXflags_def_WSG:30:28:-28|tНазад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
                break;
            case 499:
                player->CLOSE_GOSSIP_MENU(); // Starcraft Murlock
                player->SetDisplayId(29348);
                break;
            case 500:
                player->CLOSE_GOSSIP_MENU(); // Ambrose Boltspark
                player->SetDisplayId(28586);
                break;
            case 501:
                player->CLOSE_GOSSIP_MENU(); // Jonathan The Revelator
                player->SetDisplayId(15867);
                break;
            case 502:
                player->CLOSE_GOSSIP_MENU(); // Shattered Sun Magi
                player->SetDisplayId(22959);
                break;
            case 503:
                player->CLOSE_GOSSIP_MENU(); // Worgen
                player->SetDisplayId(657);
                break;
            case 504:
                player->CLOSE_GOSSIP_MENU(); // Pandaren Monk
                player->SetDisplayId(30414);
                break;
            case 505:
                player->CLOSE_GOSSIP_MENU(); // Crok Scourgebane
                player->SetDisplayId(30911);
                break;
            case 506:
                player->CLOSE_GOSSIP_MENU(); // Iron Mender
                player->SetDisplayId(26239);
                break;
            case 507:
                player->CLOSE_GOSSIP_MENU(); // Druid
                player->SetDisplayId(30472);
                break;
            case 508:
                player->CLOSE_GOSSIP_MENU(); // Priest
                player->SetDisplayId(21419);
                break;
            case 509:
                player->CLOSE_GOSSIP_MENU(); // Paladin
				player->SetDisplayId(29774);
				break;
			case 510:
				player->CLOSE_GOSSIP_MENU(); // Rogue
				player->SetDisplayId(30485);
				break;
			case 511:
				player->CLOSE_GOSSIP_MENU(); // Death Knight
				player->SetDisplayId(27153);
				break;
			case 512:
				player->CLOSE_GOSSIP_MENU(); // Warlock
				player->SetDisplayId(30487);
				break;
			case 513:
				player->CLOSE_GOSSIP_MENU(); // Warrior
				player->SetDisplayId(30474);
				break;
			case 514:
				player->CLOSE_GOSSIP_MENU(); // Mage
				player->SetDisplayId(30477);
				break;
			case 515:
				player->CLOSE_GOSSIP_MENU(); // Shaman 
				player->SetDisplayId(29768);
				break;
			case 516:
				player->CLOSE_GOSSIP_MENU(); // Hunter
				player->SetDisplayId(21379);
				break;
			case 517:
				player->CLOSE_GOSSIP_MENU(); // Coliseum Champion
				player->SetDisplayId(30634);
				break;
			case 518:
				player->CLOSE_GOSSIP_MENU(); // Coliseum Champion
				player->SetDisplayId(30771);
				break;
			case 519: // Demorph Player (remove all morphs)
				player->CLOSE_GOSSIP_MENU();
                player->DeMorph();
                break;
            case 1206: // Типа баффы
			    player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(5, "|TInterface/ICONS/Spell_Holy_ArcaneIntellect:30:28:-28|tЗабафать основные баффы", GOSSIP_SENDER_MAIN, 4000);
                player->ADD_GOSSIP_ITEM(5, "|TInterface/ICONS/Spell_Fire_Burnout:30:28:-28|t|cffFF0000Ядро Солнца", GOSSIP_SENDER_MAIN, 4004);
				player->ADD_GOSSIP_ITEM(5, "|TInterface/ICONS/Spell_Fire_FelFlameRing:30:28:-28|t|cffFF0000Ядро Солнца|cff0000FF VIPMASTER", GOSSIP_SENDER_MAIN, 4005);
				player->ADD_GOSSIP_ITEM(5, "|TInterface/ICONS/SPELL_HOLY_DEVOTIONAURA:30:28:-28|tНебесное Касание", GOSSIP_SENDER_MAIN, 4006);
				player->ADD_GOSSIP_ITEM(5, "|TInterface/ICONS/Ability_Warrior_RallyingCry:30:28:-28|tКомандирский крик", GOSSIP_SENDER_MAIN, 4001);
				player->ADD_GOSSIP_ITEM(5, "|TInterface/ICONS/Spell_Holy_GreaterBlessingofSanctuary:30:28:-28|tВеликое благословение", GOSSIP_SENDER_MAIN, 4002);
				player->ADD_GOSSIP_ITEM(5, "|TInterface/ICONS/Spell_Holy_GreaterBlessingofKings:30:28:-28|t Великое благословение могущества", GOSSIP_SENDER_MAIN, 4003);
				player->ADD_GOSSIP_ITEM(4, "|TInterface/ICONS/Achievement_BG_returnXflags_def_WSG:30:28:-28|tНазад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
                break;
            case 4000: // Power Word Fortitude
                player->CLOSE_GOSSIP_MENU();
				player->CastSpell(player, 48470, true); // Лапа
				player->CastSpell(player, 25898, true); // Каска
				player->CastSpell(player, 61316, true); // Инта Даларана
                player->CastSpell(player, 48162, true); // Прист СТАМИНА
				player->CastSpell(player, 48074, true); // Прист ДУХ
				player->CastSpell(player, 48170, true); // Молитва защиты от тёмной магии				
                break;
            case 4001: // Командирский крик (вар)
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 47440, true); // Командирский крик (вар)
                break;
            case 4002: // Великое благословение неприкосновенности
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 25899, true);
                break;
            case 4003: // Великое благословение могущества
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 48934, true);
                break;
			case 4004: // Ядро солнца
			    player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 90005, true);
                break;
			case 4005: // VIPMASTER
			    player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 90004, true);
                break;	
			case 4006: // Хрен знает что это такое
			    player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 84003, true);
                break;	
			case 1205:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Spell_Nature_Rune:30:28:-28|tСменить имя [500 VIP Money]", GOSSIP_SENDER_MAIN, 2000);
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_Mage_TormentOfTheWeak:30:28:-28|tСменить внешность [1000 VIP Money]", GOSSIP_SENDER_MAIN, 2001);
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_Racial_ShadowMeld:30:28:-28|tСменить фракцию [2500 VIP Money]", GOSSIP_SENDER_MAIN, 2002);
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_Rogue_ShadowDance:30:28:-28|tСменить рассу [2000 VIP Money]", GOSSIP_SENDER_MAIN, 2003);
				player->ADD_GOSSIP_ITEM(4, "|TInterface/ICONS/Achievement_BG_returnXflags_def_WSG:30:28:-28|tНазад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
				break;
			case 2000: // Rename
				// if (player->GetArenaPoints() < CONST_ARENA_RENAME)
				// {
					// creature->MonsterWhisper(EMOTE_NO_ARENA_POINTS, player->GetGUID());
					// player->CLOSE_GOSSIP_MENU();
				// }
				// else
				// {
					// player->CLOSE_GOSSIP_MENU();
					// player->SetAtLoginFlag(AT_LOGIN_RENAME);
					// player->ModifyArenaPoints(-CONST_ARENA_RENAME);
					// creature->MonsterWhisper(MSG_RENAME_COMPLETE, player->GetGUID());
				// }
				if (player->HasItemCount(3000003, 500))
				{	
					player->CLOSE_GOSSIP_MENU();
					player->DestroyItemCount(3000003, 500, true);
					player->SetAtLoginFlag(AT_LOGIN_RENAME);
					creature->MonsterWhisper(MSG_RENAME_COMPLETE, player->GetGUID());
					
				}
				else
				{	
					creature->MonsterWhisper(EMOTE_NO_TOKEN, player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}				
				break;
			case 2001: // Customize
				// if (player->GetArenaPoints() < CONST_ARENA_CUSTOMIZE)
				// {
					// creature->MonsterWhisper(EMOTE_NO_ARENA_POINTS, player->GetGUID());
					// player->CLOSE_GOSSIP_MENU();
				// }
				// else
				// {
					// player->CLOSE_GOSSIP_MENU();
					// player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
					// player->ModifyArenaPoints(-CONST_ARENA_CUSTOMIZE);
					// creature->MonsterWhisper(MSG_CUSTOMIZE_COMPLETE, player->GetGUID());
				// }
				if (player->HasItemCount(3000003, 1000))
				{	
					player->CLOSE_GOSSIP_MENU();
					player->DestroyItemCount(3000003, 1000, true);
					player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
					creature->MonsterWhisper(MSG_CUSTOMIZE_COMPLETE, player->GetGUID());
				}
				else
				{	
					creature->MonsterWhisper(EMOTE_NO_TOKEN, player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				break;				
			case 2002: // Change Faction
				// if (player->GetArenaPoints() < CONST_ARENA_CHANGE_FACTION)
				// {
					// creature->MonsterWhisper(EMOTE_NO_ARENA_POINTS, player->GetGUID());
					// player->CLOSE_GOSSIP_MENU();
				// }
				// else
				// {
					// player->CLOSE_GOSSIP_MENU();
					// player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
					// player->ModifyArenaPoints(-CONST_ARENA_CHANGE_FACTION);
					// creature->MonsterWhisper(MSG_CHANGE_FACTION_COMPLETE, player->GetGUID());
				// }
				if (player->HasItemCount(3000003, 2500))
				{	
					player->CLOSE_GOSSIP_MENU();
					player->DestroyItemCount(3000003, 2500, true);
					player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
					creature->MonsterWhisper(MSG_CHANGE_FACTION_COMPLETE, player->GetGUID());
					
				}
				else
				{	
					creature->MonsterWhisper(EMOTE_NO_TOKEN, player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 2003: // Change Race
				// if (player->GetArenaPoints() < CONST_ARENA_CHANGE_RACE)
				// {
					// creature->MonsterWhisper(EMOTE_NO_ARENA_POINTS, player->GetGUID());
					// player->CLOSE_GOSSIP_MENU();
				// }
				// else
				// {
					// player->CLOSE_GOSSIP_MENU();
					// player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
					// player->ModifyArenaPoints(-CONST_ARENA_CHANGE_RACE);
					// creature->MonsterWhisper(MSG_CHANGE_RACE_COMPLETE, player->GetGUID());
				// }				
				if (player->HasItemCount(3000003, 2000))
				{	
					player->CLOSE_GOSSIP_MENU();
					player->DestroyItemCount(3000003, 2000, true);
					player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
					creature->MonsterWhisper(MSG_CHANGE_RACE_COMPLETE, player->GetGUID());					
				}
				else
				{	
					creature->MonsterWhisper(EMOTE_NO_TOKEN, player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 1209: // Remove Resurrection Sickness Aura
				if(!player->HasAura(15007))
				{
					player->CLOSE_GOSSIP_MENU();
					creature->MonsterWhisper(EMOTE_NO_SICKENSS, player->GetGUID());
				}
				else
				{
					player->CLOSE_GOSSIP_MENU();
					player->RemoveAurasDueToSpell(15007);
					player->SetHealth(player->GetMaxHealth()); //Restore Health
					player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA)); // Restore Mana
					creature->MonsterWhisper(MSG_REMOVE_SICKNESS_COMPLETE, player->GetGUID());
				}
				break;
			case 1210: // Remove Deserter Aura
				if(!player->HasAura(26013))
				{
					player->CLOSE_GOSSIP_MENU();
					creature->MonsterWhisper(EMOTE_NO_DESERTER, player->GetGUID());
				}
				else
				{
					player->CLOSE_GOSSIP_MENU();
					player->RemoveAurasDueToSpell(26013);
					creature->MonsterWhisper(MSG_REMOVE_DESERTER_COMPLETE, player->GetGUID());
				}
				break;
			case 1215: // Reset Daily and Weekly Quest Cooldown
				player->CLOSE_GOSSIP_MENU();
				player->ResetDailyQuestStatus();
				player->ResetWeeklyQuestStatus();
				creature->MonsterWhisper(MSG_RESET_QUEST_STATUS_COMPLETE, player->GetGUID());
				break;
			case GOSSIP_OPTION_VENDOR: // Vip Магазин			
                player->GetSession()->SendListInventory(creature->GetGUID());
				break;	
		}
		return true;
	}
};

// Основа
class npc_premium_master : public CreatureScript
{
public:
    npc_premium_master() : CreatureScript("npc_premium_master") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		if (!player->GetSession()->IsPremium() && !player->isGameMaster())
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper(EMOTE_NO_VIP, player->GetGUID());
			return true;
		}
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Holy_DivineSpirit:30:|tПерсонаж", GOSSIP_SENDER_MAIN, 1205);
        player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Holy_ElunesGrace:30:|tБаффы", GOSSIP_SENDER_MAIN, 1206);
        player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Shaman_Hex:30:|tМорфы", GOSSIP_SENDER_MAIN, 1203);
        player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/spell_shadow_deathscream:30:|tУдалить маску после смерти", GOSSIP_SENDER_MAIN, 1209);
        player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ability_druid_cower:30:|tУдалить дизертира", GOSSIP_SENDER_MAIN, 1210);
        player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/achievement_quests_completed_01:30:|tСбросить КД Квестов", GOSSIP_SENDER_MAIN, 1215);
        player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Holy_Absolution:30:|tVIP Магазин", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		if (!player->getAttackers().empty())
		{
			creature->MonsterWhisper(EMOTE_COMBAT, player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			return false;
		}

		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{

		case 100:
		OnGossipHello(player, creature);
		break;
		
            case 1203: // Morphs
				player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(10, "Star Craft Murlock", GOSSIP_SENDER_MAIN, 499);
                player->ADD_GOSSIP_ITEM(10, "Ambrose Boltspark", GOSSIP_SENDER_MAIN, 500);
                player->ADD_GOSSIP_ITEM(10, "Jonathan The Revelator", GOSSIP_SENDER_MAIN, 501);
                player->ADD_GOSSIP_ITEM(10, "Magi", GOSSIP_SENDER_MAIN, 502);
				player->ADD_GOSSIP_ITEM(10, "Worgen", GOSSIP_SENDER_MAIN, 503);
				player->ADD_GOSSIP_ITEM(10, "Pandaren Monk", GOSSIP_SENDER_MAIN, 504);
				player->ADD_GOSSIP_ITEM(10, "Crok Scourgebane", GOSSIP_SENDER_MAIN, 505);
				player->ADD_GOSSIP_ITEM(10, "Iron Mender", GOSSIP_SENDER_MAIN, 506);
				player->ADD_GOSSIP_ITEM(10, "Druid", GOSSIP_SENDER_MAIN, 507);
				player->ADD_GOSSIP_ITEM(10, "Priest", GOSSIP_SENDER_MAIN, 508);
				player->ADD_GOSSIP_ITEM(10, "Paladin", GOSSIP_SENDER_MAIN, 509);
				player->ADD_GOSSIP_ITEM(10, "Rogue", GOSSIP_SENDER_MAIN, 510);
				player->ADD_GOSSIP_ITEM(10, "Death Knight", GOSSIP_SENDER_MAIN, 511);
				player->ADD_GOSSIP_ITEM(10, "Warlock", GOSSIP_SENDER_MAIN, 512);
				player->ADD_GOSSIP_ITEM(10, "Warrior", GOSSIP_SENDER_MAIN, 513);
				player->ADD_GOSSIP_ITEM(10, "Mage", GOSSIP_SENDER_MAIN, 514);
				player->ADD_GOSSIP_ITEM(10, "Shaman", GOSSIP_SENDER_MAIN, 515);
				player->ADD_GOSSIP_ITEM(10, "Hunter", GOSSIP_SENDER_MAIN, 516);
				player->ADD_GOSSIP_ITEM(10, "Draenei Girl White", GOSSIP_SENDER_MAIN, 517);
				player->ADD_GOSSIP_ITEM(10, "Draenei Girl Black", GOSSIP_SENDER_MAIN, 518);
				player->ADD_GOSSIP_ITEM(10, "Demorph", GOSSIP_SENDER_MAIN, 519);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Achievement_BG_returnXflags_def_WSG:30:|tНазад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
                break;
            case 499:
                player->CLOSE_GOSSIP_MENU(); // Starcraft Murlock
                player->SetDisplayId(29348);
                break;
            case 500:
                player->CLOSE_GOSSIP_MENU(); // Ambrose Boltspark
                player->SetDisplayId(28586);
                break;
            case 501:
                player->CLOSE_GOSSIP_MENU(); // Jonathan The Revelator
                player->SetDisplayId(15867);
                break;
            case 502:
                player->CLOSE_GOSSIP_MENU(); // Shattered Sun Magi
                player->SetDisplayId(22959);
                break;
            case 503:
                player->CLOSE_GOSSIP_MENU(); // Worgen
                player->SetDisplayId(657);
                break;
            case 504:
                player->CLOSE_GOSSIP_MENU(); // Pandaren Monk
                player->SetDisplayId(30414);
                break;
            case 505:
                player->CLOSE_GOSSIP_MENU(); // Crok Scourgebane
                player->SetDisplayId(30911);
                break;
            case 506:
                player->CLOSE_GOSSIP_MENU(); // Iron Mender
                player->SetDisplayId(26239);
                break;
            case 507:
                player->CLOSE_GOSSIP_MENU(); // Druid
                player->SetDisplayId(30472);
                break;
            case 508:
                player->CLOSE_GOSSIP_MENU(); // Priest
                player->SetDisplayId(21419);
                break;
            case 509:
                player->CLOSE_GOSSIP_MENU(); // Paladin
				player->SetDisplayId(29774);
				break;
			case 510:
				player->CLOSE_GOSSIP_MENU(); // Rogue
				player->SetDisplayId(30485);
				break;
			case 511:
				player->CLOSE_GOSSIP_MENU(); // Death Knight
				player->SetDisplayId(27153);
				break;
			case 512:
				player->CLOSE_GOSSIP_MENU(); // Warlock
				player->SetDisplayId(30487);
				break;
			case 513:
				player->CLOSE_GOSSIP_MENU(); // Warrior
				player->SetDisplayId(30474);
				break;
			case 514:
				player->CLOSE_GOSSIP_MENU(); // Mage
				player->SetDisplayId(30477);
				break;
			case 515:
				player->CLOSE_GOSSIP_MENU(); // Shaman 
				player->SetDisplayId(29768);
				break;
			case 516:
				player->CLOSE_GOSSIP_MENU(); // Hunter
				player->SetDisplayId(21379);
				break;
			case 517:
				player->CLOSE_GOSSIP_MENU(); // Coliseum Champion
				player->SetDisplayId(30634);
				break;
			case 518:
				player->CLOSE_GOSSIP_MENU(); // Coliseum Champion
				player->SetDisplayId(30771);
				break;
			case 519: // Demorph Player (remove all morphs)
				player->CLOSE_GOSSIP_MENU();
                player->DeMorph();
                break;
            case 1206: // Типа баффы
			    player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Holy_ArcaneIntellect:30:|tЗабафать основные баффы", GOSSIP_SENDER_MAIN, 4000);
                player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Warrior_RallyingCry:30:|tКомандирский крик", GOSSIP_SENDER_MAIN, 4001);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Holy_GreaterBlessingofSanctuary:30:|tВеликое благословение", GOSSIP_SENDER_MAIN, 4002);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Holy_GreaterBlessingofKings:30:|t Великое благословение могущества", GOSSIP_SENDER_MAIN, 4003);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Achievement_BG_returnXflags_def_WSG:30:|tНазад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
                break;
            case 4000: // Power Word Fortitude
                player->CLOSE_GOSSIP_MENU();
				player->CastSpell(player, 48470, true); // Лапа
				player->CastSpell(player, 25898, true); // Каска
				player->CastSpell(player, 61316, true); // Инта Даларана
                player->CastSpell(player, 48162, true); // Прист СТАМИНА
				player->CastSpell(player, 48074, true); // Прист ДУХ
				player->CastSpell(player, 48170, true); // Молитва защиты от тёмной магии				
                break;
            case 4001: // Командирский крик (вар)
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 47440, true); // Командирский крик (вар)
                break;
            case 4002: // Великое благословение неприкосновенности
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 25899, true);
                break;
            case 4003: // Великое благословение могущества
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 48934, true);
                break;
			case 1205:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Nature_Rune:30:|tСменить имя [500 Очков арены]", GOSSIP_SENDER_MAIN, 2000);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Mage_TormentOfTheWeak:30:|tСменить внешность [500 Очков арены]", GOSSIP_SENDER_MAIN, 2001);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Racial_ShadowMeld:30:|tСменить фракцию [1500 Очков арены]", GOSSIP_SENDER_MAIN, 2002);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Rogue_ShadowDance:30:|tСменить рассу [1000 Очков арены]", GOSSIP_SENDER_MAIN, 2003);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Achievement_BG_returnXflags_def_WSG:30:|tНазад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
				break;
			case 2000: // Rename
				if (player->GetArenaPoints() < CONST_ARENA_RENAME)
				{
					creature->MonsterWhisper(EMOTE_NO_ARENA_POINTS, player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->CLOSE_GOSSIP_MENU();
					player->SetAtLoginFlag(AT_LOGIN_RENAME);
					player->ModifyArenaPoints(-CONST_ARENA_RENAME);
					creature->MonsterWhisper(MSG_RENAME_COMPLETE, player->GetGUID());
				}		
				break;
			case 2001: // Customize
				if (player->GetArenaPoints() < CONST_ARENA_CUSTOMIZE)
				{
					creature->MonsterWhisper(EMOTE_NO_ARENA_POINTS, player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->CLOSE_GOSSIP_MENU();
					player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
					player->ModifyArenaPoints(-CONST_ARENA_CUSTOMIZE);
					creature->MonsterWhisper(MSG_CUSTOMIZE_COMPLETE, player->GetGUID());
				}
				break;				
			case 2002: // Change Faction
				if (player->GetArenaPoints() < CONST_ARENA_CHANGE_FACTION)
				{
					creature->MonsterWhisper(EMOTE_NO_ARENA_POINTS, player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->CLOSE_GOSSIP_MENU();
					player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
					player->ModifyArenaPoints(-CONST_ARENA_CHANGE_FACTION);
					creature->MonsterWhisper(MSG_CHANGE_FACTION_COMPLETE, player->GetGUID());
				}			
				break;
			case 2003: // Change Race
				if (player->GetArenaPoints() < CONST_ARENA_CHANGE_RACE)
				{
					creature->MonsterWhisper(EMOTE_NO_ARENA_POINTS, player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->CLOSE_GOSSIP_MENU();
					player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
					player->ModifyArenaPoints(-CONST_ARENA_CHANGE_RACE);
					creature->MonsterWhisper(MSG_CHANGE_RACE_COMPLETE, player->GetGUID());
				}
				break;
			case 1209: // Remove Resurrection Sickness Aura
				if(!player->HasAura(15007))
				{
					player->CLOSE_GOSSIP_MENU();
					creature->MonsterWhisper(EMOTE_NO_SICKENSS, player->GetGUID());
				}
				else
				{
					player->CLOSE_GOSSIP_MENU();
					player->RemoveAurasDueToSpell(15007);
					player->SetHealth(player->GetMaxHealth()); //Restore Health
					player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA)); // Restore Mana
					creature->MonsterWhisper(MSG_REMOVE_SICKNESS_COMPLETE, player->GetGUID());
				}
				break;
			case 1210: // Remove Deserter Aura
				if(!player->HasAura(26013))
				{
					player->CLOSE_GOSSIP_MENU();
					creature->MonsterWhisper(EMOTE_NO_DESERTER, player->GetGUID());
				}
				else
				{
					player->CLOSE_GOSSIP_MENU();
					player->RemoveAurasDueToSpell(26013);
					creature->MonsterWhisper(MSG_REMOVE_DESERTER_COMPLETE, player->GetGUID());
				}
				break;
			case 1215: // Reset Daily and Weekly Quest Cooldown
				player->CLOSE_GOSSIP_MENU();
				player->ResetDailyQuestStatus();
				player->ResetWeeklyQuestStatus();
				creature->MonsterWhisper(MSG_RESET_QUEST_STATUS_COMPLETE, player->GetGUID());
				break;
			case GOSSIP_OPTION_VENDOR: // Vip Магазин			
                player->GetSession()->SendListInventory(creature->GetGUID());
				break;	
		}
		return true;
	}
};

void CreateGossip(Player* player, uint32 menu_id) // CreateGossip(player, 4);
{
	QueryResult resultvip = WorldDatabase.PQuery("SELECT text FROM `have_vip` WHERE `menu_id` = '%u'", menu_id);
	if(resultvip)					
		{
			Field * field = resultvip->Fetch();								
			std::string text = field[0].GetString();							
			std::ostringstream gossiptext;
			gossiptext << text.c_str();
			player->ADD_GOSSIP_ITEM(10, gossiptext.str(), GOSSIP_SENDER_MAIN, menu_id);		
		}
}

void CreateGossip1(Player* player, uint32 menu_id) // CreateGossip1(player, 4);
{
	uint32 accid = player->GetSession()->GetAccountId();
	time_t curTime = time(NULL); 
	
	QueryResult resultvip = WorldDatabase.PQuery("SELECT cost, itemid, playertext, time FROM `have_vip` WHERE `menu_id` = '%u'", menu_id);
	if(resultvip)					
		{
			Field * field = resultvip->Fetch();
			uint32 cost = field[0].GetUInt32();
			uint32 itemid = field[1].GetUInt32();
			std::string playertext = field[2].GetString();
			uint32 viptime = field[3].GetUInt32();
			uint32 totaltime = uint32(curTime + viptime); // unsettime вооот
			
			if (player->HasItemCount(itemid, cost))
				{	
					player->DestroyItemCount(itemid, cost, true);
					player->CLOSE_GOSSIP_MENU(); 					
					ChatHandler(player->GetSession()).PSendSysMessage("%s", playertext.c_str());
					LoginDatabase.PExecute("DELETE FROM `account_premium` WHERE (`id`='%u');", accid);
					LoginDatabase.PExecute("INSERT INTO `account_premium` (`id`, `setdate`, `unsetdate`, `premium_type`, `active`) VALUES ('%u', '%u', '%u', '1', '1');", accid,  uint32(curTime), totaltime);
				}
				else
				{	
					ChatHandler handler(player->GetSession());
					ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemid);
					ItemLocale const* itemLocale = sObjectMgr->GetItemLocale(itemid);
					char const* name = NULL;
					if (itemLocale)
						name = itemLocale->Name[handler.GetSessionDbcLocale()].c_str();
					if ((!name || !*name) && itemTemplate)
						name = itemTemplate->Name1.c_str();
					if (!name)
						name = "Unknown item";
					
					handler.PSendSysMessage("|cffff0000# |cff14ECCFВам не хватает:|r |c%08x|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r", ItemQualityColors[itemTemplate ? itemTemplate->Quality : ITEM_QUALITY_POOR], itemid, name);
					player->CLOSE_GOSSIP_MENU();
				}
		}
}

void CreateGossip2(Player* player, uint32 menu_id) // CreateGossip1(player, 4);
{
	uint32 accid = player->GetSession()->GetAccountId();
	time_t curTime = time(NULL); 
	
	QueryResult resultvip = WorldDatabase.PQuery("SELECT cost, itemid, playertext, time FROM `have_vip` WHERE `menu_id` = '%u'", menu_id);
	if(resultvip)					
		{
			Field * field = resultvip->Fetch();
			uint32 cost = field[0].GetUInt32();
			uint32 itemid = field[1].GetUInt32();
			std::string playertext = field[2].GetString();
			uint32 viptime = field[3].GetUInt32();
			
			if (player->HasItemCount(itemid, cost))
				{	
					player->DestroyItemCount(itemid, cost, true);
					player->CLOSE_GOSSIP_MENU(); 					
					ChatHandler(player->GetSession()).PSendSysMessage("%s", playertext.c_str());
					LoginDatabase.PExecute("UPDATE `account_premium` SET `unsetdate`=`unsetdate`+%u WHERE (`id`='%u');", viptime, accid);
				}
				else
				{	
					ChatHandler handler(player->GetSession());
					ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemid);
					ItemLocale const* itemLocale = sObjectMgr->GetItemLocale(itemid);
					char const* name = NULL;
					if (itemLocale)
						name = itemLocale->Name[handler.GetSessionDbcLocale()].c_str();
					if ((!name || !*name) && itemTemplate)
						name = itemTemplate->Name1.c_str();
					if (!name)
						name = "Unknown item";
					
					handler.PSendSysMessage("|cffff0000# |cff14ECCFВам не хватает:|r |c%08x|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r", ItemQualityColors[itemTemplate ? itemTemplate->Quality : ITEM_QUALITY_POOR], itemid, name);
					player->CLOSE_GOSSIP_MENU();
				}
		}
}

void CreateGossip3(Player* player, uint32 menu_id) // CreateGossip3(player, 1);
{
	QueryResult resulttoken = WorldDatabase.PQuery("SELECT text FROM `have_token` WHERE `menu_id` = '%u'", menu_id);
	if(resulttoken)					
	{
		Field * field = resulttoken->Fetch();								
		std::string text = field[0].GetString();							
		std::ostringstream gossiptext;
		gossiptext << text.c_str();
		player->ADD_GOSSIP_ITEM(10, gossiptext.str(), GOSSIP_SENDER_MAIN, menu_id);		
	}
}

class Unell_VIP : public CreatureScript
{
public:
    Unell_VIP() : CreatureScript("Unell_VIP") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		if (!player->GetSession()->IsPremium())
		{		
			{
				CreateGossip(player, 1);
			}
			
			{
				CreateGossip(player, 2);
			}
			
			{
				CreateGossip(player, 3);
			}
			
			{
				CreateGossip(player, 7);
			}
			
		
		}
		if (player->GetSession()->IsPremium())
		{		
			{
				CreateGossip(player, 4);
			}

			{
				CreateGossip(player, 5);
			}

			{
				CreateGossip(player, 6);
			}
			
			{
				CreateGossip(player, 8);
			}
		}	
		
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
		player->PlayerTalkClass->ClearMenus();
        if (sender != GOSSIP_SENDER_MAIN)
            return true; 

        switch(action)
        {			
		case 1: // Покупка 1 день			
            CreateGossip1(player, action);
			break;
		
		case 2: // Покупка 1 день			
            CreateGossip1(player, action);
			break;
		
		case 3: // Покупка 1 день			
            CreateGossip1(player, action);
			break;	
		
		case 4: // Продлить 1 день			
			CreateGossip2(player, action);
            break;	
		
		case 5: // Продлить 7 дней
			CreateGossip2(player, action);
			break;
			
		case 6: // Продлить 7 дней
			CreateGossip2(player, action);
			break;
			
		case 7: // Покупака 3 дня
			CreateGossip2(player, action);
			break;
			
		case 8: // Продлить 3 дня
			CreateGossip2(player, action);
			break;
			
		break;
		
        return true; // invalid class, something fishy
        }
       
        return true;
    }
};

class Unell_VIP_Token : public CreatureScript
{
public:
    Unell_VIP_Token() : CreatureScript("Unell_VIP_Token") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		//player->ADD_GOSSIP_ITEM(10, "Узнать баланс", GOSSIP_SENDER_MAIN, 1);
		{
			CreateGossip3(player, 1);
		}
		{
			CreateGossip3(player, 2);
		}
		//player->ADD_GOSSIP_ITEM(10, "Перевести токенты в DN монеты", GOSSIP_SENDER_MAIN, 2);		
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        uint32 accid = player->GetSession()->GetAccountId();
		
		if (sender != GOSSIP_SENDER_MAIN)
            return true; 

        switch(action)
        {
			
		case 1: // Проверка баланса
			
			{
			QueryResult res = LoginDatabase.PQuery("SELECT token, refereraccid FROM account_token WHERE id = %u", accid);	
			if (res)
			{
			Field* field = res->Fetch();              
			uint32 token = field[0].GetUInt32();
			uint32 refid = field[1].GetUInt32();
			std::string username = Unell_Script::Unell_accuser(refid);
			
			
			if (ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
			ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВаш баланс: |cffff0000%u |cff14ECCFтокенов. Вас пригласил |cffff0000%s", token, username.c_str());
			
			if (!ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
			ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВаш баланс: |cffff0000%u |cff14ECCFтокенов.", token);
			
			}
			
			if (!res)				
				ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы ещё не совершали пожертвования на сервер");
			}
			
			player->CLOSE_GOSSIP_MENU();
			break;
			
		case 2: // Перевод монет
			
			{
			QueryResult res = LoginDatabase.PQuery("SELECT token, refereraccid FROM account_token WHERE id = %u", accid);	
			
			if (res)
			{
				Field* field = res->Fetch();              
				uint32 token = field[0].GetUInt32();
				uint32 refid = field[1].GetUInt32();
				std::string username = Unell_Script::Unell_accuser(refid);
			
			if (token == 0)
			{
				ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВаш баланс: |cffff0000%u |cff14ECCFтокенов. Перевод отклонён.", token);
			}
			
			if (token > 0)
			{
				uint32 reftoken = token / 10;
				LoginDatabase.PExecute("UPDATE `account_token` SET `token`= 0 WHERE (`id`='%u');", accid);
				
				if (ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
				{
					
				if (Unell_Script::Unell_acctoken(refid))
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", reftoken, refid);
				
				if (!Unell_Script::Unell_acctoken(refid))
					LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`, `refereraccid`) VALUES ('%u', '%u', '0')", refid, reftoken);
			
				}			
				
				QueryResult itemid1 = WorldDatabase.PQuery("SELECT itemid FROM `have_token` WHERE `menu_id` = 2");
				uint32 itemid2 = itemid1->Fetch()->GetUInt32();
				player->AddItem(itemid2, token);
				
				if (ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
				ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВаш реферал: |cffff0000%s |cff14ECCFполучает: |cffff0000%u |cff14ECCFтокенов.", username.c_str(), reftoken);				
			}
			
        	}
			if (!res)
			{
				ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы ещё не совершали пожертвования на сервер");
			}
			}
			player->CLOSE_GOSSIP_MENU();
			break;
			
			break;
		
			
		break;
		
        return true; // invalid class, something fishy
        }
       
        return true;
    }
};

/*class Unell_VIP_Spell : public PlayerScript
{
public:
    Unell_VIP_Spell() : PlayerScript("Unell_VIP_Spell") { }

    bool OnSpellCast(Player* player, Spell* spell, bool skipCheck)
    {
		//SpellInfo const* entry = sSpellMgr->GetSpellInfo(spell);
		ChatHandler handler(player);
		
		if (spell == 26662)
			handler.PSendSysMessage("|cffff0000# |cff14ECCFИспользование 26662");
		
    } 
};*/


void AddSC_Unell_VIP_System()
{
	new npc_premium_master_eternal;
	new npc_premium_master;
	new Unell_VIP;
	new Unell_VIP_Token;
	//new Unell_VIP_Spell;
}