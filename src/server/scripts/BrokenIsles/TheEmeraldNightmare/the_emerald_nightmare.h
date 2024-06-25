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

#ifndef _The_Emerald_Nightmare_h__
#define _The_Emerald_Nightmare_h__

#include "CreatureAIImpl.h"

constexpr char const* DataHeader = "TEN";
constexpr char const* TheEmeraldNightmareScriptName = "instance_the_emerald_nightmare";

constexpr uint32 const EncounterCount = 7;

enum TheEmeraldNightmareDataTypes
{
    // Encounters
    DATA_NYTHENDRA                        = 0,
    DATA_ILGYNOTH_THE_HEART_OF_CURRUPTION = 1,
    DATA_ELERETHE_RENFERAL                = 2,
    DATA_URSOC                            = 3,
    DATA_DRAGONS_OF_NIGHTMARE             = 4,
    DATA_CENARIUS                         = 5,
    DATA_XAVIUS                           = 6
};

enum TheEmeraldNightmareCreatureIds
{
    BOSS_NYTHENDRA                        = 102672
};

template <class AI, class T>
inline AI* GetTheEmeraldNightmareAI(T* obj)
{
    return GetInstanceAI<AI>(obj, TheEmeraldNightmareScriptName);
}

#define RegisterTheEmeraldNightmareCreatureAI(ai_name) RegisterCreatureAIWithFactory(ai_name, GetTheEmeraldNightmareAI)

#endif // _The_Emerald_Nightmare_h__
