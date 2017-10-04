#include "ScriptPCH.h"
#include "Config.h"
#include "Unell_Script.h"

class Unell_Referer_NPC : public CreatureScript
{
public:
    Unell_Referer_NPC() : CreatureScript("Unell_Referer_NPC") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		if (!ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
		{
			player->CLOSE_GOSSIP_MENU();
			ChatHandler handler(player);			
			handler.PSendSysMessage("|cFFFF0000#|r Реферальная система отключена");
			return true;
		}
		
		uint32 accid = player->GetSession()->GetAccountId();		
		/*QueryResult itemid1 = WorldDatabase.PQuery("SELECT itemid FROM `have_token` WHERE `menu_id` = 2");
		uint32 itemid2 = itemid1->Fetch()->GetUInt32();*/
		
		player->ADD_GOSSIP_ITEM(10, "Информация о реферальной системе", GOSSIP_SENDER_MAIN, 4);
		
		if (!Unell_Script::Unell_accref(accid))
		{
			player->ADD_GOSSIP_ITEM_EXTENDED(10, "Я хочу ввести айди пригласившего меня", GOSSIP_SENDER_MAIN, 1, "", 0, true);
			player->ADD_GOSSIP_ITEM(10, "Меня никто не приглашал", GOSSIP_SENDER_MAIN, 5);
		}
		
		if (Unell_Script::Unell_accref(accid))
		{
			QueryResult resrefid = LoginDatabase.PQuery("SELECT refid FROM `account_ref` WHERE `id` = %u", accid);
			uint32 refid = resrefid->Fetch()->GetUInt32();
			if (refid != 1)
				player->ADD_GOSSIP_ITEM(10, "Узнать кто меня прагласил", GOSSIP_SENDER_MAIN, 2);
		}
		
		player->ADD_GOSSIP_ITEM(10, "Узнать кого я пригласил", GOSSIP_SENDER_MAIN, 3);		
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        ChatHandler handler(player);
		uint32 accid = player->GetSession()->GetAccountId();
		uint32 refid = Unell_Script::Unell_refid(accid);
		std::string username = Unell_Script::Unell_accuser(refid);
		std::string username1 = Unell_Script::Unell_accuser(accid);

		player->PlayerTalkClass->ClearMenus();
		
        if (sender != GOSSIP_SENDER_MAIN)
            return true;

        switch(action)
        {
			case 2:
					player->CLOSE_GOSSIP_MENU(); 
					handler.PSendSysMessage("|cFFFF0000#|r Вас пригласил |cFFFF0000%s|r. Его айди |cFFFF0000%u|r.", username.c_str(), refid);
					break;
			case 3:
			{
				player->CLOSE_GOSSIP_MENU();
					handler.PSendSysMessage("|cFFFF0000#|r Список ваших приглашённых игроков");
					QueryResult result = LoginDatabase.PQuery("SELECT id, totaltime, totalkill FROM `account_ref` WHERE `refid` = '%u'", accid);
					if(result)
					{
						do
						{
							Field * fields = result->Fetch();
							uint32 id = fields[0].GetUInt32();
							uint32 totaltime = fields[1].GetUInt32();
							uint32 totalkill = fields[2].GetUInt32();
							std::string usernameacc = Unell_Script::Unell_accuser(id);
							std::string timeStr   = secsToTimeString(totaltime, true, false);
							
							handler.PSendSysMessage("|cffff0000# |cff14ECCFАккаунт: |cffff0000%s|cff14ECCF, Убийства: |cffff0000%u|cff14ECCF, Игровое время: |cffff0000%s", usernameacc.c_str(), totalkill, timeStr.c_str());
						} 
						while(result->NextRow());
					}
					if(!result)
					{
						handler.PSendSysMessage("|cFFFF0000#|r Вас ещё никто не указал пригласившим");
					}
			}
				break;
					
			case 4:
				player->CLOSE_GOSSIP_MENU(); 
				handler.PSendSysMessage("|cFFFF0000#|r -- Информация о реферальной системе.");
				handler.PSendSysMessage("|cFFFF0000#|r Приглашайте игроков на сервер, скажите им свой айди аккаунта.");
				handler.PSendSysMessage("|cFFFF0000#|r Приглашённому игроку нужно ввести айди аккаунта у данного нпц.");
				handler.PSendSysMessage("|cFFFF0000#|r Играя, ваш реферал приносит вам бонусы. За PvP убийства он и вы получаете бонусы (токены).");
				handler.PSendSysMessage("|cFFFF0000#|r Токены можно обменять у нпц |cFFFF0000Дэнси|r, которая находится неподалёку.");
				handler.PSendSysMessage("|cFFFF0000#|r Так же, если ваш реферер пожертвует на сервер, 10 процентов от его суммы получите и вы.");
				handler.PSendSysMessage("|cFFFF0000#|r Ваш аккаунт |cFFFF0000%s|r. Ваш айди аккаунта |cFFFF0000%u|r.", username1.c_str(), accid);
				break;
					
			case 5:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(10, "Я подтверждаю, что меня никто не приглашал!", GOSSIP_SENDER_MAIN, 6);
				player->ADD_GOSSIP_ITEM(10, "Оу, нет, я не подтверждаю!", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;
					
			case 6:
				player->CLOSE_GOSSIP_MENU();
				handler.PSendSysMessage("|cFFFF0000#|r Готово. Вы указали, что вас никто не пригласил.");
				{
					if (Unell_Script::Unell_accref(accid))
					handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы уже указали, кто вас пригласил.");
					else
					LoginDatabase.PExecute("INSERT INTO `account_ref` (`id`, `refid`) VALUES ('%u', '1')", accid);
					
					if (!Unell_Script::Unell_acctoken(accid))
						LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`, `refereraccid`) VALUES ('%u', '0', '1')", accid);
					else
						LoginDatabase.PExecute("UPDATE `account_token` SET `refereraccid`='1' WHERE (`id`='%u')", accid);
				}
				break;
					
			case 100:
				OnGossipHello(player, creature);
				break;
        }
       
        return true;
    }		
    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code)
	{
        if (!*code)
            return false;
		ChatHandler handler(player);
		uint32 ucode = uint32 (atoi(code));
		uint32 accid = player->GetSession()->GetAccountId();
		
		player->PlayerTalkClass->ClearMenus();
        if (sender != GOSSIP_SENDER_MAIN)
            return true; 

        switch(action)
        {
		case 1:
                player->CLOSE_GOSSIP_MENU();
				if (accid == ucode)
				{
					handler.PSendSysMessage("|cFFFF0000#|r Нет нет нет! Себя указывать нельзя...");
				}
				else
				{
					if (Unell_Script::Unell_accref(accid))
						handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы уже указали, кто вас пригласил.");
					else
					LoginDatabase.PExecute("INSERT INTO `account_ref` (`id`, `refid`) VALUES ('%u', '%u')", accid, ucode);
					
					if (!Unell_Script::Unell_acctoken(accid))
						LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`, `refereraccid`) VALUES ('%u', '0', '%u')", accid, ucode);
					else
						LoginDatabase.PExecute("UPDATE `account_token` SET `refereraccid`='%u' WHERE (`id`='%u')", ucode, accid);
				}
				break;
				
		case 2:
                player->CLOSE_GOSSIP_MENU();			
                break;
		
		
		break;
		
        return true; 
        }
       
        return true;
    }
};

class Unell_Login_Ref : public PlayerScript
{
public:
	Unell_Login_Ref() : PlayerScript("Unell_Login_Ref") { }	

	void OnLogin(Player* player)
	{		
		uint32 accId = player->GetSession()->GetAccountId();
		uint32 refid = Unell_Script::Unell_refid(accId);	

		uint32 Killreward1 = ConfigMgr::GetIntDefault("Unell.Referer.System.Killreward1", 50);
		uint32 Killreward2 = ConfigMgr::GetIntDefault("Unell.Referer.System.Killreward2", 100);
		uint32 Killreward3 = ConfigMgr::GetIntDefault("Unell.Referer.System.Killreward3", 500);
		uint32 Timereward1 = ConfigMgr::GetIntDefault("Unell.Referer.System.Timereward1", 86400);
		uint32 Timereward2 = ConfigMgr::GetIntDefault("Unell.Referer.System.Timereward2", 259200);
		uint32 Timereward3 = ConfigMgr::GetIntDefault("Unell.Referer.System.Timereward3", 604800);
		
		uint32 Prizetonenkill1ref = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Kill1.Referer", 50);
		uint32 Prizetonenkill2ref = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Kill2.Referer", 100);
		uint32 Prizetonenkill3ref = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Kill3.Referer", 200);
		uint32 Prizetonentime1ref = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Time1.Referer", 50);
		uint32 Prizetonentime2ref = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Time2.Referer", 100);
		uint32 Prizetonentime3ref = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Time3.Referer", 200);
		
		uint32 Prizetonenkill1 = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Kill1", 100);
		uint32 Prizetonenkill2 = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Kill2", 200);
		uint32 Prizetonenkill3 = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Kill3", 400);
		uint32 Prizetonentime1 = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Time1", 100);
		uint32 Prizetonentime2 = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Time2", 200);
		uint32 Prizetonentime3 = ConfigMgr::GetIntDefault("Unell.Referer.System.Prizetonen.Time3", 400);
		
		if (ConfigMgr::GetBoolDefault("Unell.Referer.System.Enable", true))
		{			
		
			if (Unell_Script::Unell_accref(accId))
			{
				QueryResult reskill = CharacterDatabase.PQuery("SELECT totalKills FROM `characters` WHERE `account` = '%u' ORDER BY `totalKills` DESC LIMIT 1", accId);
				QueryResult resprize = LoginDatabase.PQuery("SELECT prizekill FROM `account_ref` WHERE `id` = '%u'", accId);
				QueryResult restime = CharacterDatabase.PQuery("SELECT totaltime FROM `characters` WHERE `account` = '%u' ORDER BY `totaltime` DESC LIMIT 1", accId);
				QueryResult resprizetime = LoginDatabase.PQuery("SELECT prizetime FROM `account_ref` WHERE `id` = '%u'", accId);
				
				uint32 totalkill = reskill->Fetch()->GetUInt32();
				uint32 prizekill = resprize->Fetch()->GetUInt32();	
				uint32 totaltime = restime->Fetch()->GetUInt32();
				uint32 prizetime = resprizetime->Fetch()->GetUInt32();			
				LoginDatabase.PExecute("UPDATE `account_ref` SET `totalkill`='%u' WHERE (`id`='%u')", totalkill, accId);
				LoginDatabase.PExecute("UPDATE `account_ref` SET `totaltime`='%u' WHERE (`id`='%u')", totaltime, accId);
				
				if (totalkill >= Killreward1 && prizekill == 0)
				{
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonenkill1ref, refid);
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonenkill1, accId);
					LoginDatabase.PExecute("UPDATE `account_ref` SET `prizekill`='1' WHERE (`id`='%u')", accId);
				}

				if (totalkill >= Killreward2 && prizekill == 1)
				{
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonenkill2ref, refid);
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonenkill2, accId);
					LoginDatabase.PExecute("UPDATE `account_ref` SET `prizekill`='2' WHERE (`id`='%u')", accId);
				}

				if (totalkill >= Killreward3 && prizekill == 2)
				{
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonenkill3ref, refid);
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonenkill3, accId);
					LoginDatabase.PExecute("UPDATE `account_ref` SET `prizekill`='3' WHERE (`id`='%u')", accId);
				}
				
				if (totaltime >= Timereward1 && prizetime == 0)
				{
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonentime1ref, refid);
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonentime1, accId);
					LoginDatabase.PExecute("UPDATE `account_ref` SET `prizetime`='1' WHERE (`id`='%u')", accId);
				}
				
				if (totaltime >= Timereward2 && prizetime == 1)
				{
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonentime2ref, refid);
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonentime2, accId);
					LoginDatabase.PExecute("UPDATE `account_ref` SET `prizetime`='2' WHERE (`id`='%u')", accId);
				}
				
				if (totaltime >= Timereward3 && prizetime == 2)
				{
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonentime3ref, refid);
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+%u WHERE (`id`='%u');", Prizetonentime3, accId);
					LoginDatabase.PExecute("UPDATE `account_ref` SET `prizetime`='3' WHERE (`id`='%u')", accId);
				}			
			}
		}
	}
};

void AddSC_Unell_Referer_System()
{
	new Unell_Referer_NPC();
	new Unell_Login_Ref();
}