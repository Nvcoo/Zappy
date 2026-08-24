/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Renderer
*/

#include "../../include/renderer/Renderer.hpp"

static const sf::Color teamPalette[] = {
    sf::Color::Red,
    sf::Color::Blue,
    sf::Color::Yellow,
    sf::Color::Magenta,
    sf::Color::Cyan,
    sf::Color(255, 165, 0)
};

static const sf::Color resourceColors[RESOURCE_COUNT] = {
    sf::Color(220, 220, 60),  // FOOD
    sf::Color::White,        // LINEMATE
    sf::Color(255, 140, 0),  // DERAUMERE
    sf::Color(160, 32, 240), // SIBUR
    sf::Color(255, 105, 180),// MENDIANE
    sf::Color(0, 200, 0),    // PHIRAS
    sf::Color(0, 255, 255)   // THYSTAME
};

static const float resourceSlotX[RESOURCE_COUNT] = {0.15f, 0.38f, 0.62f, 0.85f, 0.15f, 0.38f, 0.62f};
static const float resourceSlotY[RESOURCE_COUNT] = {0.3f, 0.3f, 0.3f, 0.3f, 0.7f, 0.7f, 0.7f};

void Renderer::handleEvents()
{
    sf::Event event;

    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();
    }
}

sf::Color Renderer::getTeamColor(const std::string &team)
{
    auto it = _teamColors.find(team);

    if (it != _teamColors.end())
        return it->second;

    sf::Color color = teamPalette[_teamColors.size() % (sizeof(teamPalette) / sizeof(teamPalette[0]))];
    _teamColors[team] = color;
    return color;
}

void Renderer::drawTile(int x, int y, Tile &tile)
{
    sf::RectangleShape square(sf::Vector2f(_tileSize - 2, _tileSize - 2));

    square.setPosition(x * _tileSize + 1, y * _tileSize + 1);
    square.setFillColor(sf::Color(50, 50, 50));
    _window.draw(square);

    float markerSize = _tileSize / 6.f;

    for (int r = 0; r < RESOURCE_COUNT; r++) {
        if (tile.getResource(static_cast<Resource>(r)) <= 0)
            continue;

        sf::RectangleShape marker(sf::Vector2f(markerSize, markerSize));
        marker.setFillColor(resourceColors[r]);
        marker.setPosition(x * _tileSize + resourceSlotX[r] * _tileSize - markerSize / 2.f, y * _tileSize + resourceSlotY[r] * _tileSize - markerSize / 2.f);
        _window.draw(marker);
    }
}

void Renderer::drawEgg(const Egg &egg)
{
    float radius = _tileSize / 5.f;
    sf::CircleShape shape(radius);

    shape.setFillColor(sf::Color(222, 202, 176));
    shape.setOutlineColor(sf::Color(120, 100, 80));
    shape.setOutlineThickness(1.f);
    shape.setPosition(egg.x * _tileSize + _tileSize / 2.f - radius, egg.y * _tileSize + _tileSize / 2.f - radius);
    _window.draw(shape);
}

void Renderer::drawPlayer(const Player &player)
{
    float radius = _tileSize / 3.f;
    float centerX = player.getX() * _tileSize + _tileSize / 2.f;
    float centerY = player.getY() * _tileSize + _tileSize / 2.f;
    float angle = 0.f;

    switch (player.getOrientation()) {
        case NORTH: angle = -90.f; break;
        case EAST: angle = 0.f; break;
        case SOUTH: angle = 90.f; break;
        case WEST: angle = 180.f; break;
    }

    sf::CircleShape circle(radius);
    circle.setFillColor(getTeamColor(player.getTeam()));
    circle.setPosition(centerX - radius, centerY - radius);
    _window.draw(circle);

    sf::RectangleShape facing(sf::Vector2f(radius * 1.6f, 3.f));
    facing.setFillColor(sf::Color::White);
    facing.setOrigin(0.f, 1.5f);
    facing.setPosition(centerX, centerY);
    facing.setRotation(angle);
    _window.draw(facing);
}

void Renderer::draw(World &world)
{
    _window.clear(sf::Color::Black);
    for (int y = 0; y < world.getHeight(); y++) {
        for (int x = 0; x < world.getWidth(); x++)
            drawTile(x, y, world.getTile(x, y));
    }
    for (auto &pair : world.getEggs())
        drawEgg(pair.second);
    for (auto &pair : world.getPlayers())
        drawPlayer(pair.second);
    _window.display();
}
