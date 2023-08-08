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


#include <cstdint>
#include <array>


#pragma once


namespace ZobristHashConstants {
    namespace PRNG {
        static constexpr uint64_t SEED = 0x98f107;
        static constexpr uint64_t MULTIPLIER = 0x71abc9;
        static constexpr uint64_t SUMMAND = 0xff1b3f;
        static consteval uint64_t nextRandomNumber(uint64_t previous) {
            return MULTIPLIER * previous + SUMMAND;
        }
    }
    static consteval std::array<std::array<std::array<uint64_t, 6>, 2>, 64> calcConstants() {
        std::array<std::array<std::array<uint64_t, 6>, 2>, 64> constants{};

        uint64_t previous = PRNG::SEED;

        for (uint8_t square = 0; square < 64; square = square + 1) {
            for (uint8_t side = 0; side < 2; side = side + 1) {
                for (uint8_t type = 0; type < 6; type = type + 1) {
                    previous = PRNG::nextRandomNumber(previous);
                    constants[square][side][type] = previous;
                }
            }
        }

        return constants;
    }
    static constexpr std::array<std::array<std::array<uint64_t, 6>, 2>, 64> CONSTANTS = calcConstants();
    static constexpr uint64_t BLACK_MOVE = PRNG::nextRandomNumber(CONSTANTS[63][1][5]);
    static constexpr uint64_t WL_CASTLING = PRNG::nextRandomNumber(BLACK_MOVE);
    static constexpr uint64_t WS_CASTLING = PRNG::nextRandomNumber(WL_CASTLING);
    static constexpr uint64_t BL_CASTLING = PRNG::nextRandomNumber(WS_CASTLING);
    static constexpr uint64_t BS_CASTLING = PRNG::nextRandomNumber(BL_CASTLING);
}