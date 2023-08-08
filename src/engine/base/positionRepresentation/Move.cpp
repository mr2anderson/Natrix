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


#include "Move.hpp"


Move::Move() = default;
Move::Move(uint8_t from, uint8_t to, uint8_t attackerType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, uint8_t flag) {
    this->from = from;
    this->to = to;
    this->attackerType = attackerType;
    this->attackerSide = attackerSide;
    this->defenderType = defenderType;
    this->defenderSide = defenderSide;
    this->flag = flag;
}
void Move::setFrom(uint8_t newFrom) {
    this->from = newFrom;
}
void Move::setTo(uint8_t newTo) {
    this->to = newTo;
}
void Move::setAttackerType(uint8_t newAttackerType) {
    this->attackerType = newAttackerType;
}
void Move::setAttackerSide(uint8_t newAttackerSide) {
    this->attackerSide = newAttackerSide;
}
void Move::setDefenderType(uint8_t newDefenderType) {
    this->defenderType = newDefenderType;
}
void Move::setDefenderSide(uint8_t newDefenderSide) {
    this->defenderSide = newDefenderSide;
}
void Move::setFlag(uint8_t newFlag) {
    this->flag = newFlag;
}
uint8_t Move::getFrom() const {
    return this->from;
}
uint8_t Move::getTo() const {
    return this->to;
}
uint8_t Move::getAttackerType() const {
    return this->attackerType;
}
uint8_t Move::getAttackerSide() const {
    return this->attackerSide;
}
uint8_t Move::getDefenderType() const {
    return this->defenderType;
}
uint8_t Move::getDefenderSide() const {
    return this->defenderSide;
}
uint8_t Move::getFlag() const {
    return this->flag;
}