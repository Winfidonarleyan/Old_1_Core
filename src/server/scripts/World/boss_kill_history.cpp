#include "ScriptPCH.h"
#include "Config.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"

#define DATA_SEPARATOR ":"
#define PEOPLE_SEPARATOR " "

class boss_kill_history : public BossScript
{
    public:
        boss_kill_history() : BossScript("boss_kill_history_AllCreatureScript") { }

    void BossJustDied(Creature* creature)
    {
        if (!sWorld->getBoolConfig(CONFIG_BOSS_KILL_HISTORY_ENABLE))
            return;

        switch (creature->GetEntry())
        {
            case 31099:
            case 36633:
            case 36701:
                return;
        }

        Player* recipient = creature->GetLootRecipient();
        Map* map = creature->GetMap();
        InstanceMap* instance = map->ToInstanceMap();
        uint8 spawnMode = map->GetSpawnMode();
        uint32 GuildId = 0;
        uint32 KillerCount = 0;
        std::string bossName(creature->GetNameForLocaleIdx(sObjectMgr->GetDBCLocaleIndex()));
        std::string mapName(creature->GetMap()->GetMapName());
        std::string guildName;
        std::string TeamKill;
        bool IsGuildKill = false;

        if (!recipient || !map)
            return;

        if (map->IsBattlegroundOrArena())
            return;

        if(Group* group = recipient->GetGroup())
        {
            if (GuildId = group->IsGuildGroup())
            {
                IsGuildKill = true;
                guildName = sGuildMgr->GetGuildNameById(GuildId);
            }

            for(GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* temp = itr->getSource();

                if(!temp)
                    continue;

                KillerCount++;
                std::ostringstream PeopleData;
                PeopleData << temp->GetGUIDLow() << DATA_SEPARATOR;
                PeopleData << temp->GetName() << DATA_SEPARATOR;
                PeopleData << uint32(temp->getLevel()) << DATA_SEPARATOR;
                PeopleData << temp->GetGuildId() << DATA_SEPARATOR;
                PeopleData << uint32(temp->isAlive()) << DATA_SEPARATOR;
                PeopleData << uint32(temp->IsAtGroupRewardDistance(creature)) << PEOPLE_SEPARATOR;
                TeamKill += PeopleData.str();
            }
        }
        else
        {
            KillerCount = 1;

            std::ostringstream PeopleData;
            PeopleData << recipient->GetGUIDLow() << DATA_SEPARATOR;
            PeopleData << recipient->GetName() << DATA_SEPARATOR;
            PeopleData << uint32(recipient->getLevel()) << DATA_SEPARATOR;
            PeopleData << recipient->GetGuildId() << DATA_SEPARATOR;
            PeopleData << uint32(recipient->isAlive()) << DATA_SEPARATOR;
            PeopleData << uint32(1) << PEOPLE_SEPARATOR;
            TeamKill += PeopleData.str();
        }

        if (IsGuildKill)
        {
            int32 TextId = LANG_BOSS_KILL_HISTORY_NORMAL;
            uint32 mapMaxPlayers = instance ? instance->GetMaxPlayers() : 40;

            if (map->IsRaid())
            {
                if (spawnMode == RAID_DIFFICULTY_25MAN_HEROIC || spawnMode == RAID_DIFFICULTY_10MAN_HEROIC)
                    TextId = LANG_BOSS_KILL_HISTORY_HEROIC;
            }
            else if (spawnMode == DUNGEON_DIFFICULTY_HEROIC)
                TextId = LANG_BOSS_KILL_HISTORY_HEROIC;

            if(sWorld->getBoolConfig(CONFIG_BOSS_KILL_HISTORY_REWARD_ENABLE))
            {
                if(Group *group = recipient->GetGroup())
                    for(GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* player = itr->getSource();

                        if(player)
                            player->AddItem(sWorld->getIntConfig(CONFIG_BOSS_KILL_HISTORY_REWARD_ITEM), sWorld->getIntConfig(CONFIG_BOSS_KILL_HISTORY_REWARD_ITEM_COUNT));
                    }
            }

            sWorld->SendWorldText(TextId, mapName.c_str(), bossName.c_str(), mapMaxPlayers, guildName.c_str(), KillerCount);
        }

        CharacterDatabase.EscapeString(TeamKill);
        CharacterDatabase.EscapeString(bossName);
        CharacterDatabase.EscapeString(mapName);

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BOSS_KILL_HISTORY);
        stmt->setUInt32(0, GuildId);
        stmt->setUInt32(1, creature->GetEntry());
        stmt->setUInt32(2, KillerCount);
        stmt->setString(3, TeamKill.c_str());
        stmt->setUInt32(4, map->GetId());
        stmt->setInt8(5, uint8(map->IsRaid()));
        stmt->setString(6, bossName.c_str());
        stmt->setString(7, mapName.c_str());
        stmt->setUInt8(8, spawnMode);

        CharacterDatabase.Execute(stmt);
    }
};

void AddSC_boss_kill_history()
{
    new boss_kill_history();
}
