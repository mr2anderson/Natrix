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


#include "PsLegalMoveMaskGen.hpp"


Bitboard PsLegalMoveMaskGen::generatePawnsDefaultMask(Pieces pieces, uint8_t side) {
    if (side == SIDE::WHITE) {
        return (pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN) << 8) & pieces.getEmptyBitboard();
    }
    return (pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN) >> 8) & pieces.getEmptyBitboard();
}
Bitboard PsLegalMoveMaskGen::generatePawnsLongMask(Pieces pieces, uint8_t side) {
    Bitboard defaultMask = generatePawnsDefaultMask(pieces, side);
    if (side == SIDE::WHITE) {
        return ((defaultMask & BRows::ROWS[2]) << 8) & pieces.getEmptyBitboard();
    }
    return ((defaultMask & BRows::ROWS[5]) >> 8) & pieces.getEmptyBitboard();
}
Bitboard PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(Pieces pieces, uint8_t side, bool includeAllAttacks) {
    if (side == SIDE::WHITE) {
        Bitboard mask = (pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN) << 7) & BColumns::INV_COLUMNS[7];
        if (!includeAllAttacks) {
            mask = mask & pieces.getSideBitboard(SIDE::BLACK);
        }
        return mask;
    }

    Bitboard mask = (pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN) >> 9) & BColumns::INV_COLUMNS[7];
    if (!includeAllAttacks) {
        mask = mask & pieces.getSideBitboard(SIDE::WHITE);
    }
    return mask;
}
Bitboard PsLegalMoveMaskGen::generatePawnsRightCapturesMask(Pieces pieces, uint8_t side, bool includeAllAttacks) {
    if (side == SIDE::WHITE) {
        Bitboard mask = (pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN) << 9) & BColumns::INV_COLUMNS[0];
        if (!includeAllAttacks) {
            mask = mask & pieces.getSideBitboard(SIDE::BLACK);
        }
        return mask;
    }

    Bitboard mask = (pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN) >> 7) & BColumns::INV_COLUMNS[0];
    if (!includeAllAttacks) {
        mask = mask & pieces.getSideBitboard(SIDE::WHITE);
    }
    return mask;
}
Bitboard PsLegalMoveMaskGen::generateKnightMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures) {
    if (onlyCaptures) {
        return KnightMasks::MASKS[p] & pieces.getSideBitboard(Pieces::inverse(side));
    }
    return KnightMasks::MASKS[p] & pieces.getInvSideBitboard(side);
}
Bitboard PsLegalMoveMaskGen::generateBishopMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures) {
    Bitboard nw = calcRay(pieces, p, side, onlyCaptures, SlidersMasks::DIRECTION::NORTH_WEST, false);
    Bitboard ne = calcRay(pieces, p, side, onlyCaptures, SlidersMasks::DIRECTION::NORTH_EAST, false);
    Bitboard sw = calcRay(pieces, p, side, onlyCaptures, SlidersMasks::DIRECTION::SOUTH_WEST, true);
    Bitboard se = calcRay(pieces, p, side, onlyCaptures, SlidersMasks::DIRECTION::SOUTH_EAST, true);

    return nw | ne | sw | se;
}
Bitboard PsLegalMoveMaskGen::generateRookMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures) {
    Bitboard n = calcRay(pieces, p, side, onlyCaptures, SlidersMasks::DIRECTION::NORTH, false);
    Bitboard s = calcRay(pieces, p, side, onlyCaptures, SlidersMasks::DIRECTION::SOUTH, true);
    Bitboard w = calcRay(pieces, p, side, onlyCaptures, SlidersMasks::DIRECTION::WEST, true);
    Bitboard e = calcRay(pieces, p, side, onlyCaptures, SlidersMasks::DIRECTION::EAST, false);

    return n | s | w | e;
}
Bitboard PsLegalMoveMaskGen::generateQueenMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures) {
    Bitboard bishopMask = generateBishopMask(pieces, p, side, onlyCaptures);
    Bitboard rookMask = generateRookMask(pieces, p, side, onlyCaptures);

    return bishopMask | rookMask;
}
Bitboard PsLegalMoveMaskGen::generateKingMask(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures) {
    if (onlyCaptures) {
        return KingMasks::MASKS[p] & pieces.getSideBitboard(Pieces::inverse(side));
    }
    return KingMasks::MASKS[p] & pieces.getInvSideBitboard(side);
}
bool PsLegalMoveMaskGen::inDanger(Pieces pieces, uint8_t p, uint8_t side) {
    Bitboard oppositePawnsLeftCaptures = generatePawnsLeftCapturesMask(pieces, Pieces::inverse(side), true);
    Bitboard oppositePawnsRightCaptures = generatePawnsRightCapturesMask(pieces, Pieces::inverse(side), true);
    Bitboard oppositePawnsCaptures = oppositePawnsLeftCaptures | oppositePawnsRightCaptures;

    if (BOp::getBit(oppositePawnsCaptures, p)) {
        return true;
    }

    if (generateKnightMask(pieces, p, side, true) & pieces.getPieceBitboard(Pieces::inverse(side), PIECE::KNIGHT)) {
        return true;
    }
    if (generateBishopMask(pieces, p, side, true) & pieces.getPieceBitboard(Pieces::inverse(side), PIECE::BISHOP)) {
        return true;
    }
    if (generateRookMask(pieces, p, side, true) & pieces.getPieceBitboard(Pieces::inverse(side), PIECE::ROOK)) {
        return true;
    }
    if (generateQueenMask(pieces, p, side, true) & pieces.getPieceBitboard(Pieces::inverse(side), PIECE::QUEEN)) {
        return true;
    }
    if (generateKingMask(pieces, p, side, true) & pieces.getPieceBitboard(Pieces::inverse(side), PIECE::KING)) {
        return true;
    }

    return false;
}
Bitboard PsLegalMoveMaskGen::calcRay(Pieces pieces, uint8_t p, uint8_t side, bool onlyCaptures, uint8_t direction, bool bsr) {
    Bitboard blockers = SlidersMasks::MASKS[p][direction] & pieces.getAllBitboard();
    if (blockers == 0) {
        if (onlyCaptures) {
            return 0;
        }
        return SlidersMasks::MASKS[p][direction];
    }

    uint8_t blockingSquare;
    if (bsr) {
        blockingSquare = BOp::bsr(blockers);
    }
    else {
        blockingSquare = BOp::bsf(blockers);
    }

    Bitboard moves;
    if (onlyCaptures) {
        moves = 0;
    }
    else {
        moves = SlidersMasks::MASKS[p][direction] ^ SlidersMasks::MASKS[blockingSquare][direction];
    }

    if (BOp::getBit(pieces.getSideBitboard(side), blockingSquare)) {
        moves = BOp::set0(moves, blockingSquare);
    }
    else {
        moves = BOp::set1(moves, blockingSquare);
    }

    return moves;
}
