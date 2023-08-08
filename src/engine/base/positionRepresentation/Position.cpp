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


#include "Position.hpp"


Position::Position() = default;
Position::Position(const std::string& shortFen, uint8_t enPassant, bool wlCastling, bool wsCastling, bool blCastling, bool bsCastling, float moveCtr) {
    this->pieces = {shortFen};
    this->enPassant = enPassant;

    this->wlCastling = wlCastling;
    this->wsCastling = wsCastling;
    this->blCastling = blCastling;
    this->bsCastling = bsCastling;

    this->moveCtr = moveCtr;
    this->hash = {this->pieces, this->blackToMove(), this->wlCastling, this->wsCastling, this->blCastling, this->bsCastling};
    this->repetitionHistory.addPosition(this->hash);
    this->fiftyMovesCtr = 0;
}
std::ostream &operator<<(std::ostream &ostream, const Position& position) {
    ostream << position.pieces << "\n";

    ostream << "En passant: " << (uint32_t)position.enPassant << "\n";
    ostream << "White long castling: " << position.wlCastling << "\n";
    ostream << "White short castling: " << position.wsCastling << "\n";
    ostream << "Black long castling: " << position.blCastling << "\n";
    ostream << "Black short castling: " << position.blCastling << "\n";
    ostream << "Move counter: " << position.moveCtr << "\n";
    ostream << "Zobrist value: " << std::hex << "0x" << position.hash.getValue() << "\n" << std::dec;
    ostream << "Fifty moves counter: " << position.fiftyMovesCtr << "\n";
    ostream << "Threefold repetition counter: " << (uint32_t)position.repetitionHistory.getRepetitionNumber(position.hash);

    return ostream;
}
void Position::move(Move move) {
    this->removePiece(move.getFrom(), move.getAttackerType(), move.getAttackerSide());
    this->addPiece(move.getTo(), move.getAttackerType(), move.getAttackerSide());
    if (move.getDefenderType() != Move::NONE) {
        this->removePiece(move.getTo(), move.getDefenderType(), move.getDefenderSide());
    }

    switch (move.getFlag()) {
        case Move::FLAG::DEFAULT:
            break;

        case Move::FLAG::PAWN_LONG_MOVE:
            this->changeEnPassant((move.getFrom() + move.getTo()) / 2);
            break;
        case Move::FLAG::EN_PASSANT_CAPTURE:
            if (move.getAttackerSide() == SIDE::WHITE) {
                this->removePiece(move.getTo() - 8, PIECE::PAWN, SIDE::BLACK);
            }
            else {
                this->removePiece(move.getTo() + 8, PIECE::PAWN, SIDE::WHITE);
            }
            break;

        case Move::FLAG::WL_CASTLING:
            this->removePiece(0, PIECE::ROOK, SIDE::WHITE);
            this->addPiece(3, PIECE::ROOK, SIDE::WHITE);
            break;
        case Move::FLAG::WS_CASTLING:
            this->removePiece(7, PIECE::ROOK, SIDE::WHITE);
            this->addPiece(5, PIECE::ROOK, SIDE::WHITE);
            break;
        case Move::FLAG::BL_CASTLING:
            this->removePiece(56, PIECE::ROOK, SIDE::BLACK);
            this->addPiece(59, PIECE::ROOK, SIDE::BLACK);
            break;
        case Move::FLAG::BS_CASTLING:
            this->removePiece(63, PIECE::ROOK, SIDE::BLACK);
            this->addPiece(61, PIECE::ROOK, SIDE::BLACK);
            break;

        case Move::FLAG::PROMOTE_TO_KNIGHT:
            this->removePiece(move.getTo(), PIECE::PAWN, move.getAttackerSide());
            this->addPiece(move.getTo(), PIECE::KNIGHT, move.getAttackerSide());
            break;
        case Move::FLAG::PROMOTE_TO_BISHOP:
            this->removePiece(move.getTo(), PIECE::PAWN, move.getAttackerSide());
            this->addPiece(move.getTo(), PIECE::BISHOP, move.getAttackerSide());
            break;
        case Move::FLAG::PROMOTE_TO_ROOK:
            this->removePiece(move.getTo(), PIECE::PAWN, move.getAttackerSide());
            this->addPiece(move.getTo(), PIECE::ROOK, move.getAttackerSide());
            break;
        case Move::FLAG::PROMOTE_TO_QUEEN:
            this->removePiece(move.getTo(), PIECE::PAWN, move.getAttackerSide());
            this->addPiece(move.getTo(), PIECE::QUEEN, move.getAttackerSide());
            break;
    }

    this->pieces.updateBitboards();

    if (move.getFlag() != Move::FLAG::PAWN_LONG_MOVE) {
        this->changeEnPassant(Position::NONE);
    }

    switch (move.getFrom()) {
        case 0:
            this->removeWLCastling();
            break;
        case 4:
            this->removeWLCastling();
            this->removeWSCastling();
            break;
        case 7:
            this->removeWSCastling();
            break;
        case 56:
            this->removeBLCastling();
            break;
        case 60:
            this->removeBLCastling();
            this->removeBSCastling();
            break;
        case 63:
            this->removeBSCastling();
            break;
    }

    this->updateMoveCtr();

    this->updateFiftyMovesCtr(move.getAttackerType() == PIECE::PAWN or move.getDefenderType() != Move::NONE);

    if (move.getAttackerType() == PIECE::PAWN or move.getDefenderType() != Move::NONE) {
        this->repetitionHistory.clear();
    }
    this->repetitionHistory.addPosition(this->hash);
}
Pieces Position::getPieces() const {
    return this->pieces;
}
uint8_t Position::getEnPassant() const {
    return this->enPassant;
}
bool Position::getWLCastling() const {
    return this->wlCastling;
}
bool Position::getWSCastling() const {
    return this->wsCastling;
}
bool Position::getBLCastling() const {
    return this->blCastling;
}
bool Position::getBSCastling() const {
    return this->bsCastling;
}
bool Position::whiteToMove() const {
    return !this->blackToMove();
}
bool Position::blackToMove() const {
    return (this->moveCtr - std::floor(this->moveCtr) > 1e-4);
}
ZobristHash Position::getHash() const {
    return this->hash;
}
bool Position::fiftyMovesRuleDraw() const {
    return (this->fiftyMovesCtr == 50);
}
bool Position::threefoldRepetitionDraw() const {
    return (this->repetitionHistory.getRepetitionNumber(this->hash) == 3);
}
void Position::addPiece(uint8_t square, uint8_t type, uint8_t side) {
    if (!BOp::getBit(this->pieces.getPieceBitboard(side, type), square)) {
        this->pieces.setPieceBitboard(side, type, BOp::set1(this->pieces.getPieceBitboard(side, type), square));
        this->hash.invertPiece(square, type, side);
    }
}
void Position::removePiece(uint8_t square, uint8_t type, uint8_t side) {
    if (BOp::getBit(this->pieces.getPieceBitboard(side, type), square)) {
        this->pieces.setPieceBitboard(side, type, BOp::set0(this->pieces.getPieceBitboard(side, type), square));
        this->hash.invertPiece(square, type, side);
    }
}
void Position::changeEnPassant(uint8_t en_passant) {
    this->enPassant = en_passant;
}
void Position::removeWLCastling() {
    if (this->wlCastling) {
        this->wlCastling = false;
        this->hash.invertWLCastling();
    }
}
void Position::removeWSCastling() {
    if (this->wsCastling) {
        this->wsCastling = false;
        this->hash.invertWSCastling();
    }
}
void Position::removeBLCastling() {
    if (this->blCastling) {
        this->blCastling = false;
        this->hash.invertBLCastling();
    }
}
void Position::removeBSCastling() {
    if (this->bsCastling) {
        this->bsCastling = false;
        this->hash.invertBSCastling();
    }
}
void Position::updateMoveCtr() {
    this->moveCtr = this->moveCtr + 0.5f;
    this->hash.invertMove();
}
void Position::updateFiftyMovesCtr(bool breakEvent) {
    if (breakEvent) {
        this->fiftyMovesCtr = 0;
    }
    else {
        this->fiftyMovesCtr = this->fiftyMovesCtr + 0.5f;
    }
}