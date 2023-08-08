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


#include "ZobristHash.hpp"


ZobristHash::ZobristHash() = default;
ZobristHash::ZobristHash(Pieces pieces, bool blackToMove, bool wlCastling, bool wsCastling, bool blCastling, bool bsCastling) {
    this->value = 0;

    if (blackToMove) {
        this->invertMove();
    }
    if (wlCastling) {
        this->invertWLCastling();
    }
    if (wsCastling) {
        this->invertWSCastling();
    }
    if (blCastling) {
        this->invertBLCastling();
    }
    if (bsCastling) {
        this->invertBSCastling();
    }

    uint8_t side;
    for (uint8_t square = 0; square < 64; square = square + 1) {
        if (BOp::getBit(pieces.getSideBitboard(SIDE::WHITE), square)) {
            side = SIDE::WHITE;
        }
        else if (BOp::getBit(pieces.getSideBitboard(SIDE::BLACK), square)) {
            side = SIDE::BLACK;
        }
        else {
            continue;
        }
        for (uint8_t type = 0; type < 6; type = type + 1) {
            if (BOp::getBit(pieces.getPieceBitboard(side, type), square)) {
                this->invertPiece(square, type, side);
                break;
            }
        }
    }
}
bool operator ==(ZobristHash left, ZobristHash right) {
    return (left.value == right.value);
}
void ZobristHash::invertPiece(uint8_t square, uint8_t type, uint8_t side) {
    this->value = this->value ^ ZobristHashConstants::CONSTANTS[square][side][type];
}
void ZobristHash::invertMove() {
    this->value = this->value ^ ZobristHashConstants::BLACK_MOVE;
}
void ZobristHash::invertWLCastling() {
    this->value = this->value ^ ZobristHashConstants::WL_CASTLING;
}
void ZobristHash::invertWSCastling() {
    this->value = this->value ^ ZobristHashConstants::WS_CASTLING;
}
void ZobristHash::invertBLCastling() {
    this->value = this->value ^ ZobristHashConstants::BL_CASTLING;
}
void ZobristHash::invertBSCastling() {
    this->value = this->value ^ ZobristHashConstants::BS_CASTLING;
}
uint64_t ZobristHash::getValue() const {
    return this->value;
}