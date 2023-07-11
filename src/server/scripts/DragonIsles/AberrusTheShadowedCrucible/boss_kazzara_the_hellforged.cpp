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
#include "aberrus_the_shadowed_crucible.h"

struct boss_kazzara_the_hellforged : public BossAI
{
    boss_kazzara_the_hellforged(Creature* creature) : BossAI(creature, DATA_KAZZARA_THE_HELLFORGED)
    {
        me->SetVisible(false);
    }

    void Reset() override
    {
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        instance->SetBossState(DATA_KAZZARA_THE_HELLFORGED, IN_PROGRESS);
    }

    void JustDied(Unit* /*killer*/) override
    {
    }

    void AttackStart(Unit* victim) override
    {
        if (me->IsImmuneToPC())
            return;

        ScriptedAI::AttackStart(victim);
    }

    void JustReachedHome() override
    {
        _JustReachedHome();
        Reset();
        instance->SetBossState(DATA_KAZZARA_THE_HELLFORGED, FAIL);
    }
};

void AddSC_boss_kazzara_the_hellforged()
{
    RegisterAberrusTheShadowedCrucibleCreatureAI(boss_kazzara_the_hellforged);
}
