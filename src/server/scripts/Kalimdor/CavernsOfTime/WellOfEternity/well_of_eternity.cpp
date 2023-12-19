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

#include "SpellAuras.h"
#include "SpellScript.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "well_of_eternity.h"

enum NozdormuTexts
{
    SAY_INTRO_1 = 0,
    SAY_INTRO_2 = 1,
    SAY_INTRO_3 = 2,
    SAY_INTRO_4 = 3
};

enum Actions
{
    ACTION_NOZDORMU_INTRO = 0
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
                if (!_intro)
                {
                    Talk(SAY_INTRO_1);

                    scheduler.Schedule(12s + 970ms, [this](TaskContext /*context*/)
                    {
                        Talk(SAY_INTRO_2);
                        scheduler.Schedule(5s + 950ms, [this](TaskContext /*context*/)
                        {
                            Talk(SAY_INTRO_3);
                            scheduler.Schedule(12s + 230ms, [this](TaskContext /*context*/)
                            {
                                Talk(SAY_INTRO_4);
                            });
                        });
                    });
                    _intro = true;
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
    bool _intro = false;
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
            if (Creature* perotharn = instance->GetCreature(DATA_PEROTHARN))
                perotharn->AI()->DoAction(ACTION_PEROTHARN_INTRO);
        }
        return true;
    }
};

void AddSC_well_of_eternity()
{
    RegisterWellOfEternityCreatureAI(npc_woe_nozdormu);
    new at_woe_nozdormu_intro();
    new at_woe_perotharn_intro();
}
