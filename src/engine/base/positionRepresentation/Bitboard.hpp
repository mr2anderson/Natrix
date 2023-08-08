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


#include <iostream>
#include <cstdint>
#include <bit>
#include <array>


#pragma once


typedef uint64_t Bitboard;


namespace BOp {
    static constexpr std::array<uint8_t, 64> BIT_SCAN_TABLE = {
            0, 47,  1, 56, 48, 27,  2, 60,
            57, 49, 41, 37, 28, 16,  3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11,  4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30,  9, 24,
            13, 18,  8, 12,  7,  6,  5, 63
    };
    static constexpr Bitboard set1(Bitboard bb, uint8_t square) {
        bb = bb | (1ull << square);
        return bb;
    }
    static constexpr Bitboard set0(Bitboard bb, uint8_t square) {
        bb = bb & (~(1ull << square));
        return bb;
    }
    static constexpr bool getBit(Bitboard bb, uint8_t square) {
        return (bb & (1ull << square));
    }
    static void print(Bitboard bb) {
        for (int8_t y = 7; y >= 0; y = y - 1) {
            for (uint8_t x = 0; x < 8; x = x + 1) {
                std::cout << "|  ";

                if (BOp::getBit(bb, y * 8 + x)) std::cout << "1";
                else std::cout << "0";

                std::cout << "  ";
            }
            std::cout << "|\n";
        }
    }
    static constexpr uint8_t count1(Bitboard bb) {
        return std::popcount(bb);
    }
    static constexpr uint8_t bsf(Bitboard bb) {
        return BIT_SCAN_TABLE[((bb ^ (bb - 1)) * 0x03f79d71b4cb0a89) >> 58];
    }
    static constexpr uint8_t bsr(Bitboard bb) {
        bb = bb | (bb >> 1);
        bb = bb | (bb >> 2);
        bb = bb | (bb >> 4);
        bb = bb | (bb >> 8);
        bb = bb | (bb >> 16);
        bb = bb | (bb >> 32);
        return BIT_SCAN_TABLE[(bb * 0x03f79d71b4cb0a89) >> 58];
    }
}


namespace BRows {
    static consteval std::array<Bitboard, 8> calcRows() {
        std::array<Bitboard, 8> rows{};
        for (uint8_t y = 0; y < 8; y = y + 1) {
            for (uint8_t x = 0; x < 8; x = x + 1) {
                rows[y] = BOp::set1(rows[y], y * 8 + x);
            }
        }
        return rows;
    }
    static constexpr std::array<Bitboard, 8> ROWS = calcRows();
    static consteval std::array<Bitboard, 8> calcInvRows() {
        std::array<Bitboard, 8> invRows{};
        for (uint8_t i = 0; i < 8; i = i + 1) {
            invRows[i] = ~ROWS[i];
        }
        return invRows;
    }
    static constexpr std::array<Bitboard, 8> INV_ROWS = BRows::calcInvRows();
}


namespace BColumns {
    static consteval std::array<Bitboard, 8> calcColumns() {
        std::array<Bitboard, 8> columns{};
        for (uint8_t x = 0; x < 8; x = x + 1) {
            for (uint8_t y = 0; y < 8; y = y + 1) {
                columns[x] = BOp::set1(columns[x], y * 8 + x);
            }
        }
        return columns;
    }
    static constexpr std::array<Bitboard, 8> COLUMNS = BColumns::calcColumns();
    static consteval std::array<Bitboard, 8> calcInvColumns() {
        std::array<Bitboard, 8> invColumns{};
        for (uint8_t i = 0; i < 8; i = i + 1) {
            invColumns[i] = ~COLUMNS[i];
        }
        return invColumns;
    }
    static constexpr std::array<Bitboard, 8> INV_COLUMNS = BColumns::calcInvColumns();
}