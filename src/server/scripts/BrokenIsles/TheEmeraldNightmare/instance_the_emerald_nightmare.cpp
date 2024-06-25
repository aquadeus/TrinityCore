/*
 * Copyright (C) 2012-2017 OMFG.GG <https://www.omfg.gg/>
 *
 * This file is free software; as a special exception the author gives
 * unlimited permission to copy and/or distribute it, with or without
 * modifications, as long as this notice is preserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "InstanceScript.h"
#include "ScriptMgr.h"
#include "the_emerald_nightmare.h"

ObjectData const creatureData[] =
{
    { BOSS_NYTHENDRA, DATA_NYTHENDRA },
    { 0,              0              } // END
};

DungeonEncounterData const encounters[] =
{
    { DATA_NYTHENDRA,                        {{ 1853 }} },
    { DATA_ILGYNOTH_THE_HEART_OF_CURRUPTION, {{ 1873 }} },
    { DATA_ELERETHE_RENFERAL,                {{ 1876 }} },
    { DATA_URSOC,                            {{ 1841 }} },
    { DATA_DRAGONS_OF_NIGHTMARE,             {{ 1854 }} },
    { DATA_CENARIUS,                         {{ 1877 }} },
    { DATA_XAVIUS,                           {{ 1864 }} }
};

class instance_the_emerald_nightmare : public InstanceMapScript
{
    public:
        instance_the_emerald_nightmare() : InstanceMapScript("instance_the_emerald_nightmare", 1520) { }

        struct instance_the_emerald_nightmare_InstanceMapScript : public InstanceScript
        {
            instance_the_emerald_nightmare_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(EncounterCount);
                LoadObjectData(creatureData, nullptr);
                LoadDungeonEncounterData(encounters);
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_the_emerald_nightmare_InstanceMapScript(map);
        }
};

void AddSC_instance_the_emerald_nightmare()
{
    new instance_the_emerald_nightmare();
}
