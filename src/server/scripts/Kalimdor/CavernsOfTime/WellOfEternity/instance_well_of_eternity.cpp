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

#include "InstanceScript.h"
#include "ScriptMgr.h"
#include "well_of_eternity.h"

ObjectData const creatureData[] =
{
    { BOSS_PEROTHARN,     DATA_PEROTHARN     },
    { BOSS_QUEEN_AZSHARA, DATA_QUEEN_AZSHARA },
    { BOSS_MANNOROTH,     DATA_MANNOROTH     },
    { NPC_NOZDORMU,       DATA_NOZDORMU      },
    { 0,                  0                  }  // END
};

DoorData const doorData[] =
{
    { 0,                  0,                  EncounterDoorBehavior::OpenWhenNotInProgress }  // END
};

DungeonEncounterData const encounters[] =
{
    { DATA_PEROTHARN,     {{ 1272 }} },
    { DATA_QUEEN_AZSHARA, {{ 1273 }} },
    { DATA_MANNOROTH,     {{ 1274 }} }
};

class instance_well_of_eternity : public InstanceMapScript
{
    public:
        instance_well_of_eternity() : InstanceMapScript(WOEScriptName, 939) { }

        struct instance_well_of_eternity_InstanceMapScript: public InstanceScript
        {
            instance_well_of_eternity_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(EncounterCount);
                LoadObjectData(creatureData, nullptr);
                LoadDoorData(doorData);
                LoadDungeonEncounterData(encounters);
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_well_of_eternity_InstanceMapScript(map);
        }
};

void AddSC_instance_well_of_eternity()
{
    new instance_well_of_eternity();
}
