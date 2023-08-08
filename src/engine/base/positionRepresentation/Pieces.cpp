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


#include "Pieces.hpp"


Pieces::Pieces() = default;
Pieces::Pieces(const std::string& shortFen) {
    uint8_t x = 0;
    uint8_t y = 7;

    uint8_t side;

    for (auto buff : shortFen) {
        if (buff == '/') {
            x = 0;
            y = y - 1;
        }
        else if (std::isdigit(buff)) {
            x = x + buff - '0';
        }
        else {
            if (std::isupper(buff)) {
                buff = (char)std::tolower(buff);
                side = SIDE::WHITE;
            }
            else {
                side = SIDE::BLACK;
            }

            switch (buff) {
                case 'p':
                    this->pieceBitboards[side][PIECE::PAWN] = BOp::set1(this->pieceBitboards[side][PIECE::PAWN], y * 8 + x);
                    break;
                case 'n':
                    this->pieceBitboards[side][PIECE::KNIGHT] = BOp::set1(this->pieceBitboards[side][PIECE::KNIGHT], y * 8 + x);
                    break;
                case 'b':
                    this->pieceBitboards[side][PIECE::BISHOP] = BOp::set1(this->pieceBitboards[side][PIECE::BISHOP], y * 8 + x);
                    break;
                case 'r':
                    this->pieceBitboards[side][PIECE::ROOK] = BOp::set1(this->pieceBitboards[side][PIECE::ROOK], y * 8 + x);
                    break;
                case 'q':
                    this->pieceBitboards[side][PIECE::QUEEN] = BOp::set1(this->pieceBitboards[side][PIECE::QUEEN], y * 8 + x);
                    break;
                case 'k':
                    this->pieceBitboards[side][PIECE::KING] = BOp::set1(this->pieceBitboards[side][PIECE::KING], y * 8 + x);
                    break;
            }

            x = x + 1;
        }
    }

    this->updateBitboards();
}
std::ostream &operator<<(std::ostream &ostream, Pieces pieces) {
    for (int8_t y = 7; y >= 0; y = y - 1) {
        for (uint8_t x = 0; x < 8; x = x + 1) {
            ostream << "|  ";

            uint8_t index = y * 8 + x;

            if (BOp::getBit(pieces.pieceBitboards[SIDE::WHITE][PIECE::PAWN], index)) {
                ostream << "♙";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::WHITE][PIECE::KNIGHT], index)) {
                ostream << "♘";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::WHITE][PIECE::BISHOP], index)) {
                ostream << "♗";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::WHITE][PIECE::ROOK], index)) {
                ostream << "♖";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::WHITE][PIECE::QUEEN], index)) {
                ostream << "♕";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::WHITE][PIECE::KING], index)) {
                ostream << "♔";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::BLACK][PIECE::PAWN], index)) {
                ostream << "♟";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::BLACK][PIECE::KNIGHT], index)) {
                ostream << "♞";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::BLACK][PIECE::BISHOP], index)) {
                ostream << "♝";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::BLACK][PIECE::ROOK], index)) {
                ostream << "♜";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::BLACK][PIECE::QUEEN], index)) {
                ostream << "♛";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::BLACK][PIECE::KING], index)) {
                ostream << "♚";
            }
            else {
                ostream << " ";
            }

            ostream << "  ";
        }
        ostream << "|";
        if (y != 0) {
            ostream << "\n";
        }
    }

    return ostream;
}
void Pieces::updateBitboards() {
    this->sideBitboards[SIDE::WHITE] = this->pieceBitboards[SIDE::WHITE][PIECE::PAWN] |
                                         this->pieceBitboards[SIDE::WHITE][PIECE::KNIGHT] |
                                         this->pieceBitboards[SIDE::WHITE][PIECE::BISHOP] |
                                         this->pieceBitboards[SIDE::WHITE][PIECE::ROOK] |
                                         this->pieceBitboards[SIDE::WHITE][PIECE::QUEEN] |
                                         this->pieceBitboards[SIDE::WHITE][PIECE::KING];

    this->sideBitboards[SIDE::BLACK] = this->pieceBitboards[SIDE::BLACK][PIECE::PAWN] |
                                         this->pieceBitboards[SIDE::BLACK][PIECE::KNIGHT] |
                                         this->pieceBitboards[SIDE::BLACK][PIECE::BISHOP] |
                                         this->pieceBitboards[SIDE::BLACK][PIECE::ROOK] |
                                         this->pieceBitboards[SIDE::BLACK][PIECE::QUEEN] |
                                         this->pieceBitboards[SIDE::BLACK][PIECE::KING];

    this->invSideBitboards[SIDE::WHITE] = ~this->sideBitboards[SIDE::WHITE];
    this->invSideBitboards[SIDE::BLACK] = ~this->sideBitboards[SIDE::BLACK];

    this->all = this->sideBitboards[SIDE::WHITE] | this->sideBitboards[SIDE::BLACK];
    this->empty = ~this->all;
}
void Pieces::setPieceBitboard(uint8_t side, uint8_t piece, Bitboard bb) {
    this->pieceBitboards[side][piece] = bb;
}
std::array<std::array<Bitboard, 6>, 2> Pieces::getPieceBitboards() const {
    return this->pieceBitboards;
}
Bitboard Pieces::getPieceBitboard(uint8_t side, uint8_t piece) const {
    return this->pieceBitboards[side][piece];
}
Bitboard Pieces::getSideBitboard(uint8_t side) const {
    return this->sideBitboards[side];
}
Bitboard Pieces::getInvSideBitboard(uint8_t side) const {
    return this->invSideBitboards[side];
}
Bitboard Pieces::getAllBitboard() const {
    return this->all;
}
Bitboard Pieces::getEmptyBitboard() const {
    return this->empty;
}
uint8_t Pieces::inverse(uint8_t side) {
    return !side;
}