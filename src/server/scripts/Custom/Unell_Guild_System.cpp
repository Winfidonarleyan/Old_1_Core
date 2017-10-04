#include "ScriptPCH.h"
#include "Chat.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "DatabaseEnv.h"

void UpdateLevel(Player* player) 
{
	uint32 guild = player->GetGuildId();
	QueryResult result = CharacterDatabase.PQuery("SELECT level, xp FROM guild_level WHERE guild = %u", guild);
	if (result)
	{
		Field *fields = result->Fetch();
		uint16 level = fields[0].GetUInt16();
		uint32 xp = fields[1].GetUInt32();
		
		QueryResult maxlevel = CharacterDatabase.PQuery("SELECT max(level) FROM guild_xp_table");
		uint32 maxlvl = maxlevel->Fetch()->GetUInt32();
		QueryResult maxexp = CharacterDatabase.PQuery("SELECT xp FROM guild_xp_table WHERE level = %u", maxlvl);		
		uint32 maxxp = maxexp->Fetch()->GetUInt32();
	 
		QueryResult knowLevel = CharacterDatabase.PQuery("SELECT level FROM guild_xp_table WHERE xp > %u", xp);
		if (knowLevel)
		{
			Field *fs = knowLevel->Fetch();
			uint16 gLevel = fs[0].GetUInt16();
			
			
			if (level < gLevel)
			{
				char msg[2048];
				std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());		
				
				if (gLevel == maxlvl)
				{
					snprintf(msg, 250, "|cffff0000[Новый уровень]:|r |cff6C8CD5Гильдия достигла максимального уровня.");
					sWorld->SendGuildText(guild, msg);
				}
				else
				{
					snprintf(msg, 250, "|cffff0000[Новый уровень]:|r |cff6C8CD5Гильдия достигла %u-го уровня.", gLevel);
					sWorld->SendGuildText(guild, msg);
				}
		
				if (gLevel == 2)
				{
					snprintf(msg, 250, "|cffff0000[Новый уровень]:|r |cff6C8CD5Открыто: |cffff0000Смена цвета гильдии");
					sWorld->SendGuildText(guild, msg);
				}
			
				if (gLevel == 7)
				{
					snprintf(msg, 250, "|cffff0000[Новый уровень]:|r |cff6C8CD5Открыто: |cffff0000Воскрешение существ [35 DN монет]");
					sWorld->SendGuildText(guild, msg);
				}
				
				if (gLevel == 14)
				{
					snprintf(msg, 250, "|cffff0000[Новый уровень]:|r |cff6C8CD5Открыто: |cffff0000Воскрешение существ [30 DN монет]");
					sWorld->SendGuildText(guild, msg);
				}
				
				if (gLevel == 21)
				{
					snprintf(msg, 250, "|cffff0000[Новый уровень]:|r |cff6C8CD5Открыто: |cffff0000Воскрешение существ [25 DN монет]");
					sWorld->SendGuildText(guild, msg);
				}
				
				if (gLevel == 28)
				{
					snprintf(msg, 250, "|cffff0000[Новый уровень]:|r |cff6C8CD5Открыто: |cffff0000Воскрешение существ [20 DN монет]");
					sWorld->SendGuildText(guild, msg);
				}
			}
			
			CharacterDatabase.PQuery("UPDATE guild_level SET level = %u WHERE guild = %u", gLevel, guild);
		}
		
		
		if (xp > maxxp)
			CharacterDatabase.PQuery("UPDATE `guild_level` SET `xp`='%u' WHERE (`guild`='%u')", maxxp, guild);
		else
			CharacterDatabase.PQuery("UPDATE `guild_level` SET `xp`='%u' WHERE (`guild`='%u')", xp, guild);
	}
	sGuildMgr->LoadGuilds();
}

void InvestedRoster(Player* player, Creature* creature) 
{
	player->PlayerTalkClass->ClearMenus();
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "ТОП вложивших в гильдию: DN Монеты", GOSSIP_SENDER_MAIN, 14);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "ТОП вложивших в гильдию: Очки Чести", GOSSIP_SENDER_MAIN, 15);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "ТОП вложивших в гильдию: Босс Монетки", GOSSIP_SENDER_MAIN, 20);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
}

void Invested(Player* player, Creature* creature) 
{
	player->PlayerTalkClass->ClearMenus();
	player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_BG_kill_flag_carrierEOS:32:32:-18:0|tПожертвовать Очки Чести для гильдии", GOSSIP_SENDER_MAIN, 1, "200 очков чести = 1 очко опыта", 0, true);
	player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Achievement_BG_kill_flag_carrierEOS:32:32:-18:0|tПожертвовать Босс Монетки для гильдии", GOSSIP_SENDER_MAIN, 3, "1 Босс Монетка = 1 очко опыта", 0, true);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
}

void ReplaceColorList(Player* player, Creature* creature) 
{
	player->PlayerTalkClass->ClearMenus();
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Сменить цвет на Красный", GOSSIP_SENDER_MAIN, 7);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Сменить цвет на Синий", GOSSIP_SENDER_MAIN, 8);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Сменить цвет на Чёрный", GOSSIP_SENDER_MAIN, 9);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Сменить цвет на Белый", GOSSIP_SENDER_MAIN, 10);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Сменить цвет на Зелёный", GOSSIP_SENDER_MAIN, 11);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Сменить цвет на Желтый", GOSSIP_SENDER_MAIN, 12);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
}

void ReplaceColor(Player* player, Creature* creature, uint32 color)
{
	switch(color)
	{
		case 1:
			CharacterDatabase.PExecute("UPDATE guild_level SET color = '|cffFF0000' WHERE guild = %u", player->GetGuildId());
			break;
		case 2:
			CharacterDatabase.PExecute("UPDATE guild_level SET color = '|cff0000FF' WHERE guild = %u", player->GetGuildId());
			break;
		case 3:
			CharacterDatabase.PExecute("UPDATE guild_level SET color = '|cff000000' WHERE guild = %u", player->GetGuildId());
			break;
		case 4:
			CharacterDatabase.PExecute("UPDATE guild_level SET color = '|cffFFFFFF' WHERE guild = %u", player->GetGuildId());
			break;
		case 5:
			CharacterDatabase.PExecute("UPDATE guild_level SET color = '|cff00FF00' WHERE guild = %u", player->GetGuildId());
			break;
		case 6:
			CharacterDatabase.PExecute("UPDATE guild_level SET color = '|cffFFFF00' WHERE guild = %u", player->GetGuildId());
			break;
	}
	sGuildMgr->LoadGuilds();
	player->GetSession()->SendAreaTriggerMessage("Вы успешно сменили цвет названия гильдии!");
	player->CLOSE_GOSSIP_MENU();
}

void TakeResWorld28(Player* player, Creature* creature) 
{
	if (!player->GetGuildId()) 
	{
		player->GetSession()->SendNotification("Вы не состоите в гильдии!");
		player->CLOSE_GOSSIP_MENU();
		return;
	}
	
	ChatHandler handler(player);
	uint32 guild = player->GetGuildId();
	
	QueryResult result = CharacterDatabase.PQuery("SELECT countdn FROM guild_dntoken WHERE guild = %u", guild);
	
	if (!result)
	{
		handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Баланс гильдии |cFFFF00000 |cff6C8CD5DN Монет.");
		player->CLOSE_GOSSIP_MENU();
	}
		
	if (result)
	{
		uint32 count = result->Fetch()->GetUInt32();
		
		if (count < 20)
			handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Баланс гильдии |cFFFF0000%u |cff6C8CD5DN Монет. Нужно как минимум |cFFFF000020|cff6C8CD5.", count);
		else
		{
			CellCoord p(Trinity::ComputeCellCoord(player->GetPositionX(), player->GetPositionY()));
			Cell cell(p);
			cell.SetNoCreate();
			Trinity::RespawnDo u_do;
			Trinity::WorldObjectWorker<Trinity::RespawnDo> worker(player, u_do);
			TypeContainerVisitor<Trinity::WorldObjectWorker<Trinity::RespawnDo>, GridTypeMapContainer > obj_worker(worker);
			cell.Visit(p, obj_worker, *player->GetMap(), *player, player->GetGridActivationRange());
			
			CharacterDatabase.PExecute("UPDATE `guild_dntoken` SET `countdn`= countdn - 20 WHERE (`guild`='%u')", guild);
			handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы воскресили ближних существ. Теперь в вашем банке |cFFFF0000%u |cff6C8CD5DN Монет.", count-20);
		
			char msg[2048];
			std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());		
			snprintf(msg, 250, "|cffff0000[Воскрешение]:|r |cff14ECCF%s |cff6C8CD5использовал гильдейское воскрешение существ. Баланс: |cff14ECCF%u |cff6C8CD5DN Монет.", NameLink.c_str(), count-20);
			sWorld->SendGuildText(guild, msg);		
		}
		player->CLOSE_GOSSIP_MENU();
	}	
}

void TakeResWorld21(Player* player, Creature* creature) 
{
	if (!player->GetGuildId()) 
	{
		player->GetSession()->SendNotification("Вы не состоите в гильдии!");
		player->CLOSE_GOSSIP_MENU();
		return;
	}
	
	ChatHandler handler(player);
	uint32 guild = player->GetGuildId();
	
	QueryResult result = CharacterDatabase.PQuery("SELECT countdn FROM guild_dntoken WHERE guild = %u", guild);
	
	if (!result)
	{
		handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Баланс гильдии |cFFFF00000 |cff6C8CD5DN Монет.");
		player->CLOSE_GOSSIP_MENU();
	}
		
	if (result)
	{
		uint32 count = result->Fetch()->GetUInt32();
		
		if (count < 25)
			handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Баланс гильдии |cFFFF0000%u |cff6C8CD5DN Монет. Нужно как минимум |cFFFF000025|cff6C8CD5.", count);
		else
		{
			CellCoord p(Trinity::ComputeCellCoord(player->GetPositionX(), player->GetPositionY()));
			Cell cell(p);
			cell.SetNoCreate();
			Trinity::RespawnDo u_do;
			Trinity::WorldObjectWorker<Trinity::RespawnDo> worker(player, u_do);
			TypeContainerVisitor<Trinity::WorldObjectWorker<Trinity::RespawnDo>, GridTypeMapContainer > obj_worker(worker);
			cell.Visit(p, obj_worker, *player->GetMap(), *player, player->GetGridActivationRange());
			
			CharacterDatabase.PExecute("UPDATE `guild_dntoken` SET `countdn`= countdn - 25 WHERE (`guild`='%u')", guild);
			handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы воскресили ближних существ. Теперь в вашем банке |cFFFF0000%u |cff6C8CD5DN Монет.", count-25);
		
			char msg[2048];
			std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());		
			snprintf(msg, 250, "|cffff0000[Воскрешение]:|r |cff14ECCF%s |cff6C8CD5использовал гильдейское воскрешение существ. Баланс: |cff14ECCF%u |cff6C8CD5DN Монет.", NameLink.c_str(), count-25);
			sWorld->SendGuildText(guild, msg);		
		}
		player->CLOSE_GOSSIP_MENU();
	}	
}

void TakeResWorld14(Player* player, Creature* creature) 
{
	if (!player->GetGuildId()) 
	{
		player->GetSession()->SendNotification("Вы не состоите в гильдии!");
		player->CLOSE_GOSSIP_MENU();
		return;
	}
	
	ChatHandler handler(player);
	uint32 guild = player->GetGuildId();
	
	QueryResult result = CharacterDatabase.PQuery("SELECT countdn FROM guild_dntoken WHERE guild = %u", guild);
	
	if (!result)
	{
		handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Баланс гильдии |cFFFF00000 |cff6C8CD5DN Монет.");
		player->CLOSE_GOSSIP_MENU();
	}
		
	if (result)
	{
		uint32 count = result->Fetch()->GetUInt32();
		
		if (count < 30)
			handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Баланс гильдии |cFFFF0000%u |cff6C8CD5DN Монет. Нужно как минимум |cFFFF000030|cff6C8CD5.", count);
		else
		{
			CellCoord p(Trinity::ComputeCellCoord(player->GetPositionX(), player->GetPositionY()));
			Cell cell(p);
			cell.SetNoCreate();
			Trinity::RespawnDo u_do;
			Trinity::WorldObjectWorker<Trinity::RespawnDo> worker(player, u_do);
			TypeContainerVisitor<Trinity::WorldObjectWorker<Trinity::RespawnDo>, GridTypeMapContainer > obj_worker(worker);
			cell.Visit(p, obj_worker, *player->GetMap(), *player, player->GetGridActivationRange());
			
			CharacterDatabase.PExecute("UPDATE `guild_dntoken` SET `countdn`= countdn - 30 WHERE (`guild`='%u')", guild);
			handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы воскресили ближних существ. Теперь в вашем банке |cFFFF0000%u |cff6C8CD5DN Монет.", count-30);
		
			char msg[2048];
			std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());		
			snprintf(msg, 250, "|cffff0000[Воскрешение]:|r |cff14ECCF%s |cff6C8CD5использовал гильдейское воскрешение существ. Баланс: |cff14ECCF%u |cff6C8CD5DN Монет.", NameLink.c_str(), count-30);
			sWorld->SendGuildText(guild, msg);		
		}
		player->CLOSE_GOSSIP_MENU();
	}	
}

void TakeResWorld7(Player* player, Creature* creature) 
{
	if (!player->GetGuildId()) 
	{
		player->GetSession()->SendNotification("Вы не состоите в гильдии!");
		player->CLOSE_GOSSIP_MENU();
		return;
	}
	
	ChatHandler handler(player);
	uint32 guild = player->GetGuildId();
	
	QueryResult result = CharacterDatabase.PQuery("SELECT countdn FROM guild_dntoken WHERE guild = %u", guild);
	
	if (!result)
	{
		handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Баланс гильдии |cFFFF00000 |cff6C8CD5DN Монет.");
		player->CLOSE_GOSSIP_MENU();
	}
		
	if (result)
	{
		uint32 count = result->Fetch()->GetUInt32();
		
		if (count < 35)
			handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Баланс гильдии |cFFFF0000%u |cff6C8CD5DN Монет. Нужно как минимум |cFFFF000035|cff6C8CD5.", count);
		else
		{
			CellCoord p(Trinity::ComputeCellCoord(player->GetPositionX(), player->GetPositionY()));
			Cell cell(p);
			cell.SetNoCreate();
			Trinity::RespawnDo u_do;
			Trinity::WorldObjectWorker<Trinity::RespawnDo> worker(player, u_do);
			TypeContainerVisitor<Trinity::WorldObjectWorker<Trinity::RespawnDo>, GridTypeMapContainer > obj_worker(worker);
			cell.Visit(p, obj_worker, *player->GetMap(), *player, player->GetGridActivationRange());
			
			CharacterDatabase.PExecute("UPDATE `guild_dntoken` SET `countdn`= countdn - 35 WHERE (`guild`='%u')", guild);
			handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы воскресили ближних существ. Теперь в вашем банке |cFFFF0000%u |cff6C8CD5DN Монет.", count-35);
		
			char msg[2048];
			std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());		
			snprintf(msg, 250, "|cffff0000[Воскрешение]:|r |cff14ECCF%s |cff6C8CD5использовал гильдейское воскрешение существ. Баланс: |cff14ECCF%u |cff6C8CD5DN Монет.", NameLink.c_str(), count-35);
			sWorld->SendGuildText(guild, msg);		
		}
		player->CLOSE_GOSSIP_MENU();
	}	
}

void TakeDN(Player* player, Creature* creature, char const* code)
{
	ChatHandler handler(player);
	int32 dn = atoi(code);
	if (!player->GetGuildId()) 
	{
		player->GetSession()->SendNotification("Вы не состоите в гильдии!");
		player->CLOSE_GOSSIP_MENU();
		return;
	}
	
	if (player->HasItemCount(120000, dn))
	{
		uint32 guild = player->GetGuildId();
		std::string name = player->GetName();	
		
		QueryResult result = CharacterDatabase.PQuery("SELECT name FROM guild_level_invested WHERE guild = %u AND name = '%s'", guild, name.c_str());
		if (result)
			CharacterDatabase.PExecute("UPDATE `guild_level_invested` SET `token`= token + %u WHERE (`guild`='%u') AND (`name`='%s');", dn, guild, name.c_str());
		else
			CharacterDatabase.PExecute("INSERT INTO `guild_level_invested` (`guild`, `name`, `token`) VALUES ('%u', '%s', '%u')", guild, name.c_str(), dn);
		
		QueryResult resultdn = CharacterDatabase.PQuery("SELECT name FROM guild_level_invested WHERE guild = %u AND name = '%s'", guild, name.c_str());
		if (result)
			CharacterDatabase.PExecute("UPDATE `guild_dntoken` SET `countdn`= countdn + %u WHERE (`guild`='%u') LIMIT 1", dn, guild);
		if (!result)
			CharacterDatabase.PExecute("INSERT INTO `guild_dntoken` (`guild`, `countdn`) VALUES ('%u', '%u')", guild, dn);
		
		QueryResult resultbank = CharacterDatabase.PQuery("SELECT countdn FROM guild_dntoken WHERE guild = %u", guild);
		if (resultbank)
		{
			uint32 count = resultbank->Fetch()->GetUInt32();
			char msg[2048];
			std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());		
			snprintf(msg, 2048, "|cffff0000[Пожертвование]:|r |cff14ECCF%s |cff6C8CD5пожертвовал |cff14ECCF%u |cff6C8CD5DN Монет. Баланс гильдии: |cff14ECCF%u |cff6C8CD5DN Монет.", NameLink.c_str(), dn, count+dn);
			sWorld->SendGuildText(guild, msg);		
		}
		
		player->DestroyItemCount(120000, dn, true);
		player->CLOSE_GOSSIP_MENU();			
	}
	else
	{
		player->GetSession()->SendNotification("У вас недостаточно DN Монет!");
		player->CLOSE_GOSSIP_MENU();
	}
	UpdateLevel(player);
}

void TakeHonor(Player* player, Creature* creature, char const* code)
{
	ChatHandler handler(player);
	if (!player->GetGuildId()) 
	{
		player->GetSession()->SendNotification("Вы не состоите в гильдии!");
		player->CLOSE_GOSSIP_MENU();
		return;
	}
	
	int32 honorpoint = atoi(code);
	
	if (player->GetHonorPoints() < honorpoint)
	{
		player->GetSession()->SendNotification("У вас недостаточно очков чести!");
		player->CLOSE_GOSSIP_MENU();		
	}
	else
	{
		uint32 guild = player->GetGuildId();
		std::string name = player->GetName();
		
		QueryResult result = CharacterDatabase.PQuery("SELECT name FROM guild_level_invested WHERE guild = %u AND name = '%s'", guild, name.c_str());
		if (result)
			CharacterDatabase.PExecute("UPDATE `guild_level_invested` SET `honor`= honor + %u WHERE (`guild`='%u') AND (`name`='%s');", honorpoint, guild, name.c_str());
		else
			CharacterDatabase.PExecute("INSERT INTO `guild_level_invested` (`guild`, `name`, `honor`) VALUES ('%u', '%s', '%u')", guild, name.c_str(), honorpoint);
		
		player->ModifyHonorPoints(-honorpoint);			
		uint32 xp = honorpoint / 200;
		CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp+%u WHERE guild = %u", xp, guild);		
		char msg[2048];
		std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());		
		snprintf(msg, 2048, "|cffff0000[Пожертвование]:|r |cff14ECCF%s |cff6C8CD5пожертвовал |cff14ECCF%u |cff6C8CD5хонора и добавил |cff14ECCF%u |cff6C8CD5очков опыта", NameLink.c_str(), honorpoint, xp);
		sWorld->SendGuildText(guild, msg);
		player->CLOSE_GOSSIP_MENU();
	}
	UpdateLevel(player);
}

void TakeBossToken(Player* player, Creature* creature, char const* code)
{
	ChatHandler handler(player);
	if (!player->GetGuildId()) 
	{
		player->GetSession()->SendNotification("Вы не состоите в гильдии!");
		player->CLOSE_GOSSIP_MENU();
		return;
	}
	
	int32 bosspoint = atoi(code);
	
	if (!player->HasItemCount(120001, bosspoint))
	{
		player->GetSession()->SendNotification("У вас недостаточно Босс Монет!");
		player->CLOSE_GOSSIP_MENU();		
	}
	else
	{
		uint32 guild = player->GetGuildId();
		std::string name = player->GetName();
		
		QueryResult result = CharacterDatabase.PQuery("SELECT name FROM guild_level_invested WHERE guild = %u AND name = '%s'", guild, name.c_str());
		if (result)
			CharacterDatabase.PExecute("UPDATE `guild_level_invested` SET `bosstoken`= bosstoken + %u WHERE (`guild`='%u') AND (`name`='%s');", bosspoint, guild, name.c_str());
		else
			CharacterDatabase.PExecute("INSERT INTO `guild_level_invested` (`guild`, `name`, `bosstoken`) VALUES ('%u', '%s', '%u')", guild, name.c_str(), bosspoint);
		
		player->DestroyItemCount(120001, bosspoint, true);
		CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp+%u WHERE guild = %u", bosspoint, guild);		
		char msg[2048];
		std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());		
		snprintf(msg, 2048, "|cffff0000[Пожертвование]:|r |cff14ECCF%s |cff6C8CD5пожертвовал |cff14ECCF%u |cff6C8CD5Босс Монет и добавил |cff14ECCF%u |cff6C8CD5очков опыта", NameLink.c_str(), bosspoint, bosspoint);
		sWorld->SendGuildText(guild, msg);
		player->CLOSE_GOSSIP_MENU();
	}
	UpdateLevel(player);
}

void GuildInfo(Player* player) 
{
	if (!player->GetGuildId()) 
	{
		player->GetSession()->SendNotification("Вы не состоите в гильдии!");
		player->CLOSE_GOSSIP_MENU();
		return;
	}
	UpdateLevel(player);	
	ChatHandler handler(player);	
	uint32 guild = player->GetGuildId();
	QueryResult result = CharacterDatabase.PQuery("SELECT level, xp FROM guild_level WHERE guild = %u", guild);
	
	if (result) 
	{
		Field *fields = result->Fetch();
		uint16 level = fields[0].GetUInt16();
		uint32 xp = fields[1].GetUInt32();
		
		ChatHandler(player->GetSession()).PSendSysMessage("|cFFFF0000# |cfff4b25eУровень вашей гильдии:|r |cfffcedbb%u|r", level);
		ChatHandler(player->GetSession()).PSendSysMessage("|cFFFF0000# |cfff4b25eПрогресс уровня вашей гильдии:|r |cfffcedbb%u очков опыта|r", xp);

		QueryResult result1 = CharacterDatabase.PQuery("SELECT xp FROM guild_xp_table WHERE level = %u", level);
		if (result1) 
		{
			Field *fields = result1->Fetch();
			uint32 reqXp = fields[0].GetUInt32();
			uint32 ReqXp = reqXp-xp;
		
			if (level < 30)
				handler.PSendSysMessage("|cFFFF0000# |cfff4b25eДо %u-го уровня гильдии осталось накопить:|r |cfffcedbb%u очков опыта|r", level+1, ReqXp);
			else 
				handler.PSendSysMessage("|cFFFF0000# |cfff4b25eВаша гильдия имеет максимальный уровень!|r");
		}
	}
	
	QueryResult resultbank = CharacterDatabase.PQuery("SELECT countdn FROM guild_dntoken WHERE guild = %u", guild);
	if (resultbank)
	{
		uint32 count = resultbank->Fetch()->GetUInt32();
		handler.PSendSysMessage("|cFFFF0000#|cfff4b25e Баланс гильдии: |cFFFF0000%u |cfffcedbbDN монет.", count);
	}	
	player->CLOSE_GOSSIP_MENU();
}

void InvestedToken(Player* player, Creature* creature) 
{
	player->PlayerTalkClass->ClearMenus();
	uint32 guild = player->GetGuildId();
	ChatHandler handler(player);
	uint32 step = 1;
	
	QueryResult result = CharacterDatabase.PQuery("SELECT name, token FROM guild_level_invested WHERE token > 0 AND guild = %u ORDER BY token DESC LIMIT 15", guild);
	if (result) 
	{
		player->ADD_GOSSIP_ITEM(4, "Топ вложивших: DN монеты", GOSSIP_SENDER_MAIN, 0);
		do 
		{
			Field* fields = result->Fetch();
			std::string name = fields[0].GetString();
			uint32 token = fields[1].GetUInt32();
			
			std::stringstream buffer;
			buffer << step <<". |cff790C06" << name << "|r" << " вложил " << "|cFF1E90FF" << token << "|r DN монет" << "|r";
			player->ADD_GOSSIP_ITEM(4, buffer.str(), GOSSIP_SENDER_MAIN, 0);			
			++step;
			
		} while (result->NextRow());		
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
	}
	if (!result)
	{
		player->CLOSE_GOSSIP_MENU();
		handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Ещё никто не вкладывал |cFFFF0000DN монеты");
	}
}

void InvestedBossToken(Player* player, Creature* creature) 
{
	player->PlayerTalkClass->ClearMenus();
	uint32 guild = player->GetGuildId();
	ChatHandler handler(player);
	uint32 step = 1;
	
	QueryResult result = CharacterDatabase.PQuery("SELECT name, bosstoken FROM guild_level_invested WHERE bosstoken > 0 AND guild = %u ORDER BY bosstoken DESC LIMIT 15", guild);
	if (result) 
	{
		player->ADD_GOSSIP_ITEM(4, "Топ вложивших: Босс Монетки", GOSSIP_SENDER_MAIN, 0);
		do 
		{
			Field* fields = result->Fetch();
			std::string name = fields[0].GetString();
			uint32 bosstoken = fields[1].GetUInt32();
			
			std::stringstream buffer;
			buffer << step <<". |cff790C06" << name << "|r" << " вложил " << "|cFF1E90FF" << bosstoken << "|r Босс Монеток" << "|r";
			player->ADD_GOSSIP_ITEM(4, buffer.str(), GOSSIP_SENDER_MAIN, 0);
			++step;
			
		} while (result->NextRow());		
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
	}
	if (!result)
	{
		player->CLOSE_GOSSIP_MENU();
		handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Ещё никто не вкладывал |cFFFF0000Босс Монетки");
	}
}

void InvestedRosterHonor(Player* player, Creature* creature) 
{
	player->PlayerTalkClass->ClearMenus();
	uint32 guild = player->GetGuildId();
	ChatHandler handler(player);
	uint32 step = 1;
	
	QueryResult result = CharacterDatabase.PQuery("SELECT name, honor FROM guild_level_invested WHERE honor > 0 AND guild = %u ORDER BY honor DESC LIMIT 15", guild);
	if (result) 
	{
		player->ADD_GOSSIP_ITEM(4, "Топ вложивших: Очки Чести", GOSSIP_SENDER_MAIN, 0);
		do 
		{
			Field* fields = result->Fetch();
			std::string name = fields[0].GetString();
			uint32 honor = fields[1].GetUInt32();
			
			std::stringstream buffer;
			buffer << step <<". |cff790C06" << name << "|r" << " вложил " << "|cFF1E90FF" << honor << "|r очков чести" << "|r";
			player->ADD_GOSSIP_ITEM(4, buffer.str(), GOSSIP_SENDER_MAIN, 0);
			++step;
		} while (result->NextRow());
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
	}
	if (!result)
	{
		player->CLOSE_GOSSIP_MENU();
		handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Ещё никто не вкладывал |cFFFF0000Очки чести");
	}
}

class item_tpguild : public ItemScript
{
public:
	item_tpguild() : ItemScript("item_tpguild") { }
	
	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) 
	{
		uint32 guild = player->GetGuildId();
		QueryResult result = CharacterDatabase.PQuery("SELECT x, y, z, map FROM guildhouses WHERE guild = %u", guild);
		if (result && !player->isInCombat()) 
		{
			float x,y,z;
			uint32 map;
			std::string name;

				Field* fields = result->Fetch();
			    x = fields[0].GetFloat();
			    y = fields[1].GetFloat();
				z = fields[2].GetFloat();
				map = fields[3].GetUInt32();

				player->TeleportTo(map,x,y,z,0.0f,0U);
				player->RemoveSpellCooldown(8690, true);
				player->SetPhaseMask(1, true);
		}
		return true;
	}
	
};

class npc_guild_level : public CreatureScript
{
    public: npc_guild_level() : CreatureScript("npc_guild_level") {}

	bool OnGossipHello(Player* player, Creature* creature) 
	{
		player->PlayerTalkClass->ClearMenus();
		uint32 guild = player->GetGuildId();

		if (!guild) 
		{
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			return false;
		}
		
		QueryResult result = CharacterDatabase.PQuery("SELECT level, xp FROM guild_level WHERE guild = %u", guild);
		uint32 level = result->Fetch()->GetUInt16();

		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Magic_ManaGain:32:32:-18:0|tИнформация о моей гильдии", GOSSIP_SENDER_MAIN, 2);		
		
		if (player->GetRank() == 0 && level >= 2)
			player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Magic_ManaGain:32:32:-18:0|tСменить цвет названия ги", GOSSIP_SENDER_MAIN, 6);		

		if (level >= 7 && level <= 13 && player->GetRank() <= 1)
			player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Magic_ManaGain:32:32:-18:0|tВоскр. существ 35 DN Монет с банка ги", GOSSIP_SENDER_MAIN, 21);
		
		if (level >= 14 && level <= 20 && player->GetRank() <= 1)
			player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Magic_ManaGain:32:32:-18:0|tВоскр. существ 30 DN Монет с банка ги", GOSSIP_SENDER_MAIN, 18);
			
		if (level >= 21 && level <= 27 && player->GetRank() <= 1)
			player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Magic_ManaGain:32:32:-18:0|tВоскр. существ 25 DN Монет с банка ги", GOSSIP_SENDER_MAIN, 17);
		
		if (level >= 28 && player->GetRank() <= 1)
			player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Magic_ManaGain:32:32:-18:0|tВоскр. существ 20 DN Монет с банка ги", GOSSIP_SENDER_MAIN, 16);
		
		if (level < 30)
			player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Achievement_BG_kill_flag_carrierEOS:32:32:-18:0|tПожертвование для гильдии", GOSSIP_SENDER_MAIN, 19);
		
		player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Sword_131:32:32:-18:0|tПополнить баланс гильди DN Монетами", GOSSIP_SENDER_MAIN, 200, "Введите сумму, которую хотите пожертвовать", 0, true);		
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Temp:32:32:-18:0|tТоп вложивших в гильдию", GOSSIP_SENDER_MAIN, 13);
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_ChargeNegative:32:32:-18:0|tВыход", GOSSIP_SENDER_MAIN, 100);
	
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());

		return true;
	}

	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) 
	{
		switch (action) 
		{
			case 100:
				player->CLOSE_GOSSIP_MENU();
				break;				
			case 2:
				GuildInfo(player);
				break;				
			case 6:
				ReplaceColorList(player, creature);
				break;
			case 7:
				ReplaceColor(player, creature, 1);
				break;
			case 8:
				ReplaceColor(player, creature, 2);
				break;
			case 9:
				ReplaceColor(player, creature, 3);
				break;
			case 10:
				ReplaceColor(player, creature, 4);
				break;
			case 11:
				ReplaceColor(player, creature, 5);
				break;
			case 12:
				ReplaceColor(player, creature, 6);
				break;
			case 13:
				InvestedRoster(player, creature);
				break;
			case 14:
				InvestedToken(player, creature);
				break;
			case 15:
				InvestedRosterHonor(player, creature);
				break;
			case 16:
				TakeResWorld28(player, creature);
				break;
			case 17:
				TakeResWorld21(player, creature);
				break;	
			case 18:
				TakeResWorld14(player, creature);
				break;
			case 19:
				Invested(player, creature);
				break;
			case 20:
				InvestedBossToken(player, creature);
				break;
			case 21:
				TakeResWorld7(player, creature);
				break;
		}

	return true;
	}
	
	bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code)
    {
		switch(action)
		{
		case 200:
			TakeDN(player, creature, code);
			break;
		case 1:
			TakeHonor(player, creature, code);
			break;
		case 3:
			TakeBossToken(player, creature, code);
			break;
		}
		return true;
	}
};

/*class npc_guildhouse : public CreatureScript
{

    public: 

    npc_guildhouse() : CreatureScript("npc_guildhouse") {}

	bool OnGossipHello(Player* player, Creature* creature) {
	player->PlayerTalkClass->ClearMenus();
	uint16 count;
	uint32 guild = player->GetGuildId();
	
	if (guild)
	{
		if (player->GetRank() <= 1)
		{
			QueryResult result = CharacterDatabase.PQuery("SELECT comment FROM guildhouses WHERE guild = 0");
			if (result)
			{
				Field* fields = result->Fetch();
				std::string text = field[0].GetString();
				if (count > 0)
				{
					std::ostringstream psm;
					psm << "|TInterface/ICONS/Spell_Arcane_TeleportTheramore:32:32:-18:0|tПосмотреть место для Дома Гильдии";
					std::ostringstream kpt;
					kpt << "|TInterface/ICONS/Spell_Arcane_TeleportSilvermoon:32:32:-18:0|tКупить место для Дома Гильдии";
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, psm.str(), GOSSIP_SENDER_MAIN, 1);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, kpt.str(), GOSSIP_SENDER_MAIN, 2);
				}
			}
			else
			{
				player->GetSession()->SendNotification("Свободных мест нет!");
				player->CLOSE_GOSSIP_MENU();
				return false;
			}
		}
		QueryResult result5 = CharacterDatabase.PQuery("SELECT guild FROM guildhouses WHERE guild = %u", guild);
		if (result5) 
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/ICONS/Spell_Arcane_TeleportThunderBluff:32:32:-18:0|tТелепорт в Дом своей гильдии", GOSSIP_SENDER_MAIN, 3);

		std::ostringstream vxd;
		vxd << "|TInterface/ICONS/Spell_ChargeNegative:32:32:-18:0|tВыход";

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, vxd.str(), GOSSIP_SENDER_MAIN, 4);
	}
	else
		{
			player->SEND_GOSSIP_MENU(55004, creature->GetGUID());
		    return false;
		}
	player->SEND_GOSSIP_MENU(55001, creature->GetGUID());

	return true;
	}
	
	void LookGuildHouses(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(4, "Список доступных ГХ:", GOSSIP_SENDER_MAIN, 200);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, x, y, z, radius, cost, number, number2 FROM guildhouses WHERE guild = 0 ORDER BY cost");
		if (result) 
		{
			float x,y,z,radius;
			uint32 cost, number, number_2;
			const char* name;
			uint16 go = 0;
			do
			{
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    x = fields[1].GetFloat();
			    y = fields[2].GetFloat();
				z = fields[3].GetFloat();
			    radius = fields[4].GetFloat();
			    cost = fields[5].GetUInt32();
				number = fields[6].GetUInt32();
				number_2 = fields[7].GetUInt32();

				if (go == 30)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 101);
			        break;
				}
				
				std::stringstream buffer;
                buffer << "Посмотреть ГХ: " << "|cff177245" << name << "|r" << ". Цена: " << "|cffE024B4" << cost << " опыта" << "|r";
			    if (i == 1) player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				if (i == 2) player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number_2);

		        go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) 
	{
            switch (action) 
			{
			case 1:
				i = 1;
				LookGuildHouses(player, creature);
				break;
			case 2:
				i = 2;
				LookGuildHouses(player, creature);
				break;
			case 3:
				TeleportToGH(player);
				break;
			case 100:
				LookGuildHouses(player, creature);
				break;
			case 101:
				LookGuildHouses2(player, creature);
				break;
			case 4:
				player->CLOSE_GOSSIP_MENU();
				break;
			case 200:
				LookGuildHouses(player, creature);
				break;
			case 201:
				LookGuildHouses2(player, creature);
				break;
            }
			if (i == 1) TeleportToLook(player, action);
			if (i == 2) BuyGuildHouse(player, action);
	return true;
	}

	void BuyGuildHouse(Player* player, uint32 action)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT number2, cost FROM guildhouses WHERE guild = 0 ORDER BY cost");
		if (result) 
		{
			uint16 level, level_table;
			uint32 number_2, cost, xp, xp_table;
			uint32 guild = player->GetGuildId();
			do
			{
				Field* fields = result->Fetch();
				number_2 = fields[0].GetUInt32();
				cost = fields[1].GetUInt32();

				if (action == number_2)
				{
					
					QueryResult result2 = CharacterDatabase.PQuery("SELECT level, xp FROM guild_level WHERE guild = %u", guild);
		            if (result2)
					{
					    Field *fields = result2->Fetch();
		                level = fields[0].GetUInt16();
					    xp = fields[1].GetUInt32();
					    if (xp >= cost)
						{
							CharacterDatabase.PQuery("UPDATE guildhouses SET guild = %u WHERE number2 = %u", guild, number_2);
							CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp - %u WHERE guild = %u", cost, guild);
							if (!player->HasSpell(69541)) player->learnSpell(69541,false);
					        player->GetSession()->SendAreaTriggerMessage("Место для дома гильдии куплено!");
							ChatHandler(player->GetSession()).PSendSysMessage("|cfff4b25eВы купили место для дома гильдии. |r");
							ChatHandler(player->GetSession()).PSendSysMessage("|cfff4b25eЧтобы начать его обустраивать вызовите пандарена-монаха из списка небоевых спутников.|r");
					        player->CLOSE_GOSSIP_MENU();
							break;
						}
						else
						{
							player->GetSession()->SendNotification("Недостаточно опыта гильдии!");
							player->CLOSE_GOSSIP_MENU();
							break;
						}
					}
				}
				}			
			while (result->NextRow());
		}
	}
		
	void TeleportToGH(Player* player)
	{
		uint32 guild = player->GetGuildId();
		QueryResult result = CharacterDatabase.PQuery("SELECT x, y, z, map FROM guildhouses WHERE guild = %u", guild);
		if (result) 
		{
			float x,y,z;
			uint32 map;
			std::string name;

				Field* fields = result->Fetch();
			    x = fields[0].GetFloat();
			    y = fields[1].GetFloat();
				z = fields[2].GetFloat();
				map = fields[3].GetUInt32();

				player->TeleportTo(map,x,y,z,0.0f,0U);
				player->RemoveSpellCooldown(8690, true);
				player->SetPhaseMask(1, true);
		}
	}

	void TeleportToLook(Player* player, uint32 action)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT x, y, z, map, number, number2 FROM guildhouses WHERE guild = 0 ORDER BY cost");
		if (result) 
		{
			float x,y,z;
			uint32 number, map;
			std::string name;
			do
			{
				Field* fields = result->Fetch();
			    x = fields[0].GetFloat();
			    y = fields[1].GetFloat();
				z = fields[2].GetFloat();
				map = fields[3].GetUInt32();
				number = fields[4].GetUInt32();

				if (action == number)
				{
					player->SaveRecallPosition();
					player->TeleportTo(map,x,y,z,0.0f,0U);
					player->RemoveArenaSpellCooldowns();
					player->RemoveSpellCooldown(8690, true);
					player->SetPhaseMask(1, true);
					if (!player->HasSpell(69541)) player->learnSpell(69541,false);
					ChatHandler(player->GetSession()).PSendSysMessage("|cfff4b25eВы решили посмотреть место для дома гильдии.|r");
					ChatHandler(player->GetSession()).PSendSysMessage("|cfff4b25eЧтобы телепортироваться обратно вызовите пандарена-монаха из списка небоевых спутников|r");
					break;
				}

			}
			while (result->NextRow());
		}
	}
	
};*/

/*
class npc_guildhouse : public CreatureScript
{

    public: 

    npc_guildhouse() : CreatureScript("npc_guildhouse") {}

	bool OnGossipHello(Player* player, Creature* creature) {
	player->PlayerTalkClass->ClearMenus();
	uint16 count;
	if (uint32 guild = player->GetGuildId())
	{
	if (player->GetRankFromDB(player->GetGUID()) == 0)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(guild) FROM guildhouses WHERE guild = 0");
		if (result)
		{
		    QueryResult result2 = CharacterDatabase.PQuery("SELECT guild FROM guildhouses WHERE guild = %u", guild);
		    if (!result2)
			{
			Field* fields = result->Fetch();
			count = fields[0].GetUInt16();
		    if (count > 0)
		    {
				std::ostringstream psm;
                psm << "|TInterface/ICONS/Spell_Arcane_TeleportTheramore:32:32:-18:0|tПосмотреть место для Дома Гильдии";
				std::ostringstream kpt;
                kpt << "|TInterface/ICONS/Spell_Arcane_TeleportSilvermoon:32:32:-18:0|tКупить место для Дома Гильдии";
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, psm.str(), GOSSIP_SENDER_MAIN, 1);
	            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, kpt.str(), GOSSIP_SENDER_MAIN, 2);
		    }
			}
		}
		else
		{
			player->GetSession()->SendNotification("Свободных мест нет!");
		    player->CLOSE_GOSSIP_MENU();
		    return false;
		}
	}
	std::ostringstream tpd;
    tpd << "|TInterface/ICONS/Spell_Arcane_TeleportThunderBluff:32:32:-18:0|tТелепорт в Дом своей гильдии";
	QueryResult result5 = CharacterDatabase.PQuery("SELECT guild FROM guildhouses WHERE guild = %u", guild);
	if (result5) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, tpd.str(), GOSSIP_SENDER_MAIN, 3);

	std::ostringstream vxd;
    vxd << "|TInterface/ICONS/Spell_ChargeNegative:32:32:-18:0|tВыход";

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, vxd.str(), GOSSIP_SENDER_MAIN, 4);
	}
	else
		{
			player->SEND_GOSSIP_MENU(55004, creature->GetGUID());
		    return false;
		}
	player->SEND_GOSSIP_MENU(55001, creature->GetGUID());

	return true;
	}
	
	void LookGuildHouses(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(4, "Список доступных ГХ:", GOSSIP_SENDER_MAIN, 200);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, x, y, z, radius, cost, number, number2 FROM guildhouses WHERE guild = 0 ORDER BY cost");
		if (result) 
		{
			float x,y,z,radius;
			uint32 cost, number, number_2;
			const char* name;
			uint16 go = 0;
			do
			{
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    x = fields[1].GetFloat();
			    y = fields[2].GetFloat();
				z = fields[3].GetFloat();
			    radius = fields[4].GetFloat();
			    cost = fields[5].GetUInt32();
				number = fields[6].GetUInt32();
				number_2 = fields[7].GetUInt32();

				if (go == 30)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 101);
			        break;
				}
				
				std::stringstream buffer;
                buffer << "Посмотреть ГХ: " << "|cff177245" << name << "|r" << ". Цена: " << "|cffE024B4" << cost << " опыта" << "|r";
			    if (i == 1) player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				if (i == 2) player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number_2);

		        go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
	}

	void LookGuildHouses2(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(4, "Список доступных ГХ:", GOSSIP_SENDER_MAIN, 201);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 100);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, x, y, z, radius, cost, number, number2 FROM guildhouses WHERE guild = 0 ORDER BY cost");
		if (result) 
		{
			float x,y,z,radius;
			uint32 cost, number, number_2;
			const char* name;
			uint16 go = 0;
			do
			{
				if (go < 30)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    x = fields[1].GetFloat();
			    y = fields[2].GetFloat();
				z = fields[3].GetFloat();
			    radius = fields[4].GetFloat();
			    cost = fields[5].GetUInt32();
				number = fields[6].GetUInt32();
				number_2 = fields[7].GetUInt32();

				if (go == 60) break;
				
				std::stringstream buffer;
                buffer << "Посмотреть ГХ: " << "|cff177245" << name << "|r" << ". Цена: " << "|cffE024B4" << cost << " опыта" << "|r";
			    if (i == 1) player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				if (i == 2) player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number_2);

		        go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
	}

	uint8 i;

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) 
	{
            switch (action) 
			{
			case 1:
				i = 1;
				LookGuildHouses(player, creature);
				break;
			case 2:
				i = 2;
				LookGuildHouses(player, creature);
				break;
			case 3:
				TeleportToGH(player);
				break;
			case 100:
				LookGuildHouses(player, creature);
				break;
			case 101:
				LookGuildHouses2(player, creature);
				break;
			case 4:
				player->CLOSE_GOSSIP_MENU();
				break;
			case 200:
				LookGuildHouses(player, creature);
				break;
			case 201:
				LookGuildHouses2(player, creature);
				break;
            }
			if (i == 1) TeleportToLook(player, action);
			if (i == 2) BuyGuildHouse(player, action);
	return true;
	}

	void BuyGuildHouse(Player* player, uint32 action)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT number2, cost FROM guildhouses WHERE guild = 0 ORDER BY cost");
		if (result) 
		{
			uint16 level, level_table;
			uint32 number_2, cost, xp, xp_table;
			uint32 guild = player->GetGuildId();
			do
			{
				Field* fields = result->Fetch();
				number_2 = fields[0].GetUInt32();
				cost = fields[1].GetUInt32();

				if (action == number_2)
				{
					
					QueryResult result2 = CharacterDatabase.PQuery("SELECT level, xp FROM guild_level WHERE guild = %u", guild);
		            if (result2)
					{
					    Field *fields = result2->Fetch();
		                level = fields[0].GetUInt16();
					    xp = fields[1].GetUInt32();
					    if (xp >= cost)
						{
							CharacterDatabase.PQuery("UPDATE guildhouses SET guild = %u WHERE number2 = %u", guild, number_2);
							CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp - %u WHERE guild = %u", cost, guild);
							if (!player->HasSpell(69541)) player->learnSpell(69541,false);
					        player->GetSession()->SendAreaTriggerMessage("Место для дома гильдии куплено!");
							ChatHandler(player->GetSession()).PSendSysMessage("|cfff4b25eВы купили место для дома гильдии. |r");
							ChatHandler(player->GetSession()).PSendSysMessage("|cfff4b25eЧтобы начать его обустраивать вызовите пандарена-монаха из списка небоевых спутников.|r");
					        player->CLOSE_GOSSIP_MENU();
							break;
						}
						else
						{
							player->GetSession()->SendNotification("Недостаточно опыта гильдии!");
							player->CLOSE_GOSSIP_MENU();
							break;
						}
					}
				}
				}			
			while (result->NextRow());
		}
	}
		
	void TeleportToGH(Player* player)
	{
		uint32 guild = player->GetGuildId();
		QueryResult result = CharacterDatabase.PQuery("SELECT x, y, z, map FROM guildhouses WHERE guild = %u", guild);
		if (result) 
		{
			float x,y,z;
			uint32 map;
			std::string name;

				Field* fields = result->Fetch();
			    x = fields[0].GetFloat();
			    y = fields[1].GetFloat();
				z = fields[2].GetFloat();
				map = fields[3].GetUInt32();

				player->TeleportTo(map,x,y,z,0.0f,0U);
				player->RemoveSpellCooldown(8690, true);
				player->SetPhaseMask(1, true);
		}
	}

	void TeleportToLook(Player* player, uint32 action)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT x, y, z, map, number, number2 FROM guildhouses WHERE guild = 0 ORDER BY cost");
		if (result) 
		{
			float x,y,z;
			uint32 number, map;
			std::string name;
			do
			{
				Field* fields = result->Fetch();
			    x = fields[0].GetFloat();
			    y = fields[1].GetFloat();
				z = fields[2].GetFloat();
				map = fields[3].GetUInt32();
				number = fields[4].GetUInt32();

				if (action == number)
				{
					player->SaveRecallPosition();
					player->TeleportTo(map,x,y,z,0.0f,0U);
					player->RemoveArenaSpellCooldowns();
					player->RemoveSpellCooldown(8690, true);
					player->SetPhaseMask(1, true);
					if (!player->HasSpell(69541)) player->learnSpell(69541,false);
					ChatHandler(player->GetSession()).PSendSysMessage("|cfff4b25eВы решили посмотреть место для дома гильдии.|r");
					ChatHandler(player->GetSession()).PSendSysMessage("|cfff4b25eЧтобы телепортироваться обратно вызовите пандарена-монаха из списка небоевых спутников|r");
					break;
				}

			}
			while (result->NextRow());
		}
	}
	
};

class npc_guildhouse_builder : public CreatureScript
{

    public: 

    npc_guildhouse_builder() : CreatureScript("npc_guildhouse_builder") {}

	
	bool OnGossipHello(Player* player, Creature* creature) {
	player->PlayerTalkClass->ClearMenus();
	if (uint32 guild = player->GetGuildId()) {
		if (player->GetRankFromDB(player->GetGUID()) == 0) {
		QueryResult haveGuildHouse = CharacterDatabase.PQuery("SELECT * FROM guildhouses WHERE guild = %u", guild);
		if (haveGuildHouse)
		{
	QueryResult result5 = CharacterDatabase.PQuery("SELECT guild FROM guildhouses WHERE guild = %u", guild);
	std::ostringstream pnp;
    pnp << "|TInterface/ICONS/Spell_Holy_Rapture:32:32:-18:0|tПоставить НПС";
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, pnp.str(), GOSSIP_SENDER_MAIN, 8);
	std::ostringstream pob;
    pob << "|TInterface/ICONS/Spell_Nature_NullWard:32:32:-18:0|tПоставить объект";
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, pob.str(), GOSSIP_SENDER_MAIN, 1);
	std::ostringstream dnpc;
    dnpc << "|TInterface/ICONS/Spell_Shadow_DeathScream:32:32:-18:0|tУдалить поставленного НПС";
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, dnpc.str(), GOSSIP_SENDER_MAIN, 9);
	std::ostringstream danpc;
    danpc << "|TInterface/ICONS/Spell_Shadow_DeadofNight:32:32:-18:0|tУдалить всех НПС в Доме Гильдии";
	player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_BATTLE, danpc.str(), GOSSIP_SENDER_MAIN, 11, "Вы действительно хотите удалить всех НПС? Введите 123 для удаления!", 0, true);
	std::ostringstream dobj;
    dobj << "|TInterface/ICONS/Spell_Shadow_FingerOfDeath:32:32:-18:0|tУдалить ближайший объект";
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE,dobj.str() , GOSSIP_SENDER_MAIN, 2);
	std::ostringstream daobj;
    daobj << "|TInterface/ICONS/Spell_Shadow_GrimWard:32:32:-18:0|tУдалить все объекты в Доме Гильдии";
	player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_BATTLE, daobj.str(), GOSSIP_SENDER_MAIN, 3, "Вы действительно хотите удалить все объекты? Введите 123 для удаления!", 0, true);
	std::ostringstream prad;
    prad << "|TInterface/ICONS/Spell_ChargePositive:32:32:-18:0|tРасширить радиус ГХ на 10 ярдов(500 опыта)";
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, prad.str(), GOSSIP_SENDER_MAIN, 5);
	std::ostringstream zst;
    zst << "|TInterface/ICONS/Spell_Fire_FelHellfire:32:32:-18:0|tЗАСТРЯЛ!";
	if (result5) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, zst.str(), GOSSIP_SENDER_MAIN, 7);
		}
		else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Телепортироваться обратно", GOSSIP_SENDER_MAIN, 15);
		}
	}
	else
	{
	player->SEND_GOSSIP_MENU(55004, creature->GetGUID());
	return false;
	}
	std::ostringstream vxd;
    vxd << "|TInterface/ICONS/Spell_ChargeNegative:32:32:-18:0|tВыход";
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, vxd.str(), GOSSIP_SENDER_MAIN, 4);
	player->SEND_GOSSIP_MENU(55002, creature->GetGUID());

	return true;
	}

	void AddGobject1(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();
				if (go == 30)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 101);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
	}

	void AddGobject2(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 100);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 30)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 60)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 102);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddGobject3(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 101);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 60)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 90)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 103);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddGobject4(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 102);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 90)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 120)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 104);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddGobject5(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 103);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 120)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 150)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 105);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddGobject6(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 104);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 150)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 180)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 106);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddGobject7(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 105);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 180)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 210)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 107);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddGobject8(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 106);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 210)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 240)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 108);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddGobject9(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 107);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 240)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 270)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 109);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddGobject10(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 108);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_go ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 270)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 300) break;

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}


	void AddNPC1(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();
				if (go == 30)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 151);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
	}
	
	
	
	void AddNPC2(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 150);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 30)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 60)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 152);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}
	
	void AddNPC3(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 151);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 60)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 90)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 153);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddNPC4(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 152);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 90)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 120)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 154);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddNPC5(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 153);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 120)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 150)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 155);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddNPC6(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 154);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 150)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 180)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 156);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddNPC7(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 155);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 180)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 210)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 157);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddNPC8(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 156);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 210)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 240)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 158);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddNPC9(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 157);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 240)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 270)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Дальше", GOSSIP_SENDER_MAIN, 159);
			        break;
				}

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void AddNPC10(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<= Назад", GOSSIP_SENDER_MAIN, 158);
		QueryResult result = CharacterDatabase.PQuery("SELECT name, cost, number FROM guildhouse_npc ORDER BY name"); 
 
		if (result) 
		{
			const char* name;
			uint32 cost, number;
			uint16 go = 0;
			do
			{
				if (go < 270)
				{
					go++;
					continue;
				}
				Field* fields = result->Fetch();
			    name = fields[0].GetCString();
			    cost = fields[1].GetUInt32();
				number = fields[2].GetUInt32();

				if (go == 300) break;

				std::stringstream buffer;
                buffer << "Поставить: " << "|cff177245" << name << "|r" << ". Цена: " << "|cff242DFF" << cost << " опыта" << "|r";
				player->ADD_GOSSIP_ITEM(4, buffer.str().c_str(), GOSSIP_SENDER_MAIN, number);
				go++;
			}
			while (result->NextRow());
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

	}

	void PlusRadius(Player* player)
	{
		
		uint32 guild = player->GetGuildId();
		QueryResult result15 = CharacterDatabase.PQuery("SELECT * FROM guildhouses WHERE guild = %u", guild);
		if (!result15)
		{
		player->CLOSE_GOSSIP_MENU();
		player->GetSession()->SendNotification("У вас нет Дома Гильдии!");
		return;
		}
		QueryResult result10 = CharacterDatabase.PQuery("SELECT radius FROM guildhouses WHERE guild = %u AND radius >= 80", guild);
		                    if (result10)
							{
								player->CLOSE_GOSSIP_MENU();
								player->GetSession()->SendNotification("У вас максимальный радиус ГХ!");
								return;
							}
		uint32 level_table, xp_table, xp;
		QueryResult result2 = CharacterDatabase.PQuery("SELECT xp FROM guild_level WHERE guild = %u", guild);
		            if (result2)
					{
					    Field *fields = result2->Fetch();
					    xp = fields[0].GetUInt32();
					}
					if (xp < 500)
					{
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendNotification("Недостаточно гильдейского опыта!");
						return;
					}
		CharacterDatabase.PQuery("UPDATE guildhouses SET radius = radius+10 WHERE guild = %u", guild);
		player->GetSession()->SendAreaTriggerMessage("Радиус вашего дома гильдии расширен на 10 ярдов!");
        CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp-500 WHERE guild = %u", guild);

	}

	void DeleteAllGobjects(Player* player, Creature* creature, char const* code)
	{
		
		uint32 da = atoi(code);
		if (da != 123) return;
		uint32 lowguid;
		uint64 leader_guid = player->GetGUID();
		QueryResult result = CharacterDatabase.PQuery("SELECT guid FROM guildhouse_spawned WHERE leader_guid = %u", leader_guid);
		if (!result)
		{
			player->CLOSE_GOSSIP_MENU();
			player->GetSession()->SendNotification("Вы не поставили не единого объекта!");
		    return;
		}
		if (result) 
		{
			do
			{
			Field* fields = result->Fetch();
			lowguid = fields[0].GetUInt32();
				if(!lowguid)
            return;
		
        GameObject* obj = NULL;
        // by DB guid
        if(GameObjectData const* go_data = sObjectMgr->GetGOData(lowguid))
            obj = ChatHandler(player->GetSession()).GetObjectGlobalyWithGuidOrNearWithDbGuid(lowguid, go_data->id);

        if(!obj)
        {
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, lowguid);
            ChatHandler(player->GetSession()).SetSentErrorMessage(true);
            return;
        }

		QueryResult res = CharacterDatabase.PQuery("SELECT cost FROM guildhouse_go WHERE id = %u", obj->GetEntry());
		if (res)
		{
		Field* ff = res->Fetch();
		uint32 cost = ff[0].GetUInt32();
		
		CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp+%u WHERE guild = %u",cost ,player->GetGuildId());
		}
		
        uint64 owner_guid = obj->GetOwnerGUID();
        if(owner_guid)
        {
            Unit* owner = ObjectAccessor::GetUnit(*ChatHandler(player->GetSession()).GetSession()->GetPlayer(), owner_guid);
            if(!owner || !IS_PLAYER_GUID(owner_guid))
            {
                ChatHandler(player->GetSession()).PSendSysMessage(LANG_COMMAND_DELOBJREFERCREATURE, GUID_LOPART(owner_guid), obj->GetGUIDLow());
                ChatHandler(player->GetSession()).SetSentErrorMessage(true);
                return;
            }

            owner->RemoveGameObject(obj, false);
        }

        obj->SetRespawnTime(0);                                 // not save respawn time
        obj->Delete();
        obj->DeleteFromDB();

		player->CLOSE_GOSSIP_MENU();
			}
			while (result->NextRow());
			CharacterDatabase.PQuery("DELETE FROM guildhouse_spawned WHERE leader_guid = %u", leader_guid);
			ChatHandler(player->GetSession()).PSendSysMessage("Объекты были успешно удалены!");
		}
	}

	void DeleteAllNPC(Player* player, Creature* creature, char const* code)
	{
		
		uint32 da = atoi(code);
		if (da != 123) return;
		uint32 lowguid;
		uint64 leader_guid = player->GetGUID();
		QueryResult result = CharacterDatabase.PQuery("SELECT guid FROM guildhouse_npc_spawned WHERE leader_guid = %u", leader_guid);
		if (!result)
		{
			player->CLOSE_GOSSIP_MENU();
			player->GetSession()->SendNotification("Вы не поставили не единого НПС!");
		    return;
		}
		if (result) 
		{
			do
			{
			Field* fields = result->Fetch();
			lowguid = fields[0].GetUInt32();
			if(!lowguid) return;
		Creature* unit = NULL;
        if(CreatureData const* cr_data = sObjectMgr->GetCreatureData(lowguid))
                unit = player->GetMap()->GetCreature(MAKE_NEW_GUID(lowguid, cr_data->id, HIGHGUID_UNIT));
				
		QueryResult res = CharacterDatabase.PQuery("SELECT cost FROM guildhouse_npc WHERE id = %u", unit->GetEntry());
		if (res)
		{
		Field* ff = res->Fetch();
		uint32 cost = ff[0].GetUInt32();
		
		CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp+%u WHERE guild = %u",cost ,player->GetGuildId());
		}

        if(!unit || unit->isPet() || unit->isTotem()) return;

        // Delete the creature
        unit->CombatStop();
        unit->DeleteFromDB();
        unit->AddObjectToRemoveList();
		CharacterDatabase.PQuery("DELETE FROM guildhouse_npc_spawned WHERE guid = %u", lowguid);

		player->GetSession()->SendAreaTriggerMessage("НПС успешно удален!");

		player->CLOSE_GOSSIP_MENU();
			}
			while (result->NextRow());
			CharacterDatabase.PQuery("DELETE FROM guildhouse_npc_spawned WHERE leader_guid = %u", leader_guid);
			ChatHandler(player->GetSession()).PSendSysMessage("НПС были успешно удалены!");
		}
	}

	void DeleteNPC(Player* player, Creature* creature)
	{
		
		Creature* unit = player->GetSelectedUnit()->ToCreature();

		uint32 leader_guid = player->GetGUIDLow();
		uint64 lowguid = unit->GetGUIDLow();;


		QueryResult result = CharacterDatabase.PQuery("SELECT guid, leader_guid FROM guildhouse_npc_spawned WHERE leader_guid = %u AND guid = %u", leader_guid, lowguid);
		if (!result) 
		{
			player->CLOSE_GOSSIP_MENU();
			//player->GetSession()->SendNotification("НПС с таким номером не найден!", lowguid, leader_guid);
		    player->GetSession()->SendNotification("НПС с таким номером не найден!");
			return;
		}
		
        if(!unit || unit->isPet() || unit->isTotem()) return;

		QueryResult res = CharacterDatabase.PQuery("SELECT cost FROM guildhouse_npc WHERE id = %u", unit->GetEntry());
		if (res)
		{
		Field* ff = res->Fetch();
		uint32 cost = ff[0].GetUInt32();
		
		CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp+%u WHERE guild = %u",cost ,player->GetGuildId());
		}
		
        // Delete the creature
        unit->CombatStop();
        unit->DeleteFromDB();
        unit->AddObjectToRemoveList();
		CharacterDatabase.PQuery("DELETE FROM guildhouse_npc_spawned WHERE guid = %u", lowguid);

		player->GetSession()->SendAreaTriggerMessage("НПС успешно удален!");
		player->CLOSE_GOSSIP_MENU();
	}
	
	void ActivateGobject(Player* player, Creature* creature)
	{
        QueryResult result;
		GameEventMgr::ActiveEvents const& activeEventsList = sGameEventMgr->GetActiveEventList();

		std::ostringstream eventFilter;
            eventFilter << " AND (eventEntry IS NULL ";
            bool initString = true;

            for (GameEventMgr::ActiveEvents::const_iterator itr = activeEventsList.begin(); itr != activeEventsList.end(); ++itr)
            {
                if (initString)
                {
                    eventFilter  <<  "OR eventEntry IN (" << *itr;
                    initString = false;
                }
                else
                    eventFilter << ',' << *itr;
            }

            if (!initString)
                eventFilter << "))";
            else
                eventFilter << ')';
		result = WorldDatabase.PQuery("SELECT gameobject.guid, id, position_x, position_y, position_z, orientation, map, phaseMask, "
                "(POW(position_x - %f, 2) + POW(position_y - %f, 2) + POW(position_z - %f, 2)) AS order_ FROM gameobject "
                "LEFT OUTER JOIN game_event_gameobject on gameobject.guid = game_event_gameobject.guid WHERE map = '%i' %s ORDER BY order_ ASC LIMIT 10",
                player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetMapId(), eventFilter.str().c_str());

		if (!result)
        {
			player->GetSession()->SendNotification("Поблизости не найдено объектов вашего Дома Гильдии!");
            return;
        }

        bool found = false;
        float x, y, z, o;
        uint32 guidLow, id, phase;
        uint16 mapId;

        do
        {
            Field* fields = result->Fetch();
            guidLow = fields[0].GetUInt32();
            id =      fields[1].GetUInt32();
            x =       fields[2].GetFloat();
            y =       fields[3].GetFloat();
            z =       fields[4].GetFloat();
            o =       fields[5].GetFloat();
            mapId =   fields[6].GetUInt16();
            phase =   fields[7].GetUInt32();
			QueryResult result15 = CharacterDatabase.PQuery("SELECT guid FROM guildhouse_spawned WHERE leader_guid = %u AND guid = %u", player->GetGUIDLow(), guidLow);
            if (result15) found = true;
        } while (result->NextRow() && !found);

        if (!found)
        {
			//player->GetSession()->SendNotification(LANG_GAMEOBJECT_NOT_EXIST);
            return;
        }

        GameObjectTemplate const* objectInfo = sObjectMgr->GetGameObjectTemplate(id);

        if (!objectInfo)
        {
           // player->GetSession()->SendNotification(LANG_GAMEOBJECT_NOT_EXIST);
            return;
        }

        GameObject* target = player->GetMap()->GetGameObject(MAKE_NEW_GUID(guidLow, id, HIGHGUID_GAMEOBJECT));

        if (target)
        {
		QueryResult res = CharacterDatabase.PQuery("SELECT cost FROM guildhouse_go WHERE id = %u", target->GetEntry());
		if (res)
		{
		Field* ff = res->Fetch();
		uint32 cost = ff[0].GetUInt32();
		
		CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp+%u WHERE guild = %u",cost ,player->GetGuildId());
		}
		
		target->SetRespawnTime(0);
        target->Delete();
        target->DeleteFromDB();
		player->GetSession()->SendAreaTriggerMessage("Объект успешно удален!");
		CharacterDatabase.PQuery("DELETE FROM guildhouse_spawned WHERE guid = %u AND leader_guid = %u", guidLow, player->GetGUIDLow());
		}
        


		player->CLOSE_GOSSIP_MENU();
	}

	void SpawnNPC(Player* player, Creature* creature, uint32 action)
	{
		
		uint64 leader_guid = player->GetGUID();
		float x_p,y_p,z_p, radius;
		float p_x = player->GetPositionX();
		float p_y = player->GetPositionY();
		float p_z = player->GetPositionZ();
		uint32 guild = player->GetGuildId();
		QueryResult result = CharacterDatabase.PQuery("SELECT id, number, cost FROM guildhouse_npc ORDER BY cost");
		if (result)
		{
			uint32 number, id, cost, level, xp, level_table, xp_table;
			do
			{
				Field* fields = result->Fetch();
				id = fields[0].GetUInt32();
				number = fields[1].GetUInt32();
				cost = fields[2].GetUInt32();

				if (action == number)
				{
		QueryResult result10 = CharacterDatabase.PQuery("SELECT x, y, z, radius FROM guildhouses WHERE guild = %u", guild);
		if (result10)
		{
			Field* fields = result10->Fetch();
			x_p = fields[0].GetFloat();
		    y_p = fields[1].GetFloat();
			z_p = fields[2].GetFloat();
			radius = fields[3].GetFloat();
		}
		if (!result10)
		{
            player->CLOSE_GOSSIP_MENU();
			player->GetSession()->SendNotification("У вас нет гильдхауса!");
			return;
		}
		if (p_x > x_p+radius || p_x < x_p-radius || p_y > y_p+radius || p_y < y_p-radius || p_z > z_p+radius || p_z < z_p-radius)
		{
			player->CLOSE_GOSSIP_MENU();
			player->GetSession()->SendNotification("Вы не в своем ГХ!");
			return;
		}

					QueryResult result2 = CharacterDatabase.PQuery("SELECT level, xp FROM guild_level WHERE guild = %u", guild);
		            if (result2)
					{
						Field* fields = result2->Fetch();
				        level = fields[0].GetUInt32();
				        xp = fields[1].GetUInt32();
					}
					if (cost > xp)
					{
						player->GetSession()->SendNotification("Недостаточно гильдейского опыта!");
						return;
					}
		Player* chr = player;
        float x = chr->GetPositionX();
        float y = chr->GetPositionY();
        float z = chr->GetPositionZ();
        float o = chr->GetOrientation();
        Map *map = chr->GetMap();

        Creature* creature1 = new Creature;
        if(!creature1->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), map, chr->GetPhaseMaskForSpawn(), id, 0, chr->GetTeam(), x, y, z, o))
        {
            delete creature1;
            return;
        }

        creature1->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), chr->GetPhaseMaskForSpawn());

        uint32 db_guid = creature1->GetDBTableGUIDLow();

        // To call _LoadGoods(); _LoadQuests(); CreateTrainerSpells();
        creature1->LoadFromDB(db_guid, map);

		map->AddToMap(creature1);
        sObjectMgr->AddCreatureToGrid(db_guid, sObjectMgr->GetCreatureData(db_guid));

					CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp - %u WHERE guild = %u", cost, guild);

					CharacterDatabase.PQuery("REPLACE INTO guildhouse_npc_spawned (guid, leader_guid) VALUES (%u, %u)", db_guid, leader_guid);

	
				    AddNPC1(player, creature);
					break;
				}
					
			}
			while (result->NextRow());
		}
	}
	
	void Recall(Player* player)
	{
		player->TeleportTo(player->m_recallMap, player->m_recallX, player->m_recallY, player->m_recallZ, player->m_recallO);
	}

	void SpawnGobject(Player* player, Creature* creature, uint32 action)
	{
		
		uint64 leader_guid = player->GetGUID();
		float x_p,y_p,z_p, radius;
		float p_x = player->GetPositionX();
		float p_y = player->GetPositionY();
		float p_z = player->GetPositionZ();
		uint32 guild = player->GetGuildId();
		QueryResult result = CharacterDatabase.PQuery("SELECT id, number, cost FROM guildhouse_go ORDER BY cost");
		if (result)
		{
			uint32 number, id, cost, level, xp, level_table, xp_table;
			do
			{
				Field* fields = result->Fetch();
				id = fields[0].GetUInt32();
				number = fields[1].GetUInt32();
				cost = fields[2].GetUInt32();

				if (action == number)
				{
		QueryResult result10 = CharacterDatabase.PQuery("SELECT x, y, z, radius FROM guildhouses WHERE guild = %u", guild);
		if (result10)
		{
			Field* fields = result10->Fetch();
			x_p = fields[0].GetFloat();
		    y_p = fields[1].GetFloat();
			z_p = fields[2].GetFloat();
			radius = fields[3].GetFloat();
		}
		if (!result10)
		{
			player->CLOSE_GOSSIP_MENU();
			player->GetSession()->SendNotification("У вас нет гильдхауса!");
			return;
		}
		if (p_x > x_p+radius || p_x < x_p-radius || p_y > y_p+radius || p_y < y_p-radius || p_z > z_p+radius || p_z < z_p-radius)
		{
			player->CLOSE_GOSSIP_MENU();
			player->GetSession()->SendNotification("Вы не в своем ГХ!");
			return;
		}

					QueryResult result2 = CharacterDatabase.PQuery("SELECT level, xp FROM guild_level WHERE guild = %u", guild);
		            if (result2)
					{
						Field* fields = result2->Fetch();
				        level = fields[0].GetUInt32();
				        xp = fields[1].GetUInt32();
					}
					if (cost > xp)
					{
						player->GetSession()->SendNotification("Недостаточно гильдейского опыта!");
						return;
					}
					const GameObjectTemplate *gInfo = sObjectMgr->GetGameObjectTemplate(id);
					if(!gInfo)
                    {
						player->GetSession()->SendNotification("Неверный ID");
                        return;
                    }
					Player* chr = player;
					float x = float(chr->GetPositionX());
                    float y = float(chr->GetPositionY());
                    float z = float(chr->GetPositionZ());
                    float o = float(chr->GetOrientation());
                    Map *map = chr->GetMap();
					GameObject* pGameObj = new GameObject;
					uint32 db_lowGUID = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);
					if(!pGameObj->Create(db_lowGUID, gInfo->entry, map, chr->GetPhaseMaskForSpawn(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY))
                    {

                        delete pGameObj;
                        return;
                    }
					pGameObj->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), chr->GetPhaseMaskForSpawn());
					if(!pGameObj->LoadFromDB(db_lowGUID, map))
                    {
                        delete pGameObj;
                        return;
                    }
					map->AddToMap(pGameObj);
					sObjectMgr->AddGameobjectToGrid(db_lowGUID, sObjectMgr->GetGOData(db_lowGUID));

					CharacterDatabase.PQuery("UPDATE guild_level SET xp = xp - %u WHERE guild = %u", cost, guild);

					CharacterDatabase.PQuery("REPLACE INTO guildhouse_spawned (guid, leader_guid) VALUES (%u, %u)", db_lowGUID, leader_guid);


					QueryResult result3 = CharacterDatabase.PQuery("SELECT level, xp FROM guild_xp_table ORDER BY xp DESC");
		                    if (result3)
							{
								do
								{
									Field *fields = result3->Fetch();
									level_table = fields[0].GetUInt32();
									xp_table = fields[1].GetUInt32();
									if ((xp - cost) < xp_table) continue;
									else if ((xp - cost) < 100)
									{
										CharacterDatabase.PQuery("UPDATE guild_level SET level = 1 WHERE guild = %u", guild);
										break;
									}
									else
									{
										CharacterDatabase.PQuery("UPDATE guild_level SET level = %u WHERE guild = %u", level_table, guild);
										break;
									}
								}
								while (result3->NextRow());
							}
                    AddGobject1(player, creature);
					break;
				}
					
			}
			while (result->NextRow());
		}
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) 
	{
            switch (action) 
			{
			case 8:
				AddNPC1(player, creature);
				break;
			case 1:
				AddGobject1(player, creature);
				break;
			case 100:
				AddGobject1(player, creature);
				break;
			case 101:
				AddGobject2(player, creature);
				break;
				case 102:
				AddGobject3(player, creature);
				break;
				case 103:
				AddGobject4(player, creature);
				break;
				case 104:
				AddGobject5(player, creature);
				break;
				case 105:
				AddGobject6(player, creature);
				break;
				case 106:
				AddGobject7(player, creature);
				break;
				case 107:
				AddGobject8(player, creature);
				break;
				case 108:
				AddGobject9(player, creature);
				break;
				case 109:
				AddGobject10(player, creature);
				break;
				case 150:
				AddNPC1(player, creature);
				break;
			    case 151:
				AddNPC2(player, creature);
				break;
				case 152:
				AddNPC3(player, creature);
				break;
				case 153:
				AddNPC4(player, creature);
				break;
				case 154:
				AddNPC5(player, creature);
				break;
				case 155:
				AddNPC6(player, creature);
				break;
				case 156:
				AddNPC7(player, creature);
				break;
				case 157:
				AddNPC8(player, creature);
				break;
				case 158:
				AddNPC9(player, creature);
				break;
				case 159:
				AddNPC10(player, creature);
				break;
			case 2:
			ActivateGobject(player, creature);
			break;
			case 4:
				player->CLOSE_GOSSIP_MENU();
				break;
			case 5:
                PlusRadius(player);
				player->CLOSE_GOSSIP_MENU();
				break;
			case 7:
				TeleportToGH(player);
				break;
			case 9:
				DeleteNPC(player, creature);
				break;
			case 15:
				Recall(player);
				break;
            }
			if (action >= 10000) SpawnNPC(player, creature, action);
			else SpawnGobject(player, creature, action);
			
	return true;
	}

	void TeleportToGH(Player* player)
	{
		
		uint32 guild = player->GetGuildId();
		QueryResult result = CharacterDatabase.PQuery("SELECT x, y, z, map, radius FROM guildhouses WHERE guild = %u", guild);
		if (result) 
		{
			float x,y,z,radius;
			float p_x = player->GetPositionX();
			float p_y = player->GetPositionY();
			float p_z = player->GetPositionZ();
			uint32 map;
			std::string name;

				Field* fields = result->Fetch();
			    x = fields[0].GetFloat();
			    y = fields[1].GetFloat();
				z = fields[2].GetFloat();
				map = fields[3].GetUInt32();
				radius = fields[4].GetFloat();

				if (p_x > x+radius || p_x < x-radius || p_y > y+radius || p_y < y-radius || p_z > z+radius || p_z < z-radius)
				{
					player->CLOSE_GOSSIP_MENU();
					player->GetSession()->SendNotification("Вы не в своем ГХ!");
					return; 
				}
				player->TeleportTo(map,x,y,z,0.0f,0U);
		}
	}

	bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code)
    {
		switch(action)
		{
        case 3:
			DeleteAllGobjects(player, creature, code);
			break;
		case 11:
			DeleteAllNPC(player, creature, code);
			break;
		}
		return true;
	}

};

*/

class guildMasterChat : public PlayerScript
{
public:
        guildMasterChat() : PlayerScript("guildMasterChat") {}
 
        void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Guild* guild)
        {
			if (player->GetRank() == 0) msg = "|cFFFFD700" + msg + "|r";
        }		
};

/*class guildmaster : public CreatureScript
{
	public:
		guildmaster() : CreatureScript("guildmaster") { }

	bool isPlayerGuildLeader(Player *player)
	{
		return (player->GetRank() == 0) && (player->GetGuildId() != 0);
	}

	bool getGuildHouseCoords(uint32 guildId, float &x, float &y, float &z, uint32 &map)
	{
		if (guildId == 0)
		{
			//if player has no guild
			return false;
		}

		QueryResult result = CharacterDatabase.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `guildId` = %u", guildId);
		if (result)
		{
			Field *fields = result->Fetch();
			x = fields[0].GetFloat();
			y = fields[1].GetFloat();
			z = fields[2].GetFloat();
			map = fields[3].GetUInt32();
			return true;
		}
		return false;
	}

	void teleportPlayerToGuildHouse(Player *player, Creature *_creature)
	{
		if (player->GetGuildId() == 0)
		{
			//if player has no guild
			_creature->Whisper(MSG_NOTINGUILD, LANG_UNIVERSAL, player);
			return;
		}

		float x, y, z;
		uint32 map;

		if (getGuildHouseCoords(player->GetGuildId(), x, y, z, map))
		{
			//teleport player to the specified location
			player->TeleportTo(map, x, y, z, 0.0f);
		}
		else
			_creature->Whisper(MSG_NOGUILDHOUSE, LANG_UNIVERSAL, player);
	}

	bool showBuyList(Player *player, Creature *_creature, uint32 showFromId = 0)
	{
		//show not occupied guildhouses

		QueryResult result;
		result = WorldDatabase.PQuery("SELECT `id`, `comment` FROM `guildhouses` WHERE `guildId` = 0 AND `id` > 0 ORDER BY `id` LIMIT 15");

		if (result)
		{
			do
			{
				Field *fields = result->Fetch();
				uint32 guildhouseId = fields[0].GetInt32();
				std::string comment = fields[1].GetString();

				//send comment as a gossip item
				//transmit guildhouseId in Action variable
				player->ADD_GOSSIP_ITEM(ICON_GOSSIP_TABARD, comment, GOSSIP_SENDER_MAIN, guildhouseId + OFFSET_GH_ID_TO_ACTION);
			}
			while (result->NextRow());

			if (result->GetRowCount() == GOSSIP_COUNT_MAX)
			{
				//assume that we have additional page
				//add link to next GOSSIP_COUNT_MAX items
				player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOONDOTS, MSG_GOSSIP_NEXTPAGE, GOSSIP_SENDER_MAIN,
				guildhouseId + OFFSET_SHOWBUY_FROM);
			}

			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());

			return true;
		}
		else if (!result)
		{
			//all guildhouses are occupied
			_creature->Whisper(MSG_NOFREEGH, LANG_UNIVERSAL, player);
			player->CLOSE_GOSSIP_MENU();
		}
		else
		{
			//this condition occurs when COUNT(guildhouses) % GOSSIP_COUNT_MAX == 0
			//just show GHs from beginning
			showBuyList(player, _creature, 0);
		}

		return false;
	}

	bool isPlayerHasGuildhouse(Player *player, Creature *_creature, bool whisper = false)
	{
		QueryResult result;
		result = WorldDatabase.PQuery("SELECT `comment` FROM `guildhouses` WHERE `guildId` = %u",
		player->GetGuildId());

		if (result)
		{
			if (whisper)
			{
				//whisper to player "already have etc..."
				Field *fields = result->Fetch();
				char msg[100];
				sprintf(msg, MSG_ALREADYHAVEGH, fields[0].GetCString());
				_creature->Whisper(msg, LANG_UNIVERSAL, player);
			}

			return true;
		}

		return false;
	}

	void buyGuildhouse(Player *player, Creature *_creature, uint32 guildhouseId)
	{

		bool token = sConfigMgr->GetBoolDefault("GuildHouse.TokenOrGold", false);
		int cost = sConfigMgr->GetBoolDefault("GuildHouse.Cost", false);

		if (player->GetMoney() < COST_GH_BUY)
		{
			//show how much money player need to buy GH (in gold)
			char msg[100];
			sprintf(msg, MSG_NOTENOUGHMONEY, COST_GH_BUY - sConfigMgr->GetBoolDefault("GuildHouse.Cost", false));
			_creature->Whisper(msg, LANG_UNIVERSAL, player);
			return;
		}

		if (isPlayerHasGuildhouse(player, _creature, true))
		{
			//player already have GH
			return;
		}

		QueryResult result;

		//check if somebody already occupied this GH
		result = WorldDatabase.PQuery("SELECT `id` FROM `guildhouses` WHERE `id` = %u AND `guildId` <> 0", guildhouseId);

		if (result)
		{
			_creature->Whisper(MSG_GHOCCUPIED, LANG_UNIVERSAL, player);
			return;
		}

		//update DB
		result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = %u WHERE `id` = %u",
		player->GetGuildId(), guildhouseId);
	}

	void sellGuildhouse(Player *player, Creature *_creature)
	{
		if (isPlayerHasGuildhouse(player, _creature))
		{
			QueryResult result;
			result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = 0 WHERE `guildId` = %u",
			player->GetGuildId());

			player->ModifyMoney(sConfigMgr->GetIntDefault("GuildHouse.SellCost", false) * 10000);

			//display message e.g. "here your money etc."
			char msg[100];
			sprintf(msg, MSG_SOLD, sConfigMgr->GetIntDefault("GuildHouse.SellCost", false));
			_creature->Whisper(msg, LANG_UNIVERSAL, player);
		}
	}

	bool OnGossipSelect(Player *player, Creature *_creature, uint32 sender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		if (sender != GOSSIP_SENDER_MAIN)
		return false;

		switch (action)
		{
			case ACTION_TELE:
				//teleport player to GH
				player->CLOSE_GOSSIP_MENU();
				teleportPlayerToGuildHouse(player, _creature);
				break;
			case ACTION_SHOW_BUYLIST:
				//show list of GHs which currently not occupied
				showBuyList(player, _creature);
				break;
			case ACTION_SELL_GUILDHOUSE:
				sellGuildhouse(player, _creature);
				player->CLOSE_GOSSIP_MENU();
				break;
			default:
				if (action > OFFSET_SHOWBUY_FROM)
				{
					showBuyList(player, _creature, action - OFFSET_SHOWBUY_FROM);
				}
				else if (action > OFFSET_GH_ID_TO_ACTION)
				{
					//player clicked on buy list
					player->CLOSE_GOSSIP_MENU();

					//get guildhouseId from action
					//guildhouseId = action - OFFSET_GH_ID_TO_ACTION
					buyGuildhouse(player, _creature, action - OFFSET_GH_ID_TO_ACTION);
				}
				break;
		}

		return true;
	}


	bool OnGossipHello(Player *player, Creature *_creature)
	{
		player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOON, MSG_GOSSIP_TELE, GOSSIP_SENDER_MAIN, ACTION_TELE);

		if (player->GetRank() == 0)
		{
			
			if (isPlayerHasGuildhouse(player, _creature))
			{
				//and additional for guildhouse owner (Removed :
				player->ADD_GOSSIP_ITEM_EXTENDED(ICON_GOSSIP_GOLD, MSG_GOSSIP_SELL, GOSSIP_SENDER_MAIN, ACTION_SELL_GUILDHOUSE, MSG_SELL_CONFIRM, 0, false);
			}
			else
			{
				//show additional menu for guild leader
				player->ADD_GOSSIP_ITEM(ICON_GOSSIP_GOLD, MSG_GOSSIP_BUY, GOSSIP_SENDER_MAIN, ACTION_SHOW_BUYLIST);
			}
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
		return true;
	}
};*/

void AddSC_Unell_Guild_System()
{
	new item_tpguild();
	new npc_guild_level();
	// new npc_guildhouse();
	// new npc_guildhouse_builder();
	new guildMasterChat();
	//new guildmaster();
}