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

#include "CellImpl.h"
#include "GameObject.h"
#include "GridNotifiersImpl.h"
#include "InstanceScript.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "well_of_eternity.h"

enum PerotharnTexts
{
    SAY_INTRO_1 = 0,
    SAY_INTRO_2 = 1,
    SAY_INTRO_3 = 2
};

// 55085 - Peroth'arn
struct boss_peroth_arn : public BossAI
{
    boss_peroth_arn(Creature* creature) : BossAI(creature, DATA_PEROTHARN) { }

    void DoAction(int32 actionId) override
    {
        switch (actionId)
        {
            case ACTION_PEROTHARN_INTRO:
            {
            	Talk(SAY_INTRO_1);
                /*scheduler.Schedule(5s + 659ms, [this](TaskContext /*context*)
                {
                    Talk(SAY_INTRO_2);
                    scheduler.Schedule(3s + 622ms, [this](TaskContext /*context*)
	                {
                        Talk(SAY_INTRO_3);
	                });
                });*/
                break;
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
};

void AddSC_boss_peroth_arn()
{
	RegisterWellOfEternityCreatureAI(boss_peroth_arn);
}
