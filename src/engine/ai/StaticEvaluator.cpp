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


#include "StaticEvaluator.hpp"


int32_t StaticEvaluator::evaluate(Pieces pieces, bool showDebugInfo) {
    int32_t evaluation = 0;

    int32_t materialEvaluation = material(pieces);
    int32_t mobilityEvaluation = mobility(pieces);
    int32_t doublePawnEvaluation = doublePawn(pieces);
    int32_t connectedPawnEvaluation = connectedPawn(pieces);
    int32_t pawnPromotionEvaluation = pawnPromotion(pieces);
    int32_t kingSafetyEvaluation = kingSafety(pieces);
    int32_t endgameEvaluation = endgame(pieces, materialEvaluation >= 0);

    evaluation = evaluation + materialEvaluation;
    evaluation = evaluation + mobilityEvaluation;
    evaluation = evaluation + doublePawnEvaluation;
    evaluation = evaluation + connectedPawnEvaluation;
    evaluation = evaluation + pawnPromotionEvaluation;
    evaluation = evaluation + kingSafetyEvaluation;
    evaluation = evaluation + endgameEvaluation;

    if (showDebugInfo) {
        std::cout << "Material: " << (float)materialEvaluation / 100.f << " pawns." << std::endl;
        std::cout << "Mobility: " << (float)mobilityEvaluation / 100.f << " pawns." << std::endl;
        std::cout << "Double pawn: " << (float)doublePawnEvaluation / 100.f << " pawns." << std::endl;
        std::cout << "Connected pawn: " << (float)connectedPawnEvaluation / 100.f << " pawns." << std::endl;
        std::cout << "Pawn promotion: " << (float)pawnPromotionEvaluation / 100.f << " pawns." << std::endl;
        std::cout << "King safety: " << (float)kingSafetyEvaluation / 100.f << " pawns." << std::endl;
        std::cout << "Endgame: " << (float)endgameEvaluation / 100.f << " pawns." << std::endl;
        std::cout << "Total: " << (float)evaluation / 100.f << " pawns." << std::endl;
    }

    return evaluation;
}
int32_t StaticEvaluator::material(Pieces pieces) {
    int32_t material = 0;

    material = material + MATERIAL::PAWN * (BOp::count1(pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN)) -
                                            BOp::count1(pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN)));

    material = material + MATERIAL::KNIGHT * (BOp::count1(pieces.getPieceBitboard(SIDE::WHITE, PIECE::KNIGHT)) -
                                              BOp::count1(pieces.getPieceBitboard(SIDE::BLACK, PIECE::KNIGHT)));

    material = material + MATERIAL::BISHOP * (BOp::count1(pieces.getPieceBitboard(SIDE::WHITE, PIECE::BISHOP)) -
                                              BOp::count1(pieces.getPieceBitboard(SIDE::BLACK, PIECE::BISHOP)));

    material = material + MATERIAL::ROOK * (BOp::count1(pieces.getPieceBitboard(SIDE::WHITE, PIECE::ROOK)) -
                                            BOp::count1(pieces.getPieceBitboard(SIDE::BLACK, PIECE::ROOK)));

    material = material + MATERIAL::QUEEN * (BOp::count1(pieces.getPieceBitboard(SIDE::WHITE, PIECE::QUEEN)) -
                                             BOp::count1(pieces.getPieceBitboard(SIDE::BLACK, PIECE::QUEEN)));

    return material;
}
int32_t StaticEvaluator::mobility(Pieces pieces) {
    int32_t mobility = 0;

    std::array<std::array<Bitboard, 6>, 2> masks = pieces.getPieceBitboards();
    int32_t knightMoves = 0;
    int32_t bishopMoves = 0;
    int32_t rookMoves = 0;
    int32_t queenMoves = 0;

    Bitboard whitePawnsLeftAttacks = PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(pieces, SIDE::WHITE, true);
    Bitboard whitePawnsRightAttacks = PsLegalMoveMaskGen::generatePawnsRightCapturesMask(pieces, SIDE::WHITE, true);
    Bitboard whitePawnsAttacks = whitePawnsLeftAttacks | whitePawnsRightAttacks;
    Bitboard safeForBlack = ~whitePawnsAttacks;

    Bitboard blackPawnsLeftAttacks = PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(pieces, SIDE::BLACK, true);
    Bitboard blackPawnsRightAttacks = PsLegalMoveMaskGen::generatePawnsRightCapturesMask(pieces, SIDE::BLACK, true);
    Bitboard blackPawnsAttacks = blackPawnsLeftAttacks | blackPawnsRightAttacks;
    Bitboard safeForWhite = ~blackPawnsAttacks;

    while (masks[SIDE::WHITE][PIECE::KNIGHT]) {
        uint8_t index = BOp::bsf(masks[SIDE::WHITE][PIECE::KNIGHT]);
        masks[SIDE::WHITE][PIECE::KNIGHT] = BOp::set0(masks[SIDE::WHITE][PIECE::KNIGHT], index);
        knightMoves = knightMoves + BOp::count1(PsLegalMoveMaskGen::generateKnightMask(pieces, index, SIDE::WHITE, false) & safeForWhite);
    }
    while (masks[SIDE::WHITE][PIECE::BISHOP]) {
        uint8_t index = BOp::bsf(masks[SIDE::WHITE][PIECE::BISHOP]);
        masks[SIDE::WHITE][PIECE::BISHOP] = BOp::set0(masks[SIDE::WHITE][PIECE::BISHOP], index);
        bishopMoves = bishopMoves + BOp::count1(PsLegalMoveMaskGen::generateBishopMask(pieces, index, SIDE::WHITE, false) & safeForWhite);
    }
    while (masks[SIDE::WHITE][PIECE::ROOK]) {
        uint8_t index = BOp::bsf(masks[SIDE::WHITE][PIECE::ROOK]);
        masks[SIDE::WHITE][PIECE::ROOK] = BOp::set0(masks[SIDE::WHITE][PIECE::ROOK], index);
        rookMoves = rookMoves + BOp::count1(PsLegalMoveMaskGen::generateRookMask(pieces, index, SIDE::WHITE, false) & safeForWhite);
    }
    while (masks[SIDE::WHITE][PIECE::QUEEN]) {
        uint8_t index = BOp::bsf(masks[SIDE::WHITE][PIECE::QUEEN]);
        masks[SIDE::WHITE][PIECE::QUEEN] = BOp::set0(masks[SIDE::WHITE][PIECE::QUEEN], index);
        queenMoves = queenMoves + BOp::count1(PsLegalMoveMaskGen::generateQueenMask(pieces, index, SIDE::WHITE, false) & safeForWhite);
    }
    while (masks[SIDE::BLACK][PIECE::KNIGHT]) {
        uint8_t index = BOp::bsf(masks[SIDE::BLACK][PIECE::KNIGHT]);
        masks[SIDE::BLACK][PIECE::KNIGHT] = BOp::set0(masks[SIDE::BLACK][PIECE::KNIGHT], index);
        knightMoves = knightMoves - BOp::count1(PsLegalMoveMaskGen::generateKnightMask(pieces, index, SIDE::BLACK, false) & safeForBlack);
    }
    while (masks[SIDE::BLACK][PIECE::BISHOP]) {
        uint8_t index = BOp::bsf(masks[SIDE::BLACK][PIECE::BISHOP]);
        masks[SIDE::BLACK][PIECE::BISHOP] = BOp::set0(masks[SIDE::BLACK][PIECE::BISHOP], index);
        bishopMoves = bishopMoves - BOp::count1(PsLegalMoveMaskGen::generateBishopMask(pieces, index, SIDE::BLACK, false) & safeForBlack);
    }
    while (masks[SIDE::BLACK][PIECE::ROOK]) {
        uint8_t index = BOp::bsf(masks[SIDE::BLACK][PIECE::ROOK]);
        masks[SIDE::BLACK][PIECE::ROOK] = BOp::set0(masks[SIDE::BLACK][PIECE::ROOK], index);
        rookMoves = rookMoves - BOp::count1(PsLegalMoveMaskGen::generateRookMask(pieces, index, SIDE::BLACK, false) & safeForBlack);
    }
    while (masks[SIDE::BLACK][PIECE::QUEEN]) {
        uint8_t index = BOp::bsf(masks[SIDE::BLACK][PIECE::QUEEN]);
        masks[SIDE::BLACK][PIECE::QUEEN] = BOp::set0(masks[SIDE::BLACK][PIECE::QUEEN], index);
        queenMoves = queenMoves - BOp::count1(PsLegalMoveMaskGen::generateQueenMask(pieces, index, SIDE::BLACK, false) & safeForBlack);
    }

    mobility = mobility + MOBILITY::KNIGHT * knightMoves;
    mobility = mobility + MOBILITY::BISHOP * bishopMoves;
    mobility = mobility + MOBILITY::ROOK * rookMoves;
    mobility = mobility + MOBILITY::QUEEN * queenMoves;

    return mobility;
}
int32_t StaticEvaluator::doublePawn(Pieces pieces) {
    int32_t doublePawnsNumber = 0;

    for (uint8_t x = 0; x < 8; x = x + 1) {
        uint8_t whitePawns = BOp::count1(pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN) & BColumns::COLUMNS[x]);
        uint8_t blackPawns = BOp::count1(pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN) & BColumns::COLUMNS[x]);

        doublePawnsNumber = doublePawnsNumber + std::max(0, whitePawns - 1);
        doublePawnsNumber = doublePawnsNumber - std::max(0, blackPawns - 1);
    }

    return PAWN_STRUCTURE::DOUBLE_PAWN * doublePawnsNumber;
}
int32_t StaticEvaluator::connectedPawn(Pieces pieces) {
    int32_t connectedPawnsNumber = 0;

    Bitboard whiteCaptures = PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(pieces, SIDE::WHITE, true) | PsLegalMoveMaskGen::generatePawnsRightCapturesMask(pieces, SIDE::WHITE, true);
    Bitboard blackCaptures = PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(pieces, SIDE::BLACK, true) | PsLegalMoveMaskGen::generatePawnsRightCapturesMask(pieces, SIDE::BLACK, true);

    connectedPawnsNumber = connectedPawnsNumber + BOp::count1(whiteCaptures & pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN));
    connectedPawnsNumber = connectedPawnsNumber - BOp::count1(blackCaptures & pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN));

    return PAWN_STRUCTURE::CONNECTED_PAWN * connectedPawnsNumber;
}
int32_t StaticEvaluator::pawnPromotion(Pieces pieces) {
    int32_t pawnPromotion = 0;

    Bitboard whitePawns = pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN);
    Bitboard blackPawns = pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN);

    while (whitePawns) {
        uint8_t index = BOp::bsf(whitePawns);
        whitePawns = BOp::set0(whitePawns, index);
        if (PassedPawnMasks::WHITE_PASSED_PAWN_MASKS[index] & pieces.getPieceBitboard(SIDE::BLACK, PIECE::PAWN)) {
            pawnPromotion = pawnPromotion + PAWN_STRUCTURE::DEFAULT_PAWN_PROMOTION[index / 8];
        }
        else {
            pawnPromotion = pawnPromotion + PAWN_STRUCTURE::PASSED_PAWN_PROMOTION[index / 8];
        }
    }
    while (blackPawns) {
        uint8_t index = BOp::bsf(blackPawns);
        blackPawns = BOp::set0(blackPawns, index);
        if (PassedPawnMasks::BLACK_PASSED_PAWN_MASKS[index] & pieces.getPieceBitboard(SIDE::WHITE, PIECE::PAWN)) {
            pawnPromotion = pawnPromotion - PAWN_STRUCTURE::DEFAULT_PAWN_PROMOTION[7 - index / 8];
        }
        else {
            pawnPromotion = pawnPromotion - PAWN_STRUCTURE::PASSED_PAWN_PROMOTION[7 - index / 8];
        }
    }

    return pawnPromotion;
}
int32_t StaticEvaluator::kingSafety(Pieces pieces) {
    int32_t kingSafety = 0;

    if (BOp::count1(pieces.getAllBitboard()) <= ENDGAME::MAXIMUM_PIECES_FOR_ENDGAME) {
        return kingSafety;
    }

    uint8_t whiteKingP = BOp::bsf(pieces.getPieceBitboard(SIDE::WHITE, PIECE::KING));
    uint8_t blackKingP = BOp::bsf(pieces.getPieceBitboard(SIDE::BLACK, PIECE::KING));

    Bitboard whiteKingArea = KingMasks::MASKS[whiteKingP];
    Bitboard blackKingArea = KingMasks::MASKS[blackKingP];

    std::array<std::array<Bitboard, 6>, 2> masks = pieces.getPieceBitboards();
    int32_t knightMoves = 0;
    int32_t bishopMoves = 0;
    int32_t rookMoves = 0;
    int32_t queenMoves = 0;

    while (masks[SIDE::WHITE][PIECE::KNIGHT]) {
        uint8_t index = BOp::bsf(masks[SIDE::WHITE][PIECE::KNIGHT]);
        masks[SIDE::WHITE][PIECE::KNIGHT] = BOp::set0(masks[SIDE::WHITE][PIECE::KNIGHT], index);
        knightMoves = knightMoves + BOp::count1(PsLegalMoveMaskGen::generateKnightMask(pieces, index, SIDE::WHITE, false) & blackKingArea);
    }
    while (masks[SIDE::WHITE][PIECE::BISHOP]) {
        uint8_t index = BOp::bsf(masks[SIDE::WHITE][PIECE::BISHOP]);
        masks[SIDE::WHITE][PIECE::BISHOP] = BOp::set0(masks[SIDE::WHITE][PIECE::BISHOP], index);
        bishopMoves = bishopMoves + BOp::count1(PsLegalMoveMaskGen::generateBishopMask(pieces, index, SIDE::WHITE, false) & blackKingArea);
    }
    while (masks[SIDE::WHITE][PIECE::ROOK]) {
        uint8_t index = BOp::bsf(masks[SIDE::WHITE][PIECE::ROOK]);
        masks[SIDE::WHITE][PIECE::ROOK] = BOp::set0(masks[SIDE::WHITE][PIECE::ROOK], index);
        rookMoves = rookMoves + BOp::count1(PsLegalMoveMaskGen::generateRookMask(pieces, index, SIDE::WHITE, false) & blackKingArea);
    }
    while (masks[SIDE::WHITE][PIECE::QUEEN]) {
        uint8_t index = BOp::bsf(masks[SIDE::WHITE][PIECE::QUEEN]);
        masks[SIDE::WHITE][PIECE::QUEEN] = BOp::set0(masks[SIDE::WHITE][PIECE::QUEEN], index);
        queenMoves = queenMoves + BOp::count1(PsLegalMoveMaskGen::generateQueenMask(pieces, index, SIDE::WHITE, false) & blackKingArea);
    }
    while (masks[SIDE::BLACK][PIECE::KNIGHT]) {
        uint8_t index = BOp::bsf(masks[SIDE::BLACK][PIECE::KNIGHT]);
        masks[SIDE::BLACK][PIECE::KNIGHT] = BOp::set0(masks[SIDE::BLACK][PIECE::KNIGHT], index);
        knightMoves = knightMoves - BOp::count1(PsLegalMoveMaskGen::generateKnightMask(pieces, index, SIDE::BLACK, false) & whiteKingArea);
    }
    while (masks[SIDE::BLACK][PIECE::BISHOP]) {
        uint8_t index = BOp::bsf(masks[SIDE::BLACK][PIECE::BISHOP]);
        masks[SIDE::BLACK][PIECE::BISHOP] = BOp::set0(masks[SIDE::BLACK][PIECE::BISHOP], index);
        bishopMoves = bishopMoves - BOp::count1(PsLegalMoveMaskGen::generateBishopMask(pieces, index, SIDE::BLACK, false) & whiteKingArea);
    }
    while (masks[SIDE::BLACK][PIECE::ROOK]) {
        uint8_t index = BOp::bsf(masks[SIDE::BLACK][PIECE::ROOK]);
        masks[SIDE::BLACK][PIECE::ROOK] = BOp::set0(masks[SIDE::BLACK][PIECE::ROOK], index);
        rookMoves = rookMoves - BOp::count1(PsLegalMoveMaskGen::generateRookMask(pieces, index, SIDE::BLACK, false) & whiteKingArea);
    }
    while (masks[SIDE::BLACK][PIECE::QUEEN]) {
        uint8_t index = BOp::bsf(masks[SIDE::BLACK][PIECE::QUEEN]);
        masks[SIDE::BLACK][PIECE::QUEEN] = BOp::set0(masks[SIDE::BLACK][PIECE::QUEEN], index);
        queenMoves = queenMoves - BOp::count1(PsLegalMoveMaskGen::generateQueenMask(pieces, index, SIDE::BLACK, false) & whiteKingArea);
    }

    kingSafety = kingSafety + KING_SAFETY::KNIGHT * knightMoves;
    kingSafety = kingSafety + KING_SAFETY::BISHOP * bishopMoves;
    kingSafety = kingSafety + KING_SAFETY::ROOK * rookMoves;
    kingSafety = kingSafety + KING_SAFETY::QUEEN * queenMoves;

    return kingSafety;
}
int32_t StaticEvaluator::endgame(Pieces pieces, bool whiteStronger) {
    int32_t endgame = 0;

    if (BOp::count1(pieces.getAllBitboard()) > ENDGAME::MAXIMUM_PIECES_FOR_ENDGAME) {
        return endgame;
    }

    uint8_t attackerSide;
    uint8_t defenderSide;
    if (whiteStronger) {
        attackerSide = SIDE::WHITE;
        defenderSide = SIDE::BLACK;
    }
    else {
        attackerSide = SIDE::BLACK;
        defenderSide = SIDE::WHITE;
    }

    uint8_t attackerKingP = BOp::bsf(pieces.getPieceBitboard(attackerSide, PIECE::KING));
    int8_t attackerKingX = attackerKingP % 8;
    int8_t attackerKingY = attackerKingP / 8;

    uint8_t defenderKingP = BOp::bsf(pieces.getPieceBitboard(defenderSide, PIECE::KING));
    int8_t defenderKingX = defenderKingP % 8;
    int8_t defenderKingY = defenderKingP / 8;

    endgame = endgame + ENDGAME::PROXIMITY_KINGS * (16 - std::abs(attackerKingX - defenderKingX) - std::abs(attackerKingY - defenderKingY));
    endgame = endgame + ENDGAME::DISTANCE_WEAK_KING_MIDDLE * (std::abs(defenderKingX - 3) + std::abs(defenderKingY - 4));

    if (!whiteStronger) {
        endgame = -endgame;
    }

    return endgame;
}