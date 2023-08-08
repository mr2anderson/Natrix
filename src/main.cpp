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


#define TEST_LEGAL_MOVE_GENERATOR false
#define SHOW_UI true


#if TEST_LEGAL_MOVE_GENERATOR
#include "engine/base/moveGeneration/LegalMoveGenTester.hpp"
#endif
#if SHOW_UI
#include "ui/UI.hpp"
#endif


int main() {
    #if TEST_LEGAL_MOVE_GENERATOR
    LegalMoveGenTester::runTests();
    #endif
    #if SHOW_UI
    UI ui;
    ui.start();
    #endif

    return 0;
}