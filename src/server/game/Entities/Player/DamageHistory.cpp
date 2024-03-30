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

#include "DamageHistory.h"
#include "GameTime.h"
#include <numeric>

uint32 const DamageHistory::CleanupInterval = 60;

void DamageHistory::AddDamageTaken(uint64 damage)
{
    _takenData.emplace(GameTime::GetGameTime(), damage);
}

uint64 DamageHistory::CountDamageTaken(time_t startTime) const
{
    auto itr = _takenData.lower_bound(startTime);
    return std::accumulate(itr, _takenData.end(), uint64(0), [](uint64 damageTaken, StorageType::value_type const& pair)
    {
        return damageTaken += pair.second;
    });
}

void DamageHistory::Cleanup(time_t now)
{
    _lastCleanup = now;

    auto itr = _takenData.lower_bound(_lastCleanup - CleanupInterval);
    _takenData.erase(_takenData.begin(), itr);
}
