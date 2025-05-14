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

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "Containers.h"
#include "Conversation.h"
#include "Creature.h"
#include "InstanceScript.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "PathGenerator.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TaskScheduler.h"
#include "TemporarySummon.h"
#include "the_stonevault.h"

enum MasterMachinistsSpells
{
    SPELL_SILENCED_SPEAKER          = 439577,
    SPELL_SILENCED_SPEAKER_DAMAGE   = 457144,

    SPELL_EXHAUST_VENTS             = 445541,
    SPELL_EXHAUST_VENTS_AREATRIGGER = 471646,
    SPELL_EXHAUST_VENTS_AREATRIGGER_2 = 428820,
    SPELL_EXHAUST_VENTS_TRIGGER_DAMAGE  = 462372,
    SPELL_EXHAUST_VENTS_MISSILE     = 471618,

    SPELL_SCRAP_CUBE_AREATRIGGER    = 428555,
    SPELL_SCRAP_CUBE_DAMAGE         = 428547,




    SPELL_MOLTEN_METAL_SELECTOR     = 430097,
    SPELL_MOLTEN_METAL_MISSILE      = 430098,

    SPELL_IGNEOUS_HAMMER            = 428711,

    SPELL_LAVA_CANNON_SELECTOR      = 428120,
    SPELL_LAVA_CANNON_AREATRIGGER   = 449167,
    SPELL_LAVA_CANNON_DAMAGE        = 449169
};

enum MasterMachinistsEvents
{
    // Dorlita
    EVENT_BLAZING_CRESCENDO   = 1,
    EVENT_LAVA_CANNON,
    EVENT_IGNEOUS_HAMMER,

    // Brokk
    EVENT_MOLTEN_METAL,
    EVENT_SCRAP_SONG,
    EVENT_EXHAUST_VENTS
};

enum MasterMachinistsTexts
{
    SAY_INTRO               = 0,
    SAY_AGGRO               = 1,
    SAY_VOLATILE_SPIKE      = 2,
    SAY_REFRACTING_BEAM     = 3,
    SAY_SEISMIC_SMASH       = 4,
    SAY_SEISMIC_SMASH_ALERT = 5,
    SAY_EARTH_SHATTERER     = 6,
    SAY_SLAY                = 7,
    SAY_WIPE                = 8,
    SAY_DEATH               = 9
};

uint32 const MasterMachinistsData[2] =
{
    DATA_SPEAKER_DORLITA,
    DATA_SPEAKER_BROKK
};

namespace
{
void MasterMachinistsEncounterStart(InstanceScript* instance)
{
    if (instance->GetBossState(DATA_MASTER_MACHINISTS) == IN_PROGRESS)
        return;

    instance->SetBossState(DATA_MASTER_MACHINISTS, IN_PROGRESS);

    for (uint32 data : MasterMachinistsData)
    {
        if (Creature* speaker = instance->GetCreature(data))
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, speaker, 1);
            speaker->AI()->DoZoneInCombat();
        }
    }
}

void MasterMachinistsEncounterFail(InstanceScript* instance, EvadeReason why, Creature* invoker)
{
    if (instance->GetBossState(DATA_MASTER_MACHINISTS) == FAIL)
        return;

    instance->SetBossState(DATA_MASTER_MACHINISTS, FAIL);

    for (uint32 data : MasterMachinistsData)
    {
        if (Creature* speaker = instance->GetCreature(data))
        {
            if (speaker == invoker)
                continue;

            speaker->AI()->EnterEvadeMode(why);
        }
    }
}

void MasterMachinistsEncounterDone(InstanceScript* instance)
{
    if (instance->GetBossState(DATA_MASTER_MACHINISTS) == DONE)
        return;

    for (uint32 data : MasterMachinistsData)
    {
        if (Creature* speaker = instance->GetCreature(data))
        {
            if (speaker->IsAlive())
                return;
        }
    }

    for (uint32 data : MasterMachinistsData)
    {
        if (Creature* speaker = instance->GetCreature(data))
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, speaker);
    }

    instance->SetBossState(DATA_MASTER_MACHINISTS, DONE);
}
}

struct MasterMachinistsSharedAI : public BossAI
{
    MasterMachinistsSharedAI(Creature* creature, uint32 bossId) : BossAI(creature, bossId) { }

    void Reset() override
    {
        events.Reset();
    }

    void EnterEvadeMode(EvadeReason why) override
    {
        MasterMachinistsEncounterFail(instance, why, me);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

        _EnterEvadeMode();
        _DespawnAtEvade();
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        MasterMachinistsEncounterStart(instance);
    }

    void JustDied(Unit* /*killer*/) override
    {
        _JustDied();

        Talk(SAY_DEATH);

        MasterMachinistsEncounterDone(instance);
    }

    void KilledUnit(Unit* victim) override
    {
        if (!victim->IsPlayer())
            return;

        Talk(SAY_SLAY);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteEvent(eventId);
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }
    }
};

// 213216 - Speaker Dorlita
struct boss_speaker_dorlita : public MasterMachinistsSharedAI
{
    boss_speaker_dorlita(Creature* creature) : MasterMachinistsSharedAI(creature, DATA_SPEAKER_DORLITA) { }

    void JustDied(Unit* killer) override
    {
        MasterMachinistsSharedAI::JustDied(killer);

        if (Creature* brokk = instance->GetCreature(DATA_SPEAKER_BROKK))
            me->CastSpell(brokk, SPELL_SILENCED_SPEAKER, TRIGGERED_FULL_MASK);
    }

    void JustEngagedWith(Unit* who) override
    {
        MasterMachinistsSharedAI::JustEngagedWith(who);

        events.ScheduleEvent(EVENT_LAVA_CANNON, 12200ms);
        events.ScheduleEvent(EVENT_IGNEOUS_HAMMER, 6100ms);

        if (IsMythic() || IsMythicPlus())
            events.ScheduleEvent(EVENT_BLAZING_CRESCENDO, 45s);
        else
            events.ScheduleEvent(EVENT_BLAZING_CRESCENDO, 45700ms);
    }

    void ExecuteEvent(uint32 eventId) override
    {
        switch (eventId)
        {
            /*case EVENT_LAVA_CANNON:
            {
                if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))
                    DoCast(target, SPELL_RUINOUS_BOLT);
                events.Repeat(19400ms);
                break;
            }
            case EVENT_IGNEOUS_HAMMER:
            {
                if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))
                    DoCast(target, SPELL_RUINOUS_BOLT_ENHANCED);
                events.Repeat(13340ms);
                break;
            }
            case EVENT_BLAZING_CRESCENDO:
            {
                Talk(SAY_UNSTABLE_RUNIC_MARK);
                DoCast(SPELL_UNSTABLE_RUNIC_MARK);
                events.Repeat(52100ms);
                break;
            }
            default:
                MasterMachinistsSharedAI::ExecuteEvent(eventId);
                break;*/
        }
    }
};

// 213217 - Speaker Brokk
struct boss_speaker_brokk : public MasterMachinistsSharedAI
{
    boss_speaker_brokk(Creature* creature) : MasterMachinistsSharedAI(creature, DATA_SPEAKER_BROKK) { }

    void JustDied(Unit* killer) override
    {
        MasterMachinistsSharedAI::JustDied(killer);

        if (Creature* dorlita = instance->GetCreature(DATA_SPEAKER_DORLITA))
            me->CastSpell(dorlita, SPELL_SILENCED_SPEAKER, TRIGGERED_FULL_MASK);
    }

    void JustEngagedWith(Unit* who) override
    {
        MasterMachinistsSharedAI::JustEngagedWith(who);

        events.ScheduleEvent(EVENT_SCRAP_SONG, 15500ms);
        

        if (IsMythic() || IsMythicPlus())
        {
            events.ScheduleEvent(EVENT_MOLTEN_METAL, 4600ms);
            events.ScheduleEvent(EVENT_EXHAUST_VENTS, 34100ms);
        }
        else
        {
            events.ScheduleEvent(EVENT_MOLTEN_METAL, 3200ms);
            events.ScheduleEvent(EVENT_EXHAUST_VENTS, 5700ms);
        }
    }

    void ExecuteEvent(uint32 eventId) override
    {
        switch (eventId)
        {
            /*case EVENT_SCRAP_SONG:
            {
                if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))
                    DoCast(target, SPELL_RUINOUS_BOLT);
                events.Repeat(51900ms);
                break;
            }
            case EVENT_MOLTEN_METAL:
            {
                if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))
                    DoCast(target, SPELL_RUINOUS_BOLT_ENHANCED);
                events.Repeat(15800ms);
                break;
            }
            case EVENT_EXHAUST_VENTS:
            {
                Talk(SAY_UNSTABLE_RUNIC_MARK);
                DoCast(SPELL_UNSTABLE_RUNIC_MARK);
                events.Repeat(27900ms);
                break;
            }
            default:
                MasterMachinistsSharedAI::ExecuteEvent(eventId);
                break;*/
        }
    }
};

// 439577 - Silenced Speaker
class spell_master_machinists_silenced_speaker : public AuraScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SILENCED_SPEAKER_DAMAGE });
    }

    void HandlePeriodic(AuraEffect const* aurEff) const
    {
        Unit* target = GetTarget();

        target->CastSpell(target, SPELL_SILENCED_SPEAKER_DAMAGE, CastSpellExtraArgsInit{
            .TriggerFlags = TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR,
            .TriggeringAura = aurEff
        });
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_master_machinists_silenced_speaker::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 430097 - Molten Metal
class spell_master_machinists_molten_metal_selector : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MOLTEN_METAL_MISSILE });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/) const
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_MOLTEN_METAL_MISSILE, CastSpellExtraArgsInit{
            .TriggerFlags = TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR,
            .TriggeringSpell = GetSpell()
        });
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_master_machinists_molten_metal_selector::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 428120 - Lava Cannon
class spell_master_machinists_lava_cannon_selector : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_LAVA_CANNON_AREATRIGGER });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/) const
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_LAVA_CANNON_AREATRIGGER, false);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_master_machinists_lava_cannon_selector::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 445541 - Exhaust Vents
class spell_master_machinists_exhaust_vents_periodic : public AuraScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_EXHAUST_VENTS_MISSILE });
    }

    void HandlePeriodic(AuraEffect const* aurEff) const
    {
        GetCaster()->CastSpell(GetTarget(), SPELL_EXHAUST_VENTS_MISSILE , CastSpellExtraArgsInit{
            .TriggerFlags = TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR | TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD,
            .TriggeringAura = aurEff
        });
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_master_machinists_exhaust_vents_periodic::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 449167 - Lava Cannon
// ID - 33334
struct at_master_machinists_lava_cannon : AreaTriggerAI
{
    using AreaTriggerAI::AreaTriggerAI;

    void OnInitialize() override
    {
        Position destPos = at->GetPosition();
        at->MovePositionToFirstCollision(destPos, 100.0f, 0.0f);

        PathGenerator path(at);
        path.CalculatePath(destPos.GetPositionX(), destPos.GetPositionY(), destPos.GetPositionZ(), true);

        at->InitSplines(path.GetPath());
    }

    void OnDestinationReached() override
    {
        at->Remove();
    }

    void OnUnitEnter(Unit* unit) override
    {
        if (!unit->IsPlayer())
            return;

        Unit* caster = at->GetCaster();
        if (!caster)
            return;

        caster->CastSpell(unit, SPELL_LAVA_CANNON_DAMAGE, TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR);
    }
};

void AddSC_boss_master_machinists()
{
    RegisterTheStonevaultCreatureAI(boss_speaker_dorlita);
    RegisterTheStonevaultCreatureAI(boss_speaker_brokk);
    RegisterSpellScript(spell_master_machinists_silenced_speaker);
    RegisterSpellScript(spell_master_machinists_molten_metal_selector);
    RegisterSpellScript(spell_master_machinists_lava_cannon_selector);
    RegisterSpellScript(spell_master_machinists_exhaust_vents_periodic);

    RegisterAreaTriggerAI(at_master_machinists_lava_cannon);
}
