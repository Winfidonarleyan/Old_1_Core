#include "ScriptPCH.h"
#include "TicketMgr.h"
#include "Config.h"
#include "Unell_Script.h"

class Unell_Login : public PlayerScript
{
public:
	Unell_Login() : PlayerScript("Unell_Login") { }	

	void OnLogin(Player* player)
	{		
	    char msg[250];
		
		uint32 accId = player->GetSession()->GetAccountId();
		std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());
		uint32 refid = Unell_Script::Unell_refid(accId);
		std::string username = Unell_Script::Unell_accuser(refid);
		
	    switch(player->GetSession()->GetSecurity())
		{
			case SEC_CONSOLE:
			{
				QueryResult result = LoginDatabase.PQuery("SELECT login FROM account_login WHERE id = %u", accId);
				if (result)
				{
					Field * field = result->Fetch();
					uint32 login = field[0].GetUInt32();
					
					if (login == 1)
					{
						snprintf(msg, 250, "|cffff0000[Анонс входа]:|r |cff6C8CD5Создатель |cff14ECCF%s |cff6C8CD5входит в игровой мир", NameLink.c_str());
						sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
					}
					
				}
			}			
			break;
		}
		
		QueryResult result = LoginDatabase.PQuery("SELECT flag FROM account_hello WHERE id = %u", accId);
		if (result)
		{
			Field * field = result->Fetch();
			uint32 flag = field[0].GetUInt32();
			
			if (flag == 1)
			{
				
				if (player->GetSession()->GetSecurity() < SEC_MODERATOR)
				{
					ChatHandler handler(player);
					uint32 accid = player->GetSession()->GetAccountId();
					uint16 gmlvl = player->GetSession()->GetSecurity();
					std::string gmname = player->GetName();
					uint32 playeronline = sWorld->GetPlayerCount();
					std::string uptime = secsToTimeString(sWorld->GetUptime());
					std::string player_ip = player->GetSession()->GetRemoteAddress();
					uint16 tickets = sTicketMgr->GetOpenTicketCount();				
					
					handler.PSendSysMessage("|cffff0000##############################|r");
					handler.PSendSysMessage("|cffff0000# |cff00ff00Привет,|r %s", gmname.c_str());
					handler.PSendSysMessage("|cffff0000# |cff00ff00Ваш IP:|r %s", player_ip.c_str());  
					handler.PSendSysMessage("|cffff0000# |cff00ff00Сейчас|r %u |cff00ff00игроков онлайн|r", playeronline);
					handler.PSendSysMessage("|cffff0000# |cff00ff00Время работы сервера:|r %s", uptime.c_str());
					if (ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
					{
						if (refid != 1)
						{
							if (username == "Неизвестно")
							handler.PSendSysMessage("|cffff0000#|r Пожалуйста, укажите кто вас пригласил");
							else
							handler.PSendSysMessage("|cffff0000# |cff00ff00Вас пригласил|r %s", username.c_str());
						}
					}
					if (player->GetSession()->IsPremium())
					{
						QueryResult result = LoginDatabase.PQuery("SELECT unsetdate FROM account_premium WHERE id = %u", accid);
						if (result)
						{
							Field * field = result->Fetch();
							uint32 unsetdate = field[0].GetUInt32();
							std::string timeStr  = secsToTimeString(unsetdate - time(NULL), false, false);
							handler.PSendSysMessage("|cFFFF0000# |cff00ff00Премиум аккаунт закончится через|r %s", timeStr.c_str());
						}
					}
					handler.PSendSysMessage("|cffff0000# |cff00ff00Удачи и приятной игры|r");
					handler.PSendSysMessage("|cffff0000##############################|r");
					return;	
				}
				
				if (player->GetSession()->GetSecurity() >= SEC_MODERATOR)
				{
					ChatHandler handler(player);
					uint32 accid = player->GetSession()->GetAccountId();
					uint16 gmlvl = player->GetSession()->GetSecurity();
					std::string gmname = player->GetName();
					uint32 playeronline = sWorld->GetPlayerCount();
					std::string uptime = secsToTimeString(sWorld->GetUptime());
					std::string player_ip = player->GetSession()->GetRemoteAddress();
					uint16 tickets = sTicketMgr->GetOpenTicketCount();

					handler.PSendSysMessage("|cffff0000##############################|r");
					handler.PSendSysMessage("|cffff0000# |cff00ff00Привет,|r %s", gmname.c_str());
					handler.PSendSysMessage("|cffff0000# |cff00ff00Ваш уровень учетной записи:|r %u", gmlvl);
					handler.PSendSysMessage("|cffff0000# |cff00ff00Ваш IP:|r %s", player_ip.c_str());
					handler.PSendSysMessage("|cffff0000# |cff00ff00Сейчас|r %u |cff00ff00игроков онлайн|r", playeronline);					
					handler.PSendSysMessage("|cffff0000# |cff00ff00Открытие тикеты:|r %u", tickets);
					if (ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
					{
						if (refid != 1)
						{
							if (username == "Неизвестно")
							handler.PSendSysMessage("|cffff0000#|r Пожалуйста, укажите кто вас пригласил");
							else
							handler.PSendSysMessage("|cffff0000# |cff00ff00Вас пригласил|r %s", username.c_str());
						}
					}
					if (player->GetSession()->IsPremium())
					{
						QueryResult result = LoginDatabase.PQuery("SELECT unsetdate FROM account_premium WHERE id = %u", accid);
						if (result)
						{
							Field * field = result->Fetch();
							uint32 unsetdate = field[0].GetUInt32();
							std::string timeStr  = secsToTimeString(unsetdate - time(NULL), false, false);
							handler.PSendSysMessage("|cFFFF0000# |cff00ff00Премиум аккаунт закончится через|r %s", timeStr.c_str());
						}
					}
					handler.PSendSysMessage("|cffff0000# |cff00ff00Удачи и приятной игры|r");
					handler.PSendSysMessage("|cffff0000##############################|r");
					return;	
				}
				
			}
		}
		if (!result)
		{
			if (player->GetSession()->GetSecurity() < SEC_MODERATOR)
				{
					ChatHandler handler(player);
					uint32 accid = player->GetSession()->GetAccountId();
					uint16 gmlvl = player->GetSession()->GetSecurity();
					std::string gmname = player->GetName();
					uint32 playeronline = sWorld->GetPlayerCount();
					std::string uptime = secsToTimeString(sWorld->GetUptime());
					std::string player_ip = player->GetSession()->GetRemoteAddress();
					uint16 tickets = sTicketMgr->GetOpenTicketCount();

					handler.PSendSysMessage("|cffff0000##############################|r");
					handler.PSendSysMessage("|cffff0000# |cff00ff00Привет,|r %s", gmname.c_str());
					handler.PSendSysMessage("|cffff0000# |cff00ff00Ваш IP:|r %s", player_ip.c_str());  
					handler.PSendSysMessage("|cffff0000# |cff00ff00Сейчас|r %u |cff00ff00игроков онлайн|r", playeronline);
					handler.PSendSysMessage("|cffff0000# |cff00ff00Время работы сервера:|r %s", uptime.c_str());
					if (ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
					{
						if (refid != 1)
						{
							if (username == "Неизвестно")
							handler.PSendSysMessage("|cffff0000#|r Пожалуйста, укажите кто вас пригласил");
							else
							handler.PSendSysMessage("|cffff0000# |cff00ff00Вас пригласил|r %s", username.c_str());
						}
					}
					if (player->GetSession()->IsPremium())
					{
						QueryResult result = LoginDatabase.PQuery("SELECT unsetdate FROM account_premium WHERE id = %u", accid);
						if (result)
						{
							Field * field = result->Fetch();
							uint32 unsetdate = field[0].GetUInt32();
							std::string timeStr  = secsToTimeString(unsetdate - time(NULL), false, false);
							handler.PSendSysMessage("|cFFFF0000# |cff00ff00Премиум аккаунт закончится через|r %s", timeStr.c_str());
						}
					}
					handler.PSendSysMessage("|cffff0000# |cff00ff00Удачи и приятной игры|r");
					handler.PSendSysMessage("|cffff0000##############################|r");
					return;	
				}
				
				if (player->GetSession()->GetSecurity() >= SEC_MODERATOR)
				{
					ChatHandler handler(player);
					uint32 accid = player->GetSession()->GetAccountId();
					uint16 gmlvl = player->GetSession()->GetSecurity();
					std::string gmname = player->GetName();
					uint32 playeronline = sWorld->GetPlayerCount();
					std::string uptime = secsToTimeString(sWorld->GetUptime());
					std::string player_ip = player->GetSession()->GetRemoteAddress();
					uint16 tickets = sTicketMgr->GetOpenTicketCount();

					handler.PSendSysMessage("|cffff0000##############################|r");
					handler.PSendSysMessage("|cffff0000# |cff00ff00Привет,|r %s", gmname.c_str());
					handler.PSendSysMessage("|cffff0000# |cff00ff00Ваш уровень учетной записи:|r %u", gmlvl);
					handler.PSendSysMessage("|cffff0000# |cff00ff00Ваш IP:|r %s", player_ip.c_str());
					handler.PSendSysMessage("|cffff0000# |cff00ff00Сейчас|r %u |cff00ff00игроков онлайн|r", playeronline);					
					handler.PSendSysMessage("|cffff0000# |cff00ff00Открытие тикеты:|r %u", tickets);
					if (ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
					{
						if (refid != 1)
						{
							if (username == "Неизвестно")
							handler.PSendSysMessage("|cffff0000#|r Пожалуйста, укажите кто вас пригласил");
							else
							handler.PSendSysMessage("|cffff0000# |cff00ff00Вас пригласил|r %s", username.c_str());
						}
					}
					if (player->GetSession()->IsPremium())
					{
						QueryResult result = LoginDatabase.PQuery("SELECT unsetdate FROM account_premium WHERE id = %u", accid);
						if (result)
						{
							Field * field = result->Fetch();
							uint32 unsetdate = field[0].GetUInt32();
							std::string timeStr  = secsToTimeString(unsetdate - time(NULL), false, false);
							handler.PSendSysMessage("|cFFFF0000# |cff00ff00Премиум аккаунт закончится через|r %s", timeStr.c_str());
						}
					}
					handler.PSendSysMessage("|cffff0000# |cff00ff00Удачи и приятной игры|r");
					handler.PSendSysMessage("|cffff0000##############################|r");
					return;	
				}
		}		
	}
	void OnLogout(Player* player)
	{
		char msg[250];
		
		uint32 accId = player->GetSession()->GetAccountId();
		std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());
		uint32 refid = Unell_Script::Unell_refid(accId);
		std::string username = Unell_Script::Unell_accuser(refid);		
		
	    switch(player->GetSession()->GetSecurity())
		{
			case SEC_CONSOLE:
			{
				QueryResult result = LoginDatabase.PQuery("SELECT login FROM account_login WHERE id = %u", accId);
				if (result)
				{
					Field * field = result->Fetch();
					uint32 login = field[0].GetUInt32();
					
					if (login == 1)
					{
						snprintf(msg, 250, "|cffff0000[Анонс выхода]:|r |cff6C8CD5Создатель |cff14ECCF%s |cff6C8CD5покинул игровой мир", NameLink.c_str());
						sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
					}
					
				}
			}			
			break;
		}
	}
};

class Unell_Login_Token : public PlayerScript
{
public:
	Unell_Login_Token() : PlayerScript("Unell_Login_Token") { }	

	void OnLogin(Player* player)
	{		
		if (ConfigMgr::GetBoolDefault("Unell.Donat.Token.OnLogin", true))
		{
			if (player->GetSession()->GetSecurity() >= SEC_MODERATOR)
			{
				ChatHandler handler(player);
				uint32 itemId = ConfigMgr::GetIntDefault("Unell.Donat.Token.OnLogin.TokenID", 0);
				uint32 countconf = ConfigMgr::GetIntDefault("Unell.Donat.Token.OnLogin.Value", 0);
				
				QueryResult result = CharacterDatabase.PQuery("SELECT owner_guid, count FROM item_instance WHERE `itemEntry` = '%u' AND `count` > '%u' ORDER BY count DESC LIMIT 100", itemId, countconf);
				if(result)
				{
					handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Проверка донат валюты у игроков");
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
							
							if (ConfigMgr::GetBoolDefault("Unell.Donat.Token.OnLogin.NoGM", true))
							{
								if (!Unell_Script::Unell_isgm(owner_guid))								
								handler.PSendSysMessage("|cffff0000# |cff14ECCFНик: |cffff0000%s|cff14ECCF, Количество валюты: |cffff0000%u", name.c_str(), count);
							}
							
							if (!ConfigMgr::GetBoolDefault("Unell.Donat.Token.OnLogin.NoGM", true))
								handler.PSendSysMessage("|cffff0000# |cff14ECCFНик: |cffff0000%s|cff14ECCF, Количество валюты: |cffff0000%u", name.c_str(), count);
						}					
					} 
					while(result->NextRow());
				}
				return;
			}
		
		}		
	}
};

class Unell_Login_CommandAdd : public PlayerScript
{
public:
	Unell_Login_CommandAdd() : PlayerScript("Unell_Login_CommandAdd") { }	

	void OnLogin(Player* player)
	{		
		if (ConfigMgr::GetBoolDefault("Unell.Command.Add.LoginLogs", true))
		{
			if (player->GetSession()->GetSecurity() == SEC_CONSOLE)
			{
				ChatHandler handler(player);
				QueryResult result = CharacterDatabase.PQuery("SELECT gmplayer, count, FROM_UNIXTIME(time), targetname FROM logs_add WHERE (UNIX_TIMESTAMP() - time) < 86400 ORDER BY time DESC LIMIT 20");
				if(result) 
				{
					handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 История выдачи ивент марок за последние 24 часа.");
					do
					{
						Field * fields = result->Fetch();
						std::string gmname = fields[0].GetString();
						uint32 count = fields[1].GetUInt32();
						std::string time = fields[2].GetCString();
						std::string targetname = fields[3].GetString();
						
						handler.PSendSysMessage("|cffff0000# |cff14ECCFГМ: |cffff0000%s|cff14ECCF, Количество: |cffff0000%u|cff14ECCF, Дата: |cffff0000%s|cff14ECCF, Игроку: |cffff0000%s|cff14ECCF.", gmname.c_str(), count, time.c_str(), targetname.c_str());
					} 
					while(result->NextRow());
				}
				
				if(!result)
					handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Выдача марок за последние 24 часа не производилась.");
			}
		}		
	}
};

void AddSC_Unell_Login_System()
{
	new Unell_Login();
	new Unell_Login_Token();
	new Unell_Login_CommandAdd();
}