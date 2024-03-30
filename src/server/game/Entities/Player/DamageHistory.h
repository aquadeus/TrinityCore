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

#ifndef DamageHistory_h__
#define DamageHistory_h__

#include "Common.h"

class TC_GAME_API DamageHistory
{
    public:
        DamageHistory() : _lastCleanup(0) { }

        void AddDamageTaken(uint64 damage);
        uint64 CountDamageTaken(time_t startTime) const;
        time_t GetLastCleanup() const { return _lastCleanup; }

        void Cleanup(time_t now);

        static uint32 const CleanupInterval;

    private:
        time_t _lastCleanup;

        typedef std::multimap<time_t, uint64> StorageType;
        StorageType _takenData;
};


#endif DamageHistory_h__
