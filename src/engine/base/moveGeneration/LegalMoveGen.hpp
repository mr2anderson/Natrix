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
#include "PsLegalMoveMaskGen.hpp"


#pragma once


class LegalMoveGen {
public:
    static MoveList generate(const Position &position, uint8_t side, bool onlyCaptures = false);
private:
    static void pieceMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerP, uint8_t attackerType, uint8_t attackerSide, MoveList &moves);
    static void pawnsMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerSide, int8_t attackerIndex, bool checkDefender, uint8_t flag, MoveList &moves);

    static bool isLegal(Pieces pieces, Move move);

    static void addEnPassantCaptures(Pieces pieces, uint8_t side, uint8_t enPassant, MoveList &moves);
    static void addCastlingMoves(Pieces pieces, uint8_t side, bool lCastling, bool sCastling, MoveList &moves);
};