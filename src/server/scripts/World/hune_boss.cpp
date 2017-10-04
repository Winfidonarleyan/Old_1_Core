#include "ScriptPCH.h"
#include "LFGMgr.h"

/*######
## npc_frostlord_ahune
######*/
#define EMOTE_SUBMERGE             "Ahune Retreats. His Defenses Diminish."
#define EMOTE_EMERGE_SOON          "Ahune will soon resurface."
#define GOSSIP_STONE_ITEM          "Disturb the stone and summon Lord Ahune."

enum Spells
{
    // Ahune
    SPELL_SLIPPERY_FLOOR_AMBIENT  = 46314,
    SPELL_SUMMON_ICE_SPEAR_BUNNY  = 46359,
    SPELL_SUMMON_ICE_SPEAR_OBJECT = 46369,
    SPELL_SELF_STUN               = 46416,
    SPELL_RESURFACE               = 46402,
    SPELL_SHIELD                  = 45954,
    SPELL_BEAM_ATTACK             = 46336,
    SPELL_BEAM_ATTACK_BEAM        = 46363,
    SPELL_SUBMERGE                = 37550,
    SPELL_EMERGE                  = 50142,
    SPELL_SNOWY                   = 46423,
    // Ahunite Hailstone
    SPELL_CHILLING_AURA           = 46542,
    SPELL_PULVERIZE               =  2676,
    // Ahunite Frostwind
    SPELL_WIND_BUFFET             = 46568,
    SPELL_LIGHTNING_SHIELD        = 12550,
    // Ahunite Coldwave
    SPELL_BITTER_BLAST            = 46406,
    // Ice Spear
    SPELL_ICE_SPEAR_KNOCKBACK     = 46360,
    // Slippery Floor
    SPELL_SLIP                    = 45947, // stun
    SPELL_YOU_SLIPPED             = 45946, // decrease speed
    SPELL_SLIPPERY_FLOOR          = 45945  // periodic, channeled
};

enum Npcs
{
    NPC_FROSTLORD_AHUNE           = 25740,
    NPC_FROZEN_CORE               = 25865,
    NPC_AHUNITE_HAILSTONE         = 25755,
    NPC_AHUNITE_COLDWAVE          = 25756,
    NPC_AHUNITE_FROSTWIND         = 25757,
    NPC_SLIPPERY_FLOOR            = 25952,
    NPC_EARTHEN_RING_TOTEM        = 25961,
    NPC_ICE_SPEAR_BUNNY           = 25985
};

enum Objects
{
    GO_ICE_SPEAR                  = 188077,
    GO_ICE_STONE                  = 187882,
    GO_ICE_CHEST                  = 187892
};

enum Actions
{
    ACTION_START_EVENT
};

enum Events
{
    EVENT_EMERGE = 1,
    EVENT_EMERGE_SOON,
    EVENT_SUBMERGED,
    EVENT_EARTHEN_RING_ATTACK,
    EVENT_SUMMON_HAILSTONE,
    EVENT_SUMMON_COLDWAVE,
    EVENT_ICE_SPEAR
};

class npc_frostlord_ahune : public CreatureScript
{
    public:
        npc_frostlord_ahune() : CreatureScript("npc_frostlord_ahune") { }

        struct npc_frostlord_ahuneAI : public Scripted_NoMovementAI
        {
            npc_frostlord_ahuneAI(Creature* c) : Scripted_NoMovementAI(c), _summons(me) { }

            void Reset()
            {
                _summons.DespawnAll();
                _events.Reset();
                _submerged = false;

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
                me->SetCorpseDelay(20);
				me->SetVisible(false);
                DoCast(me, SPELL_SHIELD, true);
                DoCast(me, SPELL_SUBMERGE, true);

                me->SummonGameObject(GO_ICE_STONE, -65.65f, -159.765f, -2.25f, 0, 0, 0, 0, 0, 0);
            }

            void JustSummoned(Creature* summon)
            {
                switch (summon->GetEntry())
                {
                    case NPC_FROZEN_CORE:
                        summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                        summon->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true); 
                        summon->SetHealth(me->GetHealth());
                        summon->SetReactState(REACT_PASSIVE);
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        break;
                    case NPC_SLIPPERY_FLOOR:
                        summon->SetReactState(REACT_PASSIVE);
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                        summon->SetDisplayId(11686);
                        summon->setFaction(14);
                        me->AddAura(SPELL_SLIPPERY_FLOOR_AMBIENT, summon);
                        break;
                    case NPC_ICE_SPEAR_BUNNY:
                        summon->SetInCombatWithZone();
                        return;
                }

                _summons.Summon(summon);
                summon->SetInCombatWithZone();
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                if (me->isDead())
                    return;

                if (summon->GetEntry() == NPC_FROZEN_CORE)
                    me->SetHealth(summon->GetHealth());
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
            {
                if (summon->GetEntry() == NPC_FROZEN_CORE)
                    me->DealDamage(me, me->GetHealth());
            }

            void JustDied(Unit* /*who*/)
            {
				me->SetVisible(true);
                me->SummonGameObject(GO_ICE_CHEST, -97.095f, -203.866f, -1.191f, 1.5f, 0, 0, 0, 0, 0);
                _summons.DespawnAll();

                // lfg reward as there is no ahune entry in dbcs dungeonencounters
                // TODO: unhack
                Map* map = me->GetMap();
                if (map && map->IsDungeon())
                {
                    Map::PlayerList const& players = map->GetPlayers();
                    if (!players.isEmpty())
                        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                            if (Player* player = i->getSource())
                                if (player->GetDistance(me) < 120.0f)
                                    sLFGMgr->RewardDungeonDoneFor(286, player);
                }
            }

            void DoAction(int32 const action)
            {
				me->SetVisible(true);
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetInCombatWithZone();
                _events.ScheduleEvent(EVENT_EMERGE, 10000);

                if (GameObject* chest = me->FindNearestGameObject(GO_ICE_CHEST, 100.0f))
                    chest->Delete();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_EMERGE:
                            me->SummonCreature(NPC_SLIPPERY_FLOOR, *me, TEMPSUMMON_TIMED_DESPAWN, 90000);
                            me->RemoveAurasDueToSpell(SPELL_SELF_STUN);
                            me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            DoCast(me, SPELL_RESURFACE, true);
                            DoCast(SPELL_EMERGE);
                            _attacks = 0;
                            _events.ScheduleEvent(EVENT_EARTHEN_RING_ATTACK, 10000);
                            _events.ScheduleEvent(EVENT_SUMMON_HAILSTONE, 2000);
                            _events.ScheduleEvent(EVENT_SUMMON_COLDWAVE, 5000);
                            _events.ScheduleEvent(EVENT_ICE_SPEAR, 10000);
                            break;
                        case EVENT_EMERGE_SOON:
                            me->MonsterTextEmote(EMOTE_EMERGE_SOON, 0, true);
                            break;
                        case EVENT_SUBMERGED:
                            DoCast(me, SPELL_SELF_STUN, true);
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            me->SummonCreature(NPC_FROZEN_CORE, *me, TEMPSUMMON_TIMED_DESPAWN, 30000);
                            _submerged = true;
                            _events.ScheduleEvent(EVENT_EMERGE_SOON, 25000);
                            _events.ScheduleEvent(EVENT_EMERGE, 30000);
                            break;
                        case EVENT_EARTHEN_RING_ATTACK:
                            ++_attacks;
                            if (_attacks >= 9)
                            {
                                me->MonsterTextEmote(EMOTE_SUBMERGE, 0, true);
                                DoCast(SPELL_SUBMERGE);
                                _events.Reset();
                                _events.ScheduleEvent(EVENT_SUBMERGED, 4500);
                            }
                            else
                            {
                                if (Creature* totem = me->FindNearestCreature(NPC_EARTHEN_RING_TOTEM, 150.0f))
                                    totem->CastSpell(me, SPELL_BEAM_ATTACK_BEAM, false);
                                _events.ScheduleEvent(EVENT_EARTHEN_RING_ATTACK, 10000);
                            }
                            break;
                        case EVENT_SUMMON_HAILSTONE:
                            me->SummonCreature(NPC_AHUNITE_HAILSTONE, float(irand(-110, -80)), float(irand(-225, -215)), -1.0f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                            break;
                        case EVENT_SUMMON_COLDWAVE:
                            for (uint8 i = 0; i < 2; ++i)
                                me->SummonCreature(NPC_AHUNITE_COLDWAVE, float(irand(-110, -80)), float(irand(-225, -215)), -1.0f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                            if (_submerged)
                                me->SummonCreature(NPC_AHUNITE_FROSTWIND, float(irand(-110, -80)), float(irand(-225, -215)), -1.0f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                            _events.ScheduleEvent(EVENT_SUMMON_COLDWAVE, 7000);
                            break;
                        case EVENT_ICE_SPEAR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                                DoCast(target, SPELL_SUMMON_ICE_SPEAR_BUNNY);
                            _events.ScheduleEvent(EVENT_ICE_SPEAR, 7000);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            SummonList _summons;
            EventMap _events;
            bool _submerged;
            uint8 _attacks;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_frostlord_ahuneAI(creature);
        }
};

class npc_ahune_ice_spear : public CreatureScript
{
    public:
        npc_ahune_ice_spear() : CreatureScript("npc_ahune_ice_spear") { }

        struct npc_ahune_ice_spearAI : public Scripted_NoMovementAI
        {
            npc_ahune_ice_spearAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                _spikeTimer = 2500;
                _spiked = false;
                DoCast(me, SPELL_SUMMON_ICE_SPEAR_OBJECT, true);
            }

            void UpdateAI(uint32 const diff)
            {
                if (_spikeTimer <= diff)
                {
                    GameObject* spike = me->FindNearestGameObject(GO_ICE_SPEAR, 10.0f);
                    if (spike && !_spiked)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST, 0, 3.0f, true))
                            target->CastSpell(target, SPELL_ICE_SPEAR_KNOCKBACK, true);
                        spike->UseDoorOrButton();
                        _spiked = true;
                        _spikeTimer = 3500;
                    }
                    else if (spike)
                    {
                        spike->Delete();
                        me->DespawnOrUnsummon();
                    }
                }
                else
                    _spikeTimer -= diff;
            }

        private:
            uint32 _spikeTimer;
            bool _spiked;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ahune_ice_spearAI(creature);
        }
};

class go_ahune_ice_stone : public GameObjectScript
{
    public:
        go_ahune_ice_stone() : GameObjectScript("go_ahune_ice_stone") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STONE_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* go, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF)
            {
                if (Creature* ahune = go->FindNearestCreature(NPC_FROSTLORD_AHUNE, 100.0f, true))
                {
                    ahune->AI()->DoAction(ACTION_START_EVENT);
                    go->Delete();
                }
            }
            return true;
        }
};
 
/*######
## An Innocent Disguise
######*/
#define QUEST_INNCOENT_DISGUISE         11891
#define SPELL_DISGUISE                  46337

#define SAY_DIALOG_1            "These stones should be the last of them. Our coordination with Neptulon's forces will be impeccable." //Briatha
#define SAY_DIALOG_2            "Yess. The Tidehunter will be pleased at this development. The Firelord's hold will weaken." //Emissary
#define SAY_DIALOG_3            "And your own preparations? Will the Frost Lord have a path to the portal?" //Briatha
#define SAY_DIALOG_4            "Skar'this has informed us well. We have worked our way into the slave pens and await your cryomancerss." //Emissary
#define SAY_DIALOG_5            "The ritual in Coilfang will bring Ahune through once he is fully prepared, and the resulting clash between Firelord and Frostlord will rend the foundations of this world. Our ultimate goals are in reach at last...." //Briatha

class npc_ice_caller_briatha : public CreatureScript
{
public:
	npc_ice_caller_briatha() : CreatureScript("npc_ice_caller_briatha") { }

	CreatureAI* GetAI(Creature *_Creature) const
	{
	    return new npc_ice_caller_briathaAI (_Creature);
	}

struct npc_ice_caller_briathaAI : public ScriptedAI
{
    npc_ice_caller_briathaAI(Creature *c) : ScriptedAI(c)
    {
        Reset();
    }

    uint64 EventStarter;
    bool EventStarted;
    uint32 part;

    uint32 part_timer;

    void Reset()
    {
        EventStarter = 0;
        EventStarted = false;
    }

    void MoveInLineOfSight(Unit *who)
    {
            if(!EventStarted && who->GetTypeId() == TYPEID_PLAYER)
                if(me->IsWithinDistInMap(who, 10) && who->HasAura(SPELL_DISGUISE) && ((Player*)who)->GetQuestStatus(QUEST_INNCOENT_DISGUISE) == QUEST_STATUS_INCOMPLETE )
                    StartEvent(((Player*)who));
    }

    void StartEvent(Player* starter)
    {
        EventStarted = true;
        part = 1;
        EventStarter = starter->GetGUID();

        part_timer = 0;
    }

    void SayPart(uint32 part)
    {
        switch(part)
        {
        case 1:
            me->MonsterSay(SAY_DIALOG_1,LANG_UNIVERSAL,EventStarter);
            part_timer = 5000;
            break;
        case 2:
            if(Creature* emmissary = GetClosestCreatureWithEntry(me,25951,30))
            {
                emmissary->MonsterSay(SAY_DIALOG_2,LANG_UNIVERSAL, 0);
            }
            part_timer = 5000;
            break;
        case 3:
            me->MonsterSay(SAY_DIALOG_3,LANG_UNIVERSAL, 0);
            part_timer = 5000;
            break;
        case 4:
            if(Creature* emmissary = GetClosestCreatureWithEntry(me,25951,30))
            {
                emmissary->MonsterSay(SAY_DIALOG_4,LANG_UNIVERSAL, 0);
            }
            part_timer = 5000;
            break;
        case 5:
            me->MonsterSay(SAY_DIALOG_5,LANG_UNIVERSAL, 0);
            part_timer = 10000;
            break;
        case 6:
            if(Player* player =  Unit::GetPlayer(*me,EventStarter))
            {
                player->AreaExploredOrEventHappens(QUEST_INNCOENT_DISGUISE);
            }
            part_timer = 500;
            break;
        }
    }

    void EnterCombat(Unit *who) { }
    void UpdateAI(const uint32 diff)
    {
        if(EventStarted)
        {
            if(part_timer <= diff)
            {
                SayPart(part);
                part++;

                if( part > 6)
                {
                    Reset();
                    return;
                }
            }else part_timer -= diff;
        }

        if (!UpdateVictim() )
            return;

        DoMeleeAttackIfReady();
    }
};
};

/*######
## npc_torch_tossing_bunny
######*/

enum
{
    SPELL_TORCH_TOSSING_COMPLETE_A = 45719,
    SPELL_TORCH_TOSSING_COMPLETE_H = 46651,
    SPELL_TORCH_TOSSING_TRAINING   = 45716,
    SPELL_TORCH_TOSSING_PRACTICE   = 46630,
    SPELL_TORCH_TOSS               = 46054,
    SPELL_TARGET_INDICATOR         = 45723,
    SPELL_BRAZIERS_HIT             = 45724
};

class npc_torch_tossing_bunny : public CreatureScript
{
    public:
        npc_torch_tossing_bunny() : CreatureScript("npc_torch_tossing_bunny") { }

        struct npc_torch_tossing_bunnyAI : public ScriptedAI
        {
            npc_torch_tossing_bunnyAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                _targetTimer = urand(5000, 20000);
            }

            void SpellHit(Unit* caster, SpellEntry const* spell)
            {
                if (spell->Id == SPELL_TORCH_TOSS && me->HasAura(SPELL_TARGET_INDICATOR))
                {
                    uint8 neededHits;

                    if (caster->HasAura(SPELL_TORCH_TOSSING_TRAINING))
                        neededHits = 8;
                    else if (caster->HasAura(SPELL_TORCH_TOSSING_PRACTICE))
                        neededHits = 20;
                    else
                        return;

                    DoCast(me, SPELL_BRAZIERS_HIT, true);
                    caster->AddAura(SPELL_BRAZIERS_HIT, caster);

                    if (caster->GetAuraCount(SPELL_BRAZIERS_HIT) >= neededHits)
                    {
                        // complete quest
                        caster->CastSpell(caster, SPELL_TORCH_TOSSING_COMPLETE_A, true);
                        caster->CastSpell(caster, SPELL_TORCH_TOSSING_COMPLETE_H, true);
                        caster->RemoveAurasDueToSpell(SPELL_BRAZIERS_HIT);
                        caster->RemoveAurasDueToSpell(neededHits == 8 ? SPELL_TORCH_TOSSING_TRAINING : SPELL_TORCH_TOSSING_PRACTICE);
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (_targetTimer <= diff)
                {
                    DoCast(SPELL_TARGET_INDICATOR);
                    _targetTimer = urand(10000, 20000);
                }
                else
                    _targetTimer -= diff;
            }

        private:
            uint32 _targetTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_torch_tossing_bunnyAI(creature);
        }
};

enum TorchCatchingData
{
    SPELL_FLING_TORCH_MISSILE     = 45669,
    SPELL_TOSS_TORCH_SHADOW       = 46105,
    SPELL_TORCH_TARGET_PICKER     = 45907,
    SPELL_TORCHES_COUGHT          = 45693,
    SPELL_JUGGLE_TORCH_MISSED     = 45676,
    SPELL_TORCH_CATCHING_SUCCESS  = 46081,
    SPELL_TORCH_DAILY_SUCCESS     = 46654,
    NPC_JUGGLE_TARGET             = 25515,
    QUEST_TORCH_CATCHING_A        = 11657,
    QUEST_TORCH_CATCHING_H        = 11923,
    QUEST_MORE_TORCH_CATCHING_A   = 11924,
    QUEST_MORE_TORCH_CATCHING_H   = 11925
};

class spell_gen_torch_target_picker : public SpellScriptLoader
{
    public:
        spell_gen_torch_target_picker() : SpellScriptLoader("spell_gen_torch_target_picker") {}

        class spell_gen_torch_target_picker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_torch_target_picker_SpellScript);

            bool Validate(SpellEntry const* /*spellEntry*/)
            {
                if (!sSpellStore.LookupEntry(SPELL_FLING_TORCH_MISSILE))
                    return false;
                if (!sSpellStore.LookupEntry(SPELL_TOSS_TORCH_SHADOW))
                    return false;
                return true;
            }

            void FilterTargets(std::list<Unit*>& unitList)
            {
                Unit* caster = GetCaster();

                if (!caster)
                    return;

                std::list<Creature*> juggleList;
                caster->GetCreatureListWithEntryInGrid(juggleList, NPC_JUGGLE_TARGET, 10.0f);

                if (!juggleList.empty())
                    for (std::list<Creature*>::iterator iter = juggleList.begin(); iter != juggleList.end(); ++iter)
                        unitList.remove(*iter);

                if (unitList.empty())
                    return;

                std::list<Unit*>::iterator itr = unitList.begin();
                std::advance(itr, urand(0, unitList.size() - 1));

                Unit* target = *itr;
                unitList.clear();
                unitList.push_back(target);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                caster->CastSpell(target, SPELL_FLING_TORCH_MISSILE, true);
                caster->CastSpell(target, SPELL_TOSS_TORCH_SHADOW, true);
            }

            void Register()
            {
				OnUnitTargetSelect += SpellUnitTargetFn(spell_gen_torch_target_picker_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
				OnEffectHitTarget += SpellEffectFn(spell_gen_torch_target_picker_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_torch_target_picker_SpellScript();
        }
};

class spell_gen_juggle_torch_catch : public SpellScriptLoader
{
    public:
        spell_gen_juggle_torch_catch() : SpellScriptLoader("spell_gen_juggle_torch_catch") {}

        class spell_gen_juggle_torch_catch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_juggle_torch_catch_SpellScript);

            bool Validate(SpellEntry const* /*spellEntry*/)
            {
                if (!sSpellStore.LookupEntry(SPELL_TORCH_TARGET_PICKER))
                    return false;
                if (!sSpellStore.LookupEntry(SPELL_TORCHES_COUGHT))
                    return false;
                if (!sSpellStore.LookupEntry(SPELL_JUGGLE_TORCH_MISSED))
                    return false;
                return true;
            }

            void FilterTargets(std::list<Unit*>& unitList)
            {
                Unit* caster = GetCaster();
                Unit* juggleTarget = NULL;
                bool missed = true;

                if (unitList.empty() || !caster || !caster->ToPlayer())
                     return;

                for (std::list<Unit*>::iterator iter = unitList.begin(); iter != unitList.end(); ++iter)
                {
                    if (*iter == caster)
                        missed = false;

                    if ((*iter)->ToCreature())
                        juggleTarget = *iter;
                }

                if (missed)
                {
                    if (juggleTarget)
                        juggleTarget->CastSpell(juggleTarget, SPELL_JUGGLE_TORCH_MISSED, true);
                    caster->RemoveAurasDueToSpell(SPELL_TORCHES_COUGHT);
                }
                else
                {
                    uint8 neededCatches;

                    if (caster->ToPlayer()->GetQuestStatus(QUEST_TORCH_CATCHING_A) == QUEST_STATUS_INCOMPLETE
                        || caster->ToPlayer()->GetQuestStatus(QUEST_TORCH_CATCHING_H) == QUEST_STATUS_INCOMPLETE)
                    {
                        neededCatches = 4;
                    }
                    else if (caster->ToPlayer()->GetQuestStatus(QUEST_MORE_TORCH_CATCHING_A) == QUEST_STATUS_INCOMPLETE
                        || caster->ToPlayer()->GetQuestStatus(QUEST_MORE_TORCH_CATCHING_H) == QUEST_STATUS_INCOMPLETE)
                    {
                        neededCatches = 10;
                    }
                    else
                    {
                        caster->RemoveAurasDueToSpell(SPELL_TORCHES_COUGHT);
                        return;
                    }

                    caster->CastSpell(caster, SPELL_TORCH_TARGET_PICKER, true);
                    caster->CastSpell(caster, SPELL_TORCHES_COUGHT, true);

                    // reward quest
                    if (caster->GetAuraCount(SPELL_TORCHES_COUGHT) >= neededCatches)
                    {
                        caster->CastSpell(caster, SPELL_TORCH_CATCHING_SUCCESS, true);
                        caster->CastSpell(caster, SPELL_TORCH_DAILY_SUCCESS, true);
                        caster->RemoveAurasDueToSpell(SPELL_TORCHES_COUGHT);
                    }
                }
            }

            void Register()
            {
				OnUnitTargetSelect += SpellUnitTargetFn(spell_gen_juggle_torch_catch_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_juggle_torch_catch_SpellScript();
        }
};

/*######
## at_nats_landing
######*/
enum
{
    QUEST_NATS_BARGAIN = 11209,
    SPELL_FISH_PASTE   = 42644,
    NPC_LURKING_SHARK  = 23928
};

class at_nats_landing : public AreaTriggerScript
{
    public:
        at_nats_landing() : AreaTriggerScript("at_nats_landing") {}

        bool OnTrigger(Player* player, const AreaTriggerEntry* pAt)
        {
            if (player->GetQuestStatus(QUEST_NATS_BARGAIN) == QUEST_STATUS_INCOMPLETE && player->HasAura(SPELL_FISH_PASTE))
            {
            Creature* pShark = GetClosestCreatureWithEntry(player, NPC_LURKING_SHARK, 20.0f);

            if (!pShark)
                pShark = player->SummonCreature(NPC_LURKING_SHARK, -4246.243f, -3922.356f, -7.488f, 5.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 100000);
                pShark->AI()->AttackStart(player);
                return false;
            }
            return true;
        }
};

/*#####################
# npc_destructive_ward
#######################*/
enum NoPlaceToRun
{
   QUEST_NO_PLACE_TO_RUN = 12261,
   NPC_SMOLDERING_CONSTRUCT = 27362,
   NPC_CHARGED_WARD = 28820
};

class npc_destructive_ward : public CreatureScript
{
public:
   npc_destructive_ward() : CreatureScript("npc_destructive_ward") { }

   CreatureAI* GetAI(Creature* creature) const
   {
       return new npc_destructive_wardAI(creature);
   }

   struct npc_destructive_wardAI : public ScriptedAI
   {
       npc_destructive_wardAI(Creature* c) : ScriptedAI(c) { }

       uint32 uiWaveTimer;
       uint8 killCounter;

       void Reset()
       {
           killCounter = 0;
           uiWaveTimer = 5000;
           me->SetReactState(REACT_PASSIVE);
           me->GetMotionMaster()->MoveIdle();
       }

       void JustDied(Unit* /*killer*/)
       {
           if (Player* player = me->GetOwner()->ToPlayer())
               player->FailQuest(QUEST_NO_PLACE_TO_RUN);
       }

       void UpdateAI(const uint32 diff)
       {
           if(uiWaveTimer <= diff)
           {
               if(Creature* summoned = me->SummonCreature(NPC_SMOLDERING_CONSTRUCT, me->GetPositionX()-rand()%15, me->GetPositionY()-rand()%15, me->GetPositionZ()+3, 0.0f, TEMPSUMMON_CORPSE_DESPAWN,0))
               {
                   summoned->AI()->AttackStart(me);
                   summoned->GetMotionMaster()->Clear();
                   summoned->GetMotionMaster()->MoveChase(me);
               }
               uiWaveTimer = urand(10000, 15000);
           }
           else
               uiWaveTimer -= diff;
       }

       void SummonedCreatureDespawn(Creature* summon)
       {
           if (summon->GetEntry() == NPC_SMOLDERING_CONSTRUCT)
               killCounter++;

           if(killCounter >= 3)
           {
               if (Player* player = me->GetOwner()->ToPlayer())
                   player->KilledMonsterCredit(NPC_CHARGED_WARD,0);

               me->DespawnOrUnsummon(2000);
           }
       }
   };
};

enum evergroveData
{
   SPELL_DRUID_SIGNAL = 38782,
   SPELL_DRUID_FORM   = 39158,
   SPELL_CROW_FORM    = 38776,

   QUEST_DEATH_DOOR   = 10910,
   ITEM_DRUID_SIGNAL  = 31763,
};
class npc_evergrove_druid : public CreatureScript
{
public:
   npc_evergrove_druid() : CreatureScript("npc_evergrove_druid") { }

   struct npc_evergrove_druidAI : public ScriptedAI
   {
       npc_evergrove_druidAI(Creature* creature) : ScriptedAI(creature) { }

       Player* player;
       Position playerPos;
       bool summoned;
       float x,y,z;

       void Reset()
       {
           // Reset all variables
		   me->SetCanFly(true);
           me->SetSpeed(MOVE_FLIGHT, 3.2f, true);
           me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER | UNIT_NPC_FLAG_GOSSIP);
		   me->SetVisible(false);
           player = NULL;
           summoned = false;
       }

       void SpellHit(Unit* caster, SpellEntry const* spell)
       {
           if(summoned)
               return;

           if(spell->Id == SPELL_DRUID_SIGNAL)
           {
               summoned = true;

               if(caster)
                   player = caster->ToPlayer();

               // Prepare visuals
               me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER | UNIT_NPC_FLAG_GOSSIP);
               me->SetTarget(player->GetGUID());
               me->SetVisible(true);
               me->CastSpell(me, SPELL_CROW_FORM, true);

               // Move to player
               if(player)
               {
                   x = player->GetPositionX();
                   y = player->GetPositionY(); z = player->GetPositionZ();
                   me->GetMotionMaster()->MovePoint(0, x, y, z);
               }
           }
       }

       void MovementInform(uint32 type, uint32 id)
       {
           if(!player)
               return;

           me->UpdatePosition(x, y, z, 0.0f, true);
           if(!player->IsFlying())
           {
               me->CastSpell(me, SPELL_DRUID_FORM, true);
               me->RemoveAurasDueToSpell(SPELL_CROW_FORM);
			   me->SetCanFly(false);
               me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
           }
           me->DespawnOrUnsummon(60000);
       }

   };

   CreatureAI* GetAI(Creature* creature) const
   {
       return new npc_evergrove_druidAI(creature);
   }
};

#define GOSSIP_ITEM_DRUID_SIGNAL "I lost my Druid Seal."

class npc_antelarion_gossip : public CreatureScript
{
public:
   npc_antelarion_gossip() : CreatureScript("npc_antelarion_gossip") { }

   bool OnGossipHello(Player* player, Creature* creature)
   {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());
       if ((player->GetQuestStatus(QUEST_DEATH_DOOR) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_DEATH_DOOR) == QUEST_STATUS_REWARDED) && !player->HasItemCount(ITEM_DRUID_SIGNAL, 1))
           player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DRUID_SIGNAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
       player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

       return true;
   }

   bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
   {
       player->CLOSE_GOSSIP_MENU();
       player->AddItem(ITEM_DRUID_SIGNAL, 1);
       return true;
   }
};

enum keristraszaData
{
   QUEST_SPRINGING_TRAP = 11969,

   NPC_KERISTRASZA = 26237,
   NPC_SARAGOSA    = 26299,
   NPC_MALYGOS     = 26310,
   GO_SIGNAL_FIRE  = 194151,

   SPELL_RAELORASZ_SPARK = 62272,
   SPELL_FIRE_CORPSE     = 39199,
   SPELL_FIRE_BREATH     = 31962,
   SPELL_MALYGOS_EARTHQ  = 46853,
   SPELL_TAXI_KERISTASZA = 46814,
   SPELL_ICE_BLOCK       = 56644,
   SPELL_FROSTBOLT       = 61461,

   SAY_KERISTRASZA_1     = -1002030,
   YELL_KERISTRASZA_1    = -1002031,
   YELL_KERISTRASZA_2    = -1002032,
   YELL_KERISTRASZA_3    = -1002033,
   SAY_KERISTRASZA_2     = -1002034,

   //After Fligth
   YELL_MALYGOS_1        = -1002035,
   SAY_KERISTRASZA_3     = -1002036,
   YELL_MALYGOS_2        = -1002037,
   YELL_KERISTRASZA_4    = -1002038,
};
const Position posKeristrasza[6] =
{
   {4157.00f, 7035.00f, 215.87f, 0.00f}, // Summon position
   {4063.72f, 7084.12f, 174.86f, 0.00f}, // Land position
   {4054.51f, 7084.29f, 168.12f, 0.00f}, // Burn Corpse positon
   {4048.90f, 7083.94f, 168.21f, 0.00f}, // Saragosa Corpse Spawn
   {3800.47f, 6557.50f, 170.98f, 1.55f}, // Keristrasza 2 Spawn
   {3791.76f, 6603.61f, 179.91f, 0.00f}, // Malygos Spawn
};
class npc_signal_fire : public CreatureScript
{
public:
   npc_signal_fire() : CreatureScript("npc_signal_fire") { }

   struct npc_signal_fireAI : public ScriptedAI
   {
       npc_signal_fireAI(Creature* creature) : ScriptedAI(creature) { }

       EventMap events;
       Creature* pKeristrasza;
       Creature* pSaragosa;
       Player* player;
       bool eventRunning;

       void Reset()
       {
           // Reset al variables
           events.Reset();
           pKeristrasza, pSaragosa, player = NULL;
           eventRunning = false;
           // Unlit signal
           if(GameObject* pGo = me->FindNearestGameObject(GO_SIGNAL_FIRE, 2.0f))
               pGo->SetGoState(GO_STATE_READY);
       }

       void SpellHit(Unit* caster, SpellEntry const* spell)
       {
           if (eventRunning)
               return;

           // The invisible trigger handles the event until the Fly of Keristrasza
           if(spell->Id == SPELL_RAELORASZ_SPARK)
           {
               player = caster->ToPlayer();
               // Lit the fire signal
               if(GameObject* pGo = me->FindNearestGameObject(GO_SIGNAL_FIRE, 2.0f))
                   pGo->SetGoState(GO_STATE_ACTIVE);
               // Summon Keristrasza
               if(Creature* pFind = me->SummonCreature(NPC_KERISTRASZA, posKeristrasza[0]))
               {
                   pKeristrasza = pFind;
				   pFind->SetCanFly(true);
                   pFind->GetMotionMaster()->MovePoint(1, posKeristrasza[1]);
               }
               eventRunning = true;
               events.ScheduleEvent(1, 6000);
           }
       }

       void UpdateAI(const uint32 diff)
       {
           events.Update(diff);

           switch(events.ExecuteEvent())
           {
           case 1:
			   pKeristrasza->SetCanFly(false);
               DoScriptText(SAY_KERISTRASZA_1, pKeristrasza, player);
               events.ScheduleEvent(2, 3000);
               break;
           case 2:
               DoScriptText(YELL_KERISTRASZA_1, pKeristrasza);
               pKeristrasza->GetMotionMaster()->MovePoint(1, posKeristrasza[2]);
               events.ScheduleEvent(3, 5000);
               break;
           case 3:
               DoScriptText(YELL_KERISTRASZA_2, pKeristrasza);
               // Summon Saragosa and make her die
               if(Creature* pCorpse = me->SummonCreature(NPC_SARAGOSA, posKeristrasza[3]))
                   pSaragosa = pCorpse;
               events.ScheduleEvent(4, 3000);
               break;
           case 4:
               DoScriptText(YELL_KERISTRASZA_3, pKeristrasza);
               pKeristrasza->CastSpell(pSaragosa, SPELL_FIRE_BREATH, true);
               events.ScheduleEvent(5, 1000);
               break;
           case 5:
               me->AddAura(SPELL_FIRE_CORPSE, pSaragosa);
               events.ScheduleEvent(6, 1000);
               break;
           case 6:
               player->CastSpell(player, SPELL_MALYGOS_EARTHQ, true);
               events.ScheduleEvent(7, 3000);
               break;
           case 7:
               DoScriptText(SAY_KERISTRASZA_2, pKeristrasza, player);
               events.ScheduleEvent(8, 3000);
               break;
           case 8:
               // Passes the control to Keristrasza
               player->CastSpell(player, SPELL_TAXI_KERISTASZA, true);
               pKeristrasza->AI()->SetGUID(player->GetGUID());
               pSaragosa->DespawnOrUnsummon(10000);
               events.ScheduleEvent(9, 20000);
               break;
           case 9:
               Reset();
               break;
           }
       }

   };

   CreatureAI* GetAI(Creature* creature) const
   {
       return new npc_signal_fireAI(creature);
   }
};

class npc_keristrasza_coldarra : public CreatureScript
{
public:
   npc_keristrasza_coldarra() : CreatureScript("npc_keristrasza_coldarra") { }

   struct npc_keristrasza_coldarraAI : public ScriptedAI
   {
       npc_keristrasza_coldarraAI(Creature* creature) : ScriptedAI(creature) { }

       EventMap events;
       uint64 uiPlayer;
       bool waiting;
       bool finishedWay;
       Creature* pMalygos;

       void SetGUID(const uint64 guid, int32 /*iId*/)
       {
           me->NearTeleportTo(posKeristrasza[4].GetPositionX(), posKeristrasza[4].GetPositionY(), posKeristrasza[4].GetPositionZ(), posKeristrasza[4].GetOrientation());
		   me->SetVisible(true);
           uiPlayer = guid;
           waiting = true;
       }
       void Reset()
       {
           events.Reset();
           me->SetSpeed(MOVE_FLIGHT, 3.2f, true);
           pMalygos = NULL;
           uiPlayer = 0;
           waiting = false;
           finishedWay = false;
           me->SetReactState(REACT_PASSIVE);
       }


       void UpdateAI(const uint32 diff)
       {
           if(!waiting)
               return;

           if(!finishedWay)
           {
               if(Player* player = me->GetPlayer(*me, uiPlayer))
               {
                   if (!player->isInFlight())
                   {
                       if(me->IsWithinDist(player, 10.0f))
                       {
                           finishedWay = true;
						   me->SetVisible(true);
                           player->ToPlayer()->KilledMonsterCredit(NPC_KERISTRASZA, 0);
                           events.ScheduleEvent(1, 0);
                       }else
                           me->DespawnOrUnsummon(0);
                   }
                   else
                       return;
               }
           }

           events.Update(diff);

           switch(events.ExecuteEvent())
           {
           case 1:
			   me->SetCanFly(false);
               if(Player* player = me->GetPlayer(*me, uiPlayer))
                   DoScriptText(SAY_KERISTRASZA_3, me, player);
               events.ScheduleEvent(2, 5000);
               break;
           case 2:
               if(Creature* pSumm = me->SummonCreature(NPC_MALYGOS, posKeristrasza[5]))
               {
                   pMalygos = pSumm;
				   pSumm->SetCanFly(true);
                   pSumm->SetReactState(REACT_PASSIVE);
				   pSumm->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                   pSumm->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.4f);
                   DoScriptText(YELL_MALYGOS_2, pMalygos);
                   pMalygos->SetTarget(me->GetGUID());
                   me->SetTarget(pMalygos->GetGUID());
               }
               events.ScheduleEvent(3, 6000);
               break;
           case 3:
               DoScriptText(YELL_KERISTRASZA_4, me);
               me->AddAura(SPELL_ICE_BLOCK, me);

               if(pMalygos)
                   pMalygos->DespawnOrUnsummon(7000);
               me->DespawnOrUnsummon(7000);
               break;
           }

       }
   };

   CreatureAI* GetAI(Creature* creature) const
   {
       return new npc_keristrasza_coldarraAI(creature);
   }
};


/*##################
# npc_icc_rep_rings
###################*/

#define GMSG 20007

class npc_icc_rep_rings : public CreatureScript
{
public:
    npc_icc_rep_rings() : CreatureScript("npc_icc_rep_rings") { }

    enum AshenRings
    {
        ASHEN_VERDICT = 1156,
        C_F = 50377,
        H_F = 50378,
        MS_F = 52569,
        MA_F = 50376,
        T_F = 50375,
        C_H = 50384,
        H_H = 50386,
        MS_H = 52570,
        MA_H = 50387,
        T_H = 50388,
        C_R = 50397,
        H_R = 50399,
        MS_R = 52571,
        MA_R = 50401,
        T_R = 50403,
        C_E = 50398,
        H_E = 50400,
        MS_E = 52572,
        MA_E = 50402,
        T_E = 50404,
        QUEST_CHOOSE_YOUR_PATH = 24815
    };

    bool OnGossipHello(Player* player, Creature* creature)
    {
        uint32 friendly = 3000;
        uint32 honored = 9000;
        uint32 revered = 21000;
        uint32 exalted = 42000;
        uint32 value = player->GetReputationMgr().GetReputation(ASHEN_VERDICT);

        if (player && !player->isInCombat() && player->GetQuestStatus(QUEST_CHOOSE_YOUR_PATH) == QUEST_STATUS_COMPLETE && player->GetQuestRewardStatus(QUEST_CHOOSE_YOUR_PATH))
        {
        if (player->HasItemCount(C_F, 1))
          player->DestroyItemCount(C_F, 1, true);
        if  (player->HasItemCount(H_F,1))
          player->DestroyItemCount(H_F,1, true);
        if  (player->HasItemCount(MS_F,1))
          player->DestroyItemCount(MS_F,1, true);
        if  (player->HasItemCount(MA_F,1))
          player->DestroyItemCount(MA_F,1, true);
        if  (player->HasItemCount(T_F,1))
          player->DestroyItemCount(T_F,1, true);
        if  (player->HasItemCount(C_H,1))
          player->DestroyItemCount(C_H,1, true);
        if  (player->HasItemCount(H_H,1))
          player->DestroyItemCount(H_H,1, true);
        if  (player->HasItemCount(MS_H,1))
          player->DestroyItemCount(MS_H,1, true);
        if  (player->HasItemCount(MA_H,1))
          player->DestroyItemCount(MA_H,1, true);
        if  (player->HasItemCount(T_H,1))
          player->DestroyItemCount(T_H,1, true);
        if  (player->HasItemCount(C_R,1))
          player->DestroyItemCount(C_R,1, true);
        if  (player->HasItemCount(H_R,1))
          player->DestroyItemCount(H_R,1, true);
        if  (player->HasItemCount(MS_R,1))
          player->DestroyItemCount(MS_R,1, true);
        if  (player->HasItemCount(MA_R,1))
          player->DestroyItemCount(MA_R,1, true);
        if  (player->HasItemCount(T_R,1))
          player->DestroyItemCount(T_R,1, true);
        if  (player->HasItemCount(C_E,1))
          player->DestroyItemCount(C_E,1, true);
        if  (player->HasItemCount(H_E,1))
          player->DestroyItemCount(H_E,1, true);
        if  (player->HasItemCount(MS_E,1))
          player->DestroyItemCount(MS_E,1, true);
        if  (player->HasItemCount(MA_E,1))
          player->DestroyItemCount(MA_E,1, true);
        if  (player->HasItemCount(T_E,1))
          player->DestroyItemCount(T_E,1, true);

        if (value >= friendly && value < honored)
        {
	if  (!player->HasItemCount(C_F,1,true) && !player->HasItemCount(H_F,1,true)&& !player->HasItemCount(MS_F,1,true) 
	&& !player->HasItemCount(MA_F,1,true) && !player->HasItemCount(T_F,1,true))
	{
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring caster dps (friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring healer (friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - fuerza (friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+3);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - agilidad (friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+4);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring tank (friendly)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+5);
                player->SEND_GOSSIP_MENU(GMSG, creature->GetGUID());
                return true;
            }
        }

        if (value >= honored && value < revered)
        {
            if (!player->HasItemCount(C_F,1,true) && !player->HasItemCount(H_F,1,true) && !player->HasItemCount(MS_F,1,true) && !player->HasItemCount(MA_F,1,true) &&
                !player->HasItemCount(T_F,1,true) && !player->HasItemCount(C_H,1,true) && !player->HasItemCount(H_H,1,true) && !player->HasItemCount(MS_H,1,true) &&
                !player->HasItemCount(MA_H,1,true) && !player->HasItemCount(T_H,1,true))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring caster dps (honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+6);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring healer (honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+7);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - fuerza (honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+8);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - agilidad (honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+9);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring tank (honored)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+10);
                player->SEND_GOSSIP_MENU(GMSG, creature->GetGUID());
                return true;
            }
        }

        if (value >= revered && value < exalted)
        {
            if (!player->HasItemCount(C_F,1,true) && !player->HasItemCount(H_F,1,true) && !player->HasItemCount(MS_F,1,true) && !player->HasItemCount(MA_F,1,true) &&
                !player->HasItemCount(T_F,1,true) && !player->HasItemCount(C_H,1,true) && !player->HasItemCount(H_H,1,true) && !player->HasItemCount(MS_H,1,true) &&
                !player->HasItemCount(MA_H,1,true) && !player->HasItemCount(T_H,1,true) && !player->HasItemCount(C_R,1,true) && !player->HasItemCount(H_R,1,true) &&
                !player->HasItemCount(MS_R,1,true) && !player->HasItemCount(MA_R,1,true) && !player->HasItemCount(T_R,1,true))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring caster dps (revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+11);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring healer (revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+12);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - fuerza (revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+13);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - agilidad (revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+14);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring tank (revered)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+15);
                player->SEND_GOSSIP_MENU(GMSG, creature->GetGUID());
                return true;
            }
        }

        if (value >= exalted)
        {
            if (!player->HasItemCount(C_F,1,true) && !player->HasItemCount(H_F,1,true) && !player->HasItemCount(MS_F,1) && !player->HasItemCount(MA_F,1,true) &&
                !player->HasItemCount(T_F,1,true) && !player->HasItemCount(C_H,1,true) && !player->HasItemCount(H_H,1) && !player->HasItemCount(MS_H,1,true) &&
                !player->HasItemCount(MA_H,1,true) && !player->HasItemCount(T_H,1,true) && !player->HasItemCount(C_R,1) && !player->HasItemCount(H_R,1,true) &&
                !player->HasItemCount(MS_R,1,true) && !player->HasItemCount(MA_R,1,true) && !player->HasItemCount(T_R,1) && !player->HasItemCount(C_E,1,true) &&
                !player->HasItemCount(H_E,1,true) && !player->HasItemCount(MS_E,1,true) && !player->HasItemCount(MA_E,1) && !player->HasItemCount(T_E,1,true))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring caster dps (exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+16);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring healer (exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+17);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - fuerza (exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+18);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring melee - agilidad (exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+19);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ring tank (exalted)",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+20);
                player->SEND_GOSSIP_MENU(GMSG, creature->GetGUID());
                return true;
            }
        }
    }
} 
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();

        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->AddItem(C_F,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->AddItem(H_F,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->AddItem(MS_F,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->AddItem(MA_F,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                player->AddItem(T_F,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+6:
                player->AddItem(C_H,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+7:
                player->AddItem(H_H,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+8:
                player->AddItem(MS_H,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+9:
                player->AddItem(MA_H,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+10:
                player->AddItem(T_H,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+11:
                player->AddItem(C_R,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+12:
                player->AddItem(H_R,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+13:
                player->AddItem(MS_R,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+14:
                player->AddItem(MA_R,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+15:
                player->AddItem(T_R,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+16:
                player->AddItem(C_E,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+17:
                player->AddItem(H_E,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+18:
                player->AddItem(MS_E,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+19:
                player->AddItem(MA_E,1);
                break;
            case GOSSIP_ACTION_INFO_DEF+20:
                player->AddItem(T_E,1);
                break;
        }
        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};

/*########################
# at_legion_hold_smvalley
#########################*/
enum ToLegionHold
{
    SPELL_CRATE_DISGUISE = 37097,
    NPC_RAZUUN = 21502,
    NPC_JOVAAN = 21633
};

class at_legion_hold_smvalley : public AreaTriggerScript
{
    public:

        at_legion_hold_smvalley() : AreaTriggerScript("at_legion_hold_smvalley") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if (player->HasAura(SPELL_CRATE_DISGUISE))
            {
                player->SummonCreature(NPC_RAZUUN, -3302.599854f, 2928.284424f, 170.921692f, 2.609816f, TEMPSUMMON_TIMED_DESPAWN, 40000);
                player->SummonCreature(NPC_JOVAAN, -3306.278076f, 2932.812500f, 170.923660f, 5.535417f, TEMPSUMMON_TIMED_DESPAWN, 40000);
                player->KilledMonsterCredit(21502,0);
                return true;
            }
        }
};

/*#########################
# npc_father_kamaros (31279)
#########################*/
enum NotDeadYet
{
    QUEST_NOT_DEAD_YET_A = 13221,
    QUEST_NOT_DEAD_YET_H = 13229,
    NPC_NOT_DEAD_YET_TRIG = 99107,
    SPELL_PW_FORTITUDE = 23947,
    SPELL_PW_SHIELD = 20697
};

const Position KamarosWp[54] =
{
    {6718.48f, 3439.88f, 682.24f, 0.00f},
    {6723.54f, 3433.79f, 682.20f, 0.00f},
    {6734.04f, 3436.02f, 682.03f, 0.00f},
    {6737.66f, 3439.00f, 680.97f, 0.00f},
    {6742.07f, 3442.76f, 679.93f, 0.00f},
    {6745.65f, 3446.09f, 678.85f, 0.00f},
    {6750.47f, 3450.57f, 677.51f, 0.00f},
    {6755.94f, 3455.28f, 675.78f, 0.00f},
    {6759.58f, 3458.61f, 674.71f, 0.00f},
    {6765.04f, 3463.30f, 673.37f, 0.00f},
    {6769.22f, 3467.40f, 673.00f, 0.00f},
    {6771.95f, 3471.55f, 673.01f, 0.00f},
    {6777.21f, 3477.04f, 672.99f, 0.00f},
    {6782.91f, 3482.89f, 674.34f, 0.00f},
    {6779.54f, 3479.36f, 673.32f, 0.00f},
    {6773.88f, 3473.81f, 673.03f, 0.00f},
    {6786.32f, 3484.85f, 675.62f, 0.00f},
    {6792.51f, 3484.66f, 677.20f, 0.00f},
    {6797.39f, 3484.49f, 678.76f, 0.00f},
    {6800.64f, 3484.44f, 680.03f, 0.00f},
    {6805.98f, 3484.26f, 682.17f, 0.00f},
    {6810.80f, 3484.14f, 683.97f, 0.00f},
    {6816.15f, 3483.58f, 686.18f, 0.00f},
    {6822.91f, 3483.01f, 688.17f, 0.00f},
    {6829.09f, 3481.91f, 689.49f, 0.00f},
    {6836.49f, 3480.96f, 690.69f, 0.00f},
    {6844.57f, 3480.25f, 691.89f, 0.00f},
    {6853.38f, 3479.60f, 692.98f, 0.00f},
    {6859.95f, 3479.59f, 693.60f, 0.00f},
    {6867.60f, 3479.74f, 694.21f, 0.00f},
    {6875.25f, 3479.95f, 694.86f, 0.00f},
    {6882.69f, 3480.14f, 695.82f, 0.00f},
    {6889.85f, 3479.97f, 697.36f, 0.00f},
    {6896.19f, 3479.88f, 698.54f, 0.00f},
    {6902.49f, 3479.66f, 699.74f, 0.00f},
    {6908.08f, 3479.89f, 700.81f, 0.00f},
    {6913.89f, 3480.32f, 701.86f, 0.00f},
    {6920.57f, 3482.18f, 703.10f, 0.00f},
    {6925.61f, 3481.11f, 704.38f, 0.00f},
    {6930.92f, 3480.19f, 705.91f, 0.00f},
    {6936.04f, 3479.73f, 707.06f, 0.00f},
    {6942.06f, 3479.21f, 708.06f, 0.00f},
    {6947.22f, 3477.91f, 709.01f, 0.00f},
    {6953.44f, 3477.53f, 709.63f, 0.00f},
    {6959.23f, 3477.21f, 710.30f, 0.00f},
    {6967.35f, 3474.84f, 711.05f, 0.00f},
    {6980.54f, 3470.97f, 710.86f, 0.00f},
    {6985.85f, 3469.15f, 708.75f, 0.00f},
    {6991.36f, 3467.22f, 705.94f, 0.00f},
    {6997.30f, 3465.15f, 702.91f, 0.00f},
    {7004.14f, 3462.46f, 699.36f, 0.00f},
    {7009.42f, 3460.61f, 696.67f, 0.00f},
    {7018.43f, 3455.71f, 696.67f, 0.00f},
    {7029.21f, 3447.94f, 696.17f, 0.00f}
};

class npc_father_kamaros : public CreatureScript
{
public:
    npc_father_kamaros() : CreatureScript("npc_father_kamaros") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_NOT_DEAD_YET_A || quest->GetQuestId() == QUEST_NOT_DEAD_YET_H)
        {
            creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            creature->AI()->SetGUID(player->GetGUID());
            creature->CastSpell(player,SPELL_PW_FORTITUDE,true);
            player->CastSpell(player,SPELL_PW_SHIELD,true);
        }
        return true;
    }

    struct npc_father_kamarosAI : public ScriptedAI
    {
        npc_father_kamarosAI(Creature* creature) : ScriptedAI(creature) { }

        uint8 count;
        bool wp_reached;
        bool movementStarted;
        bool finished;
        uint64 uiPlayer;
        Player* player;
        
        void SetGUID(const uint64 uiGuid, int32 /*iId*/)
        {
            movementStarted = true;
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            me->SetReactState(REACT_PASSIVE);
            me->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
            me->GetMotionMaster()->MovePoint(0, KamarosWp[0]);
            uiPlayer = uiGuid;
        }

        void Reset()
        {
            count = 0;
            wp_reached = false;
            movementStarted = false;
            finished = false;
            uiPlayer = 0;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE || id != count)
                return;

            if (movementStarted && id < 54)
            {
                ++count;
                wp_reached = true;
            }
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if (movementStarted)
            {
                player = me->GetPlayer(*me, uiPlayer);

                if (!player || !player->isAlive() || me->GetDistance(player) > 30.0f)
                    me->DisappearAndDie();

                if (player && player->isInCombat() && player->GetUInt64Value(UNIT_FIELD_TARGET))
                    me->Attack(me->GetUnit(*me, player->GetUInt64Value(UNIT_FIELD_TARGET)), true);
            }

            if (wp_reached && !finished)
            {
                wp_reached = false;
                me->SetUnitMovementFlags(MOVEMENTFLAG_WALKING); 
                me->GetMotionMaster()->MovePoint(count, KamarosWp[count]);
            }
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who && who->GetEntry() == NPC_NOT_DEAD_YET_TRIG && me->IsWithinDistInMap(who,10.0f,true))
            {
                if (Player* player = me->GetPlayer(*me, uiPlayer))
                {
                    switch (player->GetTeam())
                    {
                        case ALLIANCE: player->GroupEventHappens(QUEST_NOT_DEAD_YET_A, me); break;
                        case HORDE: player->GroupEventHappens(QUEST_NOT_DEAD_YET_H, me); break;
                    }
                    movementStarted = false;
                    finished = true;
                    me->DespawnOrUnsummon(10000);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_father_kamarosAI(creature);
    }
};

enum ghoulSpells
{
    SPELL_SHINING_LIGTH = 43202,
    SPELL_SHINING_LIGTH_HIT_VISUAL = 46400,
};

class npc_decomposing_ghoul : public CreatureScript
{
public:
    npc_decomposing_ghoul() : CreatureScript("npc_decomposing_ghoul") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_decomposing_ghoulAI(creature);
    }

    struct npc_decomposing_ghoulAI : public ScriptedAI
    {
        npc_decomposing_ghoulAI(Creature* c) : ScriptedAI(c) { }


        void EnterCombat(Unit * who)
        {
            if (who && who->HasAura(SPELL_SHINING_LIGTH))
            {
                DoCast(SPELL_SHINING_LIGTH_HIT_VISUAL);
                EnterEvadeMode();
            }
        }
    };
};

enum irulonData
{
    QUEST_GUIDED_BY_HONOR = 11289,
    NPC_TIRION            = 24232,
    NPC_CLERIC            = 24233,
    ITEM_ASHBRINGER       = 13262,

    SAY_TIRION_1          = -1002000,
    EMOTE_TIRION_1        = -1002001,
    SAY_IRULON_1          = -1002002,
    SAY_TIRION_2          = -1002003,
    SAY_TIRION_3          = -1002004,
    SAY_IRULON_2          = -1002005,
    EMOTE_TIRION_2        = -1002006,
    SAY_TIRION_4          = -1002007,
    SAY_TIRION_5          = -1002008,
    EMOTE_TIRION_3        = -1002009,
    YELL_TIRION           = -1002010,

    ACTION_START,
};

enum iluronEvents
{
    EVENT_NONE,
    EVENT_00,
    EVENT_01,
    EVENT_02,
    EVENT_03,
    EVENT_04,
    EVENT_05,
    EVENT_06,
    EVENT_07,
    EVENT_08,
    EVENT_09,
    EVENT_10,
};

class npc_irulon_trueblade : public CreatureScript
{
public:
    npc_irulon_trueblade() : CreatureScript("npc_irulon_trueblade") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_irulon_truebladeAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* me)
    {
        if (me->isQuestGiver())
            player->PrepareQuestMenu(me->GetGUID());

        player->GroupEventHappens(QUEST_GUIDED_BY_HONOR, me);

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
        return true;
    }
    bool OnQuestReward(Player* /*player*/, Creature* me, Quest const* quest, uint32 /*item*/)
    {
        if (quest->GetQuestId() == QUEST_GUIDED_BY_HONOR)
            me->AI()->DoAction(ACTION_START);
        return true;
    }

    struct npc_irulon_truebladeAI : public ScriptedAI
    {
        npc_irulon_truebladeAI(Creature* c) : ScriptedAI(c) { }

        EventMap events;
        uint64 uiTirion;

        void Reset()
        {
            uiTirion = 0;
            events.Reset();
        }

        void DoAction(const int32 actionId)
        {
            switch(actionId)
            {
                case ACTION_START:
                    uiTirion = 0;
                    events.ScheduleEvent(EVENT_00, 1500);
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            switch(events.ExecuteEvent())
            {
                case EVENT_00:
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                    if(Creature* pTirion = me->FindNearestCreature(NPC_CLERIC, 7.0f))
                    {
                        uiTirion = pTirion->GetGUID();
                        DoScriptText(SAY_TIRION_1, pTirion);
                        pTirion->AddUnitMovementFlag(MOVEMENTFLAG_WALKING); 
                        pTirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                        pTirion->GetMotionMaster()->MovePoint(0, me->GetPositionX() + 3.0f, me->GetPositionY() + 3.0f, me->GetPositionZ() + 0.5f);
                    }
                    events.ScheduleEvent(EVENT_01, 2000);
                    break;
                case EVENT_01:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                    {
                        DoScriptText(EMOTE_TIRION_1, pTirion);
                        pTirion->UpdateEntry(NPC_TIRION);
                        pTirion->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
                    }
                    events.ScheduleEvent(EVENT_02, 1000);
                    break;
                case EVENT_02:
                    DoScriptText(SAY_IRULON_1, me);
                    events.ScheduleEvent(EVENT_03, 2000);
                    break;
                case EVENT_03:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(SAY_TIRION_2, pTirion);
                    events.ScheduleEvent(EVENT_04, 3000);
                    break;
                case EVENT_04:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(SAY_TIRION_3, pTirion);
                    events.ScheduleEvent(EVENT_05,1000);
                    break;
                case EVENT_05:
                    DoScriptText(SAY_IRULON_2, me);
                    events.ScheduleEvent(EVENT_06, 2500);
                    break;
                case EVENT_06:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(EMOTE_TIRION_2, pTirion);
                    events.ScheduleEvent(EVENT_07,1000);
                    break;
                case EVENT_07:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(SAY_TIRION_4, pTirion);
                    events.ScheduleEvent(EVENT_08,1500);
                    break;
                case EVENT_08:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(SAY_TIRION_5, pTirion);
                    events.ScheduleEvent(EVENT_09,1500);
                    break;
                case EVENT_09:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                    {
                        DoScriptText(EMOTE_TIRION_3, pTirion);
                        pTirion->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, ITEM_ASHBRINGER);
                    }
                    events.ScheduleEvent(EVENT_10,2000);
                    break;
                case EVENT_10:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                    {
                        DoScriptText(YELL_TIRION, pTirion);
                        pTirion->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
                        pTirion->DespawnOrUnsummon(5000);
                    }
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    break;
            }
        }
    };
};

/*############
# npc_terokk
#############*/
enum TerokkData
{
    SPELL_CLEAVE = 15284,
    SPELL_DIVINE_SHIELD = 40733,
    SPELL_FRENZY = 28747,
    SPELL_SHADOW_BOLT_VOLLEY = 40721,
    SPELL_WILL_OF_THE_ARAKKOA = 40722,
    SPELL_VISUAL_MARKER = 40656,
    NPC_MARKER_TRIGGER = 99109,
    SAY_SUMMONED = 0,
    SAY_CHOSEN = 1,
    SAY_IMMUNE = 2
};

class npc_terokk : public CreatureScript
{
public:
    npc_terokk() : CreatureScript("npc_terokk") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_terokkAI(creature);
    }

    struct npc_terokkAI : public ScriptedAI
    {
        npc_terokkAI(Creature* creature) : ScriptedAI(creature) { }

        bool inCombat;
        bool Chosen;
        bool isImmune;
        uint32 CleaveTimer;
        uint32 VolleyTimer;
        uint32 ShieldTimer;
        uint32 MarkerTimer;

        void Reset()
        {
            inCombat = false;
            Chosen = false;
            isImmune = false;
            CleaveTimer = 5000;
            VolleyTimer = 3000;
            ShieldTimer = 999999;
            MarkerTimer = 15000;
        }

        void EnterCombat (Unit* /*who*/)
        {
            if (!inCombat)
            {
                Talk(SAY_SUMMONED);
                inCombat = true;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (CleaveTimer <= diff)
            {
                DoCast(me->getVictim(),SPELL_CLEAVE);
                CleaveTimer = urand(9000,15000);
            } else CleaveTimer -= diff;

            if (VolleyTimer <= diff)
            {
                DoCast(me, SPELL_SHADOW_BOLT_VOLLEY);
                VolleyTimer = urand(12000,20000);
            } else VolleyTimer -= diff;

            if (MarkerTimer <= diff)
            {
                switch (urand(1,4))
                {
                    case 1:
                        if (Creature* trigger = me->SummonCreature(NPC_MARKER_TRIGGER,me->GetPositionX()+30,me->GetPositionY(),me->GetPositionZ(),0.0f,TEMPSUMMON_TIMED_DESPAWN,30000))
                            trigger->AddAura(SPELL_VISUAL_MARKER,trigger);
                            break;
                    case 2:
                        if (Creature* trigger = me->SummonCreature(NPC_MARKER_TRIGGER,me->GetPositionX(),me->GetPositionY()+30,me->GetPositionZ(),0.0f,TEMPSUMMON_TIMED_DESPAWN,30000))
                            trigger->AddAura(SPELL_VISUAL_MARKER,trigger);
                            break;
                    case 3:
                        if (Creature* trigger = me->SummonCreature(NPC_MARKER_TRIGGER,me->GetPositionX()-30,me->GetPositionY(),me->GetPositionZ(),0.0f,TEMPSUMMON_TIMED_DESPAWN,30000))
                            trigger->AddAura(SPELL_VISUAL_MARKER,trigger);
                            break;
                    case 4:
                        if (Creature* trigger = me->SummonCreature(NPC_MARKER_TRIGGER,me->GetPositionX(),me->GetPositionY()-30,me->GetPositionZ(),0.0f,TEMPSUMMON_TIMED_DESPAWN,30000))
                            trigger->AddAura(SPELL_VISUAL_MARKER,trigger);
                            break;
                }
                MarkerTimer = urand(25000,30000);
            } else MarkerTimer -= diff;

            if (HealthBelowPct(50) && !Chosen)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0))
                {
                    if (target && target->isAlive())
                    {
                        Talk(SAY_CHOSEN);
                        DoCast(target, SPELL_WILL_OF_THE_ARAKKOA);
                        Chosen = true;
                    }
                }
            }

            if (HealthBelowPct(25) && !isImmune)
            {
                if (ShieldTimer <= diff)
                {
                    Talk(SAY_IMMUNE);
                    DoCast(me, SPELL_DIVINE_SHIELD);
                    ShieldTimer = 60000;
                    isImmune = true;
                } else ShieldTimer -= diff;
            }

            if (me->FindNearestCreature(NPC_MARKER_TRIGGER,5.0f,true) && isImmune)
            {
                me->RemoveAura(SPELL_DIVINE_SHIELD);
                DoCast(me, SPELL_FRENZY);
                isImmune = false;
            }
            DoMeleeAttackIfReady();
        }
    };
    
};

enum ancientSkullPileData
{
    ITEM_TIMELOST_OFFERING = 32720,
    NPC_TEROKK             = 21838,
};

class go_ancient_skull_pile : public GameObjectScript
{
    public:
        go_ancient_skull_pile() : GameObjectScript("go_ancient_skull_pile") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            if (player->HasItemCount(ITEM_TIMELOST_OFFERING, 1))
                player->ADD_GOSSIP_ITEM( 9, "Invoking Terokk", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* go, uint32 /*uiSender*/, uint32 uiAction)
        {
            if(uiAction = GOSSIP_ACTION_INFO_DEF+1)
            {
                if(Creature* pTerokk = player->SummonCreature(NPC_TEROKK, go->GetPositionX() + 4.0f, go->GetPositionY() + 2.0f, go->GetPositionZ(), go->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                {
                    pTerokk->AI()->AttackStart(player);
                    pTerokk->GetMotionMaster()->Clear();
                    pTerokk->GetMotionMaster()->MoveChase(player);
                    player->DestroyItemCount(ITEM_TIMELOST_OFFERING, 1, true);
                }
            }
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
};

enum Says
{
    SAY_START               = -1840032,
    SAY_WINNER              = -1840043,
    SAY_END                 = -1840044,
    QUEST_MASTER_ANGLER     = 8193,
};
/*######
## npc_riggle_bassbait
######*/
class npc_riggle_bassbait : public CreatureScript
{
public:
    npc_riggle_bassbait() : CreatureScript("npc_riggle_bassbait") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver()) // If the quest is still running.
        {
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
            pPlayer->SEND_GOSSIP_MENU(7614, pCreature->GetGUID());
            return true;
        }
        // The Quest is not there anymore
        // There is a winner!
        pPlayer->SEND_GOSSIP_MENU(7714, pCreature->GetGUID());
        return true;
    }

    bool OnQuestReward(Player* pPlayer, Creature* pCreature, const Quest* pQuest, uint32 opt)
    {
        // TODO: check if this can only be called if NPC has QUESTGIVER flag.
        if (pQuest->GetQuestId() == QUEST_MASTER_ANGLER && ((npc_riggle_bassbaitAI*)(pCreature->AI()))->bEventWinnerFound == false)
        {
            DoScriptText(SAY_WINNER, pCreature,pPlayer);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            ((npc_riggle_bassbaitAI*)(pCreature->AI()))->bEventWinnerFound = true;
            Creature* creature2 = GetClosestCreatureWithEntry(pCreature,15087,60.0f);
            if (creature2)
            {
                creature2->SetFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_QUESTGIVER);
            }

            return true;
        }
    return true;
}


    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_riggle_bassbaitAI (creature);
    }

    struct npc_riggle_bassbaitAI: public ScriptedAI
    {
        npc_riggle_bassbaitAI(Creature* c) : ScriptedAI(c)
        {
            // This will keep the NPC active even if there are no players around!
            c->setActive(true);
            bEventAnnounced = bEventIsOver = bEventWinnerFound = false;
            Reset();
        }
        /* Flag to check if event was announced. True if event was announced. */
        bool bEventAnnounced;
        /* Flag to check if event is over. True if event is over. */
        bool bEventIsOver;
        /* Flag to check if someone won the event. True if someone has won.*/
        bool bEventWinnerFound;

        void Reset() { }
        void EnterCombat(Unit* who) {}

        void UpdateAI(uint32 uiDiff)
        {
            // Announce the event max 1 minute after being spawned. But only if Fishing extravaganza is running.
            if (!bEventAnnounced && time(NULL) % 60 == 0 && IsHolidayActive(HOLIDAY_FISHING_EXTRAVAGANZA))
            {
                DoScriptText(SAY_START, me);
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER); //Quest&Gossip are now active
                bEventAnnounced = true;
            }
            // The Event was started (announced) & It was not yet ended & One minute passed & the Fish are gone
            if ( bEventAnnounced && !bEventIsOver && time(NULL) % 60 == 0 && !IsHolidayActive(HOLIDAY_FISHING_EXTRAVAGANZA))
            {
                DoScriptText(SAY_END, me);
                bEventIsOver = true;

            }
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
         }
    };
};

/*######
# npc_earth_elemental
######*/
#define SPELL_ANGEREDEARTH        36213

class npc_earth_elemental : public CreatureScript
{
public:
    npc_earth_elemental() : CreatureScript("npc_earth_elemental") { }

    struct npc_earth_elementalAI : public ScriptedAI
    {
        npc_earth_elementalAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 AngeredEarth_Timer;

        void Reset()
        {
            AngeredEarth_Timer = 0;
            me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, true);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //FireBlast_Timer
            if (AngeredEarth_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_ANGEREDEARTH);
                AngeredEarth_Timer = 5000 + rand() % 15000; // 5-20 sec cd

            } else AngeredEarth_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI *GetAI(Creature* creature) const
    {
        return new npc_earth_elementalAI(creature);
    }
};

/*######
# npc_fire_elemental
######*/
#define SPELL_FIRENOVA        12470
#define SPELL_FIRESHIELD        13376
#define SPELL_FIREBLAST        57984

class npc_fire_elemental : public CreatureScript
{
public:
    npc_fire_elemental() : CreatureScript("npc_fire_elemental") { }

    struct npc_fire_elementalAI : public ScriptedAI
    {
        npc_fire_elementalAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 FireNova_Timer;
        uint32 FireShield_Timer;
        uint32 FireBlast_Timer;

        void Reset()
        {
            FireNova_Timer = 5000 + rand() % 15000; // 5-20 sec cd
            FireBlast_Timer = 5000 + rand() % 15000; // 5-20 sec cd
            FireShield_Timer = 0;
            me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

			if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            // sLog->outString("npc_fire_elemental: [UpdateAI] diff %u, FireNova_Timer %u, FireBlast_Timer %u, FireShield_Timer %u",  diff, FireNova_Timer, FireBlast_Timer, FireShield_Timer);

            //FireShield_Timer
            if (FireShield_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FIRESHIELD);
                FireShield_Timer = 2 * IN_MILLISECONDS;

            } else FireShield_Timer -= diff;

            //FireBlast_Timer
            if (FireBlast_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FIREBLAST);
                FireBlast_Timer = 5000 + rand() % 15000; // 5-20 sec cd

            } else FireBlast_Timer -= diff;

            //FireNova_Timer
            if (FireNova_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FIRENOVA);
                FireNova_Timer = 5000 + rand() % 15000; // 5-20 sec cd

            } else FireNova_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI *GetAI(Creature* creature) const
    {
        return new npc_fire_elementalAI(creature);
    }
};

class npc_explosive_sheep : public CreatureScript
{
public:
    npc_explosive_sheep() : CreatureScript("npc_explosive_sheep") {}

    struct npc_explosive_sheepAI : public ScriptedAI
    {
        npc_explosive_sheepAI(Creature* creature) : ScriptedAI(creature)
        {
            target_selected = false;
            timer = 0;
            target = 0;
        }

        bool target_selected;
        uint64 target;
        uint32 timer;

        void UpdateAI(const uint32 diff)
        {
            timer += diff;
            if (timer >= 60000) //3 minutes
                me->ForcedDespawn();
            else if(target_selected)
            {
                if (Unit* ptarget = me->GetUnit((*me), target))
                {
                    if (me->GetDistance2d(ptarget) <= 1.0f)
                        DoCast(ptarget, 4050);
                    else
                        me->GetMotionMaster()->MoveChase(ptarget);
                }
                else
                    me->ForcedDespawn();
            }
            else
            {
                if (Unit* owner = me->GetOwner())
                {
                    if (owner->getVictim())
                    {
                        target = owner->getVictim()->GetGUID();
                        target_selected = true;
                    }
                    else
                        me->GetMotionMaster()->MoveFollow(owner, 2.0f, me->GetAngle(owner));
                }
                else
                    me->ForcedDespawn();
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_explosive_sheepAI(creature);
    }
};

/*#####
# item_hallowsend_tricky_treat
#####*/
enum HallowsendTrickyTreats
{
    SPELL_TRICKY_TREAT_STACKING_SPEED = 42919,
    SPELL_UPSET_TUMMY                 = 42966
};

class item_hallowsend_tricky_treat : public ItemScript
{
public:
    item_hallowsend_tricky_treat() : ItemScript("item_hallowsend_tricky_treat") { }

   bool OnUse(Player *player, Item *pItem, SpellCastTargets const& targets)
   {
       if (player->HasAura(SPELL_UPSET_TUMMY))
       {
           player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem);
           return true;
       }
       player->CastSpell(player,SPELL_TRICKY_TREAT_STACKING_SPEED, true);
       if (player->HasAura(SPELL_TRICKY_TREAT_STACKING_SPEED))
       {
           if (player->GetAura(SPELL_TRICKY_TREAT_STACKING_SPEED)->GetStackAmount() > 2 && roll_chance_i(20 * player->GetAura(SPELL_TRICKY_TREAT_STACKING_SPEED)->GetStackAmount()))
               player->CastSpell(player,SPELL_UPSET_TUMMY,true);
       }
       return false;
   }
};

struct PostionEventoHallowend
{
    uint32 Area;
    uint8 Area_Count;
    bool AlreadyFired;
    Position SpawnPosition;
}

PostionEventoHallowends[] =
{
     87 ,  34 , false ,  -9456.598633f, 39.966614f, 62.195915f, 0.0f  ,
     87 ,  0 ,  false ,  -9456.162109f, 41.880970f, 62.011547f, 0.0f  ,
     87 ,  0 ,  false ,  -9455.980469f, 43.709557f, 62.056351f, 0.0f  ,
     87 ,  0 ,  false ,  -9461.942383f, 44.572384f, 62.474407f, 0.0f  ,
     87 ,  0 ,  false ,  -9461.984375f, 43.089458f, 62.598511f, 0.0f  ,
     87 ,  0 ,  false ,  -9462.203125f, 40.574947f, 62.672440f, 0.0f  ,
     87 ,  0 ,  false ,  -9475.374023f, 41.268730f, 64.304977f, 0.0f  ,
     87 ,  0 ,  false ,  -9471.819336f, 40.876884f, 64.311752f, 0.0f  ,
     87 ,  0 ,  false ,  -9466.619141f, 40.237621f, 64.321869f, 0.0f  ,
     87 ,  0 ,  false ,  -9462.197266f, 39.733025f, 64.328857f, 0.0f  ,
     87 ,  0 ,  false ,  -9457.063477f, 39.105618f, 64.323448f, 0.0f  ,
     87 ,  0 ,  false ,  -9452.693359f, 36.618862f, 69.778679f, 0.0f  ,
     87 ,  0 ,  false ,  -9456.606445f, 37.412331f, 69.696899f, 0.0f  ,
     87 ,  0 ,  false ,  -9462.572266f, 38.191925f, 69.859924f, 0.0f  ,
     87 ,  0 ,  false ,  -9476.938477f, 42.137085f, 71.250961f, 0.0f  ,
     87 ,  0 ,  false ,  -9479.271484f, 30.612669f, 70.269409f, 0.0f  ,
     87 ,  0 ,  false ,  -9478.808594f, 36.716347f, 64.324905f, 0.0f  ,
     87 ,  0 ,  false ,  -9479.466797f, 31.536119f, 64.346130f, 0.0f  ,
     87 ,  0 ,  false ,  -9479.929688f, 27.957096f, 64.348228f, 0.0f  ,
     87 ,  0 ,  false ,  -9480.361328f, 24.283165f, 64.418297f, 0.0f  ,
     87 ,  0 ,  false ,  -9452.841797f, 35.504967f, 64.328300f, 0.0f  ,
     87 ,  0 ,  false ,  -9453.877930f, 26.877312f, 64.328369f, 0.0f  ,
     87 ,  0 ,  false ,  -9453.395508f, 31.103748f, 64.328857f, 0.0f  ,
     87 ,  0 ,  false ,  -9465.262695f, 83.267693f, 66.926437f, 0.0f  ,
     87 ,  0 ,  false ,  -9466.190430f, 88.508972f, 66.678841f, 0.0f  ,
     87 ,  0 ,  false ,  -9461.586914f, 87.903183f, 68.835785f, 0.0f  ,
     87 ,  0 ,  false ,  -9461.333008f, 83.322281f, 68.954262f, 0.0f  ,
     87 ,  0 ,  false ,  -9457.314453f, 83.163277f, 68.416641f, 0.0f  ,
     87 ,  0 ,  false ,  -9457.511719f, 87.481514f, 68.503716f, 0.0f  ,
     87 ,  0 ,  false ,  -9454.168945f, 87.684364f, 66.910706f, 0.0f  ,
     87 ,  0 ,  false ,  -9453.876953f, 84.042046f, 66.780327f, 0.0f  ,
     87 ,  0 ,  false ,  -9452.500977f, 90.110939f, 66.963150f, 0.0f  ,
     87 ,  0 ,  false ,  -9453.014648f, 97.334198f, 67.853394f, 0.0f  ,
     87 ,  0 ,  false ,  -9466.933594f, 97.318192f, 67.665657f, 0.0f  ,
     131 , 27 , false ,  -5578.172852f, -508.167419f, 404.096893f, 0.0f  ,
     131 ,  0 ,  false ,  -5577.978027f, -512.376343f, 404.096893f, 0.0f  ,
     131 ,  0 ,  false ,  -5578.129395f, -516.255676f, 404.097656f, 0.0f  ,
     131 ,  0 ,  false ,  -5578.432129f, -522.842163f, 404.096008f, 0.0f  ,
     131 ,  0 ,  false ,  -5579.825684f, -505.745697f, 404.096344f, 0.0f  ,
     131 ,  0 ,  false ,  -5584.710449f, -505.517365f, 404.096344f, 0.0f  ,
     131 ,  0 ,  false ,  -5585.525879f, -503.660217f, 413.178284f, 0.0f  ,
     131 ,  0 ,  false ,  -5582.077637f, -503.811554f, 413.373688f, 0.0f  ,
     131 ,  0 ,  false ,  -5577.631836f, -503.804688f, 413.265656f, 0.0f  ,
     131 ,  0 ,  false ,  -5576.027344f, -510.544647f, 413.286865f, 0.0f  ,
     131 ,  0 ,  false ,  -5576.213867f, -517.652893f, 413.300690f, 0.0f  ,
     131 ,  0 ,  false ,  -5592.137695f, -510.345764f, 413.121124f, 0.0f  ,
     131 ,  0 ,  false ,  -5596.859375f, -513.450806f, 413.181976f, 0.0f  ,
     131 ,  0 ,  false ,  -5602.766602f, -513.240051f, 413.275360f, 0.0f  ,
     131 ,  0 ,  false ,  -5609.043457f, -513.394104f, 413.381927f, 0.0f  ,
     131 ,  0 ,  false ,  -5588.285156f, -462.058838f, 414.503693f, 0.0f  ,
     131 ,  0 ,  false ,  -5588.110352f, -458.775116f, 414.375275f, 0.0f  ,
     131 ,  0 ,  false ,  -5588.123535f, -454.830292f, 414.161285f, 0.0f  ,
     131 ,  0 ,  false ,  -5583.522461f, -461.959167f, 414.408997f, 0.0f  ,
     131 ,  0 ,  false ,  -5577.516113f, -462.162292f, 414.318024f, 0.0f  ,
     131 ,  0 ,  false ,  -5572.210938f, -462.232544f, 414.228027f, 0.0f  ,
     131 ,  0 ,  false ,  -5570.547363f, -459.389496f, 414.081726f, 0.0f  ,
     131 ,  0 ,  false ,  -5570.349121f, -454.491058f, 413.896912f, 0.0f  ,
     131 ,  0 ,  false ,  -5573.891113f, -456.134583f, 403.206482f, 0.0f  ,
     131 ,  0 ,  false ,  -5584.926270f, -456.245667f, 403.206238f, 0.0f  ,
     131 ,  0 ,  false ,  -5582.571289f, -461.105408f, 402.603882f, 0.0f  ,
     131 ,  0 ,  false ,  -5576.388672f, -460.653290f, 402.603882f, 0.0f  ,
     3576 ,  28 ,  false ,  -4213.185547f, -12522.341797f, 49.787685f, 0.0f  ,
     3576 ,  0 ,  false ,  -4214.963867f, -12525.010742f, 50.883205f, 0.0f  ,
     3576 ,  0 ,  false ,  -4219.444336f, -12527.609375f, 50.422890f, 0.0f  ,
     3576 ,  0 ,  false ,  -4222.613281f, -12527.093750f, 54.527485f, 0.0f  ,
     3576 ,  0 ,  false ,  -4219.314941f, -12522.365234f, 55.642303f, 0.0f  ,
     3576 ,  0 ,  false ,  -4216.854492f, -12517.195313f, 55.820114f, 0.0f  ,
     3576 ,  0 ,  false ,  -4218.576660f, -12512.222656f, 53.843594f, 0.0f  ,
     3576 ,  0 ,  false ,  -4218.751465f, -12508.384766f, 48.981907f, 0.0f  ,
     3576 ,  0 ,  false ,  -4222.914063f, -12507.796875f, 49.336563f, 0.0f  ,
     3576 ,  0 ,  false ,  -4223.995117f, -12511.844727f, 53.319275f, 0.0f  ,
     3576 ,  0 ,  false ,  -4223.807129f, -12515.146484f, 56.371929f, 0.0f  ,
     3576 ,  0 ,  false ,  -4220.514160f, -12519.626953f, 57.187836f, 0.0f  ,
     3576 ,  0 ,  false ,  -4213.557129f, -12516.336914f, 54.507278f, 0.0f  ,
     3576 ,  0 ,  false ,  -4150.668457f, -12483.409180f, 51.244488f, 0.0f  ,
     3576 ,  0 ,  false ,  -4146.772461f, -12487.382813f, 51.631248f, 0.0f  ,
     3576 ,  0 ,  false ,  -4143.254883f, -12488.479492f, 48.431683f, 0.0f  ,
     3576 ,  0 ,  false ,  -4144.871094f, -12485.075195f, 55.429489f, 0.0f  ,
     3576 ,  0 ,  false ,  -4147.387207f, -12481.426758f, 57.274059f, 0.0f  ,
     3576 ,  0 ,  false ,  -4150.745605f, -12479.188477f, 48.477905f, 0.0f  ,
     3576 ,  0 ,  false ,  -4152.065430f, -12472.902344f, 50.862717f, 0.0f  ,
     3576 ,  0 ,  false ,  -4154.771973f, -12469.408203f, 52.474388f, 0.0f  ,
     3576 ,  0 ,  false ,  -4146.585938f, -12472.744141f, 54.687321f, 0.0f  ,
     3576 ,  0 ,  false ,  -4143.433105f, -12475.541992f, 58.953270f, 0.0f  ,
     3576 ,  0 ,  false ,  -4141.323242f, -12481.124023f, 59.377552f, 0.0f  ,
     3576 ,  0 ,  false ,  -4133.463379f, -12485.035156f, 53.983967f, 0.0f  ,
     3576 ,  0 ,  false ,  -4135.257813f, -12495.936523f, 51.745071f, 0.0f  ,
     3576 ,  0 ,  false ,  -4135.921387f, -12492.143555f, 48.833118f, 0.0f  ,
     3576 ,  0 ,  false ,  -4154.284180f, -12472.077148f, 51.846313f, 0.0f  ,
     362 ,  40 ,  false ,  336.829926f, -4706.948730f, 16.889151f, 0.0f  ,
     362 ,  0 ,  false ,  339.642456f, -4708.543945f, 18.006519f, 0.0f  ,
     362 ,  0 ,  false ,  342.224396f, -4710.393066f, 16.818239f, 0.0f  ,
     362 ,  0 ,  false ,  344.960907f, -4709.553711f, 16.890581f, 0.0f  ,
     362 ,  0 ,  false ,  347.107666f, -4707.392090f, 16.872906f, 0.0f  ,
     362 ,  0 ,  false ,  349.523041f, -4705.116699f, 16.835857f, 0.0f  ,
     362 ,  0 ,  false ,  351.904236f, -4702.746094f, 16.814392f, 0.0f  ,
     362 ,  0 ,  false ,  354.097717f, -4700.604004f, 16.788067f, 0.0f  ,
     362 ,  0 ,  false ,  356.199799f, -4698.549805f, 16.764526f, 0.0f  ,
     362 ,  0 ,  false ,  358.804993f, -4696.005371f, 16.734446f, 0.0f  ,
     362 ,  0 ,  false ,  360.704559f, -4694.149902f, 16.711086f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  332.730408f, -4712.049805f, 15.277740f, 0.0f  ,
     362 ,  0 ,  false ,  317.190308f, -4772.762695f, 27.700096f, 0.0f  ,
     362 ,  0 ,  false ,  313.450867f, -4773.237793f, 27.700096f, 0.0f  ,
     362 ,  0 ,  false ,  310.523895f, -4773.609863f, 27.700096f, 0.0f  ,
     362 ,  0 ,  false ,  307.617767f, -4773.979004f, 27.700096f, 0.0f  ,
     362 ,  0 ,  false ,  303.871460f, -4774.482422f, 27.700096f, 0.0f  ,
     362 ,  0 ,  false ,  300.950226f, -4774.166504f, 27.700096f, 0.0f  ,
     362 ,  0 ,  false ,  297.887360f, -4774.443848f, 27.699116f, 0.0f  ,
     362 ,  0 ,  false ,  310.588226f, -4777.041016f, 23.035875f, 0.0f  ,
     362 ,  0 ,  false ,  302.890533f, -4777.734375f, 22.160141f, 0.0f  ,
     362 ,  0 ,  false ,  303.526398f, -4783.147949f, 22.091099f, 0.0f  ,
     362 ,  0 ,  false ,  311.356537f, -4782.477051f, 22.357243f, 0.0f  ,
     362 ,  0 ,  false ,  311.864349f, -4786.790527f, 22.512373f, 0.0f  ,
     362 ,  0 ,  false ,  303.852966f, -4787.567871f, 22.226116f, 0.0f  ,
     362 ,  0 ,  false ,  304.246216f, -4792.124512f, 22.298147f, 0.0f  ,
     362 ,  0 ,  false ,  312.697449f, -4791.288574f, 22.231749f, 0.0f  ,
     362 ,  0 ,  false ,  284.312897f, -4721.923828f, 13.982411f, 0.0f  ,
     362 ,  0 ,  false ,  282.126221f, -4720.415527f, 14.315186f, 0.0f  ,
     362 ,  0 ,  false ,  279.237915f, -4718.960938f, 14.767025f, 0.0f  ,
     362 ,  0 ,  false ,  277.069824f, -4717.780273f, 16.000343f, 0.0f  ,
     362 ,  0 ,  false ,  276.343201f, -4715.700195f, 17.616024f, 0.0f  ,
     362 ,  0 ,  false ,  276.362915f, -4713.054688f, 18.524426f, 0.0f  ,
     362 ,  0 ,  false ,  278.565430f, -4710.165527f, 18.451063f, 0.0f  ,
     362 ,  0 ,  false ,  280.094330f, -4707.666504f, 18.503857f, 0.0f  ,
     362 ,  0 ,  false ,  282.835510f, -4707.406738f, 15.029071f, 0.0f  ,
     362 ,  0 ,  false ,  284.558929f, -4709.244141f, 14.899632f, 0.0f  ,
     362 ,  0 ,  false ,  287.173279f, -4711.354004f, 14.433277f, 0.0f  ,
     362 ,  0 ,  false ,  289.262390f, -4713.011719f, 14.084586f, 0.0f  ,
     362 ,  0 ,  false ,  283.003418f, -4701.726563f, 17.097305f, 0.0f  ,
     362 ,  0 ,  false ,  280.778656f, -4701.861816f, 19.083368f, 0.0f  ,
     362 ,  0 ,  false ,  270.202362f, -4720.975586f, 15.897130f, 0.0f  ,
     362 ,  0 ,  false ,  270.199463f, -4716.922363f, 19.472425f, 0.0f  ,
     362 ,  0 ,  false ,  274.486603f, -4719.081055f, 15.034330f, 0.0f  ,
     159 ,  59 ,  false ,  2274.941895f, 252.668365f, 41.622330f, 0.0f  ,
     159 ,  0 ,  false ,  2271.454346f, 253.745346f, 45.455288f, 0.0f  ,
     159 ,  0 ,  false ,  2267.237305f, 254.750198f, 45.298695f, 0.0f  ,
     159 ,  0 ,  false ,  2264.763184f, 254.484863f, 41.623123f, 0.0f  ,
     159 ,  0 ,  false ,  2262.695313f, 257.412903f, 41.721722f, 0.0f  ,
     159 ,  0 ,  false ,  2258.168701f, 258.099304f, 41.689083f, 0.0f  ,
     159 ,  0 ,  false ,  2256.925537f, 255.833481f, 41.622211f, 0.0f  ,
     159 ,  0 ,  false ,  2254.367920f, 256.765656f, 45.932198f, 0.0f  ,
     159 ,  0 ,  false ,  2250.811279f, 257.452820f, 45.191032f, 0.0f  ,
     159 ,  0 ,  false ,  2247.595703f, 257.532532f, 41.623463f, 0.0f  ,
     159 ,  0 ,  false ,  2244.849854f, 253.516251f, 41.308601f, 0.0f  ,
     159 ,  0 ,  false ,  2243.304199f, 252.771896f, 40.534016f, 0.0f  ,
     159 ,  0 ,  false ,  2240.819336f, 253.191071f, 40.551826f, 0.0f  ,
     159 ,  0 ,  false ,  2239.991211f, 249.324326f, 40.504696f, 0.0f  ,
     159 ,  0 ,  false ,  2242.890625f, 248.904922f, 40.592785f, 0.0f  ,
     159 ,  0 ,  false ,  2244.179443f, 248.268082f, 41.625122f, 0.0f  ,
     159 ,  0 ,  false ,  2243.473633f, 244.102371f, 41.616474f, 0.0f  ,
     159 ,  0 ,  false ,  2242.311768f, 240.774567f, 45.481342f, 0.0f  ,
     159 ,  0 ,  false ,  2241.650391f, 236.973892f, 45.417168f, 0.0f  ,
     159 ,  0 ,  false ,  2241.770020f, 234.239273f, 41.600002f, 0.0f  ,
     159 ,  0 ,  false ,  2240.255127f, 230.403503f, 46.864922f, 0.0f  ,
     159 ,  0 ,  false ,  2244.803223f, 229.592178f, 46.864922f, 0.0f  ,
     159 ,  0 ,  false ,  2251.706787f, 228.477722f, 46.864922f, 0.0f  ,
     159 ,  0 ,  false ,  2252.351074f, 232.585495f, 49.898830f, 0.0f  ,
     159 ,  0 ,  false ,  2241.279785f, 234.531128f, 49.734783f, 0.0f  ,
     159 ,  0 ,  false ,  2243.065918f, 243.725128f, 49.198402f, 0.0f  ,
     159 ,  0 ,  false ,  2247.495361f, 245.338654f, 48.451134f, 0.0f  ,
     159 ,  0 ,  false ,  2247.403320f, 249.132950f, 47.998882f, 0.0f  ,
     159 ,  0 ,  false ,  2248.053223f, 257.497223f, 47.268879f, 0.0f  ,
     159 ,  0 ,  false ,  2282.821289f, 275.469238f, 48.475018f, 0.0f  ,
     159 ,  0 ,  false ,  2287.594482f, 273.995117f, 48.269260f, 0.0f  ,
     159 ,  0 ,  false ,  2294.277100f, 272.058746f, 48.804775f, 0.0f  ,
     159 ,  0 ,  false ,  2295.394043f, 266.628235f, 50.173767f, 0.0f  ,
     159 ,  0 ,  false ,  2293.394531f, 261.493561f, 49.654194f, 0.0f  ,
     159 ,  0 ,  false ,  2285.579102f, 281.685547f, 49.144688f, 0.0f  ,
     159 ,  0 ,  false ,  2286.483154f, 288.617188f, 47.933754f, 0.0f  ,
     159 ,  0 ,  false ,  2290.901367f, 286.164734f, 49.143620f, 0.0f  ,
     159 ,  0 ,  false ,  2300.947021f, 296.703705f, 48.676743f, 0.0f  ,
     159 ,  0 ,  false ,  2302.537354f, 301.321167f, 48.746048f, 0.0f  ,
     159 ,  0 ,  false ,  2304.679688f, 307.534210f, 49.174549f, 0.0f  ,
     159 ,  0 ,  false ,  2262.712646f, 322.167633f, 40.868729f, 0.0f  ,
     159 ,  0 ,  false ,  2259.568115f, 324.951965f, 42.353420f, 0.0f  ,
     159 ,  0 ,  false ,  2256.455322f, 319.867920f, 42.218792f, 0.0f  ,
     159 ,  0 ,  false ,  2258.497559f, 318.102112f, 41.262493f, 0.0f  ,
     159 ,  0 ,  false ,  2251.768066f, 321.050323f, 45.584774f, 0.0f  ,
     159 ,  0 ,  false ,  2248.273682f, 318.227020f, 45.933731f, 0.0f  ,
     159 ,  0 ,  false ,  2244.188965f, 313.004700f, 44.313381f, 0.0f  ,
     159 ,  0 ,  false ,  2240.801514f, 314.274811f, 44.793324f, 0.0f  ,
     159 ,  0 ,  false ,  2240.160645f, 309.061005f, 46.732929f, 0.0f  ,
     159 ,  0 ,  false ,  2237.880615f, 304.928070f, 46.984474f, 0.0f  ,
     159 ,  0 ,  false ,  2235.998047f, 307.378815f, 47.289276f, 0.0f  ,
     159 ,  0 ,  false ,  2232.723389f, 308.740234f, 46.884659f, 0.0f  ,
     159 ,  0 ,  false ,  2231.217529f, 305.549561f, 45.195446f, 0.0f  ,
     159 ,  0 ,  false ,  2235.197998f, 300.963928f, 44.766743f, 0.0f  ,
     159 ,  0 ,  false ,  2258.797119f, 332.812012f, 45.993340f, 0.0f  ,
     159 ,  0 ,  false ,  2261.823242f, 338.828125f, 44.637466f, 0.0f  ,
     159 ,  0 ,  false ,  2266.796143f, 339.480469f, 42.030144f, 0.0f  ,
     159 ,  0 ,  false ,  2269.283691f, 344.173828f, 41.916660f, 0.0f  ,
     159 ,  0 ,  false ,  2267.883789f, 348.258240f, 43.713596f, 0.0f  ,
     3665 ,  32 ,  false ,  9538.890625f, -6864.580566f, 32.180435f, 0.0f  ,
     3665 ,  0 ,  false ,  9545.317383f, -6864.680664f, 24.290354f, 0.0f  ,
     3665 ,  0 ,  false ,  9539.849609f, -6863.418945f, 21.899117f, 0.0f  ,
     3665 ,  0 ,  false ,  9534.633789f, -6860.602539f, 20.969927f, 0.0f  ,
     3665 ,  0 ,  false ,  9536.678711f, -6862.365234f, 32.320389f, 0.0f  ,
     3665 ,  0 ,  false ,  9531.854492f, -6862.107910f, 33.342640f, 0.0f  ,
     3665 ,  0 ,  false ,  9527.215820f, -6861.431641f, 32.332523f, 0.0f  ,
     3665 ,  0 ,  false ,  9521.734375f, -6861.298340f, 33.342361f, 0.0f  ,
     3665 ,  0 ,  false ,  9516.587891f, -6860.852539f, 32.162258f, 0.0f  ,
     3665 ,  0 ,  false ,  9513.078125f, -6864.157227f, 32.287502f, 0.0f  ,
     3665 ,  0 ,  false ,  9511.090820f, -6862.834473f, 22.868782f, 0.0f  ,
     3665 ,  0 ,  false ,  9517.139648f, -6861.322266f, 20.973946f, 0.0f  ,
     3665 ,  0 ,  false ,  9512.208008f, -6855.778809f, 23.205914f, 0.0f  ,
     3665 ,  0 ,  false ,  9510.145508f, -6851.798828f, 23.008202f, 0.0f  ,
     3665 ,  0 ,  false ,  9505.134766f, -6848.893066f, 23.350080f, 0.0f  ,
     3665 ,  0 ,  false ,  9497.835938f, -6848.266113f, 23.375212f, 0.0f  ,
     3665 ,  0 ,  false ,  9495.145508f, -6849.114746f, 31.737856f, 0.0f  ,
     3665 ,  0 ,  false ,  9494.801758f, -6845.622559f, 32.778965f, 0.0f  ,
     3665 ,  0 ,  false ,  9492.223633f, -6842.419922f, 34.687244f, 0.0f  ,
     3665 ,  0 ,  false ,  9488.187500f, -6838.931152f, 32.722904f, 0.0f  ,
     3665 ,  0 ,  false ,  9485.586914f, -6836.182617f, 33.134155f, 0.0f  ,
     3665 ,  0 ,  false ,  9483.424805f, -6833.842285f, 33.123482f, 0.0f  ,
     3665 ,  0 ,  false ,  9478.472656f, -6833.402832f, 31.652319f, 0.0f  ,
     3665 ,  0 ,  false ,  9479.270508f, -6833.205566f, 23.415434f, 0.0f  ,
     3665 ,  0 ,  false ,  9480.656250f, -6829.862793f, 23.495510f, 0.0f  ,
     3665 ,  0 ,  false ,  9480.732422f, -6824.777832f, 23.296629f, 0.0f  ,
     3665 ,  0 ,  false ,  9478.640625f, -6820.736328f, 23.355219f, 0.0f  ,
     3665 ,  0 ,  false ,  9475.358398f, -6818.134277f, 23.355270f, 0.0f  ,
     3665 ,  0 ,  false ,  9472.142578f, -6816.050781f, 23.065981f, 0.0f  ,
     3665 ,  0 ,  false ,  9482.250977f, -6833.041992f, 17.442104f, 0.0f  ,
     3665 ,  0 ,  false ,  9485.652344f, -6835.646973f, 17.438766f, 0.0f  ,
     3665 ,  0 ,  false ,  9491.399414f, -6842.145508f, 17.439112f, 0.0f  ,
     3665 ,  0 ,  false ,  9495.438477f, -6846.233398f, 17.441633f, 0.0f  ,
     0 ,  0 ,  false ,  0.0f, 0.0f, 0.0f, 0.0f  ,
};

enum HallowendFire
{
    NPC_HEADLESS_HORSEMAN_FIRE_DND          = 23537,
    NPC_SHADE_OF_THE_HORSEMAN               = 23543,
    SPELL_HEADLES_HORSEMAN_QUEST_CREDIT     = 42242,
    SPELL_HEADLESS_HORSEMAN_START_FIRE      = 42132,
    SPELL_BUCKET_LANDS                      = 42339,
    SPELL_HEADLESS_HORSEMAN_FIRE_EXTINGUISH = 42348,
    SPELL_HEADLESS_HORSEMAN_LARGE_JACK      = 44231,
    // RangoFire
    SPELL_HEADLESS_HORSEMAN_BURNING         = 42971,
    SPELL_HEADLESS_HORSEMAN_FIRE            = 42074,
    SPELL_HEADLESS_HORSEMAN_VISUAL_FIRE     = 42075,
    // http://old.wowhead.com/object=186887
    GO_LARGE_JACK                           = 186887,
};

#define ACTION_FAIL_EVENT       1
#define ACTION_START_EVENT      2
#define ACTION_PASS_EVENT       2
#define ACTION_SAY_1            3
#define ACTION_SAY_2            4
#define HALLOWEND_FIRE_ADD      1
#define HALLOWEND_FIRE_REMOVE   2

class npc_hallowend : public CreatureScript
{
public:
    npc_hallowend() : CreatureScript("npc_hallowend") { }
    
    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_hallowendAI(creature);
    }

    struct npc_hallowendAI : public ScriptedAI
    {
        npc_hallowendAI(Creature* c) : ScriptedAI(c), Fires(c) 
        {
            Area = me->GetAreaId();
            AreaFire = 0;
            while (PostionEventoHallowends[AreaFire].Area && PostionEventoHallowends[AreaFire].Area != Area)
                AreaFire++;
			me->SetVisible(false);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
            for (uint8 j = 0; j < PostionEventoHallowends[AreaFire].Area_Count; j++)
            {
                Creature *summon = me->SummonCreature(NPC_HEADLESS_HORSEMAN_FIRE_DND,PostionEventoHallowends[AreaFire + j].SpawnPosition)->ToCreature();
                if (summon)
                {
                    Fires.Summon(summon);
                    summon->AI()->SetData(0,AreaFire + j);
                }
            }
        }
        
        uint32 TimerBegin;
        uint32 TimerDuration;
        uint32 Area;
        uint32 CountPlayersEvent;
        uint8 AreaFire;
        SummonList Fires;
        bool EventProgress;
        bool SaidPhrase[2];

        void Reset()
        {
            CountPlayersEvent = 0;
            EventProgress = false;
        }
        
        uint32 GetData(uint32 )
        {
            return CountPlayersEvent;
        }

        uint32 PlayersCountRange(float dist) const
        {
            std::list<Player*> players;
            Trinity::AnyPlayerInObjectRangeCheck checker(me, dist);
            Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
            me->VisitNearbyWorldObject(dist, searcher);

            return players.size();
        }
        
        void EventComplete(float dist) const
        {
            std::list<Player*> players;
            Trinity::AnyPlayerInObjectRangeCheck checker(me, dist);
            Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
            me->VisitNearbyWorldObject(dist, searcher);
            me->CastSpell(me->GetPositionX(),me->GetPositionY(), me->GetPositionZ()+1,SPELL_HEADLESS_HORSEMAN_LARGE_JACK,true);
            for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                (*itr)->CastSpell((*itr),SPELL_HEADLES_HORSEMAN_QUEST_CREDIT,true);
        }

        void SetData(uint32 uiType, uint32 uiData)
        {
            switch (uiType)
            {
                case HALLOWEND_FIRE_REMOVE:
                    PostionEventoHallowends[uiData].AlreadyFired = false;
                    bool EventPassed = true;
                    for (uint8 j = 0; j < PostionEventoHallowends[AreaFire].Area_Count; j++) 
                        if (PostionEventoHallowends[AreaFire + j].AlreadyFired)
                            EventPassed = false;
                    if (EventPassed)
                        EventEnd(EventPassed);
                    break;
            }
        }

        void EventBegin()
        {
            CountPlayersEvent = PlayersCountRange(100.0f);

            if (!CountPlayersEvent)
                return;
            EventProgress = true;
            TimerDuration = 400*IN_MILLISECONDS;
            for (uint8 j = 0; j < PostionEventoHallowends[AreaFire].Area_Count; j++) 
                PostionEventoHallowends[AreaFire + j].AlreadyFired = false;
            for (uint8 i = 0; i < 2; i++)
                SaidPhrase[i] = false;
            EntryCheckPredicate pred(NPC_HEADLESS_HORSEMAN_FIRE_DND);
            Fires.DoAction(ACTION_START_EVENT, pred);            
            //Fires.DoAction(NPC_HEADLESS_HORSEMAN_FIRE_DND, ACTION_START_EVENT);
            Creature *summon = me->SummonCreature(NPC_SHADE_OF_THE_HORSEMAN,0,0,0)->ToCreature();
            if (summon) 
                Fires.Summon(summon);
        }
        
        void EventEnd(bool EventPassed = false) 
        {
            if (!EventPassed) 
            {
			//EntryCheckPredicate pred(NPC_HEADLESS_HORSEMAN_FIRE_DND);
            //Fires.DoAction(ACTION_FAIL_EVENT, pred);
			//EntryCheckPredicate pred(NPC_SHADE_OF_THE_HORSEMAN);
            //Fires.DoAction(ACTION_FAIL_EVENT, pred);
				Fires.DAction(NPC_HEADLESS_HORSEMAN_FIRE_DND, ACTION_FAIL_EVENT);
				Fires.DAction(NPC_SHADE_OF_THE_HORSEMAN, ACTION_FAIL_EVENT);
            }
            else 
            {
                EventComplete(100.0f);
            EntryCheckPredicate pred(NPC_SHADE_OF_THE_HORSEMAN);
            Fires.DoAction(ACTION_PASS_EVENT, pred);
                //Fires.DoAction(NPC_SHADE_OF_THE_HORSEMAN, ACTION_PASS_EVENT);
            }
            EventProgress = false;
        }
        
        void UpdateAI(const uint32 diff) 
        {
            if (!IsHolidayActive(HOLIDAY_HALLOWS_END))
                return;

                // The event begins each 1 / 4 of an hour. if during this process no one is around (10 seconds), the event began
                // Time Data works.
                // Example one hour 60 minutes we are starting every 15 minutes is divided in 4 (60 / 4) and this is therefore in seconds multiplied by 60 ((60 / 4) * 60)
            if (!EventProgress && (time(NULL)%900 < 5 || (900 - time(NULL)%900) < 5))
                EventBegin();

            if (EventProgress) 
            {
                if (!SaidPhrase[0])
                    if (TimerDuration <= 280*IN_MILLISECONDS)
                    {
                        EntryCheckPredicate pred(NPC_SHADE_OF_THE_HORSEMAN);
                        Fires.DoAction(ACTION_SAY_1, pred);                    
                        //Fires.DoAction(NPC_SHADE_OF_THE_HORSEMAN, ACTION_SAY_1);
                        SaidPhrase[0] = true;
                    } else 
                        TimerDuration -= diff;
                else 
                    if (!SaidPhrase[1])
                        if (TimerDuration <= 130*IN_MILLISECONDS)
                        {
                        EntryCheckPredicate pred(NPC_SHADE_OF_THE_HORSEMAN);
                        Fires.DoAction(ACTION_SAY_2, pred);  
                            //Fires.DoAction(NPC_SHADE_OF_THE_HORSEMAN, ACTION_SAY_2);
                            SaidPhrase[1] = true;
                        } else 
                            TimerDuration -= diff;
                    else 
                        if (TimerDuration <= diff)
                        { 
                            EventEnd();
                        } else TimerDuration -= diff;
            }
        }
    }; 
};


/*######
## http://www.wowhead.com/object=180574
## Wickerman Ember
## go_wickerman_ember
######*/

#define GOSSIP_WICKERMAN_SPELL "Smear the on my face like war paint!"

enum WickermanEmberGo
{
    SPELL_GRIM_VISAGE   = 24705,
};

class go_wickerman_ember : public GameObjectScript
{
public:
    go_wickerman_ember() : GameObjectScript("go_wickerman_ember") { }

    bool OnGossipHello(Player* player, GameObject* pGO)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_WICKERMAN_SPELL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(pGO), pGO->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject* pGO, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if (player->HasAura(SPELL_GRIM_VISAGE))
        {
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(pGO), pGO->GetGUID());
            player->CLOSE_GOSSIP_MENU();
            
        }
        else if (uiAction == GOSSIP_ACTION_INFO_DEF +1)
        {
            pGO->CastSpell(player, SPELL_GRIM_VISAGE);
            player->CLOSE_GOSSIP_MENU();
        }
        return true;
    }
};

enum WickermanGuardian
{
    SPELL_KNOCKDOWN                = 19128,
    SPELL_STRIKE                   = 18368,
    SPELL_WICKERMAN_GUARDIAN_EMBER = 25007
};

class npc_wickerman_guardian : public CreatureScript
{
public:
    npc_wickerman_guardian() : CreatureScript("npc_wickerman_guardian") { }

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_wickerman_guardianAI(creature);
    }

    struct npc_wickerman_guardianAI : public ScriptedAI
    {
        npc_wickerman_guardianAI(Creature* creature) : ScriptedAI(creature) { }
 
        uint32 KnockdownTimer;
        uint32 StrikeTimer;
 
        void Reset()
        {
            KnockdownTimer = 7000;
            StrikeTimer = 10000;
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            if (KnockdownTimer <= diff)
            {
                DoCast(me->getVictim(),SPELL_KNOCKDOWN);
                KnockdownTimer = 7000;
            }else KnockdownTimer -= diff;

            if (StrikeTimer <= diff)
            {
                DoCast(me->getVictim(),SPELL_STRIKE);
                StrikeTimer = 10000;
            }else StrikeTimer -= diff;
            
            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* killer)
        {
            DoCast(killer, SPELL_WICKERMAN_GUARDIAN_EMBER, true);
        }
    };
};

enum hallowen
{
    QUEST_INCOMING_GUMDROP  = 8358,
    EMOTE_TRAIN             = 264,
};

class npc_kali_remik : public CreatureScript
{
public:
    npc_kali_remik() : CreatureScript("npc_kali_remik") { }

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_kali_remikAI(creature);
    }

    struct npc_kali_remikAI : public ScriptedAI
    {
        npc_kali_remikAI(Creature* c) : ScriptedAI(c) {}

        void ReceiveEmote(Player* player, uint32 emote)
        {
            if (!IsHolidayActive(HOLIDAY_HALLOWS_END))
                return;

            if (player->GetQuestStatus(QUEST_INCOMING_GUMDROP) == QUEST_STATUS_INCOMPLETE && emote == EMOTE_TRAIN)
                player->KilledMonsterCredit(me->GetEntry(),0);
        }
    };
};

// http://www.wowhead.com/npc=23537
// Headless Horseman - Fire (DND)
class npc_headless_horseman_fire : public CreatureScript
{
public:
    npc_headless_horseman_fire() : CreatureScript("npc_headless_horseman_fire") { }

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_headless_horseman_fireAI(creature);
    }

    struct npc_headless_horseman_fireAI : public ScriptedAI
    {
        npc_headless_horseman_fireAI(Creature* c) : ScriptedAI(c)
        {
            RangoFire[0] = SPELL_HEADLESS_HORSEMAN_BURNING;
            RangoFire[1] = SPELL_HEADLESS_HORSEMAN_FIRE;
            RangoFire[2] = SPELL_HEADLESS_HORSEMAN_VISUAL_FIRE;
            Immuned = true;
            Fire = false;
        }

        uint32 RangoFire[3];
        uint32 Ui_ID;
        uint32 RateFire;
        uint32 IncreaseFireTimer;
        bool Fire;
        bool Immuned;
        uint8 curRangoFire;

        void SetData(uint32 type, uint32 uiId)
        {
            Ui_ID = uiId;
        }

        uint32 GetData(uint32 )
        {
            return Ui_ID;
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_FAIL_EVENT:
                    Immuned = true;
                    break;
                case ACTION_START_EVENT:
                    Immuned = false;
                    Fire = false;
                    RateFire = 0;
                    curRangoFire = 0;
                    me->RemoveAllAuras();
                    break;
            }
        }

        uint32 PlayersCountRange(float dist) const
        {
            std::list<Player*> players;
            Trinity::AnyPlayerInObjectRangeCheck checker(me, dist);
            Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
            me->VisitNearbyWorldObject(dist, searcher);

            return players.size();
        }

        Creature * FireNodeNext()
        {
           std::list<Creature*> FireNodsList;
           GetCreatureListWithEntryInGrid(FireNodsList, me, NPC_HEADLESS_HORSEMAN_FIRE_DND, 15.0f);
           
           if (!FireNodsList.empty())
           {
               FireNodsList.sort(Trinity::ObjectDistanceOrderPred(me));

               for (std::list<Creature*>::const_iterator itr = FireNodsList.begin(); itr != FireNodsList.end(); ++itr)
                   if (Creature* pNodeFire = *itr) 
                       if (!PostionEventoHallowends[pNodeFire && pNodeFire->AI()->GetData(0)].AlreadyFired)
                           return pNodeFire;
           }
           return NULL;
        }

        void SpellHit(Unit* /*caster*/, SpellEntry const* spell)
        {
            if (spell->Id == SPELL_BUCKET_LANDS && Fire && !Immuned) 
            {

                if (PlayersCountRange(5.0f))
                return;

                me->CastSpell(me,SPELL_HEADLESS_HORSEMAN_FIRE_EXTINGUISH,true);
                me->RemoveAura(RangoFire[curRangoFire]);

                if (curRangoFire) 
                {
                    curRangoFire--;
                    me->AddAura(RangoFire[curRangoFire],me);
                }
                else 
                {
                    if (me->isSummon())
                    if (Unit * pEventKeeper =  me->ToTempSummon()->GetSummoner())
                        pEventKeeper->ToCreature()->AI()->SetData(HALLOWEND_FIRE_REMOVE, Ui_ID);
                    Fire = false;
                }
                return;
            }
            if (spell->Id == SPELL_HEADLESS_HORSEMAN_START_FIRE) 
            {
                me->AddAura(RangoFire[0], me);
                PostionEventoHallowends[Ui_ID].AlreadyFired = true;
            }
        }
   
        void UpdateAI(uint32 const diff)
        {
            if (me->HasAura(RangoFire[0]) || Fire)
            {
                if (me->HasAura(RangoFire[0]) && !Fire)
                {
                    if (!RateFire)
                        if (Unit * Owner = me->ToTempSummon()->GetSummoner())
                            RateFire = Owner->ToCreature()->AI()->GetData(0);
                        else 
                            return;
                    if (!RateFire)
                        return;
                    Fire = true;
                    IncreaseFireTimer = 60000 / RateFire;
                } else
                    if (IncreaseFireTimer <= diff)
                    {
                        if (curRangoFire < 2)
                        {
                            me->RemoveAura(RangoFire[curRangoFire]);
                            curRangoFire++;
                            me->AddAura(RangoFire[curRangoFire],me);
                        } else
                            if (Creature * nextFireNode = FireNodeNext())
                            {
                                nextFireNode->AddAura(RangoFire[0],nextFireNode);
                                PostionEventoHallowends[nextFireNode->AI()->GetData(0)].AlreadyFired = true;
                            }
                            IncreaseFireTimer = 60000 / RateFire;
                    } else
                        IncreaseFireTimer -= diff;
            }
        }
    };
};

// http://www.wowhead.com/npc=23543
// Shade of the Horseman

struct WaypointsShadeOfTheHorsemans
{
    uint32 area;
    bool CastPoint;
    Position waypoint;
}

WaypointsShadeOfTheHorsemans[] =
{
      87 ,  false ,  -9449.220703f, 99.542000f, 80.433723f, 0.0f  ,
      87 ,  false ,  -9487.250977f, 96.086182f, 76.224045f, 0.0f  ,
      87 ,  false ,  -9505.733398f, 85.762619f, 78.615189f, 0.0f  ,
      87 ,  false ,  -9515.541016f, 69.045929f, 78.518990f, 0.0f  ,
      87 ,  false ,  -9506.210938f, 49.220688f, 74.766258f, 0.0f  ,
      87 ,  false ,  -9493.707031f, 52.980251f, 73.841759f, 0.0f  ,
      87 ,  false ,  -9466.496094f, 59.418045f, 68.635506f, 0.0f  ,
      87 ,  true ,  -9451.565430f, 60.701469f, 69.817856f, 0.0f  ,
      87 ,  false ,  -9433.534180f, 56.966469f, 69.535995f, 0.0f  ,
      87 ,  false ,  -9426.815430f, 37.612507f, 69.721375f, 0.0f  ,
      87 ,  false ,  -9428.989258f, 20.016560f, 71.739769f, 0.0f  ,
      131 ,  false ,  -5549.735352f, -515.127075f, 417.435242f, 0.0f  ,
      131 ,  false ,  -5563.625977f, -494.025787f, 408.276031f, 0.0f  ,
      131 ,  false ,  -5576.229492f, -484.620575f, 407.339783f, 0.0f  ,
      131 ,  false ,  -5598.328613f, -481.467041f, 405.901550f, 0.0f  ,
      131 ,  false ,  -5618.585449f, -484.472595f, 405.008942f, 0.0f  ,
      131 ,  false ,  -5638.980469f, -484.686829f, 403.672089f, 0.0f  ,
      131 ,  false ,  -5653.769531f, -490.112793f, 404.059021f, 0.0f  ,
      131 ,  false ,  -5648.979492f, -507.761230f, 413.465271f, 0.0f  ,
      131 ,  false ,  -5633.484863f, -515.791504f, 415.236389f, 0.0f  ,
      131 ,  false ,  -5619.904297f, -504.694641f, 412.783020f, 0.0f  ,
      131 ,  true ,  -5599.156738f, -492.679260f, 408.886597f, 0.0f  ,
      131 ,  false ,  -5581.895020f, -482.131866f, 411.817047f, 0.0f  ,
      131 ,  false ,  -5551.732910f, -464.940369f, 418.337616f, 0.0f  ,
      3576 ,  false ,  -4181.044922f, -12545.703125f, 58.406044f, 0.0f  ,
      3576 ,  false ,  -4184.859863f, -12545.989258f, 66.531494f, 0.0f  ,
      3576 ,  false ,  -4162.623535f, -12534.581055f, 60.958271f, 0.0f  ,
      3576 ,  false ,  -4138.957031f, -12509.273438f, 58.346157f, 0.0f  ,
      3576 ,  false ,  -4145.405762f, -12485.465820f, 61.571053f, 0.0f  ,
      3576 ,  false ,  -4170.041016f, -12472.727539f, 62.661823f, 0.0f  ,
      3576 ,  false ,  -4177.905273f, -12487.194336f, 60.646717f, 0.0f  ,
      3576 ,  true ,  -4179.975098f, -12502.522461f, 55.405643f, 0.0f  ,
      3576 ,  false ,  -4187.343750f, -12508.174805f, 56.180138f, 0.0f  ,
      3576 ,  false ,  -4208.107422f, -12512.329102f, 59.904762f, 0.0f  ,
      362 ,  false ,  275.410431f, -4664.126465f, 31.811525f, 0.0f  ,
      362 ,  false ,  289.195679f, -4688.268066f, 28.616646f, 0.0f  ,
      362 ,  true ,  313.071777f, -4734.405762f, 27.163692f, 0.0f  ,
      362 ,  false ,  336.558136f, -4735.292480f, 25.536400f, 0.0f  ,
      362 ,  false ,  364.464661f, -4734.240723f, 23.702780f, 0.0f  ,
      159 ,  false ,  2288.239014f, 370.882111f, 52.932304f, 0.0f  ,
      159 ,  false ,  2283.253174f, 357.807556f, 52.963966f, 0.0f  ,
      159 ,  false ,  2266.459473f, 319.605682f, 52.030453f, 0.0f  ,
      159 ,  true ,  2247.472656f, 284.688538f, 47.141811f, 0.0f  ,
      159 ,  false ,  2241.526123f, 277.526276f, 47.969597f, 0.0f  ,
      159 ,  false ,  2201.554688f, 251.503479f, 52.721680f, 0.0f  ,
      3665 ,  false ,  9553.414063f, -6814.344727f, 48.652557f, 0.0f  ,
      3665 ,  false ,  9545.339844f, -6787.827637f, 44.812103f, 0.0f  ,
      3665 ,  false ,  9524.579102f, -6766.849609f, 41.753448f, 0.0f  ,
      3665 ,  false ,  9502.037109f, -6770.908203f, 40.224274f, 0.0f  ,
      3665 ,  false ,  9486.791992f, -6789.646973f, 38.460819f, 0.0f  ,
      3665 ,  true ,  9505.909180f, -6822.906250f, 36.336525f, 0.0f  ,
      3665 ,  false ,  9530.319336f, -6826.905273f, 33.300465f, 0.0f  ,
      3665 ,  false ,  9546.786133f, -6818.094727f, 34.542740f, 0.0f  ,
      3665 ,  false ,  9550.537109f, -6816.340332f, 34.288609f, 0.0f  ,
      0 ,  false ,  0.0f, 0.0f, 0.0f, 0.0f  
};

class npc_shade_of_the_horseman : public CreatureScript
{
public:
    npc_shade_of_the_horseman() : CreatureScript("npc_shade_of_the_horseman") { }

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_shade_of_the_horsemanAI(creature);
    }
    
    struct npc_shade_of_the_horsemanAI : public ScriptedAI
    {
        npc_shade_of_the_horsemanAI(Creature* c) : ScriptedAI(c)
        {
            PointFisrtArea = 0;
            area = me->GetAreaId();
            while (WaypointsShadeOfTheHorsemans[PointFisrtArea].area && WaypointsShadeOfTheHorsemans[PointFisrtArea].area != area)
                PointFisrtArea++;
            TimerEventUI = 400*IN_MILLISECONDS;
            wp_reached = true;
            FlyMode();
            MovementFinished = false;
        }

        uint32 TimerEventUI;
        uint32 CountFire;
        uint32 PointCur;
        uint32 area;
        uint32 PointFisrtArea;
        bool wp_reached;
        bool MovementFinished;

        void FlyMode()
        {
			me->SetVisible(true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT | MOVEMENTFLAG_DISABLE_GRAVITY);
            me->Mount(25159);
            me->SetSpeed(MOVE_WALK, 5.0f, true);
            PointCur = 0;
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_FAIL_EVENT:
                    me->PlayDirectSound(11967);
                    me->DisappearAndDie();
                    break;
                case ACTION_PASS_EVENT:
                    me->PlayDirectSound(11968);
                    me->DisappearAndDie();
                    break;
                case ACTION_SAY_1:
                    me->PlayDirectSound(12570);
                    break;
                case ACTION_SAY_2:
                    me->PlayDirectSound(12571);
                    break;
            }
        }

        void CreateFires()
        {
            std::list<Creature*> FireNodsList;
            FireNodsList.clear();
            if (Unit * Owner = me->ToTempSummon()->GetSummoner())
                CountFire = Owner->ToCreature()->AI()->GetData(0);
            GetCreatureListWithEntryInGrid(FireNodsList, me, NPC_HEADLESS_HORSEMAN_FIRE_DND, 150.0f);
            uint32 CountFireer = 0;
            if (!FireNodsList.empty())
            {
                while (CountFireer <= CountFire)
                {
                    std::list<Creature*>::iterator itr = FireNodsList.begin();
                    std::advance(itr, urand(0, FireNodsList.size()-1));
                    me->CastSpell((*itr),SPELL_HEADLESS_HORSEMAN_START_FIRE,true);
                    CountFireer++;
                    FireNodsList.erase(itr);
                }
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;
            wp_reached = true;
            if (id == 0) 
			me->SetVisible(true);

            if (WaypointsShadeOfTheHorsemans[PointFisrtArea + PointCur].CastPoint) 
            {
                CreateFires();
                me->PlayDirectSound(11966);
            }

            PointCur++;
            if (WaypointsShadeOfTheHorsemans[PointFisrtArea + PointCur].area != area) 
            {
                MovementFinished = true;
				me->SetVisible(false);
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (wp_reached && !MovementFinished)
            {
                wp_reached = false;
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MovePoint(PointCur,WaypointsShadeOfTheHorsemans[PointFisrtArea + PointCur].waypoint);
            }
        }
    };
};

// http://www.wowhead.com/item=20413
enum HalloweenWand
{
    SPELL_PIRATE_COSTUME_MALE           = 24708,
    SPELL_PIRATE_COSTUME_FEMALE         = 24709,
    SPELL_NINJA_COSTUME_MALE            = 24711,
    SPELL_NINJA_COSTUME_FEMALE          = 24710,
    SPELL_LEPER_GNOME_COSTUME_MALE      = 24712,
    SPELL_LEPER_GNOME_COSTUME_FEMALE    = 24713,
    SPELL_GHOST_COSTUME_MALE            = 24735,
    SPELL_GHOST_COSTUME_FEMALE          = 24736,

    SPELL_HALLOWEEN_WAND_PIRATE         = 24717,
    SPELL_HALLOWEEN_WAND_NINJA          = 24718,
    SPELL_HALLOWEEN_WAND_LEPER_GNOME    = 24719,
    SPELL_HALLOWEEN_WAND_RANDOM         = 24720,
    SPELL_HALLOWEEN_WAND_SKELETON       = 24724,
    SPELL_HALLOWEEN_WAND_WISP           = 24733,
    SPELL_HALLOWEEN_WAND_GHOST          = 24737,
    SPELL_HALLOWEEN_WAND_BAT            = 24741,
};

class spell_halloween_wand : public SpellScriptLoader
{
public:
    spell_halloween_wand() : SpellScriptLoader("spell_halloween_wand") {}

    class spell_halloween_wand_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_halloween_wand_SpellScript)

        bool Validate(SpellEntry const* /*spellEntry*/)
        {
            if (!sSpellStore.LookupEntry(SPELL_PIRATE_COSTUME_MALE))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_PIRATE_COSTUME_FEMALE))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_NINJA_COSTUME_MALE))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_NINJA_COSTUME_FEMALE))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_LEPER_GNOME_COSTUME_MALE))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_LEPER_GNOME_COSTUME_FEMALE))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_GHOST_COSTUME_MALE))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_GHOST_COSTUME_FEMALE))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_HALLOWEEN_WAND_NINJA))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_HALLOWEEN_WAND_LEPER_GNOME))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_HALLOWEEN_WAND_RANDOM))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_HALLOWEEN_WAND_SKELETON))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_HALLOWEEN_WAND_WISP))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_HALLOWEEN_WAND_GHOST))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_HALLOWEEN_WAND_BAT))
                return false;
            return true;
        }

        void HandleScriptEffect()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitPlayer();

            if (!caster || !target)
                return;

            uint32 spellId = 0;
            uint8 gender = target->getGender();

            switch (GetSpellInfo()->Id)
            {
                case SPELL_HALLOWEEN_WAND_LEPER_GNOME:
                    spellId = gender ? SPELL_LEPER_GNOME_COSTUME_FEMALE : SPELL_LEPER_GNOME_COSTUME_MALE;
                    break;
                case SPELL_HALLOWEEN_WAND_PIRATE:
                    spellId = gender ? SPELL_PIRATE_COSTUME_FEMALE : SPELL_PIRATE_COSTUME_MALE;
                    break;
                case SPELL_HALLOWEEN_WAND_GHOST:
                    spellId = gender ? SPELL_GHOST_COSTUME_FEMALE : SPELL_GHOST_COSTUME_MALE;
                    break;
                case SPELL_HALLOWEEN_WAND_NINJA:
                    spellId = gender ? SPELL_NINJA_COSTUME_FEMALE : SPELL_NINJA_COSTUME_MALE;
                    break;
                case SPELL_HALLOWEEN_WAND_RANDOM:
                    spellId = RAND(SPELL_HALLOWEEN_WAND_PIRATE, SPELL_HALLOWEEN_WAND_NINJA, SPELL_HALLOWEEN_WAND_LEPER_GNOME, SPELL_HALLOWEEN_WAND_SKELETON, SPELL_HALLOWEEN_WAND_WISP, SPELL_HALLOWEEN_WAND_GHOST, SPELL_HALLOWEEN_WAND_BAT);
                    break;
            }
            caster->CastSpell(target, spellId, true);
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_halloween_wand_SpellScript::HandleScriptEffect);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_halloween_wand_SpellScript();
    }
};


enum icorebossSpells
{
	SPELL_SHADOW_BOLT_VOLLEY2    = 36736,
	SPELL_SHADOW_BOLT                   = 69068,
	SPELL_ARMY_OF_THE_DEAD         = 67874,
	SPELL_MIND_CONTROL                  = 67229,
	SPELL_PIERCING_SHADOW 	          = 36698
};

// yells
#define SAY_ICORE_BOSS_AGGRO      "I'm sorry, I cannot control myself!"
#define SAY_ICORE_BOSS_DEATH       "Thank You, I am now set free at last. If only my sister could forgive me..."
#define SAY_ICORE_BOSS_SLAY				 "Welcome to the undead!"
#define SAY_ICORE_BOSS_MIND_CONTROL        "Come join me in the underworld!"

// Event boss 2 for I-core
class boss_icore_boss : public CreatureScript
{
public:
	boss_icore_boss() : CreatureScript("boss_icore_boss") { }

	struct boss_icore_bossAI : public ScriptedAI
	{
		boss_icore_bossAI(Creature *pCreature) : ScriptedAI(pCreature){}

	uint32 BoltTimer;
	uint32 ControlTimer;
	uint32 VolleyTimer;
	uint32 PiercingTimer;
	uint32 ArmyTimer;

	void Reset()
	{
	ControlTimer = 30000;
	BoltTimer = 10000;
	VolleyTimer = 15000;
	PiercingTimer = 20000;
	ArmyTimer =25000;
	}

	void EnterCombat(Unit* /*who*/)
	{
		me->MonsterYell(SAY_ICORE_BOSS_AGGRO, LANG_UNIVERSAL, 0);
	}

	void KilledUnit(Unit* victim)
	{
		me->MonsterYell(SAY_ICORE_BOSS_SLAY, LANG_UNIVERSAL, 0);
	}

	void JustDied(Unit* /*killer*/)
	{
		me->MonsterYell(SAY_ICORE_BOSS_DEATH, LANG_UNIVERSAL, 0);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!UpdateVictim())
			return;

		// Army of the dead
		if (ArmyTimer <= uiDiff)
		{
			me->InterruptNonMeleeSpells(false);
			if (Unit *pTarget = SelectTarget(SELECT_TARGET_TOPAGGRO,0))
				DoCast(pTarget, SPELL_ARMY_OF_THE_DEAD);
			ArmyTimer = 45000;
		} else ArmyTimer -= uiDiff;

		// Mind Control
		if (ControlTimer <= uiDiff)
		{
			me->InterruptNonMeleeSpells(false);
			if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
				DoCast(pTarget, SPELL_MIND_CONTROL);
			me->MonsterYell(SAY_ICORE_BOSS_MIND_CONTROL, LANG_UNIVERSAL, 0);
			ControlTimer = 30000;
		} else ControlTimer -= uiDiff;

		// Piercing shadow
		if (PiercingTimer <= uiDiff)
		{
			me->InterruptNonMeleeSpells(false);
			if (Unit *pTarget = SelectTarget(SELECT_TARGET_TOPAGGRO,0))
				DoCast(pTarget, SPELL_PIERCING_SHADOW);
			PiercingTimer = 10000;
		} else PiercingTimer -= uiDiff;

		// AOE Shadow Bolt Volley
		if (VolleyTimer <= uiDiff)
		{
			DoCastAOE(SPELL_SHADOW_BOLT_VOLLEY2);
			VolleyTimer = 15000;
		} else VolleyTimer -= uiDiff;

		DoSpellAttackIfReady(SPELL_SHADOW_BOLT);
                       }
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new boss_icore_bossAI (pCreature);
	}
};

/*######
## go_memorial_plaque_movie
######*/

enum GOSSIPS
{
        FALL_OF_THE_LICH_KING,
};

#define GOSSIP_TEXT_SEE_FALL_OF_THE_LICH_KING   "See the fall of the Lich King."
#define GOSSIP_MESSAGE_MEMORIAL                                 15921

class go_memorial_movie : public GameObjectScript
{
public: 
        go_memorial_movie() : GameObjectScript("go_memorial_movie") { }

        bool OnGossipHello(Player *player, GameObject *go)
        {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_SEE_FALL_OF_THE_LICH_KING, GOSSIP_SENDER_MAIN, FALL_OF_THE_LICH_KING);
                player->SEND_GOSSIP_MENU(GOSSIP_MESSAGE_MEMORIAL, go->GetGUID());
                return true;
        }

        bool OnGossipSelect(Player *player, GameObject *go, uint32 /*uiSender*/, uint32 uiAction)
        {
                player->PlayerTalkClass->ClearMenus();
                switch(uiAction)
                {
                        case FALL_OF_THE_LICH_KING:
                                player->SendMovieStart(16);
                                player->CLOSE_GOSSIP_MENU(); 
                                break;
                }
                return true;
        }
};

class npc_shrazz : public CreatureScript
{
public:
    npc_shrazz() : CreatureScript("npc_shrazz") { }

    struct npc_shrazzAI : public ScriptedAI
    {
        npc_shrazzAI(Creature* c) : ScriptedAI(c) {}
        void ReceiveEmote(Player* player, uint32 emote)
        {
            if(!player)
                return;

			if (emote == TEXT_EMOTE_KISS)
                if (me->HasAura(27571))
                    if (player->GetDrunkValue() > 50)
                        if (player->HasAura(70233) || player->HasAura(70234) || player->HasAura(70235) || player->HasAura(26682))
						if (AchievementEntry const* achievement = sAchievementStore.LookupEntry(1279))
                            player->CompletedAchievement(achievement);
        }
    };

    CreatureAI *GetAI(Creature* creature) const
    {
        return new npc_shrazzAI(creature);
    }
};

enum PetCalculate
{
     SPELL_HUNTER_PET_CRIT              = 19591,
     SPELL_WARLOCK_PET_CRIT             = 35695,
     SPELL_WARLOCK_PET_HIT_EXPERTISE    = 61013,
     SPELL_HUNTER_PET_HIT_EXPERTISE     = 61017,
     SPELL_DK_PET_HIT                   = 61697,
     SPELL_SHAMAN_PET_HIT               = 61783,
};

class spell_gen_pet_calculate : public SpellScriptLoader
{
    public:
        spell_gen_pet_calculate() : SpellScriptLoader("spell_gen_pet_calculate") { }

        class spell_gen_pet_calculate_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_pet_calculate_AuraScript);

            bool Load()
            {
                if (!GetCaster() || !GetCaster()->GetOwner() || GetCaster()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                    return false;
                return true;
            }

            void CalculateAmountCritSpell(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float CritSpell = 0.0f;
                    // Crit from Intellect
                    CritSpell += owner->GetSpellCritFromIntellect();
                    // Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
                    CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
                    // Increase crit from SPELL_AURA_MOD_CRIT_PCT
                    CritSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
                    // Increase crit spell from spell crit ratings
                    CritSpell += owner->GetRatingBonusValue(CR_CRIT_SPELL);

                    amount += int32(CritSpell);
                }
            }

            void CalculateAmountCritMelee(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float CritMelee = 0.0f;
                    // Crit from Agility
                    CritMelee += owner->GetMeleeCritFromAgility();
                    // Increase crit from SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
                    CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
                    // Increase crit from SPELL_AURA_MOD_CRIT_PCT
                    CritMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
                    // Increase crit melee from melee crit ratings
                    CritMelee += owner->GetRatingBonusValue(CR_CRIT_MELEE);

                    amount += int32(CritMelee);
                }
            }

            void CalculateAmountMeleeHit(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float HitMelee = 0.0f;
                    // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
                    HitMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
                    // Increase hit melee from meele hit ratings
                    HitMelee += owner->GetRatingBonusValue(CR_HIT_MELEE);

                    amount += int32(HitMelee);
                }
            }

            void CalculateAmountSpellHit(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float HitSpell = 0.0f;
                    // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
                    HitSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                    // Increase hit spell from spell hit ratings
                    HitSpell += owner->GetRatingBonusValue(CR_HIT_SPELL);

                    amount += int32(HitSpell);
                }
            }
            
            void CalculateAmountExpertise(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float Expertise = 0.0f;
                    // Increase hit from SPELL_AURA_MOD_EXPERTISE
                    Expertise += owner->GetTotalAuraModifier(SPELL_AURA_MOD_EXPERTISE);
                    // Increase Expertise from Expertise ratings
                    Expertise += owner->GetRatingBonusValue(CR_EXPERTISE);

                    amount += int32(Expertise);
                }
            }

            void CalculateAmountExpertiseFromHit(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    // For others recalculate it from:
                    float Expertise = 0.0f;
                    // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
                    Expertise += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
                    // Increase hit melee from meele hit ratings
                    Expertise += owner->GetRatingBonusValue(CR_HIT_MELEE);

                    amount += int32(Expertise);
                }
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case SPELL_HUNTER_PET_CRIT:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountCritMelee, EFFECT_0, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountCritSpell, EFFECT_1, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
                        break;
                    case SPELL_WARLOCK_PET_CRIT:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountCritSpell, EFFECT_0, SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountCritMelee, EFFECT_1, SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
                        break;
                    case SPELL_WARLOCK_PET_HIT_EXPERTISE:
                    case SPELL_HUNTER_PET_HIT_EXPERTISE:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountExpertiseFromHit, EFFECT_2, SPELL_AURA_MOD_EXPERTISE);
                        break;
                    case SPELL_DK_PET_HIT:
                    case SPELL_SHAMAN_PET_HIT:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_pet_calculate_AuraScript();
        }
};

/*######
## npc_woodlands_walker
######*/

#define GOSSIP_WOODLANDS            "The ancients of the Lothalor Woods need your bark to survive. Will you help?"

enum eVictoriousChallenger
{ 
    QUEST_ALLY_STRENGHTEN_THE_ANCIENTS		= 12092,
    QUEST_HORDE_STRENGHTEN_THE_ANCIENTS	= 12096,

    SPELL_TOUGHEN_HIDE				= 50994,

	SAY_ATTACK				= -1999950,
	SAY_OOC				= -1999952
};

class npc_woodlands_walker : public CreatureScript
{
public:
    npc_woodlands_walker() : CreatureScript("npc_woodlands_walker") { }

    struct npc_woodlands_walkerAI : public ScriptedAI
    {
        npc_woodlands_walkerAI(Creature* pCreature) : ScriptedAI(pCreature) {}

        uint32 ToughenHideTimer;
	uint32 bAttackSay;
       
        void Reset()
        {
	me->RestoreFaction();
            ToughenHideTimer    = 15000;
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if ((ToughenHideTimer <= diff) && HealthBelowPct(50))
               {  
	DoCast(me, SPELL_TOUGHEN_HIDE);
                    ToughenHideTimer = 15000;
               }else ToughenHideTimer -= diff;
                                                                
               DoMeleeAttackIfReady();
	}

        void KilledUnit(Unit* /*victim*/)
        {
            me->RestoreFaction();
        }

    };

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {		
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(QUEST_ALLY_STRENGHTEN_THE_ANCIENTS) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(QUEST_HORDE_STRENGHTEN_THE_ANCIENTS) == QUEST_STATUS_INCOMPLETE)
        {
			pCreature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_WOODLANDS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            return true;
        }

        return false;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
		uint32 iRandom;
		iRandom = rand()%2;

        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
			pPlayer->CLOSE_GOSSIP_MENU();           
			if (iRandom == 1)
            {
				pPlayer->AddItem(36786, 1);								
				DoScriptText(SAY_OOC, pCreature);
				pCreature->Kill(pCreature);
				pCreature->RemoveCorpse();
            } else 
			{
				pCreature->setFaction(14);
				pCreature->AI()->AttackStart(pPlayer);
				DoScriptText(SAY_ATTACK, pCreature);
			}
		}

        return true;
    }

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_woodlands_walkerAI(creature);
    }
};

/*######
## npc_childrens_week
######*/
 
enum ChildrensWeek
{
        QUEST_AUCHINDOUN        = 10950,
        QUEST_JHEEL_AERIS   = 10954,
        QUEST_SEAT_NAARU        = 10956,
        QUEST_CALL_FARSEER  = 10958,
        QUEST_CAVERNS_A  = 10962,
        QUEST_CAVERNS_H  = 10963,
        QUEST_THRONE            = 10953,
        QUEST_SPOREGGAR  = 10945,
        QUEST_WHEN_IGROW  = 11975,        

        QUEST_1PLAYMATES  = 13950, 
        QUEST_2PLAYMATES  = 13951, 
        QUEST_1DRAGONSHRINE  = 13933, 
        QUEST_2DRAGONSHRINE = 13934, 
        QUEST_BIGGEST_TREE  = 13929, 
        QUEST_1DRAGON_QUEEN  = 13954, 
        QUEST_2DRAGON_QUEEN  = 13955,
        QUEST_GREAT_ONE  = 13956,
        QUEST_MIGHTY_HEMET  = 13957,        
        QUEST_HOME_BEARMAN  = 13930,

        NPC_AUCHINDOUN    = 22831,
        NPC_SEAT_NAARU    = 22851,
        NPC_CALL_FARSEER        = 22905,
        NPC_CAVERNS              = 22872,
        NPC_AERIS_LANDING   = 22838,
        NPC_SPOREGGAR      = 22829,
        NPC_THRONE_ELEMENT  = 22839,
        NPC_TRIGGER_ICORE      = 74900,        

        NPC_2TRIGGER_ICORE      = 74902,
        NPC_3TRIGGER_ICORE      = 74903,
        NPC_4TRIGGER_ICORE      = 74904,
        NPC_5TRIGGER_ICORE      = 74905,        
        NPC_6TRIGGER_ICORE      = 74906,
        NPC_7TRIGGER_ICORE      = 74907,
        NPC_8TRIGGER_ICORE      = 74908,
        NPC_9TRIGGER_ICORE      = 74909,
        NPC_10TRIGGER_ICORE      = 74910,
        NPC_11TRIGGER_ICORE      = 74911,
 
        AURA_ORPHAN_OUT  = 58818,
};
 
class npc_childrenweek_compl : public CreatureScript
{
public:
        npc_childrenweek_compl() : CreatureScript("npc_childrenweek_compl") { }
 
        CreatureAI* GetAI(Creature* creature) const
        {
                return new npc_childrenweek_complAI (creature);
        }
 
        struct npc_childrenweek_complAI : public ScriptedAI
        {
                npc_childrenweek_complAI(Creature* c) : ScriptedAI(c) {}
 
                void Reset() { }
 
                void EnterCombat(Unit* /*who*/) { }
 
                void MoveInLineOfSight(Unit* who)
                {
                        uint32 questId = 0;
 
                        if (!who)
                                return;
 
                        switch (me->GetEntry())
                        {
                                case NPC_AUCHINDOUN:
                                        questId = QUEST_AUCHINDOUN;
                                        break;
                                case NPC_SEAT_NAARU:
                                        questId = QUEST_SEAT_NAARU;
                                        break;
                                case NPC_CALL_FARSEER:
                                        questId = QUEST_CALL_FARSEER;
                                        break;
                                case NPC_CAVERNS:
		                if (who->GetTypeId() == TYPEID_PLAYER)
                                        questId = who->ToPlayer()->GetTeam() == ALLIANCE ? QUEST_CAVERNS_A : QUEST_CAVERNS_H;
                                        break;
                                case NPC_AERIS_LANDING:
                                        questId = QUEST_JHEEL_AERIS;
                                        break;
                                case NPC_SPOREGGAR:
                                        questId = QUEST_SPOREGGAR;
                                        break;
                                case NPC_THRONE_ELEMENT:
                                        questId = QUEST_THRONE;
                                        break;
                                case NPC_TRIGGER_ICORE:
                                        questId = QUEST_WHEN_IGROW;
                                        break;
                                case NPC_2TRIGGER_ICORE:
                                        questId = QUEST_1PLAYMATES;
                                        break;
                                case NPC_3TRIGGER_ICORE:
                                        questId = QUEST_2PLAYMATES;
                                        break;
                                case NPC_4TRIGGER_ICORE:
                                        questId = QUEST_1DRAGONSHRINE;
                                        break;
                                case NPC_5TRIGGER_ICORE:
                                        questId = QUEST_2DRAGONSHRINE;
                                        break;
                                case NPC_6TRIGGER_ICORE:
                                        questId = QUEST_BIGGEST_TREE;
                                        break;
                                case NPC_7TRIGGER_ICORE:
                                        questId = QUEST_1DRAGON_QUEEN;
                                        break;
                                case NPC_8TRIGGER_ICORE:
                                        questId = QUEST_2DRAGON_QUEEN;
                                        break;
                                case NPC_9TRIGGER_ICORE:
                                        questId = QUEST_GREAT_ONE;
                                        break;
                                case NPC_10TRIGGER_ICORE:
                                        questId = QUEST_MIGHTY_HEMET;
                                        break;
                                case NPC_11TRIGGER_ICORE:
                                        questId = QUEST_HOME_BEARMAN;
                                        break;
                        }
 
                        if (who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 10) && who->HasAura(AURA_ORPHAN_OUT))
                        {
                                if (who->ToPlayer()->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                                        who->ToPlayer()->AreaExploredOrEventHappens(questId);
                        }
                }
        };
};

class spell_gen_lowlvlscroll_handler : public SpellScriptLoader
{
    public:
        spell_gen_lowlvlscroll_handler() : SpellScriptLoader("spell_gen_lowlvlscroll_handler") { }

        class spell_gen_lowlvlscroll_handler_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_lowlvlscroll_handler_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
              Unit* target = GetTarget();
	  if (target->getLevelForTarget(target) < 80)
		  switch(GetId())
		  {
                    case 72590:
		            target->RemoveAura(72590);
			    break;
		    //case 72588:
		    // 	target->RemoveAura(72588);
		    //	break;
		  }
	  /*if (target->getLevelForTarget(target) < 70)
	      switch(GetId())
		  {
		case 57390:
			target->RemoveAura(57399);
			break;
		case 57294:
			target->RemoveAura(57294);
			break;
	  }*/
            }

            void Register()
            {
	     AfterEffectApply += AuraEffectApplyFn(spell_gen_lowlvlscroll_handler_AuraScript::OnApply, EFFECT_0,  SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_lowlvlscroll_handler_AuraScript();
        }
};

/*######
## npc_demoniac_scryer
######*/

#define GOSSIP_ITEM_ATTUNE          "Yes, Scryer. You may possess me."

enum eDemoniac
{
    GOSSIP_TEXTID_PROTECT           = 10659,
    GOSSIP_TEXTID_ATTUNED           = 10643,

    QUEST_DEMONIAC                  = 10838,
    NPC_HELLFIRE_WARDLING           = 22259,
    NPC_BUTTRESS                    = 22267,                
    NPC_SPAWNER                     = 22260,                

    MAX_BUTTRESS                    = 4,
    TIME_TOTAL                      = MINUTE*10*IN_MILLISECONDS,

    SPELL_SUMMONED_DEMON            = 7741,                 
    SPELL_DEMONIAC_VISITATION       = 38708,                

    SPELL_BUTTRESS_APPERANCE        = 38719,                
    SPELL_SUCKER_CHANNEL            = 38721,                
    SPELL_SUCKER_DESPAWN_MOB        = 38691
};

class npc_demoniac_scryer : public CreatureScript
{
public:
    npc_demoniac_scryer() : CreatureScript("npc_demoniac_scryer") { }

	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
	{
		//if (npc_demoniac_scryerAI* pScryerAI = dynamic_cast<npc_demoniac_scryerAI*>(pCreature->AI()))
	if (pPlayer->GetQuestStatus(QUEST_DEMONIAC) == QUEST_STATUS_INCOMPLETE)
        CAST_AI(npc_demoniac_scryer::npc_demoniac_scryerAI, pCreature->AI());
		{
			//if (pScryerAI->IsComplete)
			//{
				if (pPlayer->GetQuestStatus(QUEST_DEMONIAC) == QUEST_STATUS_INCOMPLETE)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ATTUNE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

				pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ATTUNED, pCreature->GetGUID());
				return true;
			//}
		}

		pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_PROTECT, pCreature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
	{
        pPlayer->PlayerTalkClass->ClearMenus();
		if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
		{
			pPlayer->CLOSE_GOSSIP_MENU();
			pCreature->CastSpell(pPlayer, SPELL_DEMONIAC_VISITATION, false);
		}

		return true;
	}

	CreatureAI* GetAI_npc_demoniac_scryer(Creature* pCreature)
	{
		return new npc_demoniac_scryerAI(pCreature);
	}

	struct npc_demoniac_scryerAI : public ScriptedAI
	{
		npc_demoniac_scryerAI(Creature* pCreature) : ScriptedAI(pCreature)
		{
			//IsComplete = false;
			//uiSpawnDemonTimer = 15000;
			//uiSpawnButtressTimer = 45000;
			//uiButtressCount = 0;
			//Reset();
		}

		bool IsComplete;

		uint32 uiSpawnDemonTimer;
		uint32 uiSpawnButtressTimer;
		uint32 uiButtressCount;

		void Reset() 
		{
			IsComplete = false;
			uiSpawnDemonTimer = 15000;
			uiSpawnButtressTimer = 45000;
			uiButtressCount = 0;		
		}

		void AttackedBy(Unit* pEnemy) {}
		void AttackStart(Unit* pEnemy) {}

		void DoSpawnButtress()
		{
			++uiButtressCount;

			float fAngle;

			switch(uiButtressCount)
			{
				case 1: fAngle = 0.0f; break;
				case 2: fAngle = M_PI+M_PI/2; break;
				case 3: fAngle = M_PI/2; break;
				case 4: fAngle = M_PI; break;
			}

			float fX, fY;
			me->GetNearPoint2D(fX, fY, 5.05f, fAngle);

			float fZ_Ground = me->GetMap()->GetHeight(fX, fY, MAX_HEIGHT);

			uint32 uiTime = TIME_TOTAL - (uiSpawnButtressTimer * uiButtressCount);
			me->SummonCreature(NPC_BUTTRESS, fX, fY, fZ_Ground, me->GetAngle(fX, fY), TEMPSUMMON_TIMED_DESPAWN, uiTime);
		}

		void DoSpawnDemon()
		{
			//float fX, fY, fZ;
			//me->GetRandomPoint(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 20.0f, fX, fY, fZ);
	                me->SummonCreature(NPC_HELLFIRE_WARDLING, me->GetPositionX()/*-rand()%15*/, me->GetPositionY()/*-rand()%15*/, me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                        //if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
			//me->SummonCreature(NPC_HELLFIRE_WARDLING, fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
		}

		void JustSummoned(Creature* pSummoned)
		{
			if (pSummoned->GetEntry() == NPC_HELLFIRE_WARDLING)
			{
				pSummoned->CastSpell(pSummoned, SPELL_SUMMONED_DEMON, false);
				pSummoned->AI()->AttackStart(me);
			}
			else
			{
				if (pSummoned->GetEntry() == NPC_BUTTRESS)
				{
					pSummoned->CastSpell(pSummoned, SPELL_BUTTRESS_APPERANCE, false);
					pSummoned->CastSpell(me, SPELL_SUCKER_CHANNEL, true);
				}
			}
		}

		void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
		{
			if (pTarget->GetEntry() == NPC_HELLFIRE_WARDLING && pSpell->Id == SPELL_SUCKER_DESPAWN_MOB)
				((Creature*)pTarget)->ForcedDespawn();
		}

		void UpdateAI(const uint32 uiDiff)
		{
			if (IsComplete || !me->isAlive())
				return;

			if (uiSpawnButtressTimer <= uiDiff)
			{
				if (uiButtressCount >= MAX_BUTTRESS)
				{
					me->CastSpell(me, SPELL_SUCKER_DESPAWN_MOB, false);

					if (me->isInCombat())
					{
						me->DeleteThreatList();
						me->CombatStop();
					}

					IsComplete = true;
					return;
				}

				uiSpawnButtressTimer = 45000;
				DoSpawnButtress();
			}
			else
				uiSpawnButtressTimer -= uiDiff;

			if (uiSpawnDemonTimer <= uiDiff)
			{
				DoSpawnDemon();
				uiSpawnDemonTimer = 15000;
			}
			else
				uiSpawnDemonTimer -= uiDiff;
		}
	};
};

/*#########
### Support for quests Bombing Run/Bomb them again
###############*/

#define FEL_CANONBALL                  185861
#define SPELL_THROW_BOMB               40160

#define QUEST_BOMBING_RUN_A            11102
#define QUEST_BOMBING_RUN_H            11010
#define QUEST_BOMB_THEM_AGAIN  11023

#define QUEST_CREDIT                   23118

#define ZONE_FORGE_CAMP        3785

class spell_throw_bomb : public SpellScriptLoader
{
public:
    spell_throw_bomb() : SpellScriptLoader("spell_throw_bomb") {}

    class spell_throw_bomb_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_throw_bomb_SpellScript)

        bool Validate(SpellEntry const* /*spell*/)
        {
            if (!sSpellStore.LookupEntry(SPELL_THROW_BOMB))
                return false;
            return true;
        }

        SpellCastResult CheckIfInZone()
        {
            Unit* caster = GetCaster();

                       if(caster->GetAreaId()==ZONE_FORGE_CAMP)
                               return SPELL_CAST_OK;
            else
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
        }

        void HandleBombEffect(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
                       Player* player = caster->ToPlayer();

                       if (player && (player->GetQuestStatus(QUEST_BOMBING_RUN_A)==QUEST_STATUS_INCOMPLETE
                                          || player->GetQuestStatus(QUEST_BOMBING_RUN_H)==QUEST_STATUS_INCOMPLETE
                                          || player->GetQuestStatus(QUEST_BOMB_THEM_AGAIN)==QUEST_STATUS_INCOMPLETE))
                                        player->KilledMonsterCredit(QUEST_CREDIT,0);      
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_throw_bomb_SpellScript::CheckIfInZone);
			OnEffectHitTarget += SpellEffectFn(spell_throw_bomb_SpellScript::HandleBombEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
                                              
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_throw_bomb_SpellScript();
    }
};

void AddSC_icore_custom()
{
    new npc_frostlord_ahune();
    new npc_ahune_ice_spear();
    new go_ahune_ice_stone();
    new npc_ice_caller_briatha();
    new npc_torch_tossing_bunny();
    new spell_gen_torch_target_picker();
    new spell_gen_juggle_torch_catch();
    new at_nats_landing();
    new npc_destructive_ward();
    new npc_evergrove_druid();
    new npc_antelarion_gossip();
    new npc_signal_fire();
    new npc_keristrasza_coldarra();
    new npc_icc_rep_rings();
    new at_legion_hold_smvalley();
    new npc_father_kamaros;
    new npc_decomposing_ghoul();
    new npc_irulon_trueblade();
    new npc_terokk();
    new go_ancient_skull_pile();
    new npc_riggle_bassbait();
    new npc_earth_elemental();
    new npc_fire_elemental();
    new npc_explosive_sheep();
    new item_hallowsend_tricky_treat();
    new npc_hallowend();
    new go_wickerman_ember;
    new npc_wickerman_guardian;
    new npc_kali_remik;
    new npc_headless_horseman_fire();
    new npc_shade_of_the_horseman();
    new spell_halloween_wand();
    new boss_icore_boss;
    //new spell_gen_turkey_tracker();
    //new npc_wild_turkey();
    //new npc_lonely_turkey();
    //new go_memorial_movie();
    new npc_shrazz();
    new spell_gen_pet_calculate();
    new npc_woodlands_walker();
    new npc_childrenweek_compl();
    new spell_gen_lowlvlscroll_handler();
    new npc_demoniac_scryer();
    //new spell_throw_bomb();
}
