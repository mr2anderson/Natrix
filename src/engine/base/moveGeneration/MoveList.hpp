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


#include <array>
#include "../positionRepresentation/Move.hpp"


#pragma once


class MoveList {
public:
    MoveList();

    Move &operator[](uint8_t index);
    Move operator[](uint8_t index) const;

    void push(Move move);
    [[nodiscard]] uint8_t getSize() const;
private:
    std::array<Move, 220> moves{};
    uint8_t size;
};