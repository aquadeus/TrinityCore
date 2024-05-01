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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "ragefire_chasm.h"

enum Spells
{
	SPELL_INFERNO_CHARGE_DAMAGE		= 119299,
	SPELL_INFERNO_CHARGE_CAST		= 119405,
	SPELL_INFERNO_CHARGE_SUMMON		= 119297, // Serverside
	SPELL_FIRE_BREATH				= 119420
};

enum Texts
{
    SAY_INFERNO_CHARGE              = 0
};

enum Events
{
	EVENT_INFERNO_CHARGE 			= 1,
	EVENT_FIRE_BREATH				= 2
};

// 61408 - Adarogg
struct boss_adarogg : public BossAI
{
    boss_adarogg(Creature* creature) : BossAI(creature, BOSS_ADAROGG) { }

    void Reset() override
    {
        _Reset();
    }

    void JustEngagedWith(Unit* who) override
    {
        BossAI::JustEngagedWith(who);
        events.ScheduleEvent(EVENT_INFERNO_CHARGE, 10s);
        events.ScheduleEvent(EVENT_FIRE_BREATH, 20s);
    }

    void JustDied(Unit* /*killer*/) override
    {
        _JustDied();
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
                case EVENT_INFERNO_CHARGE:
                {
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0, NonTankTargetSelector(me)))
                    {
                        Talk(SAY_INFERNO_CHARGE, target);
                        DoCast(target, SPELL_INFERNO_CHARGE_SUMMON);
                        DoCast(target, SPELL_INFERNO_CHARGE_CAST);
                    }
                    events.Repeat(15s, 20s);
                    break;
                }
                case EVENT_FIRE_BREATH:
                {
                    DoCastVictim(SPELL_FIRE_BREATH);
                    events.Repeat(15s, 20s);
                    break;
                }
                default:
                    break;
            }
        }
    }
};

// 119298 - (Serverside/Non-DB2) Inferno Charge
class spell_rfc_inferno_charge : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_INFERNO_CHARGE_DAMAGE });
    }

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (!GetCaster())
            return;

        if (Unit* target = GetHitUnit())
            target->CastSpell(target, SPELL_INFERNO_CHARGE_DAMAGE, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_rfc_inferno_charge::HandleScript, EFFECT_0, SPELL_EFFECT_CHARGE);
    }
};

void AddSC_boss_adarogg()
{
	RegisterRagefireChasmCreatureAI(boss_adarogg);
	RegisterSpellScript(spell_rfc_inferno_charge);
}
