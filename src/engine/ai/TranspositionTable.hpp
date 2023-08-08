/*
 *  Natrix
 *  Copyright (C) 2023 gth-other
 *
 *  Natrix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Natrix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Natrix.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <unordered_map>
#include "../base/positionRepresentation/ZobristHash.hpp"


#pragma once


class TranspositionTable {
public:
    static TranspositionTable* getPtr();
    TranspositionTable(const TranspositionTable &donor) = delete;

    void addEntry(ZobristHash hash, int32_t depth, uint8_t bestMoveIndex);
    [[nodiscard]] uint8_t getBestMoveIndex(ZobristHash hash) const;

    static constexpr uint8_t NONE = 255;
private:
    TranspositionTable();

    static TranspositionTable* table;

    std::unordered_map<uint64_t, std::pair<int32_t, uint8_t>> map;
};