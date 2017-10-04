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


class Unell_Area : public PlayerScript
{
public:
    Unell_Area() : PlayerScript("Unell_Area") {}

    void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea)
    {
		Battleground* bg = player->GetBattleground();

		if (bg && player->GetMap()->IsBattleground() && bg->GetStatus() == STATUS_WAIT_JOIN)		
		{
			ChatHandler handler(player->GetSession());
			if (ConfigMgr::GetBoolDefault("Unell.BattleGround.Buffs.Enable", true))
			{
			handler.PSendSysMessage("|cffff0000# |cff14ECCFВы перешли на Поле Боя. Вам были выданы классовые баффы");
			player->CastSpell(player, 25898, true);
			player->CastSpell(player, 48470, true);
			player->CastSpell(player, 48162, true);
			player->CastSpell(player, 48074, true);
			player->CastSpell(player, 48170, true);
			}
		}

		if (bg && player->GetMap()->IsBattleArena() && bg->GetStatus() == STATUS_WAIT_JOIN)		
		{
			ChatHandler handler(player->GetSession());
			// handler.PSendSysMessage("Вы в зоне Арены, которая вот вот начнётся!");
		}		
	}	
};

void AddSC_Unell_Area_System()
{
	new Unell_Area();
}