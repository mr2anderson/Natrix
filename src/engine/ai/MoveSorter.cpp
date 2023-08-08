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


#include "MoveSorter.hpp"


MoveList MoveSorter::sort(Pieces pieces, MoveList moves) {
    for (uint8_t i = 0; i < moves.getSize() - 1; i = i + 1) {
        for (uint8_t j = 0; j < moves.getSize() - i - 1; j = j + 1) {
            if (MoveSorter::evaluateMove(pieces, moves[j]) < MoveSorter::evaluateMove(pieces, moves[j + 1])) {
                std::swap(moves[j], moves[j + 1]);
            }
        }
    }
    return moves;
}
int32_t MoveSorter::evaluateMove(Pieces pieces, Move move) {
    int32_t evaluation = 0;

    if (move.getAttackerType() != PIECE::PAWN) {
        Bitboard opponentPawnsAttacks = PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(pieces, Pieces::inverse(move.getAttackerSide()), true) |
                                        PsLegalMoveMaskGen::generatePawnsRightCapturesMask(pieces, Pieces::inverse(move.getAttackerSide()), true);
        if (BOp::getBit(opponentPawnsAttacks, move.getTo())) {
            switch (move.getAttackerType()) {
                case PIECE::KNIGHT:
                    evaluation = evaluation - StaticEvaluator::MATERIAL::KNIGHT;
                    break;
                case PIECE::BISHOP:
                    evaluation = evaluation - StaticEvaluator::MATERIAL::BISHOP;
                    break;
                case PIECE::ROOK:
                    evaluation = evaluation - StaticEvaluator::MATERIAL::ROOK;
                    break;
                case PIECE::QUEEN:
                    evaluation = evaluation - StaticEvaluator::MATERIAL::QUEEN;
                    break;
            }
        }
    }

    if (move.getDefenderType() != Move::NONE) {
        switch (move.getDefenderType()) {
            case PIECE::PAWN:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::PAWN;
                break;
            case PIECE::KNIGHT:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::KNIGHT;
                break;
            case PIECE::BISHOP:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::BISHOP;
                break;
            case PIECE::ROOK:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::ROOK;
                break;
            case PIECE::QUEEN:
                evaluation = evaluation + 1000 * StaticEvaluator::MATERIAL::QUEEN;
                break;
        }
        switch (move.getAttackerType()) {
            case PIECE::PAWN:
                evaluation = evaluation - StaticEvaluator::MATERIAL::PAWN;
                break;
            case PIECE::KNIGHT:
                evaluation = evaluation - StaticEvaluator::MATERIAL::KNIGHT;
                break;
            case PIECE::BISHOP:
                evaluation = evaluation - StaticEvaluator::MATERIAL::BISHOP;
                break;
            case PIECE::ROOK:
                evaluation = evaluation - StaticEvaluator::MATERIAL::ROOK;
                break;
            case PIECE::QUEEN:
                evaluation = evaluation - StaticEvaluator::MATERIAL::QUEEN;
                break;
        }
    }

    return evaluation;
}
