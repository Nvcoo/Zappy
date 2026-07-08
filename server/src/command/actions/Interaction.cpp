/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Interaction
*/

#include "../../../include/command/Actions.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

static int calculateDir(int sendX, int sendY, int recX, int recY, orientation_t recOrientation, int width, int height)
{
    int dx = sendX - recX;
    int dy = sendY - recY;

    //wrapping adjustment
    if (2 * dx >  width)  dx -= width;
    if (2 * dx < -width)  dx += width;
    if (2 * dy >  height) dy -= height;
    if (2 * dy < -height) dy += height;
    if (dx == 0 && dy == 0)
        return 0;

    int lx;
    int ly;
    switch (recOrientation) {
        case NORTH: lx = dx; ly = -dy; break;
        case EAST: lx = dy; ly = dx; break;
        case SOUTH: lx = -dx; ly = dy; break;
        case WEST: lx = -dy; ly = -dx; break;
        default: lx = dx; ly = dy; break;
    }
    if (lx == 0 && ly > 0)
        return 1;
    if (lx < 0  && ly > 0)
        return 2;
    if (lx < 0  && ly == 0)
        return 3;
    if (lx < 0  && ly < 0)
        return 4;
    if (lx == 0 && ly < 0)
        return 5;
    if (lx > 0  && ly < 0)
        return 6;
    if (lx > 0  && ly == 0)
        return 7;
    return 8;
}

std::string executeBroadcast(Player &sender, const std::string &text, std::vector<std::shared_ptr<Client>> &clients, Map &map)
{
    for (auto &client : clients) {
        auto rec = std::dynamic_pointer_cast<Player>(client);
        if (rec == nullptr)
            continue;
        int k = calculateDir(sender.getX(), sender.getY(), rec->getX(), rec->getY(), rec->getOrientation(), map.getWidth(), map.getHeight());
        rec->sendMessage("message " + std::to_string(k) + ", " + text + "\n");
    }
    return "ok\n";
}
