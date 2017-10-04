#include "ScriptPCH.h"
#include "Language.h"
#include "Config.h"
#include "TicketMgr.h"

class Boss_Announcer : public PlayerScript
{
public:
	Boss_Announcer() : PlayerScript ("Boss_Announcer") {}

	void OnCreatureKill(Player* player, Creature* boss)
	{
		if (ConfigMgr::GetBoolDefault("Unell.Boss.Announcer", true))
        {
			if (boss->isWorldBoss() && !boss->GetInstanceId())
			{
				ChatHandler handler(player);
				std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());				
				uint32 Entry = boss->GetEntry();				
				CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
				CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
				char const* crename = NULL;
					if (CreatureLocale)
						crename = CreatureLocale->Name[handler.GetSessionDbcLocale()].c_str();
					if ((!crename || !*crename) && CreatureTemplate)
						crename = CreatureTemplate->Name.c_str();
					if (!crename)
						crename = "Unknown creature";	
				
				snprintf(msg, 250, "|cFFFF0000[Босс Анонсер]: |cff6C8CD5Игрок |cff14ECCF%s |cff6C8CD5и его группа убили мирового босса |cff14ECCF%s|r", crename, NameLink.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
			}
		}
	}
	
	void OnPlayerKilledByCreature (Creature* boss, Player* player)
	{
		if (ConfigMgr::GetBoolDefault("Unell.Boss.Announcer.Player", true))
		{
			if (boss->isWorldBoss() && !boss->GetInstanceId())
			{
				char msg[250];
				ChatHandler handler(player);
				std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());				
				uint32 Entry = boss->GetEntry();				
				CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
				CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
				char const* crename = NULL;
					if (CreatureLocale)
						crename = CreatureLocale->Name[handler.GetSessionDbcLocale()].c_str();
					if ((!crename || !*crename) && CreatureTemplate)
						crename = CreatureTemplate->Name.c_str();
					if (!crename)
						crename = "Unknown creature";
				
				snprintf(msg, 250, "|cFFFF0000[Босс Анонсер]: |cff6C8CD5Мировой босс |cff14ECCF%s |cff6C8CD5убил игрока |cff14ECCF%s|r", crename, NameLink.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);					
			}
		}
	}
};

class gm_announce : public PlayerScript
{
public:
	gm_announce() : PlayerScript("gm_announce") { }

	void OnLogin(Player* player)
	{
		std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());
			
		char msg[250];
		switch(player->GetSession()->GetSecurity())
		{
			
			case SEC_PLAYER:
			if (player->GetSession()->IsPremium())			
			{
				snprintf(msg, 250, "|cffff0000[Анонс входа]:|r |cff6C8CD5VIP |cff14ECCF%s |cff6C8CD5входит в игровой мир", NameLink.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
			}
			break;
			
			case SEC_TESTED:
			if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.1", true))
			{
				snprintf(msg, 250, "|cffff0000[Анонс входа]:|r |cff6C8CD5Исп. срок |cff14ECCF%s |cff6C8CD5входит в игровой мир", NameLink.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);	
			}
			break;
			
			case SEC_MODERATOR:
			if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.2", true))
			{	
				snprintf(msg, 250, "|cffff0000[Анонс входа]:|r |cff6C8CD5Модератор |cff14ECCF%s |cff6C8CD5входит в игровой мир", NameLink.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;
			}	
			
			case SEC_EVENTER:
			if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.3", true))
			{
				snprintf(msg, 250, "|cffff0000[Анонс входа]:|r |cff6C8CD5Ивент-мастер |cff14ECCF%s |cff6C8CD5входит в игровой мир", NameLink.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				break;
			}

			case SEC_GAMEMASTER:
			if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.4", true))
			{
				 snprintf(msg, 250, "|cffff0000[Анонс входа]:|r |cff6C8CD5Гейм-мастер |cff14ECCF%s |cff6C8CD5входит в игровой мир", NameLink.c_str());
				 sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				 break; 
			}
			
			case SEC_ADMINISTRATOR:
			if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.5", true))
			{
				 snprintf(msg, 250, "|cffff0000[Анонс входа]:|r |cff6C8CD5Администратор |cff14ECCF%s |cff6C8CD5входит в игровой мир", NameLink.c_str());
				 sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				 break;
			}
		}
	}
	
	void OnLogout(Player* player)
	{
		if (sWorld->getBoolConfig(CONFIG_UNELL_GM_ENTER))
        {
			std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());
			
			char msg[250];
			switch(player->GetSession()->GetSecurity())
			{
				
				case SEC_PLAYER:
				if (player->GetSession()->IsPremium())			
				{
					snprintf(msg, 250, "|cffff0000[Анонс выхода]:|r |cff6C8CD5VIP |cff14ECCF%s |cff6C8CD5покинул игровой мир", NameLink.c_str());
					sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				}
				break;
				
				case SEC_TESTED:
				if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.1", true))
				{
					snprintf(msg, 250, "|cffff0000[Анонс выхода]:|r |cff6C8CD5Исп. срок |cff14ECCF%s |cff6C8CD5покинул игровой мир", NameLink.c_str());
					sWorld->SendServerMessage(SERVER_MSG_STRING, msg);	
				}
				break;
				
				case SEC_MODERATOR:
				if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.2", true))
				{	
					snprintf(msg, 250, "|cffff0000[Анонс выхода]:|r |cff6C8CD5Модератор |cff14ECCF%s |cff6C8CD5покинул игровой мир", NameLink.c_str());
					sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
					break;
				}	
				
				case SEC_EVENTER:
				if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.3", true))
				{
					snprintf(msg, 250, "|cffff0000[Анонс выхода]:|r |cff6C8CD5Ивент-мастер |cff14ECCF%s |cff6C8CD5покинул игровой мир", NameLink.c_str());
					sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
					break;
				}

				case SEC_GAMEMASTER:
				if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.4", true))
				{
					 snprintf(msg, 250, "|cffff0000[Анонс выхода]:|r |cff6C8CD5Гейм-мастер |cff14ECCF%s |cff6C8CD5покинул игровой мир", NameLink.c_str());
					 sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
					 break; 
				}
				
				case SEC_ADMINISTRATOR:
				if (ConfigMgr::GetBoolDefault("Unell.GM.Enter.Level.5", true))
				{
					 snprintf(msg, 250, "|cffff0000[Анонс выхода]:|r |cff6C8CD5Администратор |cff14ECCF%s |cff6C8CD5покинул игровой мир", NameLink.c_str());
					 sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
					 break;
				}		 
			}
	    }
	}
};

class player_announce : public PlayerScript
{
public:
	player_announce() : PlayerScript("player_announce") { }

	void OnLogin(Player* player)
	{
		char msg[250];
		std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());
		uint32 playeronline = sWorld->GetPlayerCount();
		
		bool Penter = ConfigMgr::GetBoolDefault("Unell.Player.Enter.Enable", true);
		uint32 MinOnline = ConfigMgr::GetIntDefault("Unell.Player.Enter.MinOnline", 2);
		uint32 MaxOnline = ConfigMgr::GetIntDefault("Unell.Player.Enter.MaxOnline", 30);
		bool Ongm = ConfigMgr::GetBoolDefault("Unell.Player.Enter.OnliGM", true);
		
	
		switch(player->GetSession()->GetSecurity())
		{
			case SEC_PLAYER:
			if (!player->GetSession()->IsPremium() && Penter)
			{
				if (Ongm)
				{
					snprintf(msg, 250, "|cffff0000[Анонс входа]:|r |cff6C8CD5Игрок |cff14ECCF%s |cff6C8CD5входит в игровой мир", NameLink.c_str());
					sWorld->SendGMText(LANG_GM_BROADCAST, msg);
					
					if (playeronline >= MinOnline && playeronline <= MaxOnline)
					{
						snprintf(msg, 250, "|cffff0000# |cff00ff00Сейчас|r %u |cff00ff00игроков онлайн|r", playeronline);
						sWorld->SendGMText(LANG_GM_BROADCAST, msg);
					}
				}
				else
				{
					snprintf(msg, 250, "|cffff0000[Анонс входа]:|r |cff6C8CD5Игрок |cff14ECCF%s |cff6C8CD5входит в игровой мир", NameLink.c_str());
					sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
					
					if (playeronline >= MinOnline && playeronline <= MaxOnline)
					{
						snprintf(msg, 250, "|cffff0000# |cff00ff00Сейчас|r %u |cff00ff00игроков онлайн|r", playeronline);
						sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
					}
				}
			}
			break;				
		}
	}

};


void AddSC_Unell_Announcer_System()
{
	new Boss_Announcer;
	new gm_announce();
	new player_announce();
}