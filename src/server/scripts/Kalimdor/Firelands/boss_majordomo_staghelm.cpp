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
        static constexpr uint32 LeapingFlamesSelector = 101165;
        static constexpr uint32 LeapingFlames = 98476;
        static constexpr uint32 LeapingFlamesSummon = 101222;
        static constexpr uint32 ScorpionForm = 98379;
        static constexpr uint32 Fury = 97235;
        static constexpr uint32 FlameScythe = 98474;
        static constexpr uint32 FieryCyclone = 98443;
    }

    namespace Texts
    {

    }

    namespace Events
    {

    }

    namespace Positions
    {
    }

    namespace Points
    {
    }

// 52571 - Majordomo Staghelm <Archdruid of the Flame>
struct boss_majordomo_staghelm : public BossAI
{
    boss_majordomo_staghelm(Creature* creature) : BossAI(creature, DATA_MAJORDOMO_STAGHELM), _shapeShiftCount(0), { }

    void JustAppeared() override
    {
        //DoCastSelf(Spells::FelCrazed, TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR);
        //DoCast(Spells::IllicitInfusionVisual);
    }

    void Reset() override
    {
        _Reset();
        _shapeShiftCount = 0;
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
        if (spellInfo->Id == Spells::Destabilized)
            Talk(Texts::Destabilized);
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

        events.ScheduleEvent(Events::ChaosBarrage, 1s);
        events.ScheduleEvent(Events::MirrorImages, 14s);

        if (IsHeroicOrHigher())
            events.ScheduleEvent(Events::FelNova, 12s);
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
                case Events::ChaosBarrage:
                {
                    DoCastVictim(Spells::ChaosBarrage, TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR);
                    events.Repeat(3500ms);
                    break;
                }
                case Events::MirrorImages:
                {
                    Talk(Texts::MirrorImages);
                    DoCastSelf(Spells::MirrorImages, TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_DONT_REPORT_CAST_ERROR);
                    events.Repeat(30500ms);
                    break;
                }
                case Events::FelNova:
                {
                    Talk(Texts::FelNova);
                    DoCast(Spells::FelNovaSelector);
                    events.Repeat(15s);
                    break;
                }
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }
    }

private:
    uint8 _felshieldCount;
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


}

void AddSC_boss_majordomo_staghelm()
{
    using namespace Scripts::Kalimdor::Firelands::MajordomoStaghelm;

    RegisterFirelandsAI(boss_majordomo_staghelm);

    RegisterSpellScript(spell_majordomo_slaghelm_form_controller);
}
