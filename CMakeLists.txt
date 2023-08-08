cmake_minimum_required(VERSION 3.22)
project(Natrix)
set(CMAKE_CXX_STANDARD 20)
add_executable(Natrix
        src/main.cpp

        src/engine/base/positionRepresentation/Bitboard.hpp
        src/engine/base/positionRepresentation/Pieces.hpp src/engine/base/positionRepresentation/Pieces.cpp
        src/engine/base/positionRepresentation/ZobristHashConstants.hpp
        src/engine/base/positionRepresentation/ZobristHash.hpp src/engine/base/positionRepresentation/ZobristHash.cpp
        src/engine/base/positionRepresentation/RepetitionHistory.hpp src/engine/base/positionRepresentation/RepetitionHistory.cpp
        src/engine/base/positionRepresentation/Move.hpp src/engine/base/positionRepresentation/Move.cpp
        src/engine/base/positionRepresentation/Position.hpp src/engine/base/positionRepresentation/Position.cpp

        src/engine/base/moveGeneration/KnightMasks.hpp
        src/engine/base/moveGeneration/KingMasks.hpp
        src/engine/base/moveGeneration/SlidersMasks.hpp
        src/engine/base/moveGeneration/PsLegalMoveMaskGen.hpp src/engine/base/moveGeneration/PsLegalMoveMaskGen.cpp
        src/engine/base/moveGeneration/MoveList.hpp src/engine/base/moveGeneration/MoveList.cpp
        src/engine/base/moveGeneration/LegalMoveGen.hpp src/engine/base/moveGeneration/LegalMoveGen.cpp
        src/engine/base/moveGeneration/LegalMoveGenTester.hpp src/engine/base/moveGeneration/LegalMoveGenTester.cpp

        src/engine/ai/PassedPawnMasks.hpp
        src/engine/ai/StaticEvaluator.hpp src/engine/ai/StaticEvaluator.cpp
        src/engine/ai/MoveSorter.hpp src/engine/ai/MoveSorter.cpp
        src/engine/ai/TranspositionTable.hpp src/engine/ai/TranspositionTable.cpp
        src/engine/ai/SearchInterrupter.hpp src/engine/ai/SearchInterrupter.cpp
        src/engine/ai/AI.hpp src/engine/ai/AI.cpp

        src/ui/Storage.hpp src/ui/Storage.cpp
        src/ui/UI.hpp src/ui/UI.cpp)
include(FindPkgConfig)
add_definitions(-Werror)
add_definitions(-O3)
add_definitions(-flto)


find_package(Threads REQUIRED)
target_link_libraries(Natrix Threads::Threads)


target_link_libraries(Natrix sfml-system sfml-window sfml-graphics sfml-audio)