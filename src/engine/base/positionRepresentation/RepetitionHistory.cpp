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


#include "RepetitionHistory.hpp"


RepetitionHistory::RepetitionHistory() = default;
void RepetitionHistory::addPosition(ZobristHash hash) {
    this->hashes.push_back(hash);
}
void RepetitionHistory::clear() {
    this->hashes.clear();
}
uint8_t RepetitionHistory::getRepetitionNumber(ZobristHash hash) const {
    uint8_t ctr = 0;
    for (auto hash1 : this->hashes) {
        if (hash == hash1) {
            ctr = ctr + 1;
        }
    }
    return ctr;
}