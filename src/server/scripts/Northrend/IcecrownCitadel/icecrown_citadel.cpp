/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
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

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellAuraEffects.h"
#include "SmartAI.h"
#include "icecrown_citadel.h"

// Weekly quest support
// * Deprogramming                (DONE)
// * Securing the Ramparts        (DONE)
// * Residue Rendezvous           (DONE)
// * Blood Quickening             (DONE)
// * Respite for a Tormented Soul

enum Texts
{
    // Highlord Tirion Fordring (at Light's Hammer)
    SAY_TIRION_INTRO_1              = 0,
    SAY_TIRION_INTRO_2              = 1,
    SAY_TIRION_INTRO_3              = 2,
    SAY_TIRION_INTRO_4              = 3,
    SAY_TIRION_INTRO_H_5            = 4,
    SAY_TIRION_INTRO_A_5            = 5,

    // The Lich King (at Light's Hammer)
    SAY_LK_INTRO_1                  = 0,
    SAY_LK_INTRO_2                  = 1,
    SAY_LK_INTRO_3                  = 2,
    SAY_LK_INTRO_4                  = 3,
    SAY_LK_INTRO_5                  = 4,

    // Highlord Bolvar Fordragon (at Light's Hammer)
    SAY_BOLVAR_INTRO_1              = 0,

    // High Overlord Saurfang (at Light's Hammer)
    SAY_SAURFANG_INTRO_1            = 15,
    SAY_SAURFANG_INTRO_2            = 16,
    SAY_SAURFANG_INTRO_3            = 17,
    SAY_SAURFANG_INTRO_4            = 18,

    // Muradin Bronzebeard (at Light's Hammer)
    SAY_MURADIN_INTRO_1             = 13,
    SAY_MURADIN_INTRO_2             = 14,
    SAY_MURADIN_INTRO_3             = 15,

    // Deathbound Ward
    SAY_TRAP_ACTIVATE               = 0,

    // Rotting Frost Giant
    EMOTE_DEATH_PLAGUE_WARNING      = 0,

    // Sister Svalna
    SAY_SVALNA_KILL_CAPTAIN         = 1, // happens when she kills a captain
    SAY_SVALNA_KILL                 = 4,
    SAY_SVALNA_CAPTAIN_DEATH        = 5, // happens when a captain resurrected by her dies
    SAY_SVALNA_DEATH                = 6,
    EMOTE_SVALNA_IMPALE             = 7,
    EMOTE_SVALNA_BROKEN_SHIELD      = 8,

    SAY_CROK_INTRO_1                = 0, // Ready your arms, my Argent Brothers. The Vrykul will protect the Frost Queen with their lives.
    SAY_ARNATH_INTRO_2              = 5, // Even dying here beats spending another day collecting reagents for that madman, Finklestein.
    SAY_CROK_INTRO_3                = 1, // Enough idle banter! Our champions have arrived - support them as we push our way through the hall!
    SAY_SVALNA_EVENT_START          = 0, // You may have once fought beside me, Crok, but now you are nothing more than a traitor. Come, your second death approaches!
    SAY_CROK_COMBAT_WP_0            = 2, // Draw them back to us, and we'll assist you.
    SAY_CROK_COMBAT_WP_1            = 3, // Quickly, push on!
    SAY_CROK_FINAL_WP               = 4, // Her reinforcements will arrive shortly, we must bring her down quickly!
    SAY_SVALNA_RESURRECT_CAPTAINS   = 2, // Foolish Crok. You brought my reinforcements with you. Arise, Argent Champions, and serve the Lich King in death!
    SAY_CROK_COMBAT_SVALNA          = 5, // I'll draw her attacks. Return our brothers to their graves, then help me bring her down!
    SAY_SVALNA_AGGRO                = 3, // Come, Scourgebane. I'll show the master which of us is truly worthy of the title of "Champion"!
    SAY_CAPTAIN_DEATH               = 0,
    SAY_CAPTAIN_RESURRECTED         = 1,
    SAY_CAPTAIN_KILL                = 2,
    SAY_CAPTAIN_SECOND_DEATH        = 3,
    SAY_CAPTAIN_SURVIVE_TALK        = 4,
    SAY_CROK_WEAKENING_GAUNTLET     = 6,
    SAY_CROK_WEAKENING_SVALNA       = 7,
    SAY_CROK_DEATH                  = 8,
};

enum Spells
{
    // Rotting Frost Giant
    SPELL_DEATH_PLAGUE              = 72879,
    SPELL_DEATH_PLAGUE_AURA         = 72865,
    SPELL_RECENTLY_INFECTED         = 72884,
    SPELL_DEATH_PLAGUE_KILL         = 72867,
    SPELL_STOMP                     = 64652,
    SPELL_ARCTIC_BREATH             = 72848,

    // Frost Freeze Trap
    SPELL_COLDFLAME_JETS            = 70460,

    // Alchemist Adrianna
    SPELL_HARVEST_BLIGHT_SPECIMEN   = 72155,
    SPELL_HARVEST_BLIGHT_SPECIMEN25 = 72162,

    // Crok Scourgebane
    SPELL_ICEBOUND_ARMOR            = 70714,
    SPELL_SCOURGE_STRIKE            = 71488,
    SPELL_DEATH_STRIKE              = 71489,

    // Sister Svalna
    SPELL_CARESS_OF_DEATH           = 70078,
    SPELL_IMPALING_SPEAR_KILL       = 70196,
    SPELL_REVIVE_CHAMPION           = 70053,
    SPELL_UNDEATH                   = 70089,
    SPELL_IMPALING_SPEAR            = 71443,
    SPELL_AETHER_SHIELD             = 71463,
    SPELL_HURL_SPEAR                = 71466,

    // Captain Arnath
    SPELL_DOMINATE_MIND             = 14515,
    SPELL_FLASH_HEAL_NORMAL         = 71595,
    SPELL_POWER_WORD_SHIELD_NORMAL  = 71548,
    SPELL_SMITE_NORMAL              = 71546,
    SPELL_FLASH_HEAL_UNDEAD         = 71782,
    SPELL_POWER_WORD_SHIELD_UNDEAD  = 71780,
    SPELL_SMITE_UNDEAD              = 71778,

    // Captain Brandon
    SPELL_CRUSADER_STRIKE           = 71549,
    SPELL_DIVINE_SHIELD             = 71550,
    SPELL_JUDGEMENT_OF_COMMAND      = 71551,
    SPELL_HAMMER_OF_BETRAYAL        = 71784,

    // Captain Grondel
    SPELL_CHARGE                    = 71553,
    SPELL_MORTAL_STRIKE             = 71552,
    SPELL_SUNDER_ARMOR              = 71554,
    SPELL_CONFLAGRATION             = 71785,

    // Captain Rupert
    SPELL_FEL_IRON_BOMB_NORMAL      = 71592,
    SPELL_MACHINE_GUN_NORMAL        = 71594,
    SPELL_ROCKET_LAUNCH_NORMAL      = 71590,
    SPELL_FEL_IRON_BOMB_UNDEAD      = 71787,
    SPELL_MACHINE_GUN_UNDEAD        = 71788,
    SPELL_ROCKET_LAUNCH_UNDEAD      = 71786,
};

// Helper defines
// Captain Arnath
#define SPELL_FLASH_HEAL        (IsUndead ? SPELL_FLASH_HEAL_UNDEAD : SPELL_FLASH_HEAL_NORMAL)
#define SPELL_POWER_WORD_SHIELD (IsUndead ? SPELL_POWER_WORD_SHIELD_UNDEAD : SPELL_POWER_WORD_SHIELD_NORMAL)
#define SPELL_SMITE             (IsUndead ? SPELL_SMITE_UNDEAD : SPELL_SMITE_NORMAL)

// Captain Rupert
#define SPELL_FEL_IRON_BOMB     (IsUndead ? SPELL_FEL_IRON_BOMB_UNDEAD : SPELL_FEL_IRON_BOMB_NORMAL)
#define SPELL_MACHINE_GUN       (IsUndead ? SPELL_MACHINE_GUN_UNDEAD : SPELL_MACHINE_GUN_NORMAL)
#define SPELL_ROCKET_LAUNCH     (IsUndead ? SPELL_ROCKET_LAUNCH_UNDEAD : SPELL_ROCKET_LAUNCH_NORMAL)

enum EventTypes
{
    // Highlord Tirion Fordring (at Light's Hammer)
    // The Lich King (at Light's Hammer)
    // Highlord Bolvar Fordragon (at Light's Hammer)
    // High Overlord Saurfang (at Light's Hammer)
    // Muradin Bronzebeard (at Light's Hammer)
    EVENT_TIRION_INTRO_2                = 1,
    EVENT_TIRION_INTRO_3                = 2,
    EVENT_TIRION_INTRO_4                = 3,
    EVENT_TIRION_INTRO_5                = 4,
    EVENT_LK_INTRO_1                    = 5,
    EVENT_TIRION_INTRO_6                = 6,
    EVENT_LK_INTRO_2                    = 7,
    EVENT_LK_INTRO_3                    = 8,
    EVENT_LK_INTRO_4                    = 9,
    EVENT_BOLVAR_INTRO_1                = 10,
    EVENT_LK_INTRO_5                    = 11,
    EVENT_SAURFANG_INTRO_1              = 12,
    EVENT_TIRION_INTRO_H_7              = 13,
    EVENT_SAURFANG_INTRO_2              = 14,
    EVENT_SAURFANG_INTRO_3              = 15,
    EVENT_SAURFANG_INTRO_4              = 16,
    EVENT_SAURFANG_RUN                  = 17,
    EVENT_MURADIN_INTRO_1               = 18,
    EVENT_MURADIN_INTRO_2               = 19,
    EVENT_MURADIN_INTRO_3               = 20,
    EVENT_TIRION_INTRO_A_7              = 21,
    EVENT_MURADIN_INTRO_4               = 22,
    EVENT_MURADIN_INTRO_5               = 23,
    EVENT_MURADIN_RUN                   = 24,

    // Rotting Frost Giant
    EVENT_DEATH_PLAGUE                  = 25,
    EVENT_STOMP                         = 26,
    EVENT_ARCTIC_BREATH                 = 27,

    // Frost Freeze Trap
    EVENT_ACTIVATE_TRAP                 = 28,

    // Crok Scourgebane
    EVENT_SCOURGE_STRIKE                = 29,
    EVENT_DEATH_STRIKE                  = 30,
    EVENT_HEALTH_CHECK                  = 31,
    EVENT_CROK_INTRO_3                  = 32,
    EVENT_START_PATHING                 = 33,

    // Sister Svalna
    EVENT_ARNATH_INTRO_2                = 34,
    EVENT_SVALNA_START                  = 35,
    EVENT_SVALNA_RESURRECT              = 36,
    EVENT_SVALNA_COMBAT                 = 37,
    EVENT_IMPALING_SPEAR                = 38,
    EVENT_AETHER_SHIELD                 = 39,

    // Captain Arnath
    EVENT_ARNATH_FLASH_HEAL             = 40,
    EVENT_ARNATH_PW_SHIELD              = 41,
    EVENT_ARNATH_SMITE                  = 42,
    EVENT_ARNATH_DOMINATE_MIND          = 43,

    // Captain Brandon
    EVENT_BRANDON_CRUSADER_STRIKE       = 44,
    EVENT_BRANDON_DIVINE_SHIELD         = 45,
    EVENT_BRANDON_JUDGEMENT_OF_COMMAND  = 46,
    EVENT_BRANDON_HAMMER_OF_BETRAYAL    = 47,

    // Captain Grondel
    EVENT_GRONDEL_CHARGE_CHECK          = 48,
    EVENT_GRONDEL_MORTAL_STRIKE         = 49,
    EVENT_GRONDEL_SUNDER_ARMOR          = 50,
    EVENT_GRONDEL_CONFLAGRATION         = 51,

    // Captain Rupert
    EVENT_RUPERT_FEL_IRON_BOMB          = 52,
    EVENT_RUPERT_MACHINE_GUN            = 53,
    EVENT_RUPERT_ROCKET_LAUNCH          = 54,
};

enum DataTypesICC
{
    DATA_DAMNED_KILLS       = 1,
};

enum Actions
{
    // Sister Svalna
    ACTION_KILL_CAPTAIN         = 1,
    ACTION_START_GAUNTLET       = 2,
    ACTION_RESURRECT_CAPTAINS   = 3,
    ACTION_CAPTAIN_DIES         = 4,
    ACTION_RESET_EVENT          = 5,
};

enum EventIds
{
    EVENT_AWAKEN_WARD_1 = 22900,
    EVENT_AWAKEN_WARD_2 = 22907,
    EVENT_AWAKEN_WARD_3 = 22908,
    EVENT_AWAKEN_WARD_4 = 22909,
};

enum MovementPoints
{
    POINT_LAND  = 1,
};

class FrostwingVrykulSearcher
{
    public:
        FrostwingVrykulSearcher(Creature const* source, float range) : _source(source), _range(range) {}

        bool operator()(Unit* unit)
        {
            if (!unit->isAlive())
                return false;

            switch (unit->GetEntry())
            {
                case NPC_YMIRJAR_BATTLE_MAIDEN:
                case NPC_YMIRJAR_DEATHBRINGER:
                case NPC_YMIRJAR_FROSTBINDER:
                case NPC_YMIRJAR_HUNTRESS:
                case NPC_YMIRJAR_WARLORD:
                    break;
                default:
                    return false;
            }

            if (!unit->IsWithinDist(_source, _range, false))
                return false;

            return true;
        }

    private:
        Creature const* _source;
        float _range;
};

class FrostwingGauntletRespawner
{
    public:
        void operator()(Creature* creature)
        {
            switch (creature->GetOriginalEntry())
            {
                case NPC_YMIRJAR_BATTLE_MAIDEN:
                case NPC_YMIRJAR_DEATHBRINGER:
                case NPC_YMIRJAR_FROSTBINDER:
                case NPC_YMIRJAR_HUNTRESS:
                case NPC_YMIRJAR_WARLORD:
                    break;
                case NPC_CROK_SCOURGEBANE:
                case NPC_CAPTAIN_ARNATH:
                case NPC_CAPTAIN_BRANDON:
                case NPC_CAPTAIN_GRONDEL:
                case NPC_CAPTAIN_RUPERT:
                    creature->AI()->DoAction(ACTION_RESET_EVENT);
                    break;
                case NPC_SISTER_SVALNA:
                    creature->AI()->DoAction(ACTION_RESET_EVENT);
                    // return, this creature is never dead if event is reset
                    return;
                default:
                    return;
            }

            uint32 corpseDelay = creature->GetCorpseDelay();
            uint32 respawnDelay = creature->GetRespawnDelay();
            creature->SetCorpseDelay(1);
            creature->SetRespawnDelay(2);

            if (CreatureData const* data = creature->GetCreatureData())
                creature->SetPosition(data->posX, data->posY, data->posZ, data->orientation);
            creature->ForcedDespawn();

            creature->SetCorpseDelay(corpseDelay);
            creature->SetRespawnDelay(respawnDelay);
        }
};

class CaptainSurviveTalk : public BasicEvent
{
    public:
        explicit CaptainSurviveTalk(Creature const& owner) : _owner(owner) { }

        bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
        {
            _owner.AI()->Talk(SAY_CAPTAIN_SURVIVE_TALK);
            return true;
        }

    private:
        Creature const& _owner;
};

// at Light's Hammer
class npc_highlord_tirion_fordring_lh : public CreatureScript
{
    public:
        npc_highlord_tirion_fordring_lh() : CreatureScript("npc_highlord_tirion_fordring_lh") { }

        struct npc_highlord_tirion_fordringAI : public ScriptedAI
        {
            npc_highlord_tirion_fordringAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript())
            {
            }

            void Reset()
            {
                _events.Reset();
                _theLichKing = 0;
                _bolvarFordragon = 0;
                _factionNPC = 0;
                _damnedKills = 0;
                _is_intro_started = 0;
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!who || who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (!_is_intro_started && me->IsWithinDist(who, 5.0f))
                {
                        if (Creature* theLichKing = me->FindNearestCreature(NPC_THE_LICH_KING_LH, 150.0f))
                        {
                            if (Creature* bolvarFordragon = me->FindNearestCreature(NPC_HIGHLORD_BOLVAR_FORDRAGON_LH, 150.0f))
                            {
                                if (Creature* factionNPC = me->FindNearestCreature(_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? NPC_SE_HIGH_OVERLORD_SAURFANG : NPC_SE_MURADIN_BRONZEBEARD, 50.0f))
                                {
                                    me->setActive(true);
                                    _theLichKing = theLichKing->GetGUID();
                                    theLichKing->setActive(true);
                                    _bolvarFordragon = bolvarFordragon->GetGUID();
                                    bolvarFordragon->setActive(true);
                                    _factionNPC = factionNPC->GetGUID();
                                    factionNPC->setActive(true);
                                    _is_intro_started = true;
                                }
                            }
                        }

                        if (!_bolvarFordragon || !_theLichKing || !_factionNPC)
                            return;

                        Talk(SAY_TIRION_INTRO_1);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_2, 4000);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_3, 14000);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_4, 18000);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_5, 31000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_1, 35000);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_6, 51000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_2, 58000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_3, 74000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_4, 86000);
                        _events.ScheduleEvent(EVENT_BOLVAR_INTRO_1, 108000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_5, 115000);

                        if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
                        {
                            _events.ScheduleEvent(EVENT_SAURFANG_INTRO_1, 120000);
                            _events.ScheduleEvent(EVENT_TIRION_INTRO_H_7, 129000);
                            _events.ScheduleEvent(EVENT_SAURFANG_INTRO_2, 139000);
                            _events.ScheduleEvent(EVENT_SAURFANG_INTRO_3, 150000);
                            _events.ScheduleEvent(EVENT_SAURFANG_INTRO_4, 162000);
                            _events.ScheduleEvent(EVENT_SAURFANG_RUN, 170000);
                        }
                        else
                        {
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_1, 120000);
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_2, 124000);
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_3, 127000);
                            _events.ScheduleEvent(EVENT_TIRION_INTRO_A_7, 136000);
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_4, 144000);
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_5, 151000);
                            _events.ScheduleEvent(EVENT_MURADIN_RUN, 157000);
                        }
                    }
            }

            void UpdateAI(uint32 const diff)
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TIRION_INTRO_2:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            break;
                        case EVENT_TIRION_INTRO_3:
                            Talk(SAY_TIRION_INTRO_2);
                            break;
                        case EVENT_TIRION_INTRO_4:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            break;
                        case EVENT_TIRION_INTRO_5:
                            Talk(SAY_TIRION_INTRO_3);
                            break;
                        case EVENT_LK_INTRO_1:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_POINT_NO_SHEATHE);
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_1);
                            break;
                        case EVENT_TIRION_INTRO_6:
                            Talk(SAY_TIRION_INTRO_4);
                            break;
                        case EVENT_LK_INTRO_2:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_2);
                            break;
                        case EVENT_LK_INTRO_3:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_3);
                            break;
                        case EVENT_LK_INTRO_4:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_4);
                            break;
                        case EVENT_BOLVAR_INTRO_1:
                            if (Creature* bolvarFordragon = ObjectAccessor::GetCreature(*me, _bolvarFordragon))
                            {
                                bolvarFordragon->AI()->Talk(SAY_BOLVAR_INTRO_1);
                                bolvarFordragon->setActive(false);
                            }
                            break;
                        case EVENT_LK_INTRO_5:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                            {
                                theLichKing->AI()->Talk(SAY_LK_INTRO_5);
                                theLichKing->setActive(false);
                            }
                            break;
                        case EVENT_SAURFANG_INTRO_1:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_1);
                            break;
                        case EVENT_TIRION_INTRO_H_7:
                            Talk(SAY_TIRION_INTRO_H_5);
                            break;
                        case EVENT_SAURFANG_INTRO_2:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_2);
                            break;
                        case EVENT_SAURFANG_INTRO_3:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_3);
                            break;
                        case EVENT_SAURFANG_INTRO_4:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_4);
                            break;
                        case EVENT_MURADIN_RUN:
                        case EVENT_SAURFANG_RUN:
                            if (Creature* factionNPC = ObjectAccessor::GetCreature(*me, _factionNPC))
                                factionNPC->GetMotionMaster()->MovePath(factionNPC->GetDBTableGUIDLow()*10, false);
                            me->setActive(false);
                            _damnedKills = 3;
                            break;
                        case EVENT_MURADIN_INTRO_1:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_1);
                            break;
                        case EVENT_MURADIN_INTRO_2:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            break;
                        case EVENT_MURADIN_INTRO_3:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            break;
                        case EVENT_TIRION_INTRO_A_7:
                            Talk(SAY_TIRION_INTRO_A_5);
                            break;
                        case EVENT_MURADIN_INTRO_4:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_2);
                            break;
                        case EVENT_MURADIN_INTRO_5:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_3);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap _events;
            InstanceScript* const _instance;
            uint64 _theLichKing;
            uint64 _bolvarFordragon;
            uint64 _factionNPC;
            uint16 _damnedKills;
            bool _is_intro_started;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_highlord_tirion_fordringAI>(creature);
        }
};

class npc_rotting_frost_giant : public CreatureScript
{
    public:
        npc_rotting_frost_giant() : CreatureScript("npc_rotting_frost_giant") { }

        struct npc_rotting_frost_giantAI : public ScriptedAI
        {
            npc_rotting_frost_giantAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                _events.Reset();
                _events.ScheduleEvent(EVENT_DEATH_PLAGUE, 15000);
                _events.ScheduleEvent(EVENT_STOMP, urand(5000, 8000));
                _events.ScheduleEvent(EVENT_ARCTIC_BREATH, urand(10000, 15000));
            }

            void JustDied(Unit* /*killer*/)
            {
                _events.Reset();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DEATH_PLAGUE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            {
                                Talk(EMOTE_DEATH_PLAGUE_WARNING, target->GetGUID());
                                DoCast(target, SPELL_DEATH_PLAGUE);
                            }
                            _events.ScheduleEvent(EVENT_DEATH_PLAGUE, 15000);
                            break;
                        case EVENT_STOMP:
                            DoCastVictim(SPELL_STOMP);
                            _events.ScheduleEvent(EVENT_STOMP, urand(15000, 18000));
                            break;
                        case EVENT_ARCTIC_BREATH:
                            DoCastVictim(SPELL_ARCTIC_BREATH);
                            _events.ScheduleEvent(EVENT_ARCTIC_BREATH, urand(26000, 33000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_rotting_frost_giantAI>(creature);
        }
};

class npc_frost_freeze_trap : public CreatureScript
{
    public:
        npc_frost_freeze_trap() : CreatureScript("npc_frost_freeze_trap") { }

        struct npc_frost_freeze_trapAI: public Scripted_NoMovementAI
        {
            npc_frost_freeze_trapAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case 1000:
                    case 11000:
                        _events.ScheduleEvent(EVENT_ACTIVATE_TRAP, uint32(action));
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                _events.Update(diff);

                if (_events.ExecuteEvent() == EVENT_ACTIVATE_TRAP)
                {
                    DoCast(me, SPELL_COLDFLAME_JETS);
                    _events.ScheduleEvent(EVENT_ACTIVATE_TRAP, 22000);
                }
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_frost_freeze_trapAI>(creature);
        }
};

class npc_alchemist_adrianna : public CreatureScript
{
    public:
        npc_alchemist_adrianna() : CreatureScript("npc_alchemist_adrianna") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (!creature->FindCurrentSpellBySpellId(SPELL_HARVEST_BLIGHT_SPECIMEN) && !creature->FindCurrentSpellBySpellId(SPELL_HARVEST_BLIGHT_SPECIMEN25))
                if (player->HasAura(SPELL_ORANGE_BLIGHT_RESIDUE) && player->HasAura(SPELL_GREEN_BLIGHT_RESIDUE))
                    creature->CastSpell(creature, SPELL_HARVEST_BLIGHT_SPECIMEN, false);
            return false;
        }
};

class boss_sister_svalna : public CreatureScript
{
    public:
        boss_sister_svalna() : CreatureScript("boss_sister_svalna") { }

        struct boss_sister_svalnaAI : public BossAI
        {
            boss_sister_svalnaAI(Creature* creature) : BossAI(creature, DATA_SISTER_SVALNA),
                _isEventInProgress(false)
            {
            }

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();

                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                _Reset();
                me->SetReactState(REACT_DEFENSIVE);
                _isEventInProgress = false;
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_SVALNA_DEATH);

                uint64 delay = 1;
                for (uint32 i = 0; i < 4; ++i)
                {
                    if (Creature* crusader = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CAPTAIN_ARNATH + i)))
                    {
                        if (crusader->isAlive() && crusader->GetEntry() == crusader->GetCreatureData()->id)
                        {
                            crusader->m_Events.AddEvent(new CaptainSurviveTalk(*crusader), crusader->m_Events.CalculateTime(delay));
                            delay += 6000;
                        }
                    }
                }
            }

            void EnterCombat(Unit* /*attacker*/)
            {
                _EnterCombat();
                if (Creature* crok = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CROK_SCOURGEBANE)))
                    crok->AI()->Talk(SAY_CROK_COMBAT_SVALNA);
                events.ScheduleEvent(EVENT_SVALNA_COMBAT, 9000);
                events.ScheduleEvent(EVENT_IMPALING_SPEAR, urand(40000, 50000));
                events.ScheduleEvent(EVENT_AETHER_SHIELD, urand(100000, 110000));
            }

            void KilledUnit(Unit* victim)
            {
                switch (victim->GetTypeId())
                {
                    case TYPEID_PLAYER:
                        Talk(SAY_SVALNA_KILL);
                        break;
                    case TYPEID_UNIT:
                        switch (victim->GetEntry())
                        {
                            case NPC_CAPTAIN_ARNATH:
                            case NPC_CAPTAIN_BRANDON:
                            case NPC_CAPTAIN_GRONDEL:
                            case NPC_CAPTAIN_RUPERT:
                                Talk(SAY_SVALNA_KILL_CAPTAIN);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            void JustReachedHome()
            {
                _JustReachedHome();
                me->SetReactState(REACT_PASSIVE);
                me->SetCanFly(false);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_KILL_CAPTAIN:
                        me->CastCustomSpell(SPELL_CARESS_OF_DEATH, SPELLVALUE_MAX_TARGETS, 1, me, true);
                        break;
                    case ACTION_START_GAUNTLET:
                        me->setActive(true);
                        _isEventInProgress = true;
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        events.ScheduleEvent(EVENT_SVALNA_START, 25000);
                        break;
                    case ACTION_RESURRECT_CAPTAINS:
                        events.ScheduleEvent(EVENT_SVALNA_RESURRECT, 7000);
                        break;
                    case ACTION_CAPTAIN_DIES:
                        Talk(SAY_SVALNA_CAPTAIN_DEATH);
                        break;
                    case ACTION_RESET_EVENT:
                        me->setActive(false);
                        Reset();
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_HURL_SPEAR && me->HasAura(SPELL_AETHER_SHIELD))
                {
                    me->RemoveAurasDueToSpell(SPELL_AETHER_SHIELD);
                    Talk(EMOTE_SVALNA_BROKEN_SHIELD, caster->GetGUID());
                }
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE || id != POINT_LAND)
                    return;

                _isEventInProgress = false;
                me->setActive(false);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                me->SetCanFly(false);
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                switch (spell->Id)
                {
                    case SPELL_IMPALING_SPEAR_KILL:
                        me->Kill(target);
                        break;
                    case SPELL_IMPALING_SPEAR:
                        if (TempSummon* summon = target->SummonCreature(NPC_IMPALING_SPEAR, *target))
                        {
                            Talk(EMOTE_SVALNA_IMPALE, target->GetGUID());
                            summon->CastCustomSpell(VEHICLE_SPELL_RIDE_HARDCODED, SPELLVALUE_BASE_POINT0, 1, target, false);
                            summon->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK1 | UNIT_FLAG2_ALLOW_ENEMY_INTERACT);
                        }
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() && !_isEventInProgress)
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SVALNA_START:
                            Talk(SAY_SVALNA_EVENT_START);
                            break;
                        case EVENT_SVALNA_RESURRECT:
                                          me->setActive(false);
                                          me->SetCanFly(false);
                                          me->SetPosition(4356.33f, 2484.21f, 358.43f, 1.54f);
                                          me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                            Talk(SAY_SVALNA_RESURRECT_CAPTAINS);
                            me->CastSpell(me, SPELL_REVIVE_CHAMPION, false);
                            break;
                        case EVENT_SVALNA_COMBAT:
                            me->SetReactState(REACT_AGGRESSIVE);
                            Talk(SAY_SVALNA_AGGRO);
                            break;
                        case EVENT_IMPALING_SPEAR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_IMPALING_SPEAR))
                            {
                                DoCast(me, SPELL_AETHER_SHIELD);
                                DoCast(target, SPELL_IMPALING_SPEAR);
                            }
                            events.ScheduleEvent(EVENT_IMPALING_SPEAR, urand(20000, 25000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool _isEventInProgress;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<boss_sister_svalnaAI>(creature);
        }
};

class npc_crok_scourgebane : public CreatureScript
{
    public:
        npc_crok_scourgebane() : CreatureScript("npc_crok_scourgebane") { }

        struct npc_crok_scourgebaneAI : public npc_escortAI
        {
            npc_crok_scourgebaneAI(Creature* creature) : npc_escortAI(creature),
                _instance(creature->GetInstanceScript()), _respawnTime(creature->GetRespawnDelay()),
                _corpseDelay(creature->GetCorpseDelay())
            {
                SetDespawnAtEnd(false);
                SetDespawnAtFar(false);
                _isEventActive = false;
                _isEventDone = _instance->GetBossState(DATA_SISTER_SVALNA) == DONE;
                _didUnderTenPercentText = false;
            }

            void Reset()
            {
                _events.Reset();
                _events.ScheduleEvent(EVENT_SCOURGE_STRIKE, urand(7500, 12500));
                _events.ScheduleEvent(EVENT_DEATH_STRIKE, urand(25000, 30000));
                me->SetReactState(REACT_DEFENSIVE);
                _didUnderTenPercentText = false;
                _wipeCheckTimer = 1000;
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_START_GAUNTLET)
                {
                    if (_isEventDone || !me->isAlive())
                        return;

                    _isEventActive = true;
                    _isEventDone = true;
                    // Load Grid with Sister Svalna
                    me->GetMap()->LoadGrid(4356.71f, 2484.33f);
                    if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                        svalna->AI()->DoAction(ACTION_START_GAUNTLET);
                    Talk(SAY_CROK_INTRO_1);
                    _events.ScheduleEvent(EVENT_ARNATH_INTRO_2, 7000);
                    _events.ScheduleEvent(EVENT_CROK_INTRO_3, 14000);
                    _events.ScheduleEvent(EVENT_START_PATHING, 37000);
                    me->setActive(true);
                    for (uint32 i = 0; i < 4; ++i)
                        if (Creature* crusader = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_CAPTAIN_ARNATH + i)))
                            crusader->AI()->DoAction(ACTION_START_GAUNTLET);
                }
                else if (action == ACTION_RESET_EVENT)
                {
                    _isEventActive = false;
                    _isEventDone = _instance->GetBossState(DATA_SISTER_SVALNA) == DONE;
                    me->setActive(false);
                    _aliveTrash.clear();
                    _currentWPid = 0;
                }
            }

            void SetGUID(uint64 guid, int32 type/* = 0*/)
            {
                if (type == ACTION_VRYKUL_DEATH)
                {
                    _aliveTrash.erase(guid);
                    if (_aliveTrash.empty())
                    {
                        SetEscortPaused(false);
                        if (_currentWPid == 4 && _isEventActive)
                        {
                            _isEventActive = false;
                            me->setActive(false);
                            Talk(SAY_CROK_FINAL_WP);
                            if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                                svalna->AI()->DoAction(ACTION_RESURRECT_CAPTAINS);
                        }
                    }
                }
            }

            void WaypointReached(uint32 waypointId)
            {
                switch (waypointId)
                {
                    // pause pathing until trash pack is cleared
                    case 0:
                        Talk(SAY_CROK_COMBAT_WP_0);
                        if (!_aliveTrash.empty())
                            SetEscortPaused(true);
                        break;
                    case 1:
                        Talk(SAY_CROK_COMBAT_WP_1);
                        if (!_aliveTrash.empty())
                            SetEscortPaused(true);
                        break;
                    case 4:
                        if (_aliveTrash.empty() && _isEventActive)
                        {
                            _isEventActive = false;
                            me->setActive(false);
                            Talk(SAY_CROK_FINAL_WP);
                            if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                                svalna->AI()->DoAction(ACTION_RESURRECT_CAPTAINS);
                        }
                        break;
                    default:
                        break;
                }
            }

            void WaypointStart(uint32 waypointId)
            {
                _currentWPid = waypointId;
                switch (waypointId)
                {
                    case 0:
                    case 1:
                    case 4:
                    {
                        // get spawns by home position
                        float minY = 2600.0f;
                        float maxY = 2650.0f;
                        if (waypointId == 1)
                        {
                            minY -= 50.0f;
                            maxY -= 50.0f;
                            // at waypoints 1 and 2 she kills one captain
                            if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                                svalna->AI()->DoAction(ACTION_KILL_CAPTAIN);
                        }
                        else if (waypointId == 4)
                        {
                            minY -= 100.0f;
                            maxY -= 100.0f;
                        }

                        // get all nearby vrykul
                        std::list<Creature*> temp;
                        FrostwingVrykulSearcher check(me, 80.0f);
                        Trinity::CreatureListSearcher<FrostwingVrykulSearcher> searcher(me, temp, check);
                        me->VisitNearbyGridObject(80.0f, searcher);

                        _aliveTrash.clear();
                        for (std::list<Creature*>::iterator itr = temp.begin(); itr != temp.end(); ++itr)
                            if ((*itr)->GetHomePosition().GetPositionY() < maxY && (*itr)->GetHomePosition().GetPositionY() > minY)
                                _aliveTrash.insert((*itr)->GetGUID());
                        break;
                    }
                    // at waypoints 1 and 2 she kills one captain
                    case 2:
                        if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                            svalna->AI()->DoAction(ACTION_KILL_CAPTAIN);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage)
            {
                // check wipe
                if (!_wipeCheckTimer)
                {
                    _wipeCheckTimer = 1000;
                    Player* player = NULL;
                    Trinity::AnyPlayerInObjectRangeCheck check(me, 60.0f);
                    Trinity::PlayerSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, player, check);
                    me->VisitNearbyWorldObject(60.0f, searcher);
                    // wipe
                    if (!player)
                    {
                        damage *= 100;
                        if (damage >= me->GetHealth())
                        {
                            FrostwingGauntletRespawner respawner;
                            Trinity::CreatureWorker<FrostwingGauntletRespawner> worker(me, respawner);
                            me->VisitNearbyGridObject(333.0f, worker);
                            Talk(SAY_CROK_DEATH);
                        }
                        return;
                    }
                }

                if (HealthBelowPct(10))
                {
                    if (!_didUnderTenPercentText)
                    {
                        _didUnderTenPercentText = true;
                        if (_isEventActive)
                            Talk(SAY_CROK_WEAKENING_GAUNTLET);
                        else
                            Talk(SAY_CROK_WEAKENING_SVALNA);
                    }

                    damage = 0;
                    DoCast(me, SPELL_ICEBOUND_ARMOR);
                    _events.ScheduleEvent(EVENT_HEALTH_CHECK, 1000);
                }
            }

            void UpdateEscortAI(uint32 const diff)
            {
                if (_wipeCheckTimer <= diff)
                    _wipeCheckTimer = 0;
                else
                    _wipeCheckTimer -= diff;

                if (!UpdateVictim() && !_isEventActive)
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ARNATH_INTRO_2:
                            if (Creature* arnath = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_CAPTAIN_ARNATH)))
                                arnath->AI()->Talk(SAY_ARNATH_INTRO_2);
                            break;
                        case EVENT_CROK_INTRO_3:
                            Talk(SAY_CROK_INTRO_3);
                            break;
                        case EVENT_START_PATHING:
                            Start(true, true);
                            break;
                        case EVENT_SCOURGE_STRIKE:
                            DoCastVictim(SPELL_SCOURGE_STRIKE);
                            _events.ScheduleEvent(EVENT_SCOURGE_STRIKE, urand(10000, 14000));
                            break;
                        case EVENT_DEATH_STRIKE:
                            if (HealthBelowPct(20))
                                DoCastVictim(SPELL_DEATH_STRIKE);
                            _events.ScheduleEvent(EVENT_DEATH_STRIKE, urand(5000, 10000));
                            break;
                        case EVENT_HEALTH_CHECK:
                            if (HealthAbovePct(15))
                            {
                                me->RemoveAurasDueToSpell(SPELL_ICEBOUND_ARMOR);
                                _didUnderTenPercentText = false;
                            }
                            else
                            {
                                me->DealHeal(me, me->CountPctFromMaxHealth(5));
                                _events.ScheduleEvent(EVENT_HEALTH_CHECK, 1000);
                            }
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            bool CanAIAttack(Unit const* target) const
            {
                // do not see targets inside Frostwing Halls when we are not there
                if (target->GetPositionY() < 2650.0f)
                    return true;
                return false;
            }

        private:
            EventMap _events;
            std::set<uint64> _aliveTrash;
            InstanceScript* _instance;
            uint32 _currentWPid;
            uint32 _wipeCheckTimer;
            uint32 const _respawnTime;
            uint32 const _corpseDelay;
            bool _isEventActive;
            bool _isEventDone;
            bool _didUnderTenPercentText;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_crok_scourgebaneAI>(creature);
        }
};

struct npc_argent_captainAI : public ScriptedAI
{
    public:
        npc_argent_captainAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()), _firstDeath(true)
        {
            FollowAngle = PET_FOLLOW_ANGLE;
            FollowDist = PET_FOLLOW_DIST;
            IsUndead = false;
        }

        void JustDied(Unit* /*killer*/)
        {
            if (_firstDeath)
            {
                _firstDeath = false;
                Talk(SAY_CAPTAIN_DEATH);
            }
            else
                Talk(SAY_CAPTAIN_SECOND_DEATH);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_CAPTAIN_KILL);
        }

        void DoAction(int32 const action)
        {
            if (action == ACTION_START_GAUNTLET)
            {
                if (Creature* crok = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CROK_SCOURGEBANE)))
                {
                    me->SetReactState(REACT_DEFENSIVE);
                    FollowAngle = me->GetAngle(crok) + me->GetOrientation();
                    FollowDist = me->GetDistance2d(crok);
                    me->GetMotionMaster()->MoveFollow(crok, FollowDist, FollowAngle, MOTION_SLOT_IDLE);
                }

                me->setActive(true);
            }
            else if (action == ACTION_RESET_EVENT)
            {
                _firstDeath = true;
            }
        }

        void EnterCombat(Unit* /*target*/)
        {
            me->SetHomePosition(*me);
            if (IsUndead)
                DoZoneInCombat();
        }

        bool CanAIAttack(Unit const* target) const
        {
            // do not see targets inside Frostwing Halls when we are not there
            if (target->GetPositionY() < 2650.0f)
                return true;
            return false;
        }

        void EnterEvadeMode()
        {
            // not yet following
            if (me->GetMotionMaster()->GetMotionSlotType(MOTION_SLOT_IDLE) != CHASE_MOTION_TYPE || IsUndead)
            {
                ScriptedAI::EnterEvadeMode();
                return;
            }

            if (!_EnterEvadeMode())
                return;

            if (!me->GetVehicle())
            {
                me->GetMotionMaster()->Clear(false);
                if (Creature* crok = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CROK_SCOURGEBANE)))
                    me->GetMotionMaster()->MoveFollow(crok, FollowDist, FollowAngle, MOTION_SLOT_IDLE);
            }

            Reset();
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_REVIVE_CHAMPION && !IsUndead)
            {
                IsUndead = true;
                me->setDeathState(JUST_ALIVED);
                uint32 newEntry = 0;
                switch (me->GetEntry())
                {
                    case NPC_CAPTAIN_ARNATH:
                        newEntry = NPC_CAPTAIN_ARNATH_UNDEAD;
                        break;
                    case NPC_CAPTAIN_BRANDON:
                        newEntry = NPC_CAPTAIN_BRANDON_UNDEAD;
                        break;
                    case NPC_CAPTAIN_GRONDEL:
                        newEntry = NPC_CAPTAIN_GRONDEL_UNDEAD;
                        break;
                    case NPC_CAPTAIN_RUPERT:
                        newEntry = NPC_CAPTAIN_RUPERT_UNDEAD;
                        break;
                    default:
                        return;
                }

                Talk(SAY_CAPTAIN_RESURRECTED);
                me->UpdateEntry(newEntry, instance->GetData(DATA_TEAM_IN_INSTANCE), me->GetCreatureData());
                DoCast(me, SPELL_UNDEATH, true);
            }
        }

    protected:
        EventMap Events;
        InstanceScript* instance;
        float FollowAngle;
        float FollowDist;
        bool IsUndead;

    private:
        bool _firstDeath;
};

class npc_captain_arnath : public CreatureScript
{
    public:
        npc_captain_arnath() : CreatureScript("npc_captain_arnath") { }

        struct npc_captain_arnathAI : public npc_argent_captainAI
        {
            npc_captain_arnathAI(Creature* creature) : npc_argent_captainAI(creature)
            {
            }

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_ARNATH_FLASH_HEAL, urand(4000, 7000));
                Events.ScheduleEvent(EVENT_ARNATH_PW_SHIELD, urand(8000, 14000));
                Events.ScheduleEvent(EVENT_ARNATH_SMITE, urand(3000, 6000));
                if (Is25ManRaid() && IsUndead)
                    Events.ScheduleEvent(EVENT_ARNATH_DOMINATE_MIND, urand(22000, 27000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ARNATH_FLASH_HEAL:
                            if (Creature* target = FindFriendlyCreature())
                                DoCast(target, SPELL_FLASH_HEAL);
                            Events.ScheduleEvent(EVENT_ARNATH_FLASH_HEAL, urand(6000, 9000));
                            break;
                        case EVENT_ARNATH_PW_SHIELD:
                        {
                            std::list<Creature*> targets = DoFindFriendlyMissingBuff(40.0f, SPELL_POWER_WORD_SHIELD);
                            DoCast(Trinity::Containers::SelectRandomContainerElement(targets), SPELL_POWER_WORD_SHIELD);
                            Events.ScheduleEvent(EVENT_ARNATH_PW_SHIELD, urand(15000, 20000));
                            break;
                        }
                        case EVENT_ARNATH_SMITE:
                            DoCastVictim(SPELL_SMITE);
                            Events.ScheduleEvent(EVENT_ARNATH_SMITE, urand(4000, 7000));
                            break;
                        case EVENT_ARNATH_DOMINATE_MIND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_DOMINATE_MIND);
                            Events.ScheduleEvent(EVENT_ARNATH_DOMINATE_MIND, urand(28000, 37000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            Creature* FindFriendlyCreature() const
            {
                Creature* target = NULL;
                Trinity::MostHPMissingInRange u_check(me, 60.0f, 0);
                Trinity::CreatureLastSearcher<Trinity::MostHPMissingInRange> searcher(me, target, u_check);
                me->VisitNearbyGridObject(60.0f, searcher);
                return target;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_captain_arnathAI>(creature);
        }
};

class npc_captain_brandon : public CreatureScript
{
    public:
        npc_captain_brandon() : CreatureScript("npc_captain_brandon") { }

        struct npc_captain_brandonAI : public npc_argent_captainAI
        {
            npc_captain_brandonAI(Creature* creature) : npc_argent_captainAI(creature)
            {
            }

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_BRANDON_CRUSADER_STRIKE, urand(6000, 10000));
                Events.ScheduleEvent(EVENT_BRANDON_DIVINE_SHIELD, 500);
                Events.ScheduleEvent(EVENT_BRANDON_JUDGEMENT_OF_COMMAND, urand(8000, 13000));
                if (IsUndead)
                    Events.ScheduleEvent(EVENT_BRANDON_HAMMER_OF_BETRAYAL, urand(25000, 30000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BRANDON_CRUSADER_STRIKE:
                            DoCastVictim(SPELL_CRUSADER_STRIKE);
                            Events.ScheduleEvent(EVENT_BRANDON_CRUSADER_STRIKE, urand(6000, 12000));
                            break;
                        case EVENT_BRANDON_DIVINE_SHIELD:
                            if (HealthBelowPct(20))
                                DoCast(me, SPELL_DIVINE_SHIELD);
                            else
                                Events.ScheduleEvent(EVENT_BRANDON_DIVINE_SHIELD, 500);
                            break;
                        case EVENT_BRANDON_JUDGEMENT_OF_COMMAND:
                            DoCastVictim(SPELL_JUDGEMENT_OF_COMMAND);
                            Events.ScheduleEvent(EVENT_BRANDON_JUDGEMENT_OF_COMMAND, urand(8000, 13000));
                            break;
                        case EVENT_BRANDON_HAMMER_OF_BETRAYAL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_HAMMER_OF_BETRAYAL);
                            Events.ScheduleEvent(EVENT_BRANDON_HAMMER_OF_BETRAYAL, urand(45000, 60000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_captain_brandonAI>(creature);
        }
};

class npc_captain_grondel : public CreatureScript
{
    public:
        npc_captain_grondel() : CreatureScript("npc_captain_grondel") { }

        struct npc_captain_grondelAI : public npc_argent_captainAI
        {
            npc_captain_grondelAI(Creature* creature) : npc_argent_captainAI(creature)
            {
            }

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_GRONDEL_CHARGE_CHECK, 500);
                Events.ScheduleEvent(EVENT_GRONDEL_MORTAL_STRIKE, urand(8000, 14000));
                Events.ScheduleEvent(EVENT_GRONDEL_SUNDER_ARMOR, urand(3000, 12000));
                if (IsUndead)
                    Events.ScheduleEvent(EVENT_GRONDEL_CONFLAGRATION, urand(12000, 17000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GRONDEL_CHARGE_CHECK:
                            DoCastVictim(SPELL_CHARGE);
                            Events.ScheduleEvent(EVENT_GRONDEL_CHARGE_CHECK, 500);
                            break;
                        case EVENT_GRONDEL_MORTAL_STRIKE:
                            DoCastVictim(SPELL_MORTAL_STRIKE);
                            Events.ScheduleEvent(EVENT_GRONDEL_MORTAL_STRIKE, urand(10000, 15000));
                            break;
                        case EVENT_GRONDEL_SUNDER_ARMOR:
                            DoCastVictim(SPELL_SUNDER_ARMOR);
                            Events.ScheduleEvent(EVENT_GRONDEL_SUNDER_ARMOR, urand(5000, 17000));
                            break;
                        case EVENT_GRONDEL_CONFLAGRATION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_CONFLAGRATION);
                            Events.ScheduleEvent(EVENT_GRONDEL_CONFLAGRATION, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_captain_grondelAI>(creature);
        }
};

class npc_captain_rupert : public CreatureScript
{
    public:
        npc_captain_rupert() : CreatureScript("npc_captain_rupert") { }

        struct npc_captain_rupertAI : public npc_argent_captainAI
        {
            npc_captain_rupertAI(Creature* creature) : npc_argent_captainAI(creature)
            {
            }

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_RUPERT_FEL_IRON_BOMB, urand(15000, 20000));
                Events.ScheduleEvent(EVENT_RUPERT_MACHINE_GUN, urand(25000, 30000));
                Events.ScheduleEvent(EVENT_RUPERT_ROCKET_LAUNCH, urand(10000, 15000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RUPERT_FEL_IRON_BOMB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_FEL_IRON_BOMB);
                            Events.ScheduleEvent(EVENT_RUPERT_FEL_IRON_BOMB, urand(15000, 20000));
                            break;
                        case EVENT_RUPERT_MACHINE_GUN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                                DoCast(target, SPELL_MACHINE_GUN);
                            Events.ScheduleEvent(EVENT_RUPERT_MACHINE_GUN, urand(25000, 30000));
                            break;
                        case EVENT_RUPERT_ROCKET_LAUNCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                                DoCast(target, SPELL_ROCKET_LAUNCH);
                            Events.ScheduleEvent(EVENT_RUPERT_ROCKET_LAUNCH, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_captain_rupertAI>(creature);
        }
};

class npc_frostwing_vrykul : public CreatureScript
{
    public:
        npc_frostwing_vrykul() : CreatureScript("npc_frostwing_vrykul") { }

        struct npc_frostwing_vrykulAI : public SmartAI
        {
            npc_frostwing_vrykulAI(Creature* creature) : SmartAI(creature)
            {
            }

            bool CanAIAttack(Unit const* target) const
            {
                // do not see targets inside Frostwing Halls when we are not there
                return (me->GetPositionY() > 2660.0f) == (target->GetPositionY() > 2660.0f) && SmartAI::CanAIAttack(target);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_frostwing_vrykulAI(creature);
        }
};

class npc_impaling_spear : public CreatureScript
{
    public:
        npc_impaling_spear() : CreatureScript("npc_impaling_spear") { }

        struct npc_impaling_spearAI : public CreatureAI
        {
            npc_impaling_spearAI(Creature* creature) : CreatureAI(creature)
            {
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                _vehicleCheckTimer = 500;
            }

            void UpdateAI(uint32 const diff)
            {
                if (_vehicleCheckTimer <= diff)
                {
                    _vehicleCheckTimer = 500;
                    if (!me->GetVehicle())
                        me->DespawnOrUnsummon(100);
                }
                else
                    _vehicleCheckTimer -= diff;
            }

            uint32 _vehicleCheckTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_impaling_spearAI(creature);
        }
};

class spell_icc_stoneform : public SpellScriptLoader
{
    public:
        spell_icc_stoneform() : SpellScriptLoader("spell_icc_stoneform") { }

        class spell_icc_stoneform_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_icc_stoneform_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* target = GetTarget()->ToCreature())
                {
                    target->SetReactState(REACT_PASSIVE);
                    target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    target->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_CUSTOM_SPELL_02);
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* target = GetTarget()->ToCreature())
                {
                    target->SetReactState(REACT_AGGRESSIVE);
                    target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    target->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_icc_stoneform_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_icc_stoneform_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_icc_stoneform_AuraScript();
        }
};

class spell_icc_sprit_alarm : public SpellScriptLoader
{
    public:
        spell_icc_sprit_alarm() : SpellScriptLoader("spell_icc_sprit_alarm") { }

        class spell_icc_sprit_alarm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_icc_sprit_alarm_SpellScript);

            void HandleEvent(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                uint32 trapId = 0;
                switch (GetSpellInfo()->Effects[effIndex].MiscValue)
                {
                    case EVENT_AWAKEN_WARD_1:
                        trapId = GO_SPIRIT_ALARM_1;
                        break;
                    case EVENT_AWAKEN_WARD_2:
                        trapId = GO_SPIRIT_ALARM_2;
                        break;
                    case EVENT_AWAKEN_WARD_3:
                        trapId = GO_SPIRIT_ALARM_3;
                        break;
                    case EVENT_AWAKEN_WARD_4:
                        trapId = GO_SPIRIT_ALARM_4;
                        break;
                    default:
                        return;
                }

                if (GameObject* trap = GetCaster()->FindNearestGameObject(trapId, 5.0f))
                    trap->SetRespawnTime(trap->GetGOInfo()->trap.autoCloseTime);

                std::list<Creature*> wards;
                GetCaster()->GetCreatureListWithEntryInGrid(wards, NPC_DEATHBOUND_WARD, 150.0f);
                wards.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
                for (std::list<Creature*>::iterator itr = wards.begin(); itr != wards.end(); ++itr)
                {
                    if ((*itr)->isAlive() && (*itr)->HasAura(SPELL_STONEFORM))
                    {
                        (*itr)->AI()->Talk(SAY_TRAP_ACTIVATE);
                        (*itr)->RemoveAurasDueToSpell(SPELL_STONEFORM);
                        if (Unit* target = (*itr)->SelectNearestTarget(100.0f))
                            (*itr)->AI()->AttackStart(target);
                        break;
                    }
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_icc_sprit_alarm_SpellScript::HandleEvent, EFFECT_2, SPELL_EFFECT_SEND_EVENT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_icc_sprit_alarm_SpellScript();
        }
};

class DeathPlagueTargetSelector
{
    public:
        explicit DeathPlagueTargetSelector(Unit* caster) : _caster(caster) {}

        bool operator()(Unit* unit)
        {
            if (unit == _caster)
                return true;

            if (unit->GetTypeId() != TYPEID_PLAYER)
                return true;

            if (unit->HasAura(SPELL_RECENTLY_INFECTED) || unit->HasAura(SPELL_DEATH_PLAGUE_AURA))
                return true;

            return false;
        }

    private:
        Unit* _caster;
};

class spell_frost_giant_death_plague : public SpellScriptLoader
{
    public:
        spell_frost_giant_death_plague() : SpellScriptLoader("spell_frost_giant_death_plague") { }

        class spell_frost_giant_death_plague_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_frost_giant_death_plague_SpellScript);

            bool Load()
            {
                _failed = false;
                return true;
            }

            // First effect
            void CountTargets(std::list<Unit*>& unitList)
            {
                unitList.remove(GetCaster());
                _failed = unitList.empty();
            }

            // Second effect
            void FilterTargets(std::list<Unit*>& unitList)
            {
                // Select valid targets for jump
                unitList.remove_if (DeathPlagueTargetSelector(GetCaster()));
                if (!unitList.empty())
                {
                    Unit* target = Trinity::Containers::SelectRandomContainerElement(unitList);
                    unitList.clear();
                    unitList.push_back(target);
                }

                unitList.push_back(GetCaster());
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (GetHitUnit() != GetCaster())
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_DEATH_PLAGUE_AURA, true);
                else if (_failed)
                    GetCaster()->CastSpell(GetCaster(), SPELL_DEATH_PLAGUE_KILL, true);
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_frost_giant_death_plague_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_frost_giant_death_plague_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ALLY);
                OnEffectHitTarget += SpellEffectFn(spell_frost_giant_death_plague_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }

            bool _failed;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_frost_giant_death_plague_SpellScript();
        }
};

class spell_icc_harvest_blight_specimen : public SpellScriptLoader
{
    public:
        spell_icc_harvest_blight_specimen() : SpellScriptLoader("spell_icc_harvest_blight_specimen") { }

        class spell_icc_harvest_blight_specimen_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_icc_harvest_blight_specimen_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetHitUnit()->RemoveAurasDueToSpell(uint32(GetEffectValue()));
            }

            void HandleQuestComplete(SpellEffIndex /*effIndex*/)
            {
                GetHitUnit()->RemoveAurasDueToSpell(uint32(GetEffectValue()));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_icc_harvest_blight_specimen_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                OnEffectHitTarget += SpellEffectFn(spell_icc_harvest_blight_specimen_SpellScript::HandleQuestComplete, EFFECT_1, SPELL_EFFECT_QUEST_COMPLETE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_icc_harvest_blight_specimen_SpellScript();
        }
};

class AliveCheck
{
    public:
        bool operator()(Unit* unit)
        {
            return unit->isAlive();
        }
};

class TeleportToFrozenThrone : public BasicEvent
{
    public:
        TeleportToFrozenThrone(Player *player, uint8 attempts): pPlayer(player), attemptsLeft(attempts) { }

        bool Execute(uint64 /*eventTime*/, uint32 /*updateTime*/)
        {
            pPlayer->CastSpell(pPlayer, FROZEN_THRONE_TELEPORT, true);
            if (--attemptsLeft)
                pPlayer->m_Events.AddEvent(new TeleportToFrozenThrone(pPlayer, attemptsLeft), pPlayer->m_Events.CalculateTime(uint64(1500)));
            return true;
        }
    private:
        Player *pPlayer;
        uint8 attemptsLeft;
};

void TeleportPlayerToFrozenThrone(Player *player)
{
    player->m_Events.AddEvent(new TeleportToFrozenThrone(player, 3), player->m_Events.CalculateTime(uint64(5000)));
}

class spell_svalna_revive_champion : public SpellScriptLoader
{
    public:
        spell_svalna_revive_champion() : SpellScriptLoader("spell_svalna_revive_champion") { }

        class spell_svalna_revive_champion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_svalna_revive_champion_SpellScript);

            void RemoveAliveTarget(std::list<Unit*>& unitList)
            {
                unitList.remove_if(AliveCheck());
                Trinity::Containers::RandomResizeList(unitList, 2);
            }

            void Land(SpellEffIndex /*effIndex*/)
            {
                Creature* caster = GetCaster()->ToCreature();
                if (!caster)
                    return;

                Position pos;
                caster->GetPosition(&pos);
                caster->GetNearPosition(pos, 5.0f, 0.0f);
                pos.m_positionZ = caster->GetBaseMap()->GetHeight(caster->GetPhaseMask(), pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), true, 20.0f);
                pos.m_positionZ += 0.05f;
                caster->SetHomePosition(pos);
                caster->GetMotionMaster()->MovePoint(POINT_LAND, pos);
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_svalna_revive_champion_SpellScript::RemoveAliveTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
                OnEffectHit += SpellEffectFn(spell_svalna_revive_champion_SpellScript::Land, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_svalna_revive_champion_SpellScript();
        }
};

class spell_svalna_remove_spear : public SpellScriptLoader
{
    public:
        spell_svalna_remove_spear() : SpellScriptLoader("spell_svalna_remove_spear") { }

        class spell_svalna_remove_spear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_svalna_remove_spear_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Creature* target = GetHitCreature())
                {
                    if (Unit* vehicle = target->GetVehicleBase())
                        vehicle->RemoveAurasDueToSpell(SPELL_IMPALING_SPEAR);
                    target->DespawnOrUnsummon(1);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_svalna_remove_spear_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_svalna_remove_spear_SpellScript();
        }
};

class at_icc_saurfang_portal : public AreaTriggerScript
{
    public:
        at_icc_saurfang_portal() : AreaTriggerScript("at_icc_saurfang_portal") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            InstanceScript* instance = player->GetInstanceScript();
            if (!instance || instance->GetBossState(DATA_DEATHBRINGER_SAURFANG) != DONE)
                return true;

            player->TeleportTo(631, 4126.35f, 2769.23f, 350.963f, 0.0f);

            if (instance->GetData(DATA_COLDFLAME_JETS) == NOT_STARTED)
            {
                // Process relocation now, to preload the grid and initialize traps
                player->GetMap()->PlayerRelocation(player, 4126.35f, 2769.23f, 350.963f, 0.0f);

                instance->SetData(DATA_COLDFLAME_JETS, IN_PROGRESS);
                std::list<Creature*> traps;
                GetCreatureListWithEntryInGrid(traps, player, NPC_FROST_FREEZE_TRAP, 120.0f);
                traps.sort(Trinity::ObjectDistanceOrderPred(player));
                bool instant = false;
                for (std::list<Creature*>::iterator itr = traps.begin(); itr != traps.end(); ++itr)
                {
                    (*itr)->AI()->DoAction(instant ? 1000 : 11000);
                    instant = !instant;
                }
            }

            return true;
        }
};

class at_icc_shutdown_traps : public AreaTriggerScript
{
    public:
        at_icc_shutdown_traps() : AreaTriggerScript("at_icc_shutdown_traps") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                instance->SetData(DATA_COLDFLAME_JETS, DONE);
            return true;
        }
};

class at_icc_start_blood_quickening : public AreaTriggerScript
{
    public:
        at_icc_start_blood_quickening() : AreaTriggerScript("at_icc_start_blood_quickening") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_BLOOD_QUICKENING_STATE) == NOT_STARTED)
                    instance->SetData(DATA_BLOOD_QUICKENING_STATE, IN_PROGRESS);
            return true;
        }
};

class at_icc_start_frostwing_gauntlet : public AreaTriggerScript
{
    public:
        at_icc_start_frostwing_gauntlet() : AreaTriggerScript("at_icc_start_frostwing_gauntlet") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* crok = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_CROK_SCOURGEBANE)))
                    crok->AI()->DoAction(ACTION_START_GAUNTLET);
            return true;
        }
};

/*######
## npc_thrall_or_wyrnn
######*/

#define GOSSIP_1   "With all due respect, your Highness, we can do this alone."
#define GOSSIP_2   "We do wish for you to stand down, your Highness."

class npc_hellscream_or_wyrnn_icc : public CreatureScript
{
public:
    npc_hellscream_or_wyrnn_icc() : CreatureScript("npc_hellscream_or_wyrnn_icc") {}

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->HasAura(AURA_ZONE_PCT_BUFF_A_10) || player->HasAura(AURA_ZONE_PCT_BUFF_H_10))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        }
        else
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        InstanceScript* instance = (InstanceScript*)creature->GetInstanceScript();

        player->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
                instance->SetData(DATA_ZONE_BUFF_STATUS, 1);
                instance->DoRemoveAurasDueToSpellOnPlayers(AURA_ZONE_PCT_BUFF_A_10);
                instance->DoRemoveAurasDueToSpellOnPlayers(AURA_ZONE_PCT_BUFF_H_10);
                break;
        }
        return true;
    }
};

class at_zone_buff_checker : public AreaTriggerScript
{
    public:
        at_zone_buff_checker() : AreaTriggerScript("at_zone_buff_checker") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_ZONE_BUFF_STATUS) == 1)
                    if (player->HasAura(AURA_ZONE_PCT_BUFF_A_10) || player->HasAura(AURA_ZONE_PCT_BUFF_H_10))
                    {
                        instance->DoRemoveAurasDueToSpellOnPlayers(AURA_ZONE_PCT_BUFF_A_10);
                        instance->DoRemoveAurasDueToSpellOnPlayers(AURA_ZONE_PCT_BUFF_H_10);
                    }
            return true;
        }
};

struct Locations
{
      float x, y, z, o;
      uint32 id;
};

static Locations SpawnLoc[]=
{
    {4137.891f, 2506.230f, 266.843f, 5.72f},
    {4160.168f, 2527.692f, 266.843f, 5.35f},
    {4201.778f, 2529.071f, 266.843f, 3.98f},
    {4225.387f, 2506.195f, 266.843f, 3.74f},
    {4226.427f, 2461.591f, 266.843f, 2.53f},
    {4202.411f, 2439.933f, 266.843f, 2.29f},
    {4158.904f, 2439.020f, 266.843f, 0.86f},
    {4137.398f, 2461.736f, 266.843f, 0.66f},

    {4158.792f, 2484.192f, 211.032f, 6.06f},
    {4208.695f, 2484.293f, 211.032f, 3.12f},
    {4180.823f, 2484.562f, 211.032f, 2.96f}
};

enum eSays
{
    SAY_AGGRO                   = -1999955,
};

enum eSummons
{
    SPIDER_BIG                  = 37501,
    SPIDER_SMALL                = 37232,

    DUDE_MAN                    = 37228,
    DUDE_WOMAN                  = 37229
};

class sindragosas_ward : public CreatureScript
{
    public:
        sindragosas_ward() : CreatureScript("sindragosas_ward") { }

        struct sindragosas_wardAI : public ScriptedAI
        {
            sindragosas_wardAI(Creature* creature) : ScriptedAI(creature), summons(creature)
            {
                instance = me->GetInstanceScript();
                is_started = false;
                is_done = false;
                me->SetVisible(false);
            }

            void Reset()
            {
                is_started = false;
                m_uiSummonBigSpidersTimer = 0;
                m_uiSummonSmallSpidersTimer = 10000;
                m_uiSummonDudesTimer = 50000;
                m_uiWipeCheckTimer = 0;
                big_spider_wave_count = 0;
                small_spider_wave_count = 0;
                dudes_wave_count = 0;
                me->SetVisible(false);
                summons.DespawnAll();
            }

            void SummonBigSpiders()
            {
                m_uiLocNo = 0;

                for(uint8 i = 0; i < 8; i++)
                {
                    if(Creature* Summon = me->SummonCreature(SPIDER_BIG, SpawnLoc[m_uiLocNo].x, SpawnLoc[m_uiLocNo].y, SpawnLoc[m_uiLocNo].z, SpawnLoc[m_uiLocNo].o, TEMPSUMMON_CORPSE_DESPAWN))
                    {
                        Summon->SetInCombatWithZone();
                    }
                    m_uiLocNo++;
                }
            }

            void SummonSmallSpiders()
            {
                m_uiLocNo = 0;

                for(uint8 i = 0; i < 8; i++)
                {
                    if(Creature* Summon = me->SummonCreature(SPIDER_SMALL, SpawnLoc[m_uiLocNo].x, SpawnLoc[m_uiLocNo].y, SpawnLoc[m_uiLocNo].z, SpawnLoc[m_uiLocNo].o, TEMPSUMMON_CORPSE_DESPAWN))
                    {
                        Summon->SetInCombatWithZone();
                    }
                    m_uiLocNo++;
                }
            }

            void SummonDudes()
            {
                m_uiLocNo = 8;

                for(uint8 i = 0; i < 3; i++)
                {
                    switch(summon = urand(1,2))
                    {
                        case 1: summon = DUDE_MAN; break;
                        case 2: summon = DUDE_WOMAN; break;
                    }

                    if(Creature* Summon = me->SummonCreature(summon, SpawnLoc[m_uiLocNo].x, SpawnLoc[m_uiLocNo].y, SpawnLoc[m_uiLocNo].z, SpawnLoc[m_uiLocNo].o, TEMPSUMMON_CORPSE_DESPAWN))
                    {
                        Summon->SetInCombatWithZone();
                    }
                    m_uiLocNo++;
                }
            }

            void JustSummoned(Creature* summon)
            {
                if ((summon->GetEntry() == SPIDER_BIG) || (summon->GetEntry() == SPIDER_SMALL))
                {
                    summon->GetMotionMaster()->MoveFall();
                    summon->CastSpell(summon, 69887, true);
                }

                summons.Summon(summon);
            }

            void WipeCheck()
            {
                    Player* player = NULL;
                    Trinity::AnyPlayerInObjectRangeCheck check(me, 70.0f);
                    Trinity::PlayerSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, player, check);
                    me->VisitNearbyWorldObject(70.0f, searcher);

                    if(!player)
                        Reset();
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!who || who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (!is_started && me->IsWithinDist(who, 50.0f))
                {
                    DoScriptText(SAY_AGGRO, me);
                    instance->HandleGameObject(instance->GetData64(GO_SINDRAGOSA_ENTRANCE_DOOR), false);
                    is_started = true;
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (is_started)
                {
                    if (m_uiSummonBigSpidersTimer <= uiDiff && big_spider_wave_count < 3)
                    {
                        ++big_spider_wave_count;
                        SummonBigSpiders();
                        m_uiSummonBigSpidersTimer = 40000;
                    }
                    else m_uiSummonBigSpidersTimer -= uiDiff;

                    if (m_uiSummonSmallSpidersTimer <= uiDiff && small_spider_wave_count < 6)
                    {
                        ++small_spider_wave_count;
                        SummonSmallSpiders();
                        m_uiSummonSmallSpidersTimer = 20000;
                    }
                    else m_uiSummonSmallSpidersTimer -= uiDiff;

                    if (m_uiSummonDudesTimer <= uiDiff && dudes_wave_count < 2)
                    {
                        ++dudes_wave_count;
                        SummonDudes();
                        m_uiSummonDudesTimer = 45000;
                    }
                    else m_uiSummonDudesTimer -= uiDiff;

                    if (m_uiWipeCheckTimer <= uiDiff && !is_done)
                    {
                        WipeCheck();
                        m_uiWipeCheckTimer = 15000;
                    }
                    else m_uiWipeCheckTimer -= uiDiff;

                    if (big_spider_wave_count >=3 /* <-���������� ����� ��������� xD */&& small_spider_wave_count >=6 && dudes_wave_count >= 2)
                    {
                        instance->HandleGameObject(instance->GetData64(GO_SINDRAGOSA_ENTRANCE_DOOR), true);
                        instance->SetData(DATA_EVENT_BEFORE_SINDRAGOSA, DONE);
                        is_done = true;
                    }
                }
            }

        private:
            InstanceScript* instance;
            SummonList summons;
            bool is_started;
            bool is_done;
            uint8 big_spider_wave_count;
            uint8 small_spider_wave_count;
            uint8 dudes_wave_count;
            uint32 m_uiLocNo;
            uint32 m_uiSummonBigSpidersTimer;
            uint32 m_uiSummonSmallSpidersTimer;
            uint32 m_uiSummonDudesTimer;
            uint32 m_uiWipeCheckTimer;
            uint64 summon;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new sindragosas_wardAI(creature);
        }
};

void AddSC_icecrown_citadel()
{
    new npc_highlord_tirion_fordring_lh();
    new npc_rotting_frost_giant();
    new npc_frost_freeze_trap();
    new npc_alchemist_adrianna();
    new boss_sister_svalna();
    new npc_crok_scourgebane();
    new npc_captain_arnath();
    new npc_captain_brandon();
    new npc_captain_grondel();
    new npc_captain_rupert();
    new npc_frostwing_vrykul();
    new npc_impaling_spear();
    new npc_hellscream_or_wyrnn_icc();
    new sindragosas_ward();
    new spell_icc_stoneform();
    new spell_icc_sprit_alarm();
    new spell_frost_giant_death_plague();
    new spell_icc_harvest_blight_specimen();
    new spell_trigger_spell_from_caster("spell_svalna_caress_of_death", SPELL_IMPALING_SPEAR_KILL);
    new spell_svalna_revive_champion();
    new spell_svalna_remove_spear();
    new at_icc_saurfang_portal();
    new at_icc_shutdown_traps();
    new at_icc_start_blood_quickening();
    new at_icc_start_frostwing_gauntlet();
    new at_zone_buff_checker();
}
