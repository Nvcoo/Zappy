/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Interaction
*/

#include "../../../include/command/Actions.hpp"
#include <memory>
#include <string>
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

std::string executeFork(Player &player, std::vector<Team> &teams)
{
    for (auto &team: teams) {
        if (team.getName() != player.getTeamName())
            continue;
        team.addEgg(player.getX(), player.getY());
        return "ok\n";
    }
    return "ok\n";
}

std::string executeEject(Player &player, Map &map, std::vector<std::shared_ptr<Client>> &clients)
{
    std::vector<std::shared_ptr<Player>> eject;

    //Finding all players on the same tile as the player
    for (auto &client : clients) {
        auto p = std::dynamic_pointer_cast<Player>(client);
        if (p == nullptr) {
            continue;
        }
        if (p.get() == &player) {
            continue;
        }
        if (p->getX() == player.getX() && p->getY() == player.getY()) {
            eject.push_back(p);
        }
    }
    if (eject.empty()) {
        return "ok\n";
    }

    //Calculate the direction from which the ejected players came from
    int k = 0;
    switch (player.getOrientation()) {
        case NORTH: k = 5; break;
        case EAST: k = 7; break;
        case SOUTH: k = 1; break;
        case WEST: k = 3; break;
        default: k = 0; break;
    }
    int w = map.getWidth();
    int h = map.getHeight();

    for (auto &p : eject) {
        int newX = p->getX();
        int newY = p->getY();
        switch (player.getOrientation()) {
            case NORTH: newY = ((newY - 1) % h + h) % h; break;
            case EAST: newX = (newX + 1) % w; break;
            case SOUTH: newY = (newY + 1) % h; break;
            case WEST: newX = ((newX - 1) % w + w) % w; break;
        }
        p->setPos(newX, newY);
        p->sendMessage("eject: " + std::to_string(k) + "\n");
    }
    return "ok\n";
}
