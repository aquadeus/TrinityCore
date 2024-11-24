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

#include "CreatureAI.h"
#include "CreatureAIImpl.h"
#include "Containers.h"
#include "MotionMaster.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuras.h"

enum DoomwalkerSpells
{
	// Serverside spells, 32632 is confirmed, others are guessed
    SPELL_SUMMON_OVERRUN_TARGET = 32632,
    SPELL_CHARGE_OVERRUN_TARGET = 32632,
    SPELL_SELECT_OVERRUN_TARGET = 32635,

    SPELL_LIGHTNING_WRATH		= 33665,

    SPELL_FRENZY				= 33653,

    SPELL_CRUSH_ARMOR			= 33661,

    SPELL_EARTHQUAKE			= 326405,
    SPELL_KNOCKBACK				= 13360
};

enum LeaxaEvents
{
    EVENT_BLOOD_BOLT        = 1,
    EVENT_CREEPING_ROT,
    EVENT_SANGUINE_FEAST,
    EVENT_BLOOD_MIRROR
};

enum LeaxaTexts
{
    SAY_AGGRO                   = 0,
    SAY_SANGUINE_FEAST          = 1,
    SAY_ROT_AND_WITHER          = 2,
    SAY_BLOOD_MIRROR            = 3,
    SAY_ANNOUNCE_BLOOD_MIRROR   = 4,
    SAY_DEATH                   = 5
};

// 167749 - Doomwalker
struct boss_doomwalker_anniversary : public WorldBossAI
{
    boss_doomwalker_anniversary(Creature* creature) : WorldBossAI(creature) { }

    void JustDied(Unit* /*killer*/) override
    {
        WorldBossAI::_JustDied();
        Talk(SAY_DEATH);
    }

    void JustEngagedWith(Unit* who) override
    {
        WorldBossAI::JustEngagedWith(who);
        Talk(SAY_AGGRO);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;
    }
};

void AddSC_boss_doomwalker_anniversary()
{
    RegisterCreatureAI(boss_doomwalker_anniversary);
}
