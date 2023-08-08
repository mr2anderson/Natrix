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


#include "LegalMoveGenTester.hpp"


void LegalMoveGenTester::runTests() {
    std::array<Test, 6> tests{};
    tests[0] = Test(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
            Position::NONE,
            true,
            true,
            true,
            true,
            SIDE::WHITE,
            (std::array<uint64_t, 6>){1, 20, 400, 8902, 197281, 4865609}
    );
    tests[1] = Test(
            "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R",
            Position::NONE,
            true,
            true,
            true,
            true,
            SIDE::WHITE,
            (std::array<uint64_t, 6>){1, 48, 2039, 97862, 4085603, 193690690}
    );
    tests[2] = Test(
            "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8",
            Position::NONE,
            false,
            false,
            false,
            false,
            SIDE::WHITE,
            (std::array<uint64_t, 6>){1, 14, 191, 2812, 43238, 674624}
    );
    tests[3] = Test(
            "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1",
            Position::NONE,
            false,
            false,
            true,
            true,
            SIDE::WHITE,
            (std::array<uint64_t, 6>){1, 6, 264, 9467, 422333, 15833292}
    );
    tests[4] = Test(
            "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R",
            Position::NONE,
            true,
            true,
            false,
            false,
            SIDE::WHITE,
            (std::array<uint64_t, 6>){1, 44, 1486, 62379, 2103487, 89941194}
    );
    tests[5] = Test(
            "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1",
            Position::NONE,
            false,
            false,
            false,
            false,
            SIDE::WHITE,
            (std::array<uint64_t, 6>){1, 46, 2079, 89890, 3894594, 164075551}
    );
    for (const auto& test : tests) {
        runTest(test);
    }
}
void LegalMoveGenTester::runTest(const Test &test) {
    auto position = Position(test.shortFen, test.enPassant, test.wlCastling, test.wsCastling, test.blCastling, test.bsCastling, test.side);

    for (uint32_t i = 0; i < test.nodes.size(); i = i + 1) {
        uint64_t start = nsecs;

        uint64_t correct = test.nodes[i];
        uint64_t got = getNodesNumber(position, test.side, i);

        float speed = (float)got / ((float)(nsecs - start) / (float)1e+9) / (float)1e+6;

        if (correct == got) {
            std::cout << "Depth " << std::setw(4) << i << ". Correct: " << std::setw(18) << correct << ". Got: " << std::setw(18) << got << ". Speed: " << std::setw(10) << speed << " MNPS. OK." << std::endl;
        }
        else {
            std::cout << "Depth " << std::setw(4) << i << ". Correct: " << std::setw(18) << correct << ". Got: " << std::setw(18) << got << ". Speed: " << std::setw(10) << speed << " MNPS. Error." << std::endl;
            std::terminate();
        }
    }
    std::cout << std::endl;
}
uint64_t LegalMoveGenTester::getNodesNumber(const Position& position, uint8_t side, uint32_t depth) {
    if (depth == 0) {
        return 1;
    }

    MoveList moves = LegalMoveGen::generate(position, side);
    uint64_t ctr = 0;

    for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
        Move move = moves[i];

        Position copy = position;
        copy.move(move);
        ctr = ctr + getNodesNumber(copy, Pieces::inverse(side), depth - 1);
    }

    return ctr;
}