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


#include "TranspositionTable.hpp"


TranspositionTable *TranspositionTable::table = nullptr;



TranspositionTable* TranspositionTable::getPtr() {
    if (table == nullptr) {
        table = new TranspositionTable();
    }
    return table;
}
void TranspositionTable::addEntry(ZobristHash hash, int32_t depth, uint8_t bestMoveIndex) {
    auto it = this->map.find(hash.getValue());
    if (it == this->map.end()) {
        this->map[hash.getValue()] = std::make_pair(depth, bestMoveIndex);
    }
    else if (it->second.first < depth) {
        this->map.erase(it);
        this->map[hash.getValue()] = std::make_pair(depth, bestMoveIndex);
    }
}
uint8_t TranspositionTable::getBestMoveIndex(ZobristHash hash) const {
    auto it = this->map.find(hash.getValue());
    if (it == this->map.end()) {
        return NONE;
    }
    return it->second.second;
}
TranspositionTable::TranspositionTable() = default;