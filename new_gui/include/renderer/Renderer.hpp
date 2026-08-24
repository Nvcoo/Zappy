/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Renderer
*/

#ifndef RENDERER_HPP_
    #define RENDERER_HPP_

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "../world/World.hpp"

class Renderer {
    private:
        sf::RenderWindow _window;
        int _tileSize;
        std::unordered_map<std::string, sf::Color> _teamColors;
        sf::Color getTeamColor(const std::string &team);
        void drawTile(int x, int y, Tile &tile);
        void drawEgg(const Egg &egg);
        void drawPlayer(const Player &player);
    protected:
    public:
        Renderer(int width, int height, int tileSize) : _window(sf::VideoMode(width * tileSize, height * tileSize), "Zappy"), _tileSize(tileSize)
        {
            _window.setFramerateLimit(60);
        }
        bool isOpen() const
        {
            return _window.isOpen();
        }
        void handleEvents();
        void draw(World &world);
};

#endif
