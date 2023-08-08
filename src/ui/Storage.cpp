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


#include "Storage.hpp"


Storage *Storage::storage = nullptr;


Storage* Storage::getPtr() {
    if (storage == nullptr) {
        storage = new Storage();
    }
    return storage;
}
void Storage::setRoot(const std::string &newRoot) {
    this->root = newRoot;
}
void Storage::addTexture(const std::string& name, const std::string& path) {
    this->textures[name].loadFromFile(this->root + "/" + path);
}
void Storage::addFont(const std::string& name, const std::string& path) {
    this->fonts[name].loadFromFile(this->root + "/" + path);
}
void Storage::addSoundBuffer(const std::string& name, const std::string& path) {
    this->soundbuffers[name].loadFromFile(this->root + "/" + path);
}
void Storage::addMusic(const std::string& name, const std::string& path) {
    this->music[name].openFromFile(this->root + "/" + path);
}
sf::Texture *Storage::getTexture(const std::string& name) {
    return &this->textures[name];
}
sf::Font *Storage::getFont(const std::string& name) {
    return &this->fonts[name];
}
sf::SoundBuffer *Storage::getSoundBuffer(const std::string& name) {
    return &this->soundbuffers[name];
}
sf::Music *Storage::getMusic(const std::string& name) {
    return &this->music[name];
}
Storage::Storage() = default;