#include "ScriptMgr.h"
#include "Chat.h"
#include "SocialMgr.h"
#include "BattlegroundMgr.h"

class spectator_commandscript : public CommandScript
{
public:
    spectator_commandscript() : CommandScript("spectator_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand commandTable[] =
        {
            { "spectate",       SEC_PLAYER,         false, &HandleSpectateCommand,  "", NULL },
            { NULL,             0,                  false, NULL,                    "", NULL }
        };
        return commandTable;
    }

    static bool HandleSpectateCommand(ChatHandler* handler, const char* args)
    {
        if (!sWorld->getBoolConfig(CONFIG_ARENA_SPECTATORS_ENABLE))
        {
            handler->SendSysMessage(LANG_SPECTATOR_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* target;
        uint64 target_gu;
        std::string target_na;

        if (!handler->extractPlayerTarget((char*)args, &target, &target_gu, &target_na))
            return false;

        Player* _player = handler->GetSession()->GetPlayer();
        if (target == _player || target_gu == _player->GetGUID())
        {
            handler->SendSysMessage(LANG_CANT_TELEPORT_SELF);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->IsArenaSpectator())
        {
            handler->SendSysMessage(LANG_SPECTATOR_ALREADY_SPECTATOR);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (_player->GetMap()->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_SPECTATOR_ON_ARENA_OR_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target)
        {
            std::string chrNameLink = handler->playerLink(target_na);

            if (!target->InArena())
            {
                handler->PSendSysMessage(LANG_SPECTATOR_NOT_ON_ARENA, chrNameLink.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (sWorld->getBoolConfig(CONFIG_ARENA_SPECTATORS_ONLY_FRIEND))
            {
                if (!target->GetSocial()->HasFriend(_player->GetGUIDLow()))
                {
                    handler->PSendSysMessage(LANG_SPECTATOR_FORBID, chrNameLink.c_str());
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }

            if (_player->isInFlight())
            {
                handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            else
                _player->SaveRecallPosition();

            _player->RemovePet(NULL, PET_SAVE_AS_CURRENT, true);
            _player->SetBattlegroundId(target->GetBattlegroundId(), target->GetBattlegroundTypeId());
            _player->SetBattlegroundEntryPoint();

            handler->PSendSysMessage(LANG_APPEARING_AT, chrNameLink.c_str());

            float x, y, z;
            target->GetContactPoint(_player, x, y, z);

            _player->TeleportTo(target->GetMapId(), x, y, z, _player->GetAngle(target));
            _player->SetArenaSpectator(true);
            _player->SetPhaseMask(target->GetPhaseMask(), true);
        }
        return true;
    }
};

enum NpcSpectatorAtions
{
    // will be used for scrolling
    NPC_SPECTATOR_ACTION_LIST_GAMES = 1000,
    NPC_SPECTATOR_ACTION_LIST_TOP_GAMES = 2000,
    NPC_SPECTATOR_ACTION_SELECTED_PLAYER = 3000
};

const uint16 TopGamesRating = 1800;
const uint8 GamesOnPage = 20;

class npc_arena_spectator : public CreatureScript
{
    public:
        npc_arena_spectator() : CreatureScript("npc_arena_spectator") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Смотреть игры с высоким рейтингом...", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_TOP_GAMES);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Смотреть игры с низким рейтингом", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_TOP_GAMES);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action >= NPC_SPECTATOR_ACTION_LIST_GAMES && action < NPC_SPECTATOR_ACTION_LIST_TOP_GAMES)
            {
                ShowPage(player, action - NPC_SPECTATOR_ACTION_LIST_GAMES, false);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else if (action >= NPC_SPECTATOR_ACTION_LIST_TOP_GAMES && action < NPC_SPECTATOR_ACTION_LIST_TOP_GAMES)
            {
                ShowPage(player, action - NPC_SPECTATOR_ACTION_LIST_TOP_GAMES, true);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else
            {
                uint64 guid = action - NPC_SPECTATOR_ACTION_SELECTED_PLAYER;
                if (Player* target = ObjectAccessor::FindPlayer(guid))
                {
                    ChatHandler handler(player->GetSession());
                    char const* pTarget = target->GetName();
                    spectator_commandscript::HandleSpectateCommand(&handler, pTarget);
                }
            }
            return true;
        }

        std::string GetClassNameById(uint8 id)
        {
            std::string sClass = "";
            switch (id)
            {
                case CLASS_WARRIOR: sClass = "Воин "; break;
                case CLASS_PALADIN: sClass = "Паладин "; break;
                case CLASS_HUNTER: sClass = "Охотник "; break;
                case CLASS_ROGUE: sClass = "Разбойник "; break;
                case CLASS_PRIEST: sClass = "Жрец "; break;
                case CLASS_DEATH_KNIGHT: sClass = "Рыцарь Смерти "; break;
                case CLASS_SHAMAN: sClass = "Шаман "; break;
                case CLASS_MAGE: sClass = "Маг "; break;
                case CLASS_WARLOCK: sClass = "Варлок "; break;
                case CLASS_DRUID: sClass = "Друид "; break;
            }
            return sClass;
        }

        std::string GetGamesStringData(Battleground* team, uint16 mmr)
        {
            std::string teamsMember[BG_TEAMS_COUNT];
            uint32 firstTeamId = 0;
            for (Battleground::BattlegroundPlayerMap::const_iterator itr = team->GetPlayers().begin(); itr != team->GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    if (player->IsArenaSpectator())
                        continue;

                    uint32 team = itr->second.Team;
                    if (!firstTeamId)
                        firstTeamId = team;

                    teamsMember[firstTeamId == team] += GetClassNameById(player->getClass());
                }

            std::string data = teamsMember[0] + "(";
            std::stringstream ss;
            std::stringstream sstwo;
            ss << mmr;
            data += ss.str();
            data += ") - " + teamsMember[1];
            return data;
        }

        uint64 GetFirstPlayerGuid(Battleground* team)
        {
            for (Battleground::BattlegroundPlayerMap::const_iterator itr = team->GetPlayers().begin(); itr != team->GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                    return itr->first;
            return 0;
        }

        void ShowPage(Player *player, uint16 page, bool isTop)
        {
            uint16 highGames = 0;
            uint16 lowGames = 0;
            bool haveNextPage = false;
            for (uint8 i = BATTLEGROUND_NA; i <= BATTLEGROUND_RV; ++i)
            {
                if (!sBattlegroundMgr->IsArenaType((BattlegroundTypeId)i))
                    continue;

                BattlegroundSet bgs = sBattlegroundMgr->GetBattlegroundsByType((BattlegroundTypeId)i);
                for (BattlegroundSet::iterator itr = bgs.begin(); itr != bgs.end(); ++itr)
                {
                    Battleground* arena = itr->second;

                    if (!arena->GetPlayersSize())
                        continue;

                    uint16 mmr = arena->GetArenaMatchmakerRatingByIndex(0) + arena->GetArenaMatchmakerRatingByIndex(1);
                    mmr /= 2;

                    if (isTop && mmr >= TopGamesRating)
                    {
                        highGames++;
                        if (highGames > (page + 1) * GamesOnPage)
                        {
                            haveNextPage = true;
                            break;
                        }

                        if (highGames >= page * GamesOnPage)
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmr), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));
                    }
                    else if (!isTop && mmr < TopGamesRating)
                    {
                        lowGames++;
                        if (lowGames > (page + 1) * GamesOnPage)
                        {
                            haveNextPage = true;
                            break;
                        }

                        if (lowGames >= page * GamesOnPage)
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmr), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));
                    }
                }
            }

            if (page > 0)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Предыдущая...", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES + page - 1);

            if (haveNextPage)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Следующая...", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES + page + 1);
        }
};

void AddSC_spectator_commandscript()
{
    new spectator_commandscript();
}
