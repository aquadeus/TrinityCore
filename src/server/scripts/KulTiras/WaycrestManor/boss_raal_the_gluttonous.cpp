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
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "waycrest_manor.h"

enum RaalTheGluttonousSpells
{
    SPELL_TENDERIZE_AURA       = 280030,
    SPELL_TENDERIZE_SELECTOR   = 280031,
    SPELL_TENDERIZE_DAMAGE     = 264923,
    SPELL_CALL_SERVANT_DUMMY   = 264931,
    SPELL_CALL_SERVANT_SUMMON  = 264936,
    SPELL_WEL_FED              = 265005,
    SPELL_ROTTEN_EXPULSION_SELECTOR     = 264693,
    SPELL_ROTTEN_EXPULSION_CAST     = 264694,
    SPELL_ROTTEN_EXPULSION_MISSILE = 264697,
    SPELL_ROTTEN_EXPULSION_SUMMON   = 268231,

    // Heroic/Mythic
    SPELL_PLAYER_DETECTION     = 274651,
};

enum RaalTheGluttonousTexts
{
    // Shared
    SAY_AGGRO               = 0,
    SAY_CLAIM_THE_IRIS      = 1,
    SAY_SLAY                = 3,
    SAY_DEATH               = 4,
    SAY_DIRE_RITUAL_ALERT   = 5,
    SAY_DIRE_RITUAL         = 6,
};

enum RaalTheGluttonousEvents
{
    EVENT_TENDERIZE          = 1,
    EVENT_CALL_SERVANTS,
    EVENT_ROTTEN_EXPULSION
};

enum RaalTheGluttonousActions
{
    ACTION_CLAIM_THE_IRIS_INTRO  = 1,
    ACTION_MALADY_CLAIM_THE_IRIS = 2,
    ACTION_BRIAR_CLAIM_THE_IRIS  = 3,
    ACTION_SOLENA_CLAIM_THE_IRIS = 4
};

struct boss_raal_the_gluttonous : public BossAI
{
    boss_raal_the_gluttonous(Creature* creature) : BossAI(creature, DATA_RAAL_THE_GLUTTONOUS) { }

    void JustDied(Unit* /*killer*/) override
    {
        _JustDied();
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
    }

    /*void JustAppeared() override
    {
        me->SetPowerType(POWER_ENERGY);
        DoCast(SPELL_POWER_DISPLAY_TANTRUM);

        if (instance->GetData(DATA_CRAGMAW_CRAWG_EATING))
            return;

        DoCastSelf(SPELL_CRAWG_EATING);
        me->SetEmoteState(EMOTE_STATE_EAT);
        if (TempSummon* summon = me->SummonCreature(NPC_FETID_MAGGOT, FetidMaggotSpawn))
        {
            _fetidMaggotGuid = summon->GetGUID();
            summon->CastSpell(nullptr, SPELL_FEIGN_DEATH, true);
        }
    }*/

    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        summons.DespawnAll();
        _EnterEvadeMode();
        _DespawnAtEvade();
    }

    void JustEngagedWith(Unit* who) override
    {
        BossAI::JustEngagedWith(who);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me, 1);
        events.ScheduleEvent(EVENT_TENDERIZE, 20900ms);
        events.ScheduleEvent(EVENT_ROTTEN_EXPULSION, 5100ms);
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
            switch (events.ExecuteEvent())
            {
                case EVENT_TENDERIZE:
                {
                    me->CastSpell(me, SPELL_TENDERIZE_AURA, CastSpellExtraArgs(TRIGGERED_FULL_MASK).AddSpellMod(SPELLVALUE_AURA_STACK, 3));

                    events.ScheduleEvent(EVENT_CALL_SERVANTS, 3s);
                    events.Repeat(36000ms);
                    break;
                }
                case EVENT_CALL_SERVANTS:
                {
                    //DoCast(SPELL_ROTTEN_EXPULSION_SELECTOR);
                    break;
                }
                case EVENT_ROTTEN_EXPULSION:
                {
                    //DoCast(SPELL_ROTTEN_EXPULSION_SELECTOR);  // Doesnt seem to work with the selector, also is limited to 1 target when this spell hits everybody
                    DoCast(SPELL_ROTTEN_EXPULSION_CAST);
                    events.Repeat(20s, 30s);
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

// 264693 - Rotten Expulsion
class spell_raal_the_gluttonous_rotten_explosion_selector : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ROTTEN_EXPULSION_CAST });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_ROTTEN_EXPULSION_CAST, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_raal_the_gluttonous_rotten_explosion_selector::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 264694 - Rotten Expulsion
class spell_raal_the_gluttonous_rotten_explosion_cast : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ROTTEN_EXPULSION_MISSILE });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_ROTTEN_EXPULSION_MISSILE, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_raal_the_gluttonous_rotten_explosion_cast::HandleHitTarget, EFFECT_ALL, SPELL_EFFECT_DUMMY);
    }
};

// 264698 - Rotten Expulsion
class spell_raal_the_gluttonous_rotten_explosion_areatrigger : public SpellScript
{
    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ROTTEN_EXPULSION_SUMMON });
    }

    void InitSummon()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        Map* map = caster->GetMap();
        if (!map)
            return;

        if (map->IsDungeon() && map->IsHeroicOrHigher())
            caster->CastSpell(*GetExplTargetDest(), SPELL_ROTTEN_EXPULSION_SUMMON, true);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_raal_the_gluttonous_rotten_explosion_areatrigger::InitSummon);
    }
};

void AddSC_boss_raal_the_gluttonous()
{
    RegisterWaycrestManorCreatureAI(boss_raal_the_gluttonous);
    //RegisterSpellScript(spell_raal_the_gluttonous_rotten_explosion_selector);
    RegisterSpellScript(spell_raal_the_gluttonous_rotten_explosion_cast);
    RegisterSpellScript(spell_raal_the_gluttonous_rotten_explosion_areatrigger);
}
