/*
Field * field = resultvip->Fetch();				
uint32 menu_id = field[0].GetUInt32();
std::string text = field[1].GetString();
*/

/*double Roll1 = rand_chance();
double Roll2 = rand_norm();
handler->PSendSysMessage("|cffff0000# |cff00ff00rand_chance: %d", Roll1);
handler->PSendSysMessage("|cffff0000# |cff00ff00rand_norm: %d", Roll2);*/

#include "ScriptPCH.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Common.h"
#include "Player.h"
#include "WorldSession.h"
#include "Language.h"
#include "Log.h"
#include "SpellAuras.h"
#include "World.h"
#include "Config.h"
#include "SystemConfig.h"
#include "revision.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"
#include "GitRevision.h"
#include "TicketMgr.h"
#include "Battleground.h"
#include "Unell_Script.h"
#include "ScriptedCreature.h"

class Unell_commandscript : public CommandScript
{
public:
    Unell_commandscript() : CommandScript("Unell_commandscript") { }
	
	ChatCommand* GetCommands() const
    {
		static ChatCommand unellCommandOwnerNPCVendorTable[] = 
        {
            { "clear",         SEC_CONSOLE,  		false, &HandleUnellOwnNPCVendorClear,		"", NULL },
			{ "cost",          SEC_CONSOLE,  		false, &HandleUnellOwnNPCVendorCost,		"", NULL },
			{ "free",          SEC_CONSOLE,  		false, &HandleUnellOwnNPCVendorFree,		"", NULL },
			{ "addmoreitem",   SEC_CONSOLE,  		false, &HandleUnellOwnNPCVendorAddMoreItem,	"", NULL },
			{ "delmoreitem",   SEC_CONSOLE,  		false, &HandleUnellOwnNPCVendorDelMoreItem,	"", NULL },
			{ NULL,             0,              	false, NULL,   								"", NULL }
        };
		
		static ChatCommand unellCommandOwnerNPCTable[] = 
        {
            { "display",       SEC_CONSOLE,  		false, &HandleUnellOwnNPCDisplay,	"", NULL },
			{ "vendor",		   SEC_CONSOLE, 		false, NULL,             	   		"", unellCommandOwnerNPCVendorTable },
			{ NULL,             0,              	false, NULL,   						"", NULL }
        };
		
		static ChatCommand unellCommandOwnerTable[] = 
        {
            { "npc",		   SEC_CONSOLE, 		false, NULL,             	   		"", unellCommandOwnerNPCTable },
			{ "displayid",     SEC_CONSOLE, 		false, &HandleUnellDisplayID,		"", NULL },
			{ "alldisplayid",  SEC_CONSOLE, 		false, &HandleUnellDisplayIDAll,	"", NULL },
			{ "moredisplay",   SEC_CONSOLE, 		false, &HandleUnellDisplayIDMoreItem,"", NULL },
			{ "token",         SEC_CONSOLE, 		false, &HandleUnellToken, 			"", NULL },
			{ "addtokenchar",  SEC_CONSOLE, 		false, &HandleUnellTokenAddChar, 	"", NULL },
			{ "addtokenacc",   SEC_CONSOLE, 		false, &HandleUnellTokenAddAcc, 	"", NULL },			
			{ "test",          SEC_CONSOLE, 		false, &HandleUnellTest,			"", NULL },
			{ NULL,             0,              	false, NULL,   						"", NULL }
        };	
		
		static ChatCommand unellCommandCheatTable[] = 
        {
            { "cast",          SEC_ADMINISTRATOR,  	false, &HandleUnellCheatCast,		"", NULL },			
			{ NULL,             0,              	false, NULL,   						"", NULL }
        };
		
		static ChatCommand unellCommandTable[] = 
        {
            { "ilvl",          SEC_PLAYER,  		false, &HandleUnellItemLevel,		"", NULL },
			{ "buff",          SEC_PLAYER,  		false, &HandleBuffCommand,			"", NULL },
			{ "buffvor",       SEC_PLAYER,  		false, &HandleBuffCommandVor,		"", NULL },
			{ "home",          SEC_PLAYER,  		false, &HandleUnellHome,			"", NULL },			
			{ "info",          SEC_PLAYER,  		false, &HandleUnellInfo,			"", NULL },
			{ "heal",          SEC_PLAYER,  		false, &HandleUnellHeal, 			"", NULL },
			{ "timeap",		   SEC_PLAYER,  		false, &HandleUnellTimeAP, 			"", NULL },
			{ "viptime",	   SEC_PLAYER,  		false, &HandleUnellTimeVIP, 		"", NULL },
			{ "loot",		   SEC_PLAYER,  		false, &HandleUnellNpcLoot, 		"", NULL },
			{ "toptime",	   SEC_PLAYER,  		false, &HandleUnellTopTime, 		"", NULL },	
			{ NULL,             0,              	false, NULL,   						"", NULL }
        };
		
		static ChatCommand unellCommandGMTable[] = 
        {
			{ "add",           SEC_GAMEMASTER,  	false, &HandleUnellAdd, 			"", NULL },			
			{ NULL,             0,              	false, NULL,   						"", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "uplayer",		SEC_PLAYER,       	false, NULL,             	    	"", unellCommandTable },
			{ "ugm",			SEC_GAMEMASTER,    	false, NULL,             	   		"", unellCommandGMTable },
			{ "uown",			SEC_CONSOLE, 		false, NULL,             	   		"", unellCommandOwnerTable },
			{ "ucheat",			SEC_ADMINISTRATOR, 	false, NULL,             	   		"", unellCommandCheatTable },		
			{ "gmhelp",			SEC_PLAYER,      	false, &HandleGMHelpCommand,   		"", NULL },			
            { NULL,             0,                  false, NULL,                 		"", NULL }
        };
		
        return commandTable;
    }
	
	static bool HandleUnellTest(ChatHandler * handler, const char* args)
    {
		Player* player = handler->GetSession()->GetPlayer();
		
		if(player->duel)
		{
            handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы находитесь в дуэли");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы не находитесь в дуэли");
		return true;
    }
	
	static bool HandleUnellTopTime(ChatHandler* handler, const char* /*args*/)
    {
		Player *player = handler->GetSession()->GetPlayer();	
			
		if (!ConfigMgr::GetBoolDefault("Unell.Top.Enable", true))
		{
			handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Система топов сервера была отключена");
			handler->SetSentErrorMessage(true);
			return false;
		}
	
		if (ConfigMgr::GetBoolDefault("Unell.Top.TotalTime.Enable", true))
		{
			QueryResult totaltimeres = CharacterDatabase.PQuery("SELECT value FROM `worldstates` WHERE `entry` = '40000'");
			uint32 totaltime = totaltimeres->Fetch()->GetUInt32();
			std::string timeStr   = secsToTimeString(totaltime - time(NULL), false, false);
			handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Топ: |cFFFF0000Игровое время|cff6C8CD5. Окончание через |cff14ECCF%s", timeStr.c_str());
		}
		
		if (ConfigMgr::GetBoolDefault("Unell.Top.TotalKill.Enable", true))
		{
			QueryResult totalkillres = CharacterDatabase.PQuery("SELECT value FROM `worldstates` WHERE `entry` = '40001'");
			uint32 totalkill = totalkillres->Fetch()->GetUInt32();	
			std::string killStr   = secsToTimeString(totalkill - time(NULL), false, false);	
			handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Топ: |cFFFF0000PvP убийства|cff6C8CD5. Окончание через |cff14ECCF%s", killStr.c_str());		
		}
		
		return true;
	}
	
	static bool HandleUnellOwnNPCVendorAddMoreItem(ChatHandler * handler, const char* args)
    {
		Player* player = handler->GetSession()->GetPlayer();
		Creature* creature = handler->getSelectedCreature();
		
		if (!creature || creature->isPet())
        {
            handler->SendSysMessage("|cffff0000#|r Да выбери ты нпц нормального, ну чего за дела!");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!creature->isVendor())
        {
            handler->SendSysMessage("|cffff0000#|r Ты всерьёз думаешь, что это вендор?");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		uint32 Entry = creature->GetEntry();				
		CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
		CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
		char const* crename = NULL;
			if (CreatureLocale)
				crename = CreatureLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!crename || !*crename) && CreatureTemplate)
				crename = CreatureTemplate->Name.c_str();
			if (!crename)
				crename = "Unknown creature";
		
		char* Item_start = strtok((char*)args, "-");
		if (!Item_start)
		{
			handler->PSendSysMessage("|cFFFF0000# |cff00ff00Неверно введён начальный предмет");
			return false;
		}			
		
		uint32 Itemstart = (uint32)atoi(Item_start);
		
		char* Item_end = strtok(NULL, " ");
		if (!Item_end)
		{
			handler->PSendSysMessage("|cFFFF0000# |cff00ff00Неверно введён конечный предмет");
			return false;
		}
		
		uint32 Itemend = (uint32)atoi(Item_end);
		
		char* vcost = strtok(NULL, " ");
		uint32 extendedcost = vcost ? atol(vcost) : 0;
		
		uint32 maxcount = 0;
		uint32 incrtime = 0;		
		uint32 vendor_entry = Entry;
		uint32 step = 1;
		
		handler->PSendSysMessage("|cFFFF0000# |cff00ff00Заливка вещей в нпц-вендора: |cffff0000%s|cff00ff00 с ценой |cffff0000%u|cff00ff00.", crename, extendedcost);
		
		for(int i = Itemstart; i < Itemend+1; i++)
		{
			ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(i);
			ItemLocale const* itemLocale = sObjectMgr->GetItemLocale(i);
			char const* name = NULL;
			if (itemLocale)
				name = itemLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!name || !*name) && itemTemplate)
				name = itemTemplate->Name1.c_str();
			if (!name)
				name = "Unknown item";			
			
			if (!sObjectMgr->IsVendorItemValid(vendor_entry, i, maxcount, incrtime, extendedcost, handler->GetSession()->GetPlayer()))
			{
				handler->SetSentErrorMessage(true);				
				continue;
			}
			
			sObjectMgr->AddVendorItem(vendor_entry, i, maxcount, incrtime, extendedcost);
			handler->PSendSysMessage("|cFFFF0000# |cff00ff00%u.|r Заливка итема |c%08x|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r",
			step, ItemQualityColors[itemTemplate ? itemTemplate->Quality : ITEM_QUALITY_POOR], i, name);
			
			++step;
		}

		handler->PSendSysMessage("|cFFFF0000# |cff00ff00Готово. Залито |cFFFF0000%u |cff00ff00итемов.", step-1);
		return true;	
    }
	
	static bool HandleUnellOwnNPCVendorDelMoreItem(ChatHandler * handler, const char* args)
    {
		Player* player = handler->GetSession()->GetPlayer();
		Creature* creature = handler->getSelectedCreature();
		
		if (!creature || creature->isPet())
        {
            handler->SendSysMessage("|cffff0000#|r Да выбери ты нпц нормального, ну чего за дела!");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!creature->isVendor())
        {
            handler->SendSysMessage("|cffff0000#|r Ты всерьёз думаешь, что это вендор?");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		uint32 Entry = creature->GetEntry();
		CreatureTemplate const* cInfo = creature->GetCreatureTemplate();				
		CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
		CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
		char const* crename = NULL;
			if (CreatureLocale)
				crename = CreatureLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!crename || !*crename) && CreatureTemplate)
				crename = CreatureTemplate->Name.c_str();
			if (!crename)
				crename = "Unknown creature";
		
		char* Item_start = strtok((char*)args, "-");
		if (!Item_start)
			return false;
		
		char* Item_end = strtok(NULL, " ");
		if (!Item_end)
			return false;

		uint32 Itemstart = (uint32)atoi(Item_start);
		uint32 Itemend = (uint32)atoi(Item_end);
		
		uint32 vendor_entry = Entry;
		uint32 step = 1;		
		
		handler->PSendSysMessage("|cFFFF0000# |cff00ff00Чистка вещей у нпц-вендора: |cffff0000%s|cff00ff00.", crename);
		
		for(int i = Itemstart; i < Itemend+1; i++)
		{
			ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(i);
			ItemLocale const* itemLocale = sObjectMgr->GetItemLocale(i);
			char const* name = NULL;
			if (itemLocale)
				name = itemLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!name || !*name) && itemTemplate)
				name = itemTemplate->Name1.c_str();
			if (!name)
				name = "Unknown item";
			
			if (!sObjectMgr->RemoveVendorItem(Entry, i))
			{
				handler->PSendSysMessage(LANG_ITEM_NOT_IN_LIST, i);
				handler->SetSentErrorMessage(true);
				continue;
			}
			
			handler->PSendSysMessage("|cFFFF0000# |cff00ff00%u.|r Удаление итема |c%08x|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r",
			step, ItemQualityColors[itemTemplate ? itemTemplate->Quality : ITEM_QUALITY_POOR], i, name);
			
			++step;
		}
		
		handler->PSendSysMessage("|cFFFF0000# |cff00ff00Готово. Удалено |cFFFF0000%u |cff00ff00итемов.", step-1);

		return true;	
    }
	
	static bool HandleUnellOwnNPCVendorClear(ChatHandler * handler, const char* args)
    {
		Player* player = handler->GetSession()->GetPlayer();
		Creature* creature = handler->getSelectedCreature();
		
		if (!creature || creature->isPet())
        {
            handler->SendSysMessage("|cffff0000#|r Да выбери ты нпц, ну чего за дела!");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!creature->isVendor())
        {
            handler->SendSysMessage("|cffff0000#|r Ты всерьёз думаешь, что это вендор?");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		uint32 Entry = creature->GetEntry();
		CreatureTemplate const* cInfo = creature->GetCreatureTemplate();				
		CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
		CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
		char const* crename = NULL;
			if (CreatureLocale)
				crename = CreatureLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!crename || !*crename) && CreatureTemplate)
				crename = CreatureTemplate->Name.c_str();
			if (!crename)
				crename = "Unknown creature";
		
		QueryResult result = WorldDatabase.PQuery("SELECT item FROM npc_vendor WHERE entry = '%u'", Entry);
        if(result)
        {
			handler->PSendSysMessage("|cffff0000# |cff00ff00Удалены все предметы у нпц-вендора: |cffff0000%s|cff00ff00.", crename);
            do
            {
                Field * fields = result->Fetch();
                uint32 itemId = fields[0].GetUInt32();
				
                sObjectMgr->RemoveVendorItem(Entry, itemId);
            } 
            while(result->NextRow());
        }
		if(!result)
			handler->PSendSysMessage("|cffff0000# |cff00ff00Предметов у нпц-вендора: |cffff0000%s|cff00ff00, не обнаружено.", crename);
		
		return true;
    }
	
	static bool HandleUnellOwnNPCVendorCost(ChatHandler * handler, const char* args)
    {
		if (!*args)
		{
			handler->PSendSysMessage("|cffff0000# |cff00ff00Нужно указать айди цены.");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		Player* player = handler->GetSession()->GetPlayer();
		Creature* creature = handler->getSelectedCreature();	
		
		if (!creature || creature->isPet())
        {
            handler->SendSysMessage("|cffff0000#|r Да выбери ты нпц, ну чего за дела!");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!creature->isVendor())
        {
            handler->SendSysMessage("|cffff0000#|r Ты всерьёз думаешь, что это вендор?");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		uint32 costid = (uint32) atoi((char*)args);
		uint32 Entry = creature->GetEntry();
		CreatureTemplate const* cInfo = creature->GetCreatureTemplate();				
		CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
		CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
		char const* crename = NULL;
			if (CreatureLocale)
				crename = CreatureLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!crename || !*crename) && CreatureTemplate)
				crename = CreatureTemplate->Name.c_str();
			if (!crename)
				crename = "Unknown creature";	

		WorldDatabase.PExecute("UPDATE `npc_vendor` SET `ExtendedCost`='%u' WHERE (`entry`='%u')", costid, Entry);		
		handler->PSendSysMessage("|cffff0000# |cff00ff00Смена цены на все предметы у нпц-вендора: |cffff0000%s|cff00ff00. Новавя цена |cffff0000%u|cff00ff00. Перезагрузите таблицу |cffff0000npc_vendor|cff00ff00.", crename, costid);
		
		return true;	
    }
	
	static bool HandleUnellOwnNPCVendorFree(ChatHandler * handler, const char* args)
    {
		
		Player* player = handler->GetSession()->GetPlayer();
		Creature* creature = handler->getSelectedCreature();	
		
		if (!creature || creature->isPet())
        {
            handler->SendSysMessage("|cffff0000#|r Да выбери ты нпц, ну чего за дела!");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!creature->isVendor())
        {
            handler->SendSysMessage("|cffff0000#|r Ты всерьёз думаешь, что это вендор?");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		uint32 Entry = creature->GetEntry();
		CreatureTemplate const* cInfo = creature->GetCreatureTemplate();				
		CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
		CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
		char const* crename = NULL;
			if (CreatureLocale)
				crename = CreatureLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!crename || !*crename) && CreatureTemplate)
				crename = CreatureTemplate->Name.c_str();
			if (!crename)
				crename = "Unknown creature";
		

		WorldDatabase.PExecute("UPDATE `npc_vendor` SET `ExtendedCost`='0' WHERE (`entry`='%u')", Entry);		
		handler->PSendSysMessage("|cffff0000# |cff00ff00Теперь все предметы у нпц-вендора: |cffff0000%s|cff00ff00. Бесплатные. Перезагрузите таблицу |cffff0000npc_vendor|cff00ff00.", crename);
		
		return true;	
    }
	
	static bool HandleUnellOwnNPCDisplay(ChatHandler * handler, const char* args)
    {
		if (!*args)
            return false;
		
		uint32 displayId = (uint32) atoi((char*)args);
        Creature* creature = handler->getSelectedCreature();		
		Player* player = handler->GetSession()->GetPlayer();
		
		if (!creature || creature->isPet())
        {
            handler->SendSysMessage("|cffff0000#|r Да выбери ты нпц уже, ну чего за дела!");
            handler->SetSentErrorMessage(true);
            return false;
        }		
		
		creature->SetDisplayId(displayId);
        creature->SetNativeDisplayId(displayId);
        creature->SaveToDB();
		
		uint32 Entry = creature->GetEntry();
		CreatureTemplate const* cInfo = creature->GetCreatureTemplate();				
		CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
		CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
		char const* crename = NULL;
			if (CreatureLocale)
				crename = CreatureLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!crename || !*crename) && CreatureTemplate)
				crename = CreatureTemplate->Name.c_str();
			if (!crename)
				crename = "Unknown creature";
			
		QueryResult resultmodel = WorldDatabase.PQuery("SELECT modelid1 FROM creature_template WHERE entry = '%u'", Entry);
		
		if (!resultmodel)
			handler->PSendSysMessage("|cffff0000# |cff00ff00Не обнаружена модель.");

		uint32 modelid = resultmodel->Fetch()->GetUInt32();
		
		WorldDatabase.PExecute("UPDATE `creature_template` SET `modelid1`='%u' WHERE (`entry`='%u')", displayId, Entry);
		
		handler->PSendSysMessage("|cffff0000# |cff00ff00Для нпц: |cffff0000%s|cff00ff00, произведена смена модели с |cffff0000%u |cff00ff00на |cffff0000%u|cff00ff00.", crename, modelid, displayId);		
		return true;	
    }
	
	static bool HandleUnellItemLevel(ChatHandler * handler, const char* args)
    {
		Player* player = handler->GetSession()->GetPlayer();
		uint32 avgILvl = player->GetAverageItemLevel();
		handler->PSendSysMessage("|cffff0000# |cff00ff00Средний уровень ваших предметов:|r %u", avgILvl);
		return true;	
    }	
	
	static bool HandleUnellDisplayIDAll(ChatHandler * handler, const char* args)
    {

		if (!*args)
		{
			handler->PSendSysMessage("|cffff0000# |cff00ff00Нужно ввести название предметов на анг. либо на русском языке");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		std::string argstr = (char*)args;
		handler->PSendSysMessage("|cffff0000# |cff00ff00Запрос на дисплей айди преметов по названию:|r %s", argstr.c_str());

		Player* player = handler->GetSession()->GetPlayer();
		std::string namepart = args;
		std::wstring wnamepart;

		// converting string that we try to find to lower case
		if (!Utf8toWStr(namepart, wnamepart))
			return false;

		wstrToLower(wnamepart);

		bool found = false;
		uint32 count = 0;
		uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

		// Search in `item_template`
		ItemTemplateContainer const* its = sObjectMgr->GetItemTemplateStore();
		for (ItemTemplateContainer::const_iterator itr = its->begin(); itr != its->end(); ++itr)
		{
			int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
			if (loc_idx >= 0)
			{
				uint8 uloc_idx = uint8(loc_idx);
				if (ItemLocale const* il = sObjectMgr->GetItemLocale(itr->second.ItemId))
				{
					if (il->Name.size() > uloc_idx && !il->Name[uloc_idx].empty())
					{
						std::string name = il->Name[uloc_idx];

						if (Utf8FitTo(name, wnamepart))
						{
							if (maxResults && count++ == maxResults)
							{
								handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
								return true;
							}
							
							QueryResult resultdisplay = WorldDatabase.PQuery("SELECT displayid FROM item_template WHERE entry = '%u'", itr->second.ItemId);
							uint32 displayid = resultdisplay->Fetch()->GetUInt32();

							if (player->GetSession())
								handler->PSendSysMessage("|cffff0000# |cff00ff00Дисплей айди|r %u |cff00ff00-|r |cffffffff|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r ", displayid, itr->second.ItemId, name.c_str());
							else
								handler->PSendSysMessage("%d - %s", itr->second.ItemId, name.c_str());

							if (!found)
								found = true;

							continue;
						}
					}
				}
			}

			std::string name = itr->second.Name1;
			if (name.empty())
				continue;

			if (Utf8FitTo(name, wnamepart))
			{
				if (maxResults && count++ == maxResults)
				{
					handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
					return true;
				}
				
				QueryResult resultdisplay = WorldDatabase.PQuery("SELECT displayid FROM item_template WHERE entry = '%u'", itr->second.ItemId);
				uint32 displayid = resultdisplay->Fetch()->GetUInt32();

				if (player->GetSession())
					handler->PSendSysMessage("|cffff0000# |cff00ff00Дисплей айди|r %u |cff00ff00-|r |cffffffff|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r ", displayid, itr->second.ItemId, name.c_str());
				else
					handler->PSendSysMessage("%d - %s", itr->second.ItemId, name.c_str());

				if (!found)
					found = true;
			}
		}

		if (!found)
			handler->PSendSysMessage("|cffff0000# |cff00ff00Не найдено предметов по запросу:|r %s", argstr.c_str());

	return true;	
    }
	
	static bool HandleUnellDisplayID(ChatHandler * handler, const char* args)
    {

		if (!*args)
		{
			handler->PSendSysMessage("|cffff0000# |cff00ff00Нужно ввести айди итема, либо линкануть его");
			handler->SetSentErrorMessage(true);
			return false;
		}

		uint32 itemId = 0;

		if (args[0] == '[')                                        // [name] manual form
		{
			char* citemName = strtok((char*)args, "]");

			if (citemName && citemName[0])
			{
				std::string itemName = citemName+1;
				WorldDatabase.EscapeString(itemName);

				PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_ITEM_TEMPLATE_BY_NAME);
				stmt->setString(0, itemName);
				PreparedQueryResult result = WorldDatabase.Query(stmt);

				if (!result)
				{
					handler->PSendSysMessage(LANG_COMMAND_COULDNOTFIND, citemName+1);
					handler->SetSentErrorMessage(true);
					return false;
				}
				itemId = result->Fetch()->GetUInt16();
			}
			else
				return false;
		}
		else                                                    // item_id or [name] Shift-click form |cffffffff|Hitem:item_id:0:0:0|h[name]|h|r
		{
			char* cId = handler->extractKeyFromLink((char*)args, "Hitem");
			if (!cId)
				return false;
			itemId = atol(cId);
		}
		
		ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(itemId);
		if (!pProto)
		{
			handler->PSendSysMessage("|cffff0000# |cff00ff00Невозможно найти дисплей айди, верноятно предмета:|r %u |cff00ff00нет в базе", itemId);
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		QueryResult resultdisplay = WorldDatabase.PQuery("SELECT displayid FROM item_template WHERE entry = '%u'", itemId);
        uint32 displayid = resultdisplay->Fetch()->GetUInt32();
		
		ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemId);
		ItemLocale const* itemLocale = sObjectMgr->GetItemLocale(itemId);
		char const* name = NULL;
		if (itemLocale)
			name = itemLocale->Name[handler->GetSessionDbcLocale()].c_str();
		if ((!name || !*name) && itemTemplate)
			name = itemTemplate->Name1.c_str();
		if (!name)
			name = "Unknown item"; // |cffff0000# |cff00ff00Дисплей айди|r %u |cff00ff00-|r 
		
		handler->PSendSysMessage("|cffff0000# |cff00ff00Дисплей айди|r %u |cff00ff00-|r |c%08x|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r",
		displayid, ItemQualityColors[itemTemplate ? itemTemplate->Quality : ITEM_QUALITY_POOR], itemId, name);
		return true;
    }
	
	static bool HandleUnellDisplayIDMoreItem(ChatHandler * handler, const char* args)
    {

		if (!*args)
		{
			handler->PSendSysMessage("|cffff0000# |cff00ff00Нужно ввести диапазон предметов");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		char* Item_start = strtok((char*)args, "-");
		if (!Item_start)
		{
			handler->PSendSysMessage("|cFFFF0000# |cff00ff00Неверно введён начальный предмет");
			return false;
		}			
		
		uint32 Itemstart = (uint32)atoi(Item_start);
		
		char* Item_end = strtok(NULL, " ");
		if (!Item_end)
		{
			handler->PSendSysMessage("|cFFFF0000# |cff00ff00Неверно введён конечный предмет");
			return false;
		}
		
		uint32 Itemend = (uint32)atoi(Item_end);	
	
		for(int32 i = Itemstart; i < Itemend+1; i++)
		{
			uint32 itemId = i;
			
			ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(itemId);
			if (!pProto)
			{
				handler->PSendSysMessage("|cffff0000# |cff00ff00Невозможно найти дисплей айди, верноятно предмета:|r %u |cff00ff00нет в базе", itemId);
				handler->SetSentErrorMessage(true);
				continue;
			}
			
			QueryResult resultdisplay = WorldDatabase.PQuery("SELECT displayid FROM item_template WHERE entry = '%u'", itemId);
			uint32 displayid = resultdisplay->Fetch()->GetUInt32();
			
			ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemId);
			ItemLocale const* itemLocale = sObjectMgr->GetItemLocale(itemId);
			char const* name = NULL;
			if (itemLocale)
				name = itemLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!name || !*name) && itemTemplate)
				name = itemTemplate->Name1.c_str();
			if (!name)
				name = "Unknown item"; // |cffff0000# |cff00ff00Дисплей айди|r %u |cff00ff00-|r 
			
			handler->PSendSysMessage("|cffff0000# |cff00ff00Айди|r %u |cff00ff00Дисплей|r %u |cff00ff00-|r |c%08x|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r",
			itemId, displayid, ItemQualityColors[itemTemplate ? itemTemplate->Quality : ITEM_QUALITY_POOR], itemId, name);
			}
		
		return true;
    }
	
	static bool HandleUnellCheatCast(ChatHandler * handler, const char * args)
    {
        if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
            return false;

        std::string argstr = (char*)args;

        if (!*args)
            argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(CHEAT_CASTTIME)) ? "off" : "on";

        if (argstr == "off")
        {
            handler->GetSession()->GetPlayer()->SetCommandStatusOff(CHEAT_CASTTIME);
            handler->SendSysMessage("|cFFFF0000#|cff6C8CD5 Чит на произнесение заклинаний: |cffff0000Выключен.");
            return true;
        }
        else if (argstr == "on")
        {
            handler->GetSession()->GetPlayer()->SetCommandStatusOn(CHEAT_CASTTIME);
            handler->SendSysMessage("|cFFFF0000#|cff6C8CD5 Чит на произнесение заклинаний: |cff00ff00Включен.");
            return true;
        }
        return false;
    }
	
	static bool HandleUnellNpcLoot(ChatHandler * handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
		Creature* target = handler->getSelectedCreature();		
		
        if (!target)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }		
		
		uint32 Entry = target->GetEntry();
		CreatureTemplate const* cInfo = target->GetCreatureTemplate();
		uint32 Entryloot = cInfo->lootid;		
		CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
		CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
		char const* crename = NULL;
			if (CreatureLocale)
				crename = CreatureLocale->Name[handler->GetSessionDbcLocale()].c_str();
			if ((!crename || !*crename) && CreatureTemplate)
				crename = CreatureTemplate->Name.c_str();
			if (!crename)
				crename = "Unknown creature";				
		
		
		
		QueryResult resultloot = WorldDatabase.PQuery("SELECT item, mincountOrRef, maxcount FROM creature_loot_template WHERE entry = '%u'", Entryloot);
		if(resultloot)
				{
					handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Информация о луте существа: |cffff0000%s", crename);
					do
					{
						Field * fields = resultloot->Fetch();
						uint32 itemid = fields[0].GetUInt32();
						int32 mincount = fields[1].GetInt32();	
						uint32 maxcount = fields[2].GetUInt32();

						ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemid);
						ItemLocale const* itemLocale = sObjectMgr->GetItemLocale(itemid);
						char const* name = NULL;
						if (itemLocale)
							name = itemLocale->Name[handler->GetSessionDbcLocale()].c_str();
						if ((!name || !*name) && itemTemplate)
							name = itemTemplate->Name1.c_str();
						if (!name)
							name = "Unknown item";
						
						if (mincount < 0)
							{
								handler->PSendSysMessage("|cFFFF0000 - Reference |c%08x|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r",
								ItemQualityColors[itemTemplate ? itemTemplate->Quality : ITEM_QUALITY_POOR], itemid, name);
							}
						else
						{
							handler->PSendSysMessage("|cFFFF0000 - От |cff14ECCF%u |cFFFF0000до |cff14ECCF%u |c%08x|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r",
								mincount, maxcount, ItemQualityColors[itemTemplate ? itemTemplate->Quality : ITEM_QUALITY_POOR], itemid, name);
						}	
					}
					while(resultloot->NextRow());
				}
				
				if(!resultloot)
				{
					handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Существо: |cffff0000%s|cff6C8CD5, не имеет лута.", crename);
				}
				
		return true;			
    }
	
	static bool HandleGMHelpCommand(ChatHandler * handler, const char * /*args*/)
    {
        Player *player = handler->GetSession()->GetPlayer();
		
		std::string NameLink = handler->GetNameLink(player);
		char msg[250];
		snprintf(msg, 250, "|cFFFF0000# |cff6C8CD5Игрок |cff14ECCF%s |cff6C8CD5желает помощи от состава игровых мастеров", NameLink.c_str());
		sWorld->SendGMText(LANG_GM_BROADCAST, msg);
		
		bool first = true;
        bool footer = false;

        TRINITY_READ_GUARD(HashMapHolder<Player>::LockType, *HashMapHolder<Player>::GetLock());
        HashMapHolder<Player>::MapType const& m = sObjectAccessor->GetPlayers();
        for (HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
        {
            AccountTypes itrSec = itr->second->GetSession()->GetSecurity();
            if ((itr->second->isGameMaster() || (!AccountMgr::IsPlayerAccount(itrSec) && itrSec <= AccountTypes(sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_GM_LIST)))) &&
                (!handler->GetSession() || itr->second->IsVisibleGloballyFor(handler->GetSession()->GetPlayer())))
            {
                if (first)
                {
                    first = false;
                    footer = true;
                }
                char const* name = itr->second->GetName();
                uint8 security = itrSec;
                uint8 max = ((16 - strlen(name)) / 2);
                uint8 max2 = max;
                if ((max + max2 + strlen(name)) == 16)
                    max2 = max - 1;
                if (handler->GetSession())
                    handler->PSendSysMessage("|cFFFF0000# |cFFFF0000%s|cff6C8CD5 Уровень доступа |cff14ECCF%u", name, security);
                else
                    handler->PSendSysMessage("|%*s%s%*s|   %u  |", max, " ", name, max2, " ", security);
            }
        }
        if (footer)
           handler->SendSysMessage("|cFFFF0000#|cff6C8CD5 Вам ответит один из этих игровых мастеров нашего сервера");
        if (first)
			handler->SendSysMessage("|cFFFF0000#|cff6C8CD5 В данный момент сейчас нет онлайн игровых мастеров");		
		return true;
    }
	
	static bool HandleBuffCommand(ChatHandler * handler, const char * args)
    {
        Player *player = handler->GetSession()->GetPlayer();	
		
		if(player->duel || player->GetMap()->IsBattleArena() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		player->RemoveAurasByType(SPELL_AURA_MOUNTED);		
		uint32 auras[] = { 57623, 25898, 48470, 48162, 48074, 6346, 61316 };
		static const size_t sizeaura = sizeof(auras) / sizeof(*auras);
		for(int i = 0; i < sizeaura; i++)
		player->CastSpell(player, auras[i], true);	
	    handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы получили обычные баффы.");
		return true;
    }
	
	static bool HandleBuffCommandVor(ChatHandler * handler, const char * args)
    {
        Player *player = handler->GetSession()->GetPlayer();

		if(!player->GetSession()->IsPremium())
		{
			handler->SendSysMessage("Вы не являетсь VIP игроком");
            handler->SetSentErrorMessage(true);
			return false;
		}
		else
		{
			if(player->duel || player->GetMap()->IsBattleArena() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
			{
				handler->SendSysMessage(VIP_CANT_DO);
				handler->SetSentErrorMessage(true);
				return false;
			}
			
			player->RemoveAurasByType(SPELL_AURA_MOUNTED);			
			uint32 auras[] = {23735, 23736, 23737, 23738, 23766, 23767, 23768, 23769};
			static const size_t sizeaura = sizeof(auras) / sizeof(*auras);		
			for(int i = 0; i < sizeaura; i++)
			player->CastSpell(player, auras[i], true);	
			handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы получили баффы Воржея.");
			return true;
		}
		return false;
    }
	
	static bool HandleUnellHome(ChatHandler* handler, const char* /*args*/)
    {
        Player *chr = handler->GetSession()->GetPlayer();        

        if(chr->isInCombat() || chr->isInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Сейчас вы будете дома :)");
		chr->RemoveSpellCooldown(8690, true);
        chr->CastSpell(chr,8690,false);
		chr->RemoveSpellCooldown(8690, true);
        return true;
    }
	
	static bool HandleUnellHeal(ChatHandler* handler, const char* /*args*/)
    {
        Player *chr = handler->GetSession()->GetPlayer();
		uint32 PvPZoneID = ConfigMgr::GetIntDefault("Unell.PvP.Announce.Kill.ZoneID", 17);

        if(chr->GetZoneId() == PvPZoneID || chr->isInCombat() || chr->duel || chr->isInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }

		chr->CastSpell(chr,23965,false);
        return true;
    }
	
	static bool HandleUnellInfo(ChatHandler* handler, const char* args)
    {
        Player* target = handler->getSelectedPlayer();

		if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		uint32 accid = target->GetSession()->GetAccountId();
		std::string gmlvl;		
		std::string tNameLink = handler->GetNameLink(target);
		int64 muteTime        = target->GetSession()->m_muteTime;
		int64 totaltime       = target->GetTotalPlayedTime();
		std::string timeStr   = secsToTimeString(totaltime, false, false);
		uint32 latency     	  = target->GetSession()->GetLatency();
		
		
		
		switch(target->GetSession()->GetSecurity())
		{
			case 0:            gmlvl = "Игрок";                    break;		
		    case 1:            gmlvl = "Модератор";                break;
            case 2:            gmlvl = "Старший Модератор";        break;
		    case 3:            gmlvl = "Игровой Мастер";           break;
		    case 4:            gmlvl = "Старший Игровой Мастер";   break;
		    case 5:            gmlvl = "Администратор";            break;		    
			case 6:            gmlvl = "Создатель";                break;	
        }

		handler->PSendSysMessage("|cffff0000# |cff00ff00Информация о игроке %s", tNameLink.c_str());
		handler->PSendSysMessage("|cffff0000# |cff00ff00Уровень доступа:|r |cff14ECCF%s", gmlvl.c_str());
		if(target->GetSession()->IsPremium())
			{
				QueryResult result = LoginDatabase.PQuery("SELECT unsetdate FROM account_premium WHERE id = %u", accid);
				if (result)
				{
					Field * field = result->Fetch();
					uint32 unsetdate = field[0].GetUInt32();
					std::string timeStr   = secsToTimeString(unsetdate - time(NULL), false, false);
					handler->PSendSysMessage("|cffff0000# |cff00ff00Статус VIP аккаунта:|r |cff14ECCFИмеет VIP");
					handler->PSendSysMessage("|cFFFF0000#|cff00ff00 Премиум аккаунт закончится через: |cff14ECCF%s", timeStr.c_str());
				}	
			}
		else
            handler->PSendSysMessage("|cffff0000# |cff00ff00Статус VIP аккаунта:|r |cff14ECCFНе имеет VIP");
		
		if (!target->CanSpeak())
		{
			if (muteTime > 0)
				handler->PSendSysMessage("|cffff0000# |cff00ff00Оставшееся время мута: |cff14ECCF%s", secsToTimeString(muteTime - time(NULL), false, false).c_str());
		}
		
		handler->PSendSysMessage("|cffff0000# |cff00ff00Игровое время: |cff14ECCF%s", timeStr.c_str());
		if (latency == 0)
			handler->PSendSysMessage("|cffff0000# |cff00ff00Задержка: |cff14ECCFНеопределенно");
		else
		handler->PSendSysMessage("|cffff0000# |cff00ff00Задержка: |cff14ECCF%u мс", latency);
		
		return true;
    }
	
	static bool HandleUnellUnixTime(ChatHandler* handler, const char* /*args*/)
    {
		time_t curTime = time(NULL);		
		uint32 dtime = uint32(curTime + 86400);
		uint32 wtime = uint32(curTime + 604800);
		uint32 mtime = uint32(curTime + 2592000);
        Player *player = handler->GetSession()->GetPlayer();
		handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Сейчас время |cFFFF0000%u", uint32(curTime));
		handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Время через 7 дней |cFFFF0000%u", dtime);
		handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Время через 30 дней |cFFFF0000%u", mtime);
        return true;
    }

	static bool HandleUnellAdd(ChatHandler* handler, const char* args)
	{
		if (!ConfigMgr::GetBoolDefault("Unell.Command.Add", true))
		{
			handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Эта команда была отключена");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		Player* target = handler->getSelectedPlayer();
		
		if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }	
		
		uint32 itemId = ConfigMgr::GetIntDefault("Unell.Command.Add.itemId", 0);
		Player* playerses = handler->GetSession()->GetPlayer();
		Player* plTarget = handler->getSelectedPlayer();
		

		char* px = strtok((char*)args, " ");
		if (!px)
			return false;

		int32 count = (uint32)atoi(px);
		
		if (count == 0)
			count = 1;
		
		if (!plTarget)
			plTarget = playerses;

		sLog->outDetail(handler->GetSession()->GetTrinityString(LANG_ADDITEM), itemId, count);

	   
		//Subtract
		if (count < 0)
		{
			plTarget->DestroyItemCount(itemId, -count, true, false);
			handler->PSendSysMessage(LANG_REMOVEITEM, itemId, -count, handler->GetNameLink(plTarget).c_str());
			return true;
		}

		//Adding items
		uint32 noSpaceForCount = 0;

		// check space and find places
		ItemPosCountVec dest;
		InventoryResult msg = plTarget->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, count, &noSpaceForCount);
		if (msg != EQUIP_ERR_OK)                               // convert to possible store amount
			count -= noSpaceForCount;

		if (count == 0 || dest.empty())                         // can't add any
		{
			handler->PSendSysMessage(LANG_ITEM_CANNOT_CREATE, itemId, noSpaceForCount);
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		Item* item = plTarget->StoreNewItem(dest, itemId, true, Item::GenerateItemRandomPropertyId(itemId));

		// remove binding (let GM give it to another player later)
		if (playerses == plTarget)
			for (ItemPosCountVec::const_iterator itr = dest.begin(); itr != dest.end(); ++itr)
				if (Item* item1 = playerses->GetItemByPos(itr->pos))
					item1->SetBinding(false);

		if (count > 0 && item)
		{
			playerses->SendNewItem(item, count, false, true);
			if (playerses != plTarget)
				plTarget->SendNewItem(item, count, true, false);
		}

		if (noSpaceForCount > 0)
			handler->PSendSysMessage(LANG_ITEM_CANNOT_CREATE, itemId, noSpaceForCount);
		
		std::string gmname = playerses->GetName();
		std::string targetname = target->GetName();
		uint32 gmaccid = playerses->GetSession()->GetAccountId();
		uint32 targetaccid = target->GetSession()->GetAccountId();
		time_t curTime = time(NULL);
		uint32 localtime = curTime;

		CharacterDatabase.PExecute("INSERT INTO `logs_add` (`gmplayer`, `gmaccount`, `count`, `time`, `targetname`, `targetacc`) VALUES ('%s', '%u', '%u', '%u', '%s', '%u')", gmname.c_str(), gmaccid, count, localtime, targetname.c_str(), targetaccid);
		
		return true;
	}
	
	static bool HandleUnellTimeAP(ChatHandler* handler, const char* /*args*/)
    {
        QueryResult result1 = CharacterDatabase.PQuery("SELECT value FROM worldstates WHERE entry = 20001");
            if (result1)
            {
            Field * fields = NULL;
            fields = result1->Fetch();
            uint32 value = fields[0].GetUInt32();
			std::string timeStr = secsToTimeString(value - time(NULL), false, false);
			handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Начисление очков арены через |cff14ECCF%s", timeStr.c_str());			
		}
        return true;
    }
	
	static bool HandleUnellTimeVIP(ChatHandler* handler, const char* /*args*/)
    {
		Player *player = handler->GetSession()->GetPlayer();
		uint32 accid = player->GetSession()->GetAccountId();
		QueryResult result = LoginDatabase.PQuery("SELECT unsetdate FROM account_premium WHERE id = %u", accid);
		if (result)
		{
			Field * field = result->Fetch();
			uint32 unsetdate = field[0].GetUInt32();
			std::string timeStr   = secsToTimeString(unsetdate - time(NULL), false, false);
			handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Премиум аккаунт закончится через |cff14ECCF%s", timeStr.c_str());
		}
	    return true;
	
	}
	
	static bool HandleUnellToken(ChatHandler* handler, const char* /*args*/)
	{
		Player *player = handler->GetSession()->GetPlayer();
		uint32 accid = player->GetSession()->GetAccountId();
		handler->PSendSysMessage("|cFFFF0000#|cff6C8CD5 Проверка донат валюты у игроков");
		QueryResult result = CharacterDatabase.PQuery("SELECT owner_guid, count FROM item_instance WHERE `itemEntry` = '120000' AND `count` > '500' ORDER BY count DESC LIMIT 10");
        if(result)
        {
            do
            {
                Field * fields = result->Fetch();
				uint32 owner_guid = fields[0].GetUInt32();
                uint32 count = fields[1].GetUInt32();
				QueryResult namechar = CharacterDatabase.PQuery("SELECT guid, name FROM characters WHERE guid = %u", owner_guid);
				 if(namechar)
					{
						Field * fields = namechar->Fetch();
						uint32 guid = fields[0].GetUInt32();
						std::string name = fields[1].GetString();
						handler->PSendSysMessage("|cffff0000# |cff14ECCFНик: |cffff0000%s|cff14ECCF, Количество валюты: |cffff0000%u", name.c_str(), count);
					}
            } 
            while(result->NextRow());
        }
        return true;
	}
	
	static bool HandleUnellTokenAddChar(ChatHandler* handler, const char* args)
	{
		char* nameStr;
		char* tokenStr;
		handler->extractOptFirstArg((char*)args, &nameStr, &tokenStr);
		
		if (!tokenStr)
        return false;
	
		Player* target;
		uint64 target_guid;
		std::string target_name;
		if (!handler->extractPlayerTarget(nameStr, &target, &target_guid, &target_name))
        return false;

		uint32 accountId = target ? target->GetSession()->GetAccountId() : sObjectMgr->GetPlayerAccountIdByGUID(target_guid);
		if (!target)
        if (WorldSession* session = sWorld->FindSession(accountId))
            target = session->GetPlayer();
		
		uint32 token = uint32 (atoi (tokenStr));
			
		LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", token, accountId);
		LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`) VALUES ('%u', '%u');", accountId, token);
				
		QueryResult result = LoginDatabase.PQuery("SELECT id, username FROM account WHERE id = %u", accountId);
		if(result)
		{
			Field * fields = result->Fetch();
			uint32 id = fields[0].GetUInt32();
			std::string username = fields[1].GetString();
			QueryResult res = LoginDatabase.PQuery("SELECT token FROM account_token WHERE id = %u", accountId);	
			if (res)
			{
			Field* field = res->Fetch();              
			uint32 balance = field[0].GetUInt32();
			handler->PSendSysMessage("|cffff0000# |cff14ECCFВыдача на аккаунт: |cffff0000%s|cff14ECCF количеством: |cffff0000%u |cff14ECCFтокенов. Баланс |cffff0000%u", username.c_str(), token, balance); 
			}
			
		}		
		
        return true;
	}

	static bool HandleUnellTokenAddAcc(ChatHandler* handler, const char* args)
	{
		char* nameStr;
		char* tokenStr;
		handler->extractOptFirstArg((char*)args, &nameStr, &tokenStr);
		
		if (!tokenStr)
        return false;
		
		std::string p_username = strtok ((char*)args, " ");
		
		QueryResult accname = LoginDatabase.PQuery("SELECT id FROM account WHERE username = '%s';", p_username.c_str());
		
		if(accname)
		{
			
			Field * fields = accname->Fetch();
			uint32 accountId = fields[0].GetUInt32();		
			uint32 token = uint32 (atoi (tokenStr));
			
			LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", token, accountId);
			LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`) VALUES ('%u', '%u');", accountId, token);
			
			QueryResult res = LoginDatabase.PQuery("SELECT token FROM account_token WHERE id = %u;", accountId);	
			if (res)
			{
			Field* field = res->Fetch();              
			uint32 balance = field[0].GetUInt32();
			handler->PSendSysMessage("|cffff0000# |cff14ECCFВыдача на аккаунт: |cffff0000%s|cff14ECCF количеством: |cffff0000%u |cff14ECCFтокенов. Баланс |cffff0000%u", p_username.c_str(), token, balance); 
			}		
		}		
		return true;	
	}
	
};

class NPC_Unell_CH : public CreatureScript
{
public:
    NPC_Unell_CH() : CreatureScript("NPC_Unell_CH") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		AccountTypes gmLevel = player->GetSession()->GetSecurity();
        uint32 acclevel = uint32(gmLevel);
		uint32 accid = player->GetSession()->GetAccountId();
		player->PlayerTalkClass->ClearMenus();
		
		if (acclevel >= 6)
		{
			QueryResult resflaglogin = LoginDatabase.PQuery("SELECT login FROM account_login WHERE id = %u", accid);
			if (!resflaglogin)
				player->ADD_GOSSIP_ITEM(10, "|cffff0000[Включено]|r Оповещение о входе в мир", GOSSIP_SENDER_MAIN, 2);
			else
			{
				uint32 flaglog = resflaglogin->Fetch()->GetUInt32();
				
				if (flaglog == 1)
				player->ADD_GOSSIP_ITEM(10, "|cff008000[Включено]|r Оповещение о входе в мир", GOSSIP_SENDER_MAIN, 2);
			
				if (flaglog == 0)
					player->ADD_GOSSIP_ITEM(10, "|cffff0000[Выключено]|r Оповещение о входе в мир", GOSSIP_SENDER_MAIN, 1);
			}
		}
			
			QueryResult resflaghello = LoginDatabase.PQuery("SELECT flag FROM account_hello WHERE id = %u", accid);				
			if (!resflaghello)
				player->ADD_GOSSIP_ITEM(10, "|cff008000[Включено]|r Приветственное сообщение при входе в мир", GOSSIP_SENDER_MAIN, 4);
			else
			{
				uint32 flaghel = resflaghello->Fetch()->GetUInt32();
			
				if (flaghel == 1)
					player->ADD_GOSSIP_ITEM(10, "|cff008000[Включено]|r Приветственное сообщение при входе в мир", GOSSIP_SENDER_MAIN, 4);
				
				if (flaghel == 0)
					player->ADD_GOSSIP_ITEM(10, "|cffff0000[Выключено]|r Приветственное сообщение при входе в мир", GOSSIP_SENDER_MAIN, 3);
			}
			
		player->ADD_GOSSIP_ITEM(10, "ТЕСТ ГОССИП", GOSSIP_SENDER_MAIN, 100);	
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        uint32 accid = player->GetSession()->GetAccountId();		
		player->PlayerTalkClass->ClearMenus();
		AccountTypes gmLevel = player->GetSession()->GetSecurity();
        uint32 acclevel = uint32(gmLevel);

        switch(action)
        {
			case 100:
			OnGossipHello(player, creature);
			break;
			

		case 1: // Включить оповещение		
			{	
				QueryResult res = LoginDatabase.PQuery("SELECT login FROM account_login WHERE id = %u", accid);	
			
				if (res)					
				{
					LoginDatabase.PExecute("UPDATE `account_login` SET `login`='1' WHERE (`id`='%u')", accid);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы |cffff0000включили |cff14ECCFоповещение о входе в мир");
				}
				
				if (!res)
				{
					LoginDatabase.PExecute("INSERT INTO `account_login` (`id`, `login`) VALUES ('%u', '1')", accid);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы |cffff0000включили |cff14ECCFоповещение о входе в мир");
				}
        	
			}
			player->CLOSE_GOSSIP_MENU();
			break;
			
		case 2: // Выключить оповещение
			
			{	
				QueryResult res = LoginDatabase.PQuery("SELECT login FROM account_login WHERE id = %u", accid);	
			
				if (res)					
				{
					LoginDatabase.PExecute("UPDATE `account_login` SET `login`='0' WHERE (`id`='%u')", accid);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы |cffff0000выключили |cff14ECCFоповещение о входе в мир");
				}
				
				if (!res)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы ещё не включали оповещение в входе в игровой мир");
				}
        	
			}
			player->CLOSE_GOSSIP_MENU();
			break;	
		
			
		case 3: // Включить оповещение	account_hello	
			{	
				QueryResult res = LoginDatabase.PQuery("SELECT flag FROM account_hello WHERE id = %u", accid);	
			
				if (res)					
				{
					LoginDatabase.PExecute("UPDATE `account_hello` SET `flag`='1' WHERE (`id`='%u')", accid);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы |cffff0000включили |cff14ECCFсообщение при входе в мир");
				}
				
				if (!res)
				{
					LoginDatabase.PExecute("INSERT INTO `account_hello` (`id`, `flag`) VALUES ('%u', '1')", accid);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы |cffff0000включили |cff14ECCFсообщение при входе в мир");
				}
        	
			}
			player->CLOSE_GOSSIP_MENU();
			break;
			
		case 4: // Выключить оповещение
			
			{	
				QueryResult res = LoginDatabase.PQuery("SELECT flag FROM account_hello WHERE id = %u", accid);	
			
				if (res)					
				{
					LoginDatabase.PExecute("UPDATE `account_hello` SET `flag`='0' WHERE (`id`='%u')", accid);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы |cffff0000выключили |cff14ECCFсообщение при входе в мир");
				}
				
				if (!res)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВы ещё не включали сообщение при входе в игровой мир");
				}
        	
			}
			player->CLOSE_GOSSIP_MENU();
			break;			
	
        return true; 
        }
       
        return true;
    }
};

class Unell_Visual_NPC : public CreatureScript
{
public:
    Unell_Visual_NPC() : CreatureScript("Unell_Visual_NPC") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		player->ADD_GOSSIP_ITEM(10, "Удаление всех аур", GOSSIP_SENDER_MAIN, 100);
		
		if(player->HasAura(72523)) // ШМ
		{
			player->ADD_GOSSIP_ITEM(10, "|cff008000[Включено]|r Тёмная Скорбь", GOSSIP_SENDER_MAIN, 1);
		}
		else
			player->ADD_GOSSIP_ITEM(10, "|cffff0000[Выключено]|r Тёмная Скорбь", GOSSIP_SENDER_MAIN, 2);
		
		if(player->HasAura(47503)) // Сифон
		{
			player->ADD_GOSSIP_ITEM(10, "|cff008000[Включено]|r Сифон жизнь", GOSSIP_SENDER_MAIN, 3);
		}
		else
			player->ADD_GOSSIP_ITEM(10, "|cffff0000[Выключено]|r Сифон жизнь", GOSSIP_SENDER_MAIN, 4);
		
		if(player->HasAura(52619)) // Свечение
		{
			player->ADD_GOSSIP_ITEM(10, "|cff008000[Включено]|r Свечение", GOSSIP_SENDER_MAIN, 5);
		}
		else
			player->ADD_GOSSIP_ITEM(10, "|cffff0000[Выключено]|r Свечение", GOSSIP_SENDER_MAIN, 6);
		
		if(player->HasAura(22578)) // Сияние
		{
			player->ADD_GOSSIP_ITEM(10, "|cff008000[Включено]|r Сияние (чёрное)", GOSSIP_SENDER_MAIN, 7);
		}
		else
			player->ADD_GOSSIP_ITEM(10, "|cffff0000[Выключено]|r Сияние (чёрное)", GOSSIP_SENDER_MAIN, 8);
		
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        ChatHandler handler(player);
		
		player->PlayerTalkClass->ClearMenus();
        if (sender != GOSSIP_SENDER_MAIN)
            return true;

        switch(action)
        {
		
		case 100:
                player->CLOSE_GOSSIP_MENU(); // UN AURA
                player->RemoveAurasDueToSpell(72523);
				player->RemoveAurasDueToSpell(47503);
				player->RemoveAurasDueToSpell(52619);
				player->RemoveAurasDueToSpell(22578);
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 С вас сняты все эффекты");
                break;
		
		case 1:
                player->CLOSE_GOSSIP_MENU(); // ШМ
                player->RemoveAurasDueToSpell(72523);
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 С вас снят эффект |cFFFF0000Аура Тёмной Скорби");
                break;
				
		case 2:
                player->CLOSE_GOSSIP_MENU(); // ШМ
                player->AddAura(72523, player);
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 На вас наложен эффект |cFFFF0000Аура Тёмной Скорби");
                break;
				
		case 3:
                player->CLOSE_GOSSIP_MENU(); // Сифон
                player->RemoveAurasDueToSpell(47503);
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 С вас снят эффект |cFFFF0000Сифон жизнь");
                break;
		
		case 4:
                player->CLOSE_GOSSIP_MENU(); // Сифон
                player->AddAura(47503, player);
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 На вас наложен эффект |cFFFF0000Сифон жизнь");
                break;		
				
		case 5:
                player->CLOSE_GOSSIP_MENU(); // Свечение
                player->RemoveAurasDueToSpell(52619);
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 С вас снят эффект |cFFFF0000Свечение");
                break;
		
		case 6:
                player->CLOSE_GOSSIP_MENU(); // Свечение
                player->AddAura(52619, player);
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 На вас наложен эффект |cFFFF0000Свечение");
                break;
		
		case 7:
                player->CLOSE_GOSSIP_MENU(); // Сияние
                player->RemoveAurasDueToSpell(22578);
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 С вас снят эффект |cFFFF0000Сияние");
                break;
		
		case 8:
                player->CLOSE_GOSSIP_MENU(); // Сияние
                player->AddAura(22578, player);
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 На вас наложен эффект |cFFFF0000Сияние");
                break;
				
		break;
		
        return true; // invalid class, something fishy
        }
       
        return true;
    }
};

void AddSC_Unell_Custom()
{
    new Unell_commandscript();
	new NPC_Unell_CH();
	new Unell_Visual_NPC();	
}