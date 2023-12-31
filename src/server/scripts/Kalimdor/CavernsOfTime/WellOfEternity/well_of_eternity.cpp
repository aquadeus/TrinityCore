/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
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

#include "CombatAI.h"
#include "InstanceScript.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Vehicle.h"
#include "well_of_eternity.h"

enum Texts
{
    // Nozdormu (intro)
    SAY_INTRO_1         = 0,
    SAY_INTRO_2         = 1,
    SAY_INTRO_3         = 2,
    SAY_INTRO_4         = 3,

    // Illidan Stormrage (Part 1)
    SAY_GREETING_1      = 0,
    SAY_GREETING_2      = 1,
    SAY_GREETING_IDLE_1 = 2,
    SAY_CLOAK_1         = 3,
    SAY_ESCORT_1        = 4,
    SAY_ESCORT_2        = 5,
    SAY_DISTRACT_1      = 6,
    SAY_DISTRACT_END_1  = 7,
    SAY_IDLE_1          = 8,
    SAY_COMBAT_2        = 9,
    SAY_WAITING_1       = 10,
    SAY_COMBAT_1        = 11,
    SAY_ESCORT_5        = 12,
    SAY_IDLE_2          = 13,
    SAY_COMBAT_4        = 14,
    SAY_WAITING_2       = 15,
    SAY_INTRO_17        = 16,
    SAY_INTRO_18        = 17,
    SAY_INTRO_19        = 18,
    SAY_INTRO_20        = 19,
    SAY_INTRO_21        = 20,
    SAY_INTRO_22        = 21,
    SAY_INTRO_23        = 22,
    SAY_INTRO_24        = 23,
    SAY_INTRO_25        = 24,
    SAY_INTRO_26        = 25,
    SAY_INTRO_27        = 26,
    SAY_INTRO_28        = 27
};

enum Actions
{
    // Intro
    ACTION_NOZDORMU_INTRO       = 1,

    // Illidan Stormrage
    ACTION_ILLIDAN_INTRO        = 2,
    ACTION_TALK                 = 3,
    ACTION_SKIP_TALK            = 4,
    ACTION_MOVE_TO_DISTRACT     = 5,
    ACTION_START_DISTRACTION    = 6
};

enum Spells
{
    // Intro
    SPELL_SUMMON_FIRE_WALL                   = 105243,
    SPELL_SUMMON_FEL_FIREWALL_COSMETIC       = 105247,
    SPELL_SUMMON_FEL_FIREWALL_COSMETIC_PULSE = 105250,

    // Legion Demon
    SPELL_STRIKE_FEAR                        = 103913,

    // Illidan Stormrage
    SPELL_SHADOWCLOAK                        = 110231, // Casted on Pets
    SPELL_SHADOWCLOAK_VEHICLE                = 105915, // ToDo: Fix mount vehicle when aura is removed and applied again on Illidan
    SPELL_SHADOW_WALK                        = 103020,
    SPELL_SHADOW_AMBUSHER                    = 103018,
    SPELL_WALL_OF_SHADOW                     = 104400,

    // Misc
    SPELL_DISTRACT_DEMONS_STATIONARY         = 110082,
    SPELL_SHRINK                             = 59632   // Serverside
};

enum MovementMisc
{
    // Illidan Stormrage
    PATH_ILLIDAN_INTRO  = 5550000,
    WAYPOINT_GOSSIP     = 4,

    PATH_ILLIDAN_ESCORT = 5550001,
    WAYPOINT_TALK       = 2,
    WAYPOINT_DISTRACT   = 9,

    // Well of Eternity Stalker
    POINT_START_MOVING  = 0
};

enum Events
{
    // Legion Demon
    EVENT_STRIKE_FEAR = 1
};

enum Gossip
{
    MENU_ID_ILLIDAN_1     = 13162,
    MENU_ID_ILLIDAN_2     = 13163,
    GOSSIP_OPTION_ID      = 0
};

enum Tasks
{
    TASK_WAITING = 1
};

Position const wallOfShadowStalkerPos  = { 3312.3826f, -4906.39f, 181.07674f };

static Position const illidanSummonPos[4] =
{
    { 3173.6746f, -4875.5103f, 194.43994f, 5.3581609f },
    { 3294.2f,    -4981.97f,   181.16032f, 0.8726646f },
    { 3444.98f,   -4886.34f,   181.16032f, 4.0142574f },
    { 3471.1199f, -4839.83f,   194.21544f, 2.0071287f }
};

static Position const firewallPos[7] =
{
    { 3204.6145f, -4935.3647f, 194.41118f },
    { 3182.0986f, -4933.1196f, 194.41118f },
    { 3201.0647f, -4937.5664f, 194.41118f },
    { 3185.1438f, -4935.9785f, 194.41118f },
    { 3197.0664f, -4938.774f,  194.41118f },
    { 3188.825f,  -4937.952f,  194.41118f },
    { 3192.8916f, -4938.9053f, 194.41118f }
};

// Areatrigger - 7387
struct at_woe_nozdormu_intro : AreaTriggerScript
{
public:
    at_woe_nozdormu_intro() : AreaTriggerScript("at_woe_nozdormu_intro") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) override
    {
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            if (Creature* nozdormu = instance->GetCreature(DATA_NOZDORMU))
                nozdormu->AI()->DoAction(ACTION_NOZDORMU_INTRO);
        }
        return true;
    }
};

// Areatrigger - 7029
struct at_woe_perotharn_intro : AreaTriggerScript
{
public:
    at_woe_perotharn_intro() : AreaTriggerScript("at_woe_perotharn_intro") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) override
    {
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            if (Creature* perotharn = instance->GetCreature(BOSS_PEROTHARN))
                perotharn->AI()->DoAction(ACTION_PEROTHARN_INTRO);
        }
        return true;
    }
};

// 55624 - Nozdormu
struct npc_woe_nozdormu : public ScriptedAI
{
    npc_woe_nozdormu(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()) { }

    void DoAction(int32 actionId) override
    {
        switch (actionId)
        {
            case ACTION_NOZDORMU_INTRO:
            {
                if (!_nozdormuIntro)
                {
                    Talk(SAY_INTRO_1);

                    scheduler.Schedule(12s + 970ms, [this](TaskContext context)
                    {
                        Talk(SAY_INTRO_2);
                        context.Schedule(5s + 950ms, [this](TaskContext context)
                        {
                            Talk(SAY_INTRO_3);
                            context.Schedule(12s + 230ms, [this](TaskContext /*context*/)
                            {
                                Talk(SAY_INTRO_4);
                            });
                        });
                    });
                    _nozdormuIntro = true;
                    break;
                }
            }
            default:
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);
    }

private:
    TaskScheduler scheduler;
    InstanceScript* _instance;
    bool _nozdormuIntro = false;
};

// 55503 - Legion Demon
struct npc_woe_legion_demon : public ScriptedAI
{
    npc_woe_legion_demon(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        if (me->HasStringId("legion_demon_woe_intro"))
            me->SetImmuneToPC(true);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_STRIKE_FEAR, 7s);
    }

    void WaypointReached(uint32 waypointId, uint32 /*pathId*/) override
    {
        if (!me->HasStringId("legion_demon_woe_intro"))
            return;

        if (waypointId == 3)
        {
            if (Creature* stalker = me->FindNearestCreature(NPC_FIRE_WALL_STALKER, 25.0f))
                stalker->CastSpell(stalker, SPELL_SUMMON_FEL_FIREWALL_COSMETIC, TRIGGERED_FULL_MASK);

            DoCast(SPELL_SUMMON_FIRE_WALL);
        }
    }

    void JustDied(Unit* /*killer*/) override
    {
        if (GameObject* go = me->FindNearestGameObject(GO_LARGE_FIRE_WALL_DOOR, 75.0f))
            go->SetGoState(GO_STATE_DESTROYED);

        if (GameObject* go = me->FindNearestGameObject(GO_COURTYARD_DOOR, 75.0f))
            go->SetGoState(GO_STATE_DESTROYED);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        switch (events.ExecuteEvent())
        {
            case EVENT_STRIKE_FEAR:
                DoCast(SPELL_STRIKE_FEAR);
                events.ScheduleEvent(EVENT_STRIKE_FEAR, 7s);
                break;
            default:
                break;
        }

        DoMeleeAttackIfReady();
    }
private:
    EventMap events;
};

// 105247 - Summon Fel Firewall Cosmetic PH
class spell_woe_summon_fel_firewall_cosmetic_ph : public AuraScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SUMMON_FEL_FIREWALL_COSMETIC_PULSE });
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        switch (aurEff->GetTickNumber())
        {
            case 1:
            {
                GetTarget()->CastSpell(firewallPos[0], SPELL_SUMMON_FEL_FIREWALL_COSMETIC_PULSE, TRIGGERED_FULL_MASK);
                GetTarget()->CastSpell(firewallPos[1], SPELL_SUMMON_FEL_FIREWALL_COSMETIC_PULSE, TRIGGERED_FULL_MASK);
                break;
            }
            case 2:
            {
                GetTarget()->CastSpell(firewallPos[2], SPELL_SUMMON_FEL_FIREWALL_COSMETIC_PULSE, TRIGGERED_FULL_MASK);
                GetTarget()->CastSpell(firewallPos[3], SPELL_SUMMON_FEL_FIREWALL_COSMETIC_PULSE, TRIGGERED_FULL_MASK);
                break;
            }
            case 3:
            {
                GetTarget()->CastSpell(firewallPos[4], SPELL_SUMMON_FEL_FIREWALL_COSMETIC_PULSE, TRIGGERED_FULL_MASK);
                GetTarget()->CastSpell(firewallPos[5], SPELL_SUMMON_FEL_FIREWALL_COSMETIC_PULSE, TRIGGERED_FULL_MASK);
                break;
            }
            case 4:
            {
                GetTarget()->CastSpell(firewallPos[6], SPELL_SUMMON_FEL_FIREWALL_COSMETIC_PULSE, TRIGGERED_FULL_MASK);
                break;
            }
            default:
                break;
        }
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GameObject* go = GetTarget()->FindNearestGameObject(GO_LARGE_FIRE_WALL_DOOR, 15.0f))
            go->SetGoState(GO_STATE_READY);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_woe_summon_fel_firewall_cosmetic_ph::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        AfterEffectRemove += AuraEffectRemoveFn(spell_woe_summon_fel_firewall_cosmetic_ph::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 105243 - Summon Fire Wall
class spell_woe_summon_fire_wall : public SpellScript
{
    void HandleAfterCast()
    {
        GetCaster()->ToCreature()->SetImmuneToPC(false);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_woe_summon_fire_wall::HandleAfterCast);
    }
};

// Areatrigger - 7066
struct at_woe_illidan_intro : AreaTriggerScript
{
public:
    at_woe_illidan_intro() : AreaTriggerScript("at_woe_illidan_intro") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) override
    {
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            if (instance->GetData(DATA_ILLIDAN_START_INTRO) != DONE)
            {
                player->SummonCreature(NPC_ILLIDAN_PART_1, illidanSummonPos[0], TEMPSUMMON_MANUAL_DESPAWN);

                if (Creature* illidan = instance->GetCreature(DATA_ILLIDAN_1))
                    illidan->AI()->DoAction(ACTION_ILLIDAN_INTRO);
            }

        }
        return true;
    }
};

// Areatrigger - 7144
struct at_woe_illidan_skip_talk : AreaTriggerScript
{
public:
    at_woe_illidan_skip_talk() : AreaTriggerScript("at_woe_illidan_skip_talk") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) override
    {
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            if (instance->GetData(DATA_ILLIDAN_START_ESCORT) != DONE)
            {
                if (Creature* illidan = instance->GetCreature(DATA_ILLIDAN_1))
                    illidan->AI()->DoAction(ACTION_SKIP_TALK);
            }
        }
        return true;
    }
};

// 55500 - Illidan Stormrage (Part 1)
struct npc_woe_illidan_part_1 : public VehicleAI
{
    npc_woe_illidan_part_1(Creature* creature) : VehicleAI(creature), _instance(creature->GetInstanceScript()) { }

    void DoAction(int32 actionId) override
    {
        switch (actionId)
        {
            case ACTION_ILLIDAN_INTRO:
            {
                me->SetVisible(true);
                scheduler.Schedule(1s + 116ms, [this](TaskContext context)
                {
                    me->GetMotionMaster()->MovePath(PATH_ILLIDAN_INTRO, false);
                    context.Schedule(804ms, [this](TaskContext /*context*/)
                    {
                        Talk(SAY_GREETING_1);

                        scheduler.Schedule(45s, 60s, TASK_WAITING, [this](TaskContext task)
                        {
                            Talk(SAY_GREETING_IDLE_1);
                            task.Repeat();
                        });
                    });
                });
                _instance->SetData(DATA_ILLIDAN_START_INTRO, DONE);
                break;
            }
            case ACTION_TALK:
            {
                _instance->SetData(DATA_ILLIDAN_START_ESCORT, DONE);
                _instance->DoCastSpellOnPlayers(SPELL_SHADOWCLOAK_PLAYER);
                scheduler.CancelGroup(TASK_WAITING);
                DoCast(SPELL_SHADOWCLOAK_VEHICLE);
                Talk(SAY_ESCORT_1);
                DoAction(ACTION_MOVE_TO_DISTRACT);
                break;
            }
            case ACTION_SKIP_TALK:
            {
                _instance->SetData(DATA_ILLIDAN_START_ESCORT, DONE);
                _instance->DoCastSpellOnPlayers(SPELL_SHADOWCLOAK_PLAYER);
                scheduler.CancelGroup(TASK_WAITING);
                DoCast(SPELL_SHADOWCLOAK_VEHICLE);
                me->RemoveNpcFlag(UNIT_NPC_FLAG_GOSSIP);
                Talk(SAY_ESCORT_1);
                DoAction(ACTION_MOVE_TO_DISTRACT);
                break;
            }
            case ACTION_MOVE_TO_DISTRACT:
            {
                scheduler.Schedule(3s + 700ms, [this](TaskContext /*context*/)
                {
                    me->GetMotionMaster()->MovePath(PATH_ILLIDAN_ESCORT, false);
                });
                break;
            }
            case ACTION_START_DISTRACTION:
            {
                scheduler.Schedule(4s + 200ms, [this](TaskContext /*context*/)
                {
                    DoCast(SPELL_WALL_OF_SHADOW);
                });
                break;
            }
            default:
                break;
        }
    }

    void WaypointReached(uint32 waypointId, uint32 pathId) override
    {
        if (pathId == PATH_ILLIDAN_INTRO && waypointId == WAYPOINT_GOSSIP)
        {
            me->RemoveAurasDueToSpell(SPELL_SHADOWCLOAK_VEHICLE);
            Talk(SAY_GREETING_2);
            me->SetNpcFlag(UNIT_NPC_FLAG_GOSSIP);
        }
        else if (pathId == PATH_ILLIDAN_ESCORT)
        {
            if (waypointId == WAYPOINT_TALK)
                Talk(SAY_ESCORT_2);
            else if (waypointId == WAYPOINT_DISTRACT)
            {
                Talk(SAY_DISTRACT_1);
                DoAction(ACTION_START_DISTRACTION);
                // me->SetNpcFlag(UNIT_NPC_FLAG_GOSSIP); Another gossip
            }
        }
    }

    bool OnGossipHello(Player* player) override
    {
        InitGossipMenuFor(player, MENU_ID_ILLIDAN_1);
        AddGossipItemFor(player, MENU_ID_ILLIDAN_1, GOSSIP_OPTION_ID, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        SendGossipMenuFor(player, player->GetGossipTextId(MENU_ID_ILLIDAN_1, me), me->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
    {
        uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
        ClearGossipMenuFor(player);
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
            {
                InitGossipMenuFor(player, MENU_ID_ILLIDAN_2);
                AddGossipItemFor(player, MENU_ID_ILLIDAN_2, GOSSIP_OPTION_ID, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                SendGossipMenuFor(player, player->GetGossipTextId(MENU_ID_ILLIDAN_2, me), me->GetGUID());

                Talk(SAY_CLOAK_1);
                _instance->DoCastSpellOnPlayers(SPELL_SHADOWCLOAK_PLAYER);
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                me->RemoveNpcFlag(UNIT_NPC_FLAG_GOSSIP);
                DoAction(ACTION_TALK);
                break;
            }
        }
        return true;
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);
    }

private:
    TaskScheduler scheduler;
    InstanceScript* _instance;
};

// 58200 - Distract Demon Stalker
struct npc_woe_distract_demon_stalker : public ScriptedAI
{
    npc_woe_distract_demon_stalker(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        DoCast(SPELL_SHRINK);
    }

    void JustAppeared() override
    {
        DoCast(SPELL_DISTRACT_DEMONS_STATIONARY);

        scheduler.Schedule(1s + 200ms, [this](TaskContext /*context*/)
        {
            me->RemoveAurasDueToSpell(SPELL_SHRINK);
        });
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);
    }

private:
    TaskScheduler scheduler;
};

// 103004 - Shadowcloak
class spell_woe_shadowcloak : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHADOW_WALK_VEHICLE });
    }

    void HandleEffect(SpellEffIndex /*effIndex*/)
    {
        GetHitUnit()->CastSpell(GetHitUnit(), SPELL_SHADOW_WALK_VEHICLE, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_woe_shadowcloak::HandleEffect, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

class spell_woe_shadowcloak_aura : public AuraScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SHADOW_WALK_VEHICLE, SPELL_SHADOW_WALK });
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        /*int32 duration = 0;
        if (Aura* aura = eventInfo.GetActor()->GetAura(SPELL_SHADOW_WALK))
            duration = aura->GetStackAmount();

        CastSpellExtraArgs args;
        args.TriggerFlags = TRIGGERED_FULL_MASK;
        args.AddSpellMod(SPELLVALUE_DURATION, GetDuration() * duration);
        eventInfo.GetActor()->CastSpell(eventInfo.GetActor(), SPELL_SHADOW_AMBUSHER, args);*/

        eventInfo.GetActor()->RemoveAurasDueToSpell(SPELL_SHADOW_WALK_VEHICLE);
        eventInfo.GetActor()->RemoveAurasDueToSpell(SPELL_SHADOW_WALK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_woe_shadowcloak_aura::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 110755 - (Serverside/Non-DB2) Shadowcloak Dismount
class spell_woe_shadowcloak_dismount : public AuraScript
{
    void HandleDismount(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasByType(SPELL_AURA_MOUNTED);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_woe_shadowcloak_dismount::HandleDismount, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 110121 - Distract Demon Missile
// 110103 - (Serverside/Non-DB2) Distract Demon Summon
class spell_woe_distract_demon_missile : public SpellScript
{
    void SelectTarget(WorldObject*& target)
    {
        Creature* stalker = GetCaster()->FindNearestCreatureWithOptions(50.0f, { .StringId = "wall_of_shadow_stalker" });
        _wallOfShadowGUID = stalker->GetGUID();
        target = stalker;
    }

    void StartMoving(SpellEffIndex /*effIndex*/)
    {
        if (Creature* stalker = ObjectAccessor::GetCreature(*GetCaster(), _wallOfShadowGUID))
        {
            stalker->GetMotionMaster()->MovePoint(POINT_START_MOVING, wallOfShadowStalkerPos);
            stalker->DespawnOrUnsummon(4s);
        }
    }

    void Register() override
    {
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_woe_distract_demon_missile::SelectTarget, EFFECT_0, TARGET_DEST_NEARBY_ENTRY);
        OnEffectHit += SpellEffectFn(spell_woe_distract_demon_missile::StartMoving, EFFECT_0, SPELL_EFFECT_ANY);
    }

private:
    ObjectGuid _wallOfShadowGUID;
};

// 104400 - Wall of Shadow
class spell_woe_wall_of_shadow : public AuraScript
{
    void PeriodicTick(AuraEffect const* /*aurEff*/)
    {
        // SPELL_ATTR1_TRACK_TARGET_IN_CHANNEL doesn't work with DEST targetsfSetOrientation
        GetCaster()->SetFacingToObject(GetTarget());
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_woe_wall_of_shadow::PeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// 54506 - Well of Eternity Stalker
struct npc_well_of_eternity_stalker : public ScriptedAI
{
    npc_well_of_eternity_stalker(Creature* creature) : ScriptedAI(creature) { }

    void UpdateAI(uint32 /*diff*/) override
    {
    }
};

void AddSC_well_of_eternity()
{
    // Intro
    new at_woe_nozdormu_intro();
    new at_woe_perotharn_intro();
    RegisterWellOfEternityCreatureAI(npc_woe_nozdormu);
    RegisterWellOfEternityCreatureAI(npc_woe_legion_demon);
    RegisterSpellScript(spell_woe_summon_fel_firewall_cosmetic_ph);
    RegisterSpellScript(spell_woe_summon_fire_wall);

    // First boss stuff
    new at_woe_illidan_intro();
    new at_woe_illidan_skip_talk();
    RegisterWellOfEternityCreatureAI(npc_woe_illidan_part_1);
    RegisterWellOfEternityCreatureAI(npc_woe_distract_demon_stalker);
    RegisterSpellAndAuraScriptPair(spell_woe_shadowcloak, spell_woe_shadowcloak_aura);
    RegisterSpellScript(spell_woe_shadowcloak_dismount);
    RegisterSpellScript(spell_woe_distract_demon_missile);
    RegisterSpellScript(spell_woe_wall_of_shadow);
    //RegisterWellOfEternityCreatureAI(npc_well_of_eternity_stalker);
}
