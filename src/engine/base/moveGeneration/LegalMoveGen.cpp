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


#include "LegalMoveGen.hpp"


MoveList LegalMoveGen::generate(const Position &position, uint8_t side, bool onlyCaptures) {
    MoveList moves;

    Bitboard pawnsLeftCaptures = PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(position.getPieces(), side, false);
    Bitboard pawnsRightCaptures = PsLegalMoveMaskGen::generatePawnsRightCapturesMask(position.getPieces(), side, false);

    int8_t pawnsLeftCaptureIndex;
    int8_t pawnsRightCaptureIndex;

    if (side == SIDE::WHITE) {
        pawnsLeftCaptureIndex = -7;
        pawnsRightCaptureIndex = -9;
    }
    else {
        pawnsLeftCaptureIndex = 9;
        pawnsRightCaptureIndex = 7;
    }
    pawnsMaskToMoves(position.getPieces(), pawnsLeftCaptures, side, pawnsLeftCaptureIndex, true,Move::FLAG::DEFAULT, moves);
    pawnsMaskToMoves(position.getPieces(), pawnsRightCaptures, side, pawnsRightCaptureIndex, true,Move::FLAG::DEFAULT, moves);

    if (!onlyCaptures) {
        Bitboard pawnsDefault = PsLegalMoveMaskGen::generatePawnsDefaultMask(position.getPieces(), side);
        Bitboard pawnsLong = PsLegalMoveMaskGen::generatePawnsLongMask(position.getPieces(), side);

        int8_t pawnDefaultIndex;
        int8_t pawnLongIndex;
        if (side == SIDE::WHITE) {
            pawnDefaultIndex = -8;
            pawnLongIndex = -16;
        }
        else {
            pawnDefaultIndex = 8;
            pawnLongIndex = 16;
        }
        pawnsMaskToMoves(position.getPieces(), pawnsDefault, side, pawnDefaultIndex, false,Move::FLAG::DEFAULT, moves);
        pawnsMaskToMoves(position.getPieces(), pawnsLong, side, pawnLongIndex, false,Move::FLAG::PAWN_LONG_MOVE, moves);
    }

    Bitboard allKnights = position.getPieces().getPieceBitboard(side, PIECE::KNIGHT);
    Bitboard allBishops = position.getPieces().getPieceBitboard(side, PIECE::BISHOP);
    Bitboard allRooks = position.getPieces().getPieceBitboard(side, PIECE::ROOK);
    Bitboard allQueens = position.getPieces().getPieceBitboard(side, PIECE::QUEEN);
    uint8_t attackerP;
    Bitboard mask;
    while (allKnights) {
        attackerP = BOp::bsf(allKnights);
        allKnights = BOp::set0(allKnights, attackerP);
        mask = PsLegalMoveMaskGen::generateKnightMask(position.getPieces(), attackerP, side, onlyCaptures);
        pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::KNIGHT, side, moves);
    }
    while (allBishops) {
        attackerP = BOp::bsf(allBishops);
        allBishops = BOp::set0(allBishops, attackerP);
        mask = PsLegalMoveMaskGen::generateBishopMask(position.getPieces(), attackerP, side, onlyCaptures);
        pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::BISHOP, side, moves);
    }
    while (allRooks) {
        attackerP = BOp::bsf(allRooks);
        allRooks = BOp::set0(allRooks, attackerP);
        mask = PsLegalMoveMaskGen::generateRookMask(position.getPieces(), attackerP, side, onlyCaptures);
        pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::ROOK, side, moves);
    }
    while (allQueens) {
        attackerP = BOp::bsf(allQueens);
        allQueens = BOp::set0(allQueens, attackerP);
        mask = PsLegalMoveMaskGen::generateQueenMask(position.getPieces(), attackerP, side, onlyCaptures);
        pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::QUEEN, side, moves);
    }
    attackerP = BOp::bsf(position.getPieces().getPieceBitboard(side, PIECE::KING));
    mask = PsLegalMoveMaskGen::generateKingMask(position.getPieces(), attackerP, side, onlyCaptures);
    pieceMaskToMoves(position.getPieces(), mask, attackerP, PIECE::KING, side, moves);

    addEnPassantCaptures(position.getPieces(), side, position.getEnPassant(), moves);
    if (!onlyCaptures) {
        if (side == SIDE::WHITE) {
            addCastlingMoves(position.getPieces(), SIDE::WHITE, position.getWLCastling(), position.getWSCastling(),moves);
        }
        else {
            addCastlingMoves(position.getPieces(), SIDE::BLACK, position.getBLCastling(), position.getBSCastling(),moves);
        }
    }

    return moves;
}
void LegalMoveGen::pieceMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerP, uint8_t attackerType, uint8_t attackerSide, MoveList &moves) {
    while (mask) {
        uint8_t defenderP = BOp::bsf(mask);
        mask = BOp::set0(mask, defenderP);

        uint8_t defenderType = Move::NONE;
        for (uint8_t i = 0; i < 6; i = i + 1) {
            if (BOp::getBit(pieces.getPieceBitboard(Pieces::inverse(attackerSide), i), defenderP)) {
                defenderType = i;
                break;
            }
        }

        Move move = {attackerP, defenderP, attackerType, attackerSide, defenderType, Pieces::inverse(attackerSide)};

        if (isLegal(pieces, move)) {
            moves.push(move);
        }
    }
}
void LegalMoveGen::pawnsMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerSide, int8_t attackerIndex, bool checkDefender, uint8_t flag, MoveList &moves) {
    uint8_t defenderType = Move::NONE;

    while (mask) {
        uint8_t defenderP = BOp::bsf(mask);
        mask = BOp::set0(mask, defenderP);

        if (checkDefender) {
            defenderType = Move::NONE;
            for (uint8_t i = 0; i < 6; i = i + 1) {
                if (BOp::getBit(pieces.getPieceBitboard(Pieces::inverse(attackerSide), i), defenderP)) {
                    defenderType = i;
                    break;
                }
            }
        }

        Move move = {(uint8_t)(defenderP + attackerIndex), defenderP, PIECE::PAWN, attackerSide, defenderType, Pieces::inverse(attackerSide), flag};

        if (isLegal(pieces, move)) {
            if (defenderP < 8 or defenderP > 55) {
                moves.push({(uint8_t)(defenderP + attackerIndex), defenderP, 0, attackerSide, defenderType,Pieces::inverse(attackerSide), Move::FLAG::PROMOTE_TO_KNIGHT});
                moves.push({(uint8_t)(defenderP + attackerIndex), defenderP, 0, attackerSide, defenderType,Pieces::inverse(attackerSide), Move::FLAG::PROMOTE_TO_BISHOP});
                moves.push({(uint8_t)(defenderP + attackerIndex), defenderP, 0, attackerSide, defenderType,Pieces::inverse(attackerSide), Move::FLAG::PROMOTE_TO_ROOK});
                moves.push({(uint8_t)(defenderP + attackerIndex), defenderP, 0, attackerSide, defenderType,Pieces::inverse(attackerSide), Move::FLAG::PROMOTE_TO_QUEEN});
            }
            else {
                moves.push(move);
            }
        }
    }
}
bool LegalMoveGen::isLegal(Pieces pieces, Move move) {
    pieces.setPieceBitboard(move.getAttackerSide(), move.getAttackerType(), BOp::set0(pieces.getPieceBitboard(move.getAttackerSide(), move.getAttackerType()), move.getFrom()));
    pieces.setPieceBitboard(move.getAttackerSide(), move.getAttackerType(), BOp::set1(pieces.getPieceBitboard(move.getAttackerSide(), move.getAttackerType()), move.getTo()));
    if (move.getDefenderType() != Move::NONE) {
        pieces.setPieceBitboard(move.getDefenderSide(), move.getDefenderType(), BOp::set0(pieces.getPieceBitboard(move.getDefenderSide(), move.getDefenderType()), move.getTo()));
    }
    if (move.getFlag() == Move::FLAG::EN_PASSANT_CAPTURE) {
        if (move.getAttackerSide() == SIDE::WHITE) {
            pieces.setPieceBitboard(SIDE::BLACK, PIECE::PAWN, BOp::set0(pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN), move.getTo() - 8));
        }
        else {
            pieces.setPieceBitboard(SIDE::WHITE, PIECE::PAWN, BOp::set0(pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN), move.getTo() + 8));
        }
    }

    pieces.updateBitboards();

    return !PsLegalMoveMaskGen::inDanger(pieces, BOp::bsf(pieces.getPieceBitboard(move.getAttackerSide(), PIECE::KING)), move.getAttackerSide());
}
void LegalMoveGen::addEnPassantCaptures(Pieces pieces, uint8_t side, uint8_t enPassant, MoveList &moves) {
    if (enPassant == Position::NONE) {
        return;
    }

    if (side == SIDE::WHITE) {
        if (enPassant % 8 != 7 and BOp::getBit(pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN), enPassant - 7)) {
            auto move = Move((uint8_t)(enPassant - 7), enPassant, PIECE::PAWN, SIDE::WHITE, Move::NONE, Move::NONE, Move::FLAG::EN_PASSANT_CAPTURE);
            if (isLegal(pieces, move)) {
                moves.push(move);
            }
        }
        if (enPassant % 8 != 0 and BOp::getBit(pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN), enPassant - 9)) {
            auto move = Move((uint8_t)(enPassant - 9), enPassant, PIECE::PAWN, SIDE::WHITE, Move::NONE, Move::NONE, Move::FLAG::EN_PASSANT_CAPTURE);
            if (isLegal(pieces, move)) {
                moves.push(move);
            }
        }
    }
    else {
        if (enPassant % 8 != 0 and BOp::getBit(pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN), enPassant + 7)) {
            auto move = Move((uint8_t)(enPassant + 7), enPassant, PIECE::PAWN, SIDE::BLACK, Move::NONE, Move::NONE, Move::FLAG::EN_PASSANT_CAPTURE);
            if (isLegal(pieces, move)) {
                moves.push(move);
            }
        }
        if (enPassant % 8 != 7 and BOp::getBit(pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN), enPassant + 9)) {
            auto move = Move((uint8_t)(enPassant + 9), enPassant, PIECE::PAWN, SIDE::BLACK, Move::NONE, Move::NONE, Move::FLAG::EN_PASSANT_CAPTURE);
            if (isLegal(pieces, move)) {
                moves.push(move);
            }
        }
    }
}
void LegalMoveGen::addCastlingMoves(Pieces pieces, uint8_t side, bool lCastling, bool sCastling, MoveList &moves) {
    uint8_t index;
    uint8_t longCastlingFlag;
    uint8_t shortCastlingFlag;
    if (side == SIDE::WHITE) {
        index = 0;
        longCastlingFlag = Move::FLAG::WL_CASTLING;
        shortCastlingFlag = Move::FLAG::WS_CASTLING;
    }
    else {
        index = 56;
        longCastlingFlag = Move::FLAG::BL_CASTLING;
        shortCastlingFlag = Move::FLAG::BS_CASTLING;
    }

    if (lCastling and
        BOp::getBit(pieces.getPieceBitboard(side, PIECE::ROOK), 0 + index) and
        BOp::getBit(pieces.getEmptyBitboard(), 1 + index) and
        BOp::getBit(pieces.getEmptyBitboard(), 2 + index) and
        BOp::getBit(pieces.getEmptyBitboard(), 3 + index) and
        !PsLegalMoveMaskGen::inDanger(pieces, BOp::bsf(pieces.getPieceBitboard(side, PIECE::KING)), side) and
        !PsLegalMoveMaskGen::inDanger(pieces, 2 + index, side) and
        !PsLegalMoveMaskGen::inDanger(pieces, 3 + index, side)) {

        moves.push({(uint8_t)(4 + index), (uint8_t)(2 + index), PIECE::KING, side, Move::NONE, Move::NONE, longCastlingFlag});
    }
    if (sCastling and
        BOp::getBit(pieces.getPieceBitboard(side, PIECE::ROOK), 7 + index) and
        BOp::getBit(pieces.getEmptyBitboard(), 5 + index) and
        BOp::getBit(pieces.getEmptyBitboard(), 6 + index) and
        !PsLegalMoveMaskGen::inDanger(pieces, BOp::bsf(pieces.getPieceBitboard(side, PIECE::KING)), side) and
        !PsLegalMoveMaskGen::inDanger(pieces, 5 + index, side) and
        !PsLegalMoveMaskGen::inDanger(pieces, 6 + index, side)) {

        moves.push({(uint8_t)(4 + index), (uint8_t)(6 + index), PIECE::KING, side, Move::NONE, Move::NONE, shortCastlingFlag});
    }
}
