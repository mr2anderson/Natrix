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


#include "../positionRepresentation/Position.hpp"
#include "KnightMasks.hpp"
#include "KingMasks.hpp"
#include "SlidersMasks.hpp"


#pragma once


class PsLegalMoveMaskGen {
public:
    static Bitboard generatePawnsDefaultMask(Pieces pieces, uint8_t side);
    static Bitboard generatePawnsLongMask(Pieces pieces, uint8_t side);
    static Bitboard generatePawnsLeftCapturesMask(Pieces pieces, uint8_t side, bool includeAllAttacks=false);
    static Bitboard generatePawnsRightCapturesMask(Pieces pieces, uint8_t side, bool includeAllAttacks=false);

    static Bitboard generateKnightMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures=false);
    static Bitboard generateBishopMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures=false);
    static Bitboard generateRookMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures=false);
    static Bitboard generateQueenMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures=false);
    static Bitboard generateKingMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures=false);

    static bool inDanger(Pieces pieces, uint8_t p, uint8_t side);
private:
    static Bitboard calcRay(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures, uint8_t direction, bool bsr);
};