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


#include <unordered_map>
#include <cstdint>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#pragma once


class Storage {
public:
    static Storage* getPtr();
    Storage(const Storage &donor) = delete;

    void setRoot(const std::string &newRoot);

    void addTexture(const std::string& name, const std::string& path);
    void addFont(const std::string& name, const std::string& path);
    void addSoundBuffer(const std::string& name, const std::string& path);
    void addMusic(const std::string& name, const std::string& path);

    [[nodiscard]] sf::Texture *getTexture(const std::string& name);
    [[nodiscard]] sf::Font *getFont(const std::string& name);
    [[nodiscard]] sf::SoundBuffer *getSoundBuffer(const std::string& name);
    [[nodiscard]] sf::Music *getMusic(const std::string& name);
private:
    Storage();

    static Storage* storage;

    std::string root;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    std::unordered_map<std::string, sf::SoundBuffer> soundbuffers;
    std::unordered_map<std::string, sf::Music> music;
};