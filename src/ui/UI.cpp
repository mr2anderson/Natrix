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


#include "UI.hpp"


UI::UI() {
    Storage::getPtr()->setRoot("../data");
    Storage::getPtr()->addTexture("logotype", "images/logotype.png");
    Storage::getPtr()->addTexture("whitePawn", "images/chessmen/white/pawn.png");
    Storage::getPtr()->addTexture("whiteKnight", "images/chessmen/white/knight.png");
    Storage::getPtr()->addTexture("whiteBishop", "images/chessmen/white/bishop.png");
    Storage::getPtr()->addTexture("whiteRook", "images/chessmen/white/rook.png");
    Storage::getPtr()->addTexture("whiteQueen", "images/chessmen/white/queen.png");
    Storage::getPtr()->addTexture("whiteKing", "images/chessmen/white/king.png");
    Storage::getPtr()->addTexture("blackPawn", "images/chessmen/black/pawn.png");
    Storage::getPtr()->addTexture("blackKnight", "images/chessmen/black/knight.png");
    Storage::getPtr()->addTexture("blackBishop", "images/chessmen/black/bishop.png");
    Storage::getPtr()->addTexture("blackRook", "images/chessmen/black/rook.png");
    Storage::getPtr()->addTexture("blackQueen", "images/chessmen/black/queen.png");
    Storage::getPtr()->addTexture("blackKing", "images/chessmen/black/king.png");
    Storage::getPtr()->addSoundBuffer("move", "sounds/move.ogg");
    Storage::getPtr()->addSoundBuffer("capture", "sounds/capture.ogg");
    Storage::getPtr()->addSoundBuffer("message", "sounds/message.ogg");

    this->window.create(sf::VideoMode(600, 600), "", sf::Style::Default);
    this->window.setFramerateLimit(20);

    this->position = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", Position::NONE, true, true, true, true, 1);
    this->buff = sf::Vector2i(INT32_MAX, INT32_MAX);
}
void UI::start() {
    sf::Sound sound;
    for (; ;) {
        while (this->window.pollEvent(this->event)) {
            if (this->event.type == sf::Event::Closed) {
                return;
            }
            if (this->event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, (float)this->window.getSize().x, (float)this->window.getSize().y);
                this->window.setView(sf::View(visibleArea));
            }
            else if (this->event.type == sf::Event::MouseButtonPressed and this->getStatus() == STATUS::WHITE_TO_MOVE) {
                sf::Vector2i mouse = sf::Mouse::getPosition(this->window);
                sf::Vector2i cell = sf::Vector2i(INT32_MAX, INT32_MAX);
                for (int32_t x = 0; x < 8; x = x + 1) {
                    for (int32_t y = 0; y < 8; y = y + 1) {
                        if (mouse.x > (int32_t)this->getCellPosition(x, y).x and mouse.y > (int32_t)this->getCellPosition(x, y).y and
                            mouse.x < (int32_t)(this->getCellPosition(x, y).x + this->getCellSize().x) and mouse.y < (int32_t)(this->getCellPosition(x, y).y + this->getCellSize().y)) {
                            cell = sf::Vector2i(x, y);
                        }
                    }
                }
                if (cell != sf::Vector2i(INT32_MAX, INT32_MAX)) {
                    uint8_t promotionFlag;
                    if ((float)mouse.x - this->getCellPosition(cell.x, cell.y).x < this->getCellSize().x / 2) {
                        if ((float)mouse.y - this->getCellPosition(cell.x, cell.y).y < this->getCellSize().y / 2) {
                            promotionFlag = Move::FLAG::PROMOTE_TO_KNIGHT;
                        }
                        else {
                            promotionFlag = Move::FLAG::PROMOTE_TO_BISHOP;
                        }
                    }
                    else {
                        if ((float)mouse.y - this->getCellPosition(cell.x, cell.y).y < this->getCellSize().y / 2) {
                            promotionFlag = Move::FLAG::PROMOTE_TO_ROOK;
                        }
                        else {
                            promotionFlag = Move::FLAG::PROMOTE_TO_QUEEN;
                        }
                    }
                    if (cell == buff) {
                        buff = sf::Vector2i(INT32_MAX, INT32_MAX);
                        sound.setBuffer(*Storage::getPtr()->getSoundBuffer("move"));
                        sound.play();
                    }
                    else if (BOp::getBit(this->position.getPieces().getSideBitboard(SIDE::WHITE), cell.y * 8 + cell.x)) {
                        buff = cell;
                        sound.setBuffer(*Storage::getPtr()->getSoundBuffer("move"));
                        sound.play();
                    }
                    else {
                        MoveList moves = LegalMoveGen::generate(this->position, SIDE::WHITE);
                        int32_t moveId = -1;
                        for (int32_t i = 0; i < moves.getSize(); i = i + 1) {
                            Move move = moves[i];
                            if (move.getFrom() == buff.y * 8 + buff.x and move.getTo() == cell.y * 8 + cell.x and
                               ((moves[i].getFlag() != Move::FLAG::PROMOTE_TO_KNIGHT and moves[i].getFlag() != Move::FLAG::PROMOTE_TO_BISHOP and moves[i].getFlag() != Move::FLAG::PROMOTE_TO_ROOK and moves[i].getFlag() != Move::FLAG::PROMOTE_TO_QUEEN) or promotionFlag == moves[i].getFlag())) {
                                moveId = i;
                                break;
                            }
                        }
                        this->buff = sf::Vector2i(INT32_MAX, INT32_MAX);
                        if (moveId != -1) {
                            this->position.move(moves[moveId]);
                            sound.setBuffer(*Storage::getPtr()->getSoundBuffer("move"));
                            sound.play();
                            if (moves[moveId].getDefenderType() != Move::NONE or moves[moveId].getFlag() == Move::FLAG::EN_PASSANT_CAPTURE) {
                                sound.setBuffer(*Storage::getPtr()->getSoundBuffer("capture")); // Этот звук отменяет предыдущий, так как используется общая переменная.
                                sound.play();
                            }
                            if (this->getStatus() == STATUS::WHITE_WON or this->getStatus() == STATUS::DRAW) {
                                sound.setBuffer(*Storage::getPtr()->getSoundBuffer("message")); // Аналогично.
                                sound.play();
                            }
                            else {
                                this->update();
                                Move move = AI::getBestMove(this->position, SIDE::BLACK, 2000);
                                this->position.move(move);
                                sound.setBuffer(*Storage::getPtr()->getSoundBuffer("move"));
                                sound.play();
                                if (move.getDefenderType() != Move::NONE or move.getFlag() == Move::FLAG::EN_PASSANT_CAPTURE) {
                                    sound.setBuffer(*Storage::getPtr()->getSoundBuffer("capture"));
                                    sound.play();
                                }
                                if (this->getStatus() == STATUS::BLACK_WON or this->getStatus() == STATUS::DRAW) {
                                    sound.setBuffer(*Storage::getPtr()->getSoundBuffer("message"));
                                    sound.play();
                                }
                            }
                        }
                    }
                }
            }
        }
        this->update();
    }
}
void UI::update() {
    this->window.clear(sf::Color(22, 21, 18));
    this->drawCells();
    this->drawPieces();
    this->drawSelectedPieceMoves();
    this->updateWindowTitle();
    this->window.display();
}
void UI::drawCells() {
    for (int32_t x = 0; x < 8; x = x + 1) {
        for (int32_t y = 0; y < 8; y = y + 1) {
            sf::RectangleShape cell;
            cell.setSize(this->getCellSize());
            cell.setPosition(this->getCellPosition(x, y));
            if ((x + y) % 2 == 0) {
                cell.setFillColor(sf::Color(181, 136, 99));
            }
            else {
                cell.setFillColor(sf::Color(240, 217, 181));
            }
            this->window.draw(cell);
        }
    }
}
void UI::drawPieces() {
    for (int32_t x = 0; x < 8; x = x + 1) {
        for (int32_t y = 0; y < 8; y = y + 1) {
            sf::Sprite sprite;
            std::string name = this->getTextureName(x, y);
            if (name.empty()) {
                continue;
            }
            sprite.setTexture(*Storage::getPtr()->getTexture(name));
            sprite.setScale(this->getCellSize().x / sprite.getLocalBounds().width, this->getCellSize().y / sprite.getLocalBounds().height);
            sprite.setPosition(this->getCellPosition(x, y));
            if (sf::Vector2i(x, y) == this->buff)  {
                sf::Vector2f oldSize = sf::Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
                sprite.setScale(sprite.getScale().x * 1.2f, sprite.getScale().y * 1.2f);
                sf::Vector2f newSize = sf::Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
                sprite.setPosition(sprite.getPosition().x - (newSize.x - oldSize.x) / 2, sprite.getPosition().y - (newSize.y - oldSize.y) / 2);
            }
            this->window.draw(sprite);
        }
    }
}
void UI::drawSelectedPieceMoves() {
    if (this->buff == sf::Vector2i(INT32_MAX, INT32_MAX)) {
        return;
    }

    std::string name = this->getTextureName(this->buff.x, this->buff.y);
    if (name.empty()) {
        return;
    }
    std::string side = name.substr(0, 5);

    MoveList moves;
    if (side == "white") {
        moves = LegalMoveGen::generate(this->position, SIDE::WHITE);
    }
    else {
        moves = LegalMoveGen::generate(this->position, SIDE::BLACK);
    }

    for (int32_t i = 0; i < moves.getSize(); i = i + 1) {
        if (moves[i].getFrom() == buff.y * 8 + buff.x) {
            uint8_t x = moves[i].getTo() % 8;
            uint8_t y = moves[i].getTo() / 8;
            if (moves[i].getAttackerType() == PIECE::PAWN and (y == 0 or y == 7)) {
                sf::Sprite sprite;
                sprite.setTexture(*Storage::getPtr()->getTexture(side + "Knight"));
                sprite.setScale(this->getCellSize().x / sprite.getLocalBounds().width * 0.5f, this->getCellSize().y / sprite.getLocalBounds().height * 0.5f);
                sprite.setColor(sf::Color(255, 255, 255, 63));

                sprite.setPosition(this->getCellPosition(x, y));
                this->window.draw(sprite);

                sprite.setTexture(*Storage::getPtr()->getTexture(side + "Bishop"));
                sprite.setPosition(sf::Vector2f(this->getCellPosition(x, y).x, this->getCellPosition(x, y).y + this->getCellSize().y / 2));
                this->window.draw(sprite);

                sprite.setTexture(*Storage::getPtr()->getTexture(side + "Rook"));
                sprite.setPosition(sf::Vector2f(this->getCellPosition(x, y).x + this->getCellSize().x / 2, this->getCellPosition(x, y).y));
                this->window.draw(sprite);

                sprite.setTexture(*Storage::getPtr()->getTexture(side + "Queen"));
                sprite.setPosition(sf::Vector2f(this->getCellPosition(x, y).x + this->getCellSize().x / 2, this->getCellPosition(x, y).y + this->getCellSize().y / 2));
                this->window.draw(sprite);
            }
            else {
                sf::Sprite sprite;
                sprite.setTexture(*Storage::getPtr()->getTexture(name));
                sprite.setScale(this->getCellSize().x / sprite.getLocalBounds().width, this->getCellSize().y / sprite.getLocalBounds().height);
                sprite.setPosition(this->getCellPosition(x, y));
                sprite.setColor(sf::Color(255, 255, 255, 63));
                this->window.draw(sprite);
            }
        }
    }
}
void UI::updateWindowTitle() {
    uint8_t status = this->getStatus();
    switch (status) {
        case STATUS::WHITE_TO_MOVE:
            this->window.setTitle(L"Natrix UI — Ваш ход");
            break;
        case STATUS::BLACK_TO_MOVE:
            this->window.setTitle(L"Natrix UI — Компьютер думает...");
            break;
        case STATUS::WHITE_WON:
            this->window.setTitle(L"Natrix UI — Вы победили");
            break;
        case STATUS::BLACK_WON:
            this->window.setTitle(L"Natrix UI — Вы проиграли");
            break;
        case STATUS::DRAW:
            this->window.setTitle(L"Natrix UI — Ничья");
            break;
    }
}
sf::Vector2f UI::getCellSize() {
    uint32_t side = (std::min(this->window.getSize().x, this->window.getSize().y) - 2 * BOARD_MARGIN) / 8;
    return {(float)side, (float)side};
}
sf::Vector2f UI::getCellPosition(int32_t x, int32_t y) {
    sf::Vector2f size = this->getCellSize();
    return {BOARD_MARGIN + size.x * (float)x, (float)this->window.getSize().y - BOARD_MARGIN - size.y * (float)(y + 1)};
}
uint8_t UI::getStatus() {
    if (this->position.fiftyMovesRuleDraw() or this->position.threefoldRepetitionDraw()) {
        return STATUS::DRAW;
    }

    if (!this->position.getPieces().getPieceBitboard(SIDE::WHITE, PIECE::PAWN) and
        !this->position.getPieces().getPieceBitboard(SIDE::BLACK, PIECE::PAWN) and
        !this->position.getPieces().getPieceBitboard(SIDE::WHITE, PIECE::ROOK) and
        !this->position.getPieces().getPieceBitboard(SIDE::BLACK, PIECE::ROOK) and
        !this->position.getPieces().getPieceBitboard(SIDE::WHITE, PIECE::QUEEN) and
        !this->position.getPieces().getPieceBitboard(SIDE::BLACK, PIECE::QUEEN) and

        BOp::count1(this->position.getPieces().getPieceBitboard(SIDE::WHITE, PIECE::KNIGHT) |
                        this->position.getPieces().getPieceBitboard(SIDE::WHITE, PIECE::BISHOP)) < 2 and

        BOp::count1(this->position.getPieces().getPieceBitboard(SIDE::BLACK, PIECE::KNIGHT) |
                        this->position.getPieces().getPieceBitboard(SIDE::BLACK, PIECE::BISHOP)) < 2) {

        return STATUS::DRAW;
    }

    if (this->position.blackToMove()) {
        MoveList moves = LegalMoveGen::generate(this->position, SIDE::BLACK);
        if (moves.getSize() == 0) {
            if (PsLegalMoveMaskGen::inDanger(this->position.getPieces(),BOp::bsf(this->position.getPieces().getPieceBitboard(SIDE::BLACK, PIECE::KING)), SIDE::BLACK)) {
                return STATUS::WHITE_WON;
            }
            return STATUS::DRAW;
        }
        return STATUS::BLACK_TO_MOVE;
    }

    MoveList moves = LegalMoveGen::generate(this->position, SIDE::WHITE);
    if (moves.getSize() == 0) {
        if (PsLegalMoveMaskGen::inDanger(this->position.getPieces(),BOp::bsf(this->position.getPieces().getPieceBitboard(SIDE::WHITE, PIECE::KING)), SIDE::WHITE)) {
            return STATUS::BLACK_WON;
        }
        return STATUS::DRAW;
    }
    return STATUS::WHITE_TO_MOVE;
}
std::string UI::getTextureName(int32_t x, int32_t y) const {
    int32_t id = y * 8 + x;
    std::array<std::array<Bitboard, 6>, 2> bs = this->position.getPieces().getPieceBitboards();

    if (BOp::getBit(bs[SIDE::WHITE][PIECE::PAWN], id)) {
        return "whitePawn";
    }
    if (BOp::getBit(bs[SIDE::WHITE][PIECE::KNIGHT], id)) {
        return "whiteKnight";
    }
    if (BOp::getBit(bs[SIDE::WHITE][PIECE::BISHOP], id)) {
        return "whiteBishop";
    }
    if (BOp::getBit(bs[SIDE::WHITE][PIECE::ROOK], id)) {
        return "whiteRook";
    }
    if (BOp::getBit(bs[SIDE::WHITE][PIECE::QUEEN], id)) {
        return "whiteQueen";
    }
    if (BOp::getBit(bs[SIDE::WHITE][PIECE::KING], id)) {
        return "whiteKing";
    }
    if (BOp::getBit(bs[SIDE::BLACK][PIECE::PAWN], id)) {
        return "blackPawn";
    }
    if (BOp::getBit(bs[SIDE::BLACK][PIECE::KNIGHT], id)) {
        return "blackKnight";
    }
    if (BOp::getBit(bs[SIDE::BLACK][PIECE::BISHOP], id)) {
        return "blackBishop";
    }
    if (BOp::getBit(bs[SIDE::BLACK][PIECE::ROOK], id)) {
        return "blackRook";
    }
    if (BOp::getBit(bs[SIDE::BLACK][PIECE::QUEEN], id)) {
        return "blackQueen";
    }
    if (BOp::getBit(bs[SIDE::BLACK][PIECE::KING], id)) {
        return "blackKing";
    }

    return "";
}
