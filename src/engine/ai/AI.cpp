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


#include "AI.hpp"


Move AI::getBestMove(const Position& position, uint8_t side, int32_t ms) {
    std::cout << position << std::endl;
    StaticEvaluator::evaluate(position.getPieces(), true);

    int64_t start = nsecs;
    SearchInterrupter::getPtr()->resume();

    std::cout << "Search started." << std::endl;

    int32_t eval;
    bool gameWasFinished;
    Move move;

    for (int32_t i = 1; i < 1000; i = i + 1) {
        std::future<std::tuple<int32_t, bool, Move>> thread = std::async(alphaBeta, position, side, i);
        bool continueSearch = true;
        while (thread.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            if ((nsecs - start) / 1000000 >= ms) {
                continueSearch = false;
                break;
            }
            usleep(20000);
        }

        if (continueSearch or i == 1) {
            std::tie(eval, gameWasFinished, move) = thread.get();
        }
        else {
            SearchInterrupter::getPtr()->interrupt();
            std::tuple<int32_t, bool, Move> trash = thread.get();
            break;
        }

        std::cout << "base depth: " << std::setw(4) << i << ". Evaluation: " << std::setw(6) << (float)eval / 100.0f << " pawns.  Time: " << std::setw(10) << (nsecs - start) / (int32_t)1e+6 << " ms." << std::endl;
        if (gameWasFinished) {
            break;
        }
    }

    std::cout << "Search finished." << std::endl << std::endl;

    return move;
}
std::tuple<int32_t, bool, Move> AI::alphaBeta(const Position& position, uint8_t side, int32_t depthLeft) {
    if (side == SIDE::WHITE) {
        return alphaBetaMax(position, INF::NEGATIVE, INF::POSITIVE, depthLeft);
    }
    return alphaBetaMin(position, INF::NEGATIVE, INF::POSITIVE, depthLeft);
}
std::tuple<int32_t, bool, Move> AI::alphaBetaMin(const Position &position, int32_t alpha, int32_t beta, int32_t depthLeft, int32_t depthCurrent) {
    if (SearchInterrupter::getPtr()->interrupted()) {
        return std::make_tuple(0, false, Move());
    }
    if (depthLeft == 0) {
        return std::make_tuple(alphaBetaMinOnlyCaptures(position, alpha, beta), false, Move());
    }
    if (position.fiftyMovesRuleDraw() or position.threefoldRepetitionDraw()) {
        return std::make_tuple(0, true, Move());
    }

    MoveList moves = LegalMoveGen::generate(position, SIDE::BLACK);
    moves = MoveSorter::sort(position.getPieces(), moves);
    Move bestMove;
    uint8_t bestMoveIndex;
    bool gameWasFinishedOnBestMove;

    uint8_t tableResult = TranspositionTable::getPtr()->getBestMoveIndex(position.getHash());
    if (tableResult < moves.getSize()) {
        std::swap(moves[0], moves[tableResult]);
    }

    bool check = PsLegalMoveMaskGen::inDanger(position.getPieces(), BOp::bsf(position.getPieces().getPieceBitboard(SIDE::BLACK, PIECE::KING)), SIDE::BLACK);
    if (moves.getSize() == 0) {
        if (check) {
            return std::make_tuple(INF::POSITIVE - depthLeft, true, Move());
        }
        return std::make_tuple(0, true, Move());
    }

    for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
        Move move = moves[i];

        Position copy = position;
        copy.move(move);
        std::tuple<int32_t, bool, Move> a = alphaBetaMax(copy, alpha, beta, depthLeft - !check, depthCurrent + 1);
        int32_t evaluation = std::get<0>(a);
        bool gameWasFinished = std::get<1>(a);

        if (evaluation <= alpha) {
            TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
            return std::make_tuple(alpha, gameWasFinishedOnBestMove, bestMove);
        }
        if (evaluation < beta) {
            bestMove = move;
            bestMoveIndex = i;
            gameWasFinishedOnBestMove = gameWasFinished;
            beta = evaluation;
        }
    }

    TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
    return std::make_tuple(beta, gameWasFinishedOnBestMove, bestMove);
}
std::tuple<int32_t, bool, Move> AI::alphaBetaMax(const Position &position, int32_t alpha, int32_t beta, int32_t depthLeft, int32_t depthCurrent) {
    if (SearchInterrupter::getPtr()->interrupted()) {
        return std::make_tuple(0, false, Move());
    }
    if (depthLeft == 0) {
        return std::make_tuple(alphaBetaMaxOnlyCaptures(position, alpha, beta), false, Move());
    }
    if (position.fiftyMovesRuleDraw() or position.threefoldRepetitionDraw()) {
        return std::make_tuple(0, true, Move());
    }

    MoveList moves = LegalMoveGen::generate(position, SIDE::WHITE);
    moves = MoveSorter::sort(position.getPieces(), moves);
    Move bestMove;
    uint8_t bestMoveIndex;
    bool gameWasFinishedOnBestMove;

    uint8_t tableResult = TranspositionTable::getPtr()->getBestMoveIndex(position.getHash());
    if (tableResult < moves.getSize()) {
        std::swap(moves[0], moves[tableResult]);
    }

    bool check = PsLegalMoveMaskGen::inDanger(position.getPieces(), BOp::bsf(position.getPieces().getPieceBitboard(SIDE::WHITE, PIECE::KING)), SIDE::WHITE);
    if (moves.getSize() == 0) {
        if (check) {
            return std::make_tuple(INF::NEGATIVE + depthLeft, true, Move());
        }
        return std::make_tuple(0, true, Move());
    }

    for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
        Move move = moves[i];

        Position copy = position;
        copy.move(move);
        std::tuple<int32_t, bool, Move> a = alphaBetaMin(copy, alpha, beta, depthLeft - !check, depthCurrent + 1);
        int32_t evaluation = std::get<0>(a);
        bool gameWasFinished = std::get<1>(a);

        if (evaluation >= beta) {
            TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
            return std::make_tuple(beta, gameWasFinishedOnBestMove, bestMove);
        }
        if (evaluation > alpha) {
            bestMove = move;
            bestMoveIndex = i;
            gameWasFinishedOnBestMove = gameWasFinished;
            alpha = evaluation;
        }
    }

    TranspositionTable::getPtr()->addEntry(position.getHash(), depthCurrent, bestMoveIndex);
    return std::make_tuple(alpha, gameWasFinishedOnBestMove, bestMove);
}
int32_t AI::alphaBetaMinOnlyCaptures(const Position& position, int32_t alpha, int32_t beta) {
    if (SearchInterrupter::getPtr()->interrupted()) {
        return 0;
    }

    int32_t evaluation = StaticEvaluator::evaluate(position.getPieces());
    if (evaluation <= alpha) {
        return alpha;
    }
    if (evaluation < beta) {
        beta = evaluation;
    }

    MoveList moves = LegalMoveGen::generate(position, SIDE::BLACK, true);
    moves = MoveSorter::sort(position.getPieces(), moves);

    for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
        Move move = moves[i];

        Position copy = position;
        copy.move(move);
        evaluation = alphaBetaMaxOnlyCaptures(copy, alpha, beta);

        if (evaluation <= alpha) {
            return alpha;
        }
        if (evaluation < beta) {
            beta = evaluation;
        }
    }

    return beta;
}
int32_t AI::alphaBetaMaxOnlyCaptures(const Position& position, int32_t alpha, int32_t beta) {
    if (SearchInterrupter::getPtr()->interrupted()) {
        return 0;
    }

    int32_t evaluation = StaticEvaluator::evaluate(position.getPieces());
    if (evaluation >= beta) {
        return beta;
    }
    if (evaluation > alpha) {
        alpha = evaluation;
    }

    MoveList moves = LegalMoveGen::generate(position, SIDE::WHITE, true);
    moves = MoveSorter::sort(position.getPieces(), moves);

    for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
        Move move = moves[i];

        Position copy = position;
        copy.move(move);
        evaluation = alphaBetaMinOnlyCaptures(copy, alpha, beta);

        if (evaluation >= beta) {
            return beta;
        }
        if (evaluation > alpha) {
            alpha = evaluation;
        }
    }

    return alpha;
}