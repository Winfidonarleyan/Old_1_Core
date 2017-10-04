#include "ScriptPCH.h"
#include "Chat.h"
#include "Unell_Script.h"
#include "Config.h"

uint32 TimeChecker = 0;

void InitTopTotalTime()
{
    time_t curtime = time(NULL);
	sLog->outDetail("Инициализация топа: Игровое время"); 
	uint32 time32 = curtime;
	uint32 TimeTotalTime = ConfigMgr::GetIntDefault("Unell.Top.TotalTime.Time.Reset", 3600);
	
	QueryResult timetop = CharacterDatabase.PQuery("SELECT value FROM `worldstates` WHERE `entry` = '40000'");
	if (timetop)
	{
		uint32 toptime = timetop->Fetch()->GetUInt32();
		if (time32 > toptime)
			CharacterDatabase.PExecute("UPDATE `worldstates` SET `value`=%u+%u WHERE (`entry`='40000')", time32, TimeTotalTime);
	}
	if (!timetop)
		CharacterDatabase.PExecute("INSERT INTO `worldstates` (`entry`, `value`) VALUES ('40000', %u+%u)", time32, TimeTotalTime);
}

void InitTopKill()
{
    time_t curtime = time(NULL);
	sLog->outDetail("Инициализация топа: PvP Убийства"); 
	uint32 time32 = curtime;
	uint32 TimeTotalKill = ConfigMgr::GetIntDefault("Unell.Top.TotalKill.Time.Reset", 3600);
	
	QueryResult timetop = CharacterDatabase.PQuery("SELECT value FROM `worldstates` WHERE `entry` = '40001'");
	if (timetop)
	{
		uint32 toptime = timetop->Fetch()->GetUInt32();
		if (time32 > toptime)
			CharacterDatabase.PExecute("UPDATE `worldstates` SET `value`=%u+%u WHERE (`entry`='40001')", time32, TimeTotalKill);
	}
	if (!timetop)
		CharacterDatabase.PExecute("INSERT INTO `worldstates` (`entry`, `value`) VALUES ('40001', %u+%u)", time32, TimeTotalKill);
}

void InitTotalTimeTop()
{
	char msg[2048];
	uint32 step = 1;
	
	QueryResult totaltimeres = CharacterDatabase.PQuery("SELECT account, name, totaltime FROM `characters` ORDER BY `totaltime` DESC LIMIT 3");
	if (totaltimeres)
	{
		snprintf(msg, 2048, "|cFFFF0000[Топы сервера]: |cff6C8CD5Игровое время. Выдача наград за игровое время.");
		sWorld->SendServerMessage(SERVER_MSG_STRING, msg);		
		do
		{
			Field * fields = totaltimeres->Fetch();
			uint32 account = fields[0].GetUInt32();
			std::string name = fields[1].GetString();
			uint32 totaltime = fields[2].GetUInt32();		
			std::string timeStr   = secsToTimeString(totaltime, true, false);
			
			
			if (step == 1)
			{
				if (Unell_Script::Unell_acctoken(account))
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+200 WHERE (`id`='%u');", account);
				else
					LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`) VALUES ('%u', '200');", account);
			
				snprintf(msg, 2048, "|cFFFF0000%u. |cff6C8CD5Игрок |cff14ECCF%s|cff6C8CD5. Игровое время: |cff14ECCF%s|cff6C8CD5. Получает |cff14ECCF200 |cff6C8CD5токенов.|r", step, name.c_str(), timeStr.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);				
			}
			
			if (step == 2)
			{
				if (Unell_Script::Unell_acctoken(account))
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+100 WHERE (`id`='%u');", account);
				else
					LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`) VALUES ('%u', '100');", account);
			
				snprintf(msg, 2048, "|cFFFF0000%u. |cff6C8CD5Игрок |cff14ECCF%s|cff6C8CD5. Игровое время: |cff14ECCF%s|cff6C8CD5. Получает |cff14ECCF100 |cff6C8CD5токенов.|r", step, name.c_str(), timeStr.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
			}
			
			if (step == 3)
			{
				if (Unell_Script::Unell_acctoken(account))
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+50 WHERE (`id`='%u');", account);
				else
					LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`) VALUES ('%u', '50');", account);
				
				snprintf(msg, 2048, "|cFFFF0000%u. |cff6C8CD5Игрок |cff14ECCF%s|cff6C8CD5. Игровое время: |cff14ECCF%s|cff6C8CD5. Получает |cff14ECCF50 |cff6C8CD5токенов.|r", step, name.c_str(), timeStr.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);			
			}
			
			++step;
		
		} while (totaltimeres->NextRow());
	}
}

void InitPvPKillTop()
{
	char msg[2048];
	uint32 step = 1;
	
	QueryResult totaltimeres = CharacterDatabase.PQuery("SELECT account, name, totalKills FROM `characters` ORDER BY `totalKills` DESC LIMIT 3");
	if (totaltimeres)
	{
		snprintf(msg, 2048, "|cFFFF0000[Топы сервера]: |cff6C8CD5Количество убийств. Выдача наград за PvP килы.");
		sWorld->SendServerMessage(SERVER_MSG_STRING, msg);		
		do
		{
			Field * fields = totaltimeres->Fetch();
			uint32 account = fields[0].GetUInt32();
			std::string name = fields[1].GetString();
			uint32 totalkill = fields[2].GetUInt32();	
			
			
			if (step == 1)
			{
				if (Unell_Script::Unell_acctoken(account))
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+200 WHERE (`id`='%u');", account);
				else
					LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`) VALUES ('%u', '200');", account);
			
				snprintf(msg, 2048, "|cFFFF0000%u. |cff6C8CD5Игрок |cff14ECCF%s|cff6C8CD5. Количество убийств: |cff14ECCF%u|cff6C8CD5. Получает |cff14ECCF200 |cff6C8CD5токенов.|r", step, name.c_str(), totalkill);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);				
			}
			
			if (step == 2)
			{
				if (Unell_Script::Unell_acctoken(account))
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+100 WHERE (`id`='%u');", account);
				else
					LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`) VALUES ('%u', '100');", account);
			
				snprintf(msg, 2048, "|cFFFF0000%u. |cff6C8CD5Игрок |cff14ECCF%s|cff6C8CD5. Количество убийств: |cff14ECCF%u|cff6C8CD5. Получает |cff14ECCF100 |cff6C8CD5токенов.|r", step, name.c_str(), totalkill);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
			}
			
			if (step == 3)
			{
				if (Unell_Script::Unell_acctoken(account))
					LoginDatabase.PExecute("UPDATE `account_token` SET `token`=`token`+50 WHERE (`id`='%u');", account);
				else
					LoginDatabase.PExecute("INSERT INTO `account_token` (`id`, `token`) VALUES ('%u', '50');", account);
				
				snprintf(msg, 2048, "|cFFFF0000%u. |cff6C8CD5Игрок |cff14ECCF%s|cff6C8CD5. Количество убийств: |cff14ECCF%u|cff6C8CD5. Получает |cff14ECCF50 |cff6C8CD5токенов.|r", step, name.c_str(), totalkill);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);			
			}
			
			++step;
		
		} while (totaltimeres->NextRow());
	}
}

class Unell_World : public WorldScript
{	
public:
    Unell_World() : WorldScript("Unell_World") {}		
	
	void OnStartup()
	{	
		if (ConfigMgr::GetBoolDefault("Unell.Top.Enable", true))
		{
			if (ConfigMgr::GetBoolDefault("Unell.Top.TotalTime.Enable", true))
			{
				InitTopTotalTime();
			}
			
			if (ConfigMgr::GetBoolDefault("Unell.Top.TotalKill.Enable", true))
			{
				InitTopKill();
			}
		}			
	}
	
	void OnUpdate(uint32 diff)
	{		
		if (ConfigMgr::GetBoolDefault("Unell.Top.Enable", true))
		{
			if (TimeChecker < diff)
			{
				uint32 CheckTime = ConfigMgr::GetIntDefault("Unell.Top.CheckTime", 60000);
				time_t timenow = time(NULL);
				uint32 time32 = timenow;
				
				if (ConfigMgr::GetBoolDefault("Unell.Top.TotalTime.Enable", true))
				{
					QueryResult totaltimeres = CharacterDatabase.PQuery("SELECT value FROM `worldstates` WHERE `entry` = '40000'");
					if (totaltimeres)
					{
						uint32 totaltime = totaltimeres->Fetch()->GetUInt32();			
						if (time32 > totaltime)
						{
							InitTotalTimeTop();
							uint32 TimeTotalTime = ConfigMgr::GetIntDefault("Unell.Top.TotalTime.Time.Reset", 3600);
							time_t NextTime = time(NULL) + TimeTotalTime;
							sWorld->setWorldState(40000, uint32(NextTime));					
						}
					}
				}					

				if (ConfigMgr::GetBoolDefault("Unell.Top.TotalKill.Enable", true))
				{
					QueryResult totaltkillres = CharacterDatabase.PQuery("SELECT value FROM `worldstates` WHERE `entry` = '40001'");
					if (totaltkillres)
					{
						uint32 totaltkill = totaltkillres->Fetch()->GetUInt32();			
						if (time32 > totaltkill)
						{
							InitPvPKillTop();
							uint32 TimeTotalKill = ConfigMgr::GetIntDefault("Unell.Top.TotalKill.Time.Reset", 3600);
							time_t NextTime = time(NULL) + TimeTotalKill;
							sWorld->setWorldState(40001, uint32(NextTime));
						}
					}
				}				
				TimeChecker = CheckTime; // check 1 min
			}
			else
				TimeChecker -= diff;
		}
	}
};

void AddSC_Unell_Top_System()
{
	new Unell_World();
}