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
#include "well_of_eternity.h"
#include "InstanceScript.h"

ObjectData const creatureData[] =
{
    { NPC_PEROTHARN,      BOSS_PEROTHARN     },
    { NPC_QUEEN_AZSHARA,  BOSS_QUEEN_AZSHARA },
    { NPC_MANNOROTH,      BOSS_MANNOROTH     },
    { NPC_NOZDORMU,       DATA_NOZDORMU      },
    { NPC_ILLIDAN_PART_1, DATA_ILLIDAN_1     },
    { 0,                  0                  }  // END
};

DungeonEncounterData const encounters[] =
{
    { BOSS_PEROTHARN,       {{ 1272 }}  },
    { BOSS_QUEEN_AZSHARA,   {{ 1273 }}  },
    { BOSS_MANNOROTH,       {{ 1274 }}  }
};

class instance_well_of_eternity : public InstanceMapScript
{
public:
    instance_well_of_eternity() : InstanceMapScript(WoEScriptName, 939) { }

    struct instance_well_of_eternity_InstanceMapScript : public InstanceScript
    {
        instance_well_of_eternity_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
        {
            SetHeaders(DataHeader);
            SetBossNumber(EncounterCount);
            LoadObjectData(creatureData, nullptr);
            LoadDungeonEncounterData(encounters);

            IllidanEscortState = NOT_STARTED;
            IllidanIntroState = NOT_STARTED;
        }

        void OnPlayerEnter(Player* /*player*/) override
        {
            if (GetData(DATA_ILLIDAN_START_ESCORT) == DONE)
                DoCastSpellOnPlayers(SPELL_SHADOWCLOAK_PLAYER);
        }

        void SetData(uint32 type, uint32 data) override
        {
            switch (type)
            {
                case DATA_ILLIDAN_START_ESCORT:
                {
                    IllidanEscortState = data;
                    break;
                }
                case DATA_ILLIDAN_START_INTRO:
                {
                    IllidanIntroState = data;
                    break;
                }
                default:
                    break;
            }
        }

        uint32 GetData(uint32 type) const override
        {
            switch (type)
            {
                case DATA_ILLIDAN_START_ESCORT:
                    return IllidanEscortState;
                case DATA_ILLIDAN_START_INTRO:
                    return IllidanIntroState;
                default:
                    break;
            }

            return 0;
        }

        uint8 IllidanEscortState;
        uint8 IllidanIntroState;
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
