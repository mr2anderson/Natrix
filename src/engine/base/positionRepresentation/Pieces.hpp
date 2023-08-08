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


#include <cctype>
#include "Bitboard.hpp"


#pragma once


enum PIECE {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5
};


enum SIDE {
    WHITE = 0,
    BLACK = 1
};


class Pieces {
public:
    Pieces();
    Pieces(const std::string& shortFen);

    friend std::ostream& operator <<(std::ostream& ostream, Pieces pieces);

    void updateBitboards();

    void setPieceBitboard(uint8_t side, uint8_t piece, Bitboard bb);

    [[nodiscard]] std::array<std::array<Bitboard, 6>, 2> getPieceBitboards() const;
    [[nodiscard]] Bitboard getPieceBitboard(uint8_t side, uint8_t piece) const;
    [[nodiscard]] Bitboard getSideBitboard(uint8_t side) const;
    [[nodiscard]] Bitboard getInvSideBitboard(uint8_t side) const;
    [[nodiscard]] Bitboard getAllBitboard() const;
    [[nodiscard]] Bitboard getEmptyBitboard() const;

    static uint8_t inverse(uint8_t side);
private:
    std::array<std::array<Bitboard, 6>, 2> pieceBitboards{};
    std::array<Bitboard, 2> sideBitboards{};
    std::array<Bitboard, 2> invSideBitboards{};
    Bitboard all;
    Bitboard empty;
};