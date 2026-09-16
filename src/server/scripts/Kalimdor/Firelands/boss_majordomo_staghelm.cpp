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

#include "Creature.h"
#include "InstanceScript.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "firelands.h"

namespace Scripts::Kalimdor::Firelands::MajordomoStaghelm
{
    namespace Spells
    {
        static constexpr uint32 ZeroEnergyZeroRegen = 72242;
        static constexpr uint32 FormController = 98386;
        static constexpr uint32 ClumpCheck = 98399;
        static constexpr uint32 CatForm = 98374;
        static constexpr uint32 ScorpionForm = 98379;
        static constexpr uint32 LeapingFlamesSelector = 101165;
        static constexpr uint32 LeapingFlames = 98476;
        static constexpr uint32 Fury = 97235;
        static constexpr uint32 FlameScythe = 98474;
        static constexpr uint32 FieryCyclone = 98443;
        static constexpr uint32 Berserk = 47008;
        static constexpr uint32 Adrenaline = 97238;
        static constexpr uint32 BurningOrbsSummon = 98565;
        static constexpr uint32 SearingSeeds = 98450;
        static constexpr uint32 SearingSeed = 98620;
    }

    namespace Texts
    {
        static constexpr uint8 Intro1 = 0;
        static constexpr uint8 Intro2 = 1;
        static constexpr uint8 Intro3 = 2;
        static constexpr uint8 Aggro  = 3;
    }

    namespace Events
    {
        static constexpr uint8 Berserk = 1;
        static constexpr uint8 CheckEnergy = 2;
    }

    namespace Phases
    {
        static constexpr uint8 PhaseCat = 1;
        static constexpr uint8 PhaseScorpion =2;
        static constexpr uint8 PhaseHuman = 3;
    }

// 52571 - Majordomo Staghelm <Archdruid of the Flame>
struct boss_majordomo_staghelm : public BossAI
{
    boss_majordomo_staghelm(Creature* creature) : BossAI(creature, DATA_MAJORDOMO_STAGHELM), { }

    void JustAppeared() override
    {
        me->SetPowerType(me->GetPowerType());
        me->SetPower(me->GetPowerType(), 0);
    }

    void Reset() override
    {
        _Reset();
    }

    void JustDied(Unit* /*killer*/) override
    {
        _JustDied();
        Talk(Texts::Death);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        instance->SetBossState(DATA_MAJORDOMO_STAGHELM, DONE);
    }

    void SpellHit(WorldObject* /*caster*/, SpellInfo const* spellInfo) override
    {
        if (spellInfo->Id == Spells::CatForm)
            PhaseEvents(Phases::PhaseCat);
        else if (spellInfo->Id == Spells::ScorpionForm)
            PhaseEvents(Phases::PhaseScorpion);
    }

    void EnterEvadeMode(EvadeReason why) override
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        instance->SetBossState(DATA_MAJORDOMO_STAGHELM, FAIL);

        Talk(Texts::Wipe);

        summons.DespawnAll();
        _EnterEvadeMode();
        _DespawnAtEvade();
    }

    void JustEngagedWith(Unit* who) override
    {
        BossAI::JustEngagedWith(who);
        Talk(Texts::Aggro);

        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me, 1);
        instance->SetBossState(DATA_MAJORDOMO_STAGHELM, IN_PROGRESS);

        DoCastSelf(Spells::FormController);

        events.ScheduleEvent(Events::Berserk, 10min);
        events.ScheduleEvent(Events::CheckEnergy, 500ms);

        //if (IsHeroic())
            //events.ScheduleEvent(Events::FelNova, 12s);
    }

    void PhaseEvents(uint8 phase)
    {
        events.Reset();

        switch (phase)
        {
            case PhaseCat:
            {
                me->RemoveAurasDueToSpell(Spells::ScorpionForm);
                me->RemoveAurasDueToSpell(Spells::Adrenaline);
                me->SetPower(me->GetPowerType(), 0);
                DoCastSelf(Spells::CatForm);
                DoCastSelf(Spells::Rage);
                events.SetPhase(PhaseCat);
                break;
            }
            case PhaseScorpion:
            {
                me->RemoveAurasDueToSpell(Spells::CatForm);
                me->RemoveAurasDueToSpell(Spells::Adrenaline);
                me->SetPower(me->GetPowerType(), 0);
                DoCastSelf(Spells::ScorpionForm);
                DoCastSelf(Spells::Rage);
                events.SetPhase(PhaseScorpion);
                break;
            }

            case PhaseHuman:
            {
                break;
            }
            default:
                break;
        }
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
            switch (eventId)
            {
                case Events::Berserk:
                {
                    DoCastSelf(Spells::Berserk);
                    break;
                }
                case Events::CheckEnergy:
                {
                    if (!me->GetPower(POWER_ENERGY) == me->GetMaxPower(POWER_ENERGY))
                        return;

                    if (events.IsInPhase(PhaseScorpion))
                        DoCastVictim(Spells::FlameScythe);
                    if (events.IsInPhase(PhaseCat))
                        DoCast(Spells::LeapingFlamesSelector);

                    events.Repeat(500ms);
                    break;
                }
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }
    }
};

// 98386 - Form Controller
class spell_majordomo_slaghelm_form_controller : public AuraScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ Spells::ClumpCheck });
    }

    void HandlePeriodicEffect(AuraEffect const* aurEff) const
    {
        Unit* target = GetTarget();
        target->CastSpell(target, Spells::ClumpCheck, CastSpellExtraArgsInit{
            .TriggerFlags = TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR,
            .TriggeringAura = aurEff
        });
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_majordomo_slaghelm_form_controller::HandlePeriodicEffect, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 101165 - Leaping Flames
class spell_majordomo_slaghelm_leaping_flames_selector : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ Spells::LeapingFlames });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/) const
    {
        GetCaster()->CastSpell(GetHitUnit()->GetPosition(), Spells::LeapingFlames, CastSpellExtraArgsInit{
            .TriggerFlags = TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR,
            .TriggeringSpell = GetSpell()
        });
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_majordomo_slaghelm_leaping_flames_selector::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 98476 - Leaping Flames
class spell_majordomo_slaghelm_leaping_flames_jump : public SpellScript
{
    void HandleScript(SpellEffIndex /*effIndex*/) const
    {
        GetCaster()->CastSpell(GetCaster()->GetPosition(), GetEffectValueAsInt(), TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR);
    }

    void Register() override
    {
        OnEffectLaunch += SpellEffectFn(spell_majordomo_slaghelm_leaping_flames_jump::HandleScript, EFFECT_0, SPELL_EFFECT_JUMP_DEST);
    }
};

// 98451 - Burning Orbs
class spell_majordomo_staghelm_burning_orbs : public SpellScript
{
    bool Validate(SpellInfo const* /*spell*/) override
    {
        return ValidateSpellInfo({ Spells::BurningOrbsSummon });
    }

    void HandleOrbs() const
    {
        Unit* caster = GetCaster();
        uint8 OrbsCount = caster->GetMap()->GetPlayersCountExceptGMs() / 5;

        if (OrbsCount < 5)
            OrbsCount = 1;

        for (uint8 i = 0; i < OrbsCount; ++i)
            caster->CastSpell(caster, Spells::BurningOrbsSummon, TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_majordomo_staghelm_burning_orbs::HandleOrbs);
    }
};
};

void AddSC_boss_majordomo_staghelm()
{
    using namespace Scripts::Kalimdor::Firelands::MajordomoStaghelm;

    RegisterFirelandsAI(boss_majordomo_staghelm);

    RegisterSpellScript(spell_majordomo_slaghelm_form_controller);
    RegisterSpellScript(spell_majordomo_slaghelm_leaping_flames_selector);
    RegisterSpellScript(spell_majordomo_slaghelm_leaping_flames_jump);
    RegisterSpellScript(spell_majordomo_staghelm_burning_orbs);

}
