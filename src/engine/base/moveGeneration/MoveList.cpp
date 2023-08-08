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


#include "MoveList.hpp"


MoveList::MoveList() {
    this->size = 0;
}
Move &MoveList::operator[](uint8_t index) {
    return this->moves[index];
}
Move MoveList::operator[](uint8_t index) const {
    return this->moves[index];
}
void MoveList::push(Move move) {
    this->moves[this->size] = move;
    this->size = this->size + 1;
}
uint8_t MoveList::getSize() const {
    return this->size;
}