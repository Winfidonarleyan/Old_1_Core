#include "ScriptPCH.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Common.h"
#include "Player.h"
#include "WorldSession.h"
#include "SpellAuras.h"
#include "World.h"
#include "Config.h"
#include "SystemConfig.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"

class Unell_Event : public PlayerScript
{
public:
	Unell_Event() : PlayerScript ("Unell_Event") {}

	void OnCreatureKill(Player* player, Creature* boss)
	{
			char msg[250];
			uint32 bossid = boss->GetEntry();
			std::string NameLink = ChatHandler(player->GetSession()).playerLink(player->GetName());
			if (bossid == 92000)
			{
				snprintf(msg, 250, "|cffff0000[Убийство босса Альянса]:|r |cff6C8CD5Игрок |cff14ECCF%s |cff6C8CD5 и его группа уже убили босса Альянса и получили флаг", NameLink.c_str());
			    sWorld->SendZoneText(45, msg);
				player->AddAura(23335, player);
			}

			if (bossid == 92001)
			{
				snprintf(msg, 250, "|cffff0000[Убийство босса Орды]:|r |cff6C8CD5Игрок |cff14ECCF%s |cff6C8CD5 и его группа уже убили босса Орды и получили флаг", NameLink.c_str());
			    sWorld->SendZoneText(45, msg);
				player->AddAura(23333, player);
			}
			
	}
};

class Unell_Event_FlagA : public GameObjectScript
{
    public:

        Unell_Event_FlagA() : GameObjectScript("Unell_Event_FlagA") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            player->CastSpell(player, 14268, true);
            return true;
        }
};

class Unell_Event_FlagO : public GameObjectScript
{
    public:

        Unell_Event_FlagO() : GameObjectScript("Unell_Event_FlagO") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            player->CastSpell(player, 14267, true);
            return true;
        }
};

void AddSC_Unell_Custom_Event()
{
   	new Unell_Event();
	new Unell_Event_FlagA();
	new Unell_Event_FlagO();
	
}