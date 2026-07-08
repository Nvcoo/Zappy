/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Actions
*/

#include "../../include/command/Actions.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

void rotateLeft(Player &player)
{
    switch (player.getOrientation()) {
        case NORTH:
            player.setOrientation(WEST);
            break;
        case WEST:
            player.setOrientation(SOUTH);
            break;
        case SOUTH:
            player.setOrientation(EAST);
            break;
        case EAST:
            player.setOrientation(NORTH);
            break;
    }
}

void rotateRight(Player &player)
{
    switch (player.getOrientation()) {
        case NORTH:
            player.setOrientation(EAST);
            break;
        case WEST:
            player.setOrientation(NORTH);
            break;
        case SOUTH:
            player.setOrientation(WEST);
            break;
        case EAST:
            player.setOrientation(SOUTH);
            break;
    }
}

void moveForward(Player &player, Map &map)
{
    int x = player.getX();
    int y = player.getY();

    switch (player.getOrientation()) {
        case NORTH:
            y -= 1;
            break;
        case SOUTH:
            y += 1;
            break;
        case EAST:
            x += 1;
            break;
        case WEST:
            x -= 1;
            break;
    }
    x = ((x % map.getWidth()) + map.getWidth()) % map.getWidth();
    y = ((y % map.getHeight()) + map.getHeight()) % map.getHeight();
    player.setPos(x, y);
}

std::string executeInventory(Player &player)
{
    std::string response = "[";
    response += "food " + std::to_string(player.getInventory(FOOD));
    response += ", linemate " + std::to_string(player.getInventory(LINEMATE));
    response += ", deraumere " + std::to_string(player.getInventory(DERAUMERE));
    response += ", sibur " + std::to_string(player.getInventory(SIBUR));
    response += ", mendiane " + std::to_string(player.getInventory(MENDIANE));
    response += ", phiras " + std::to_string(player.getInventory(PHIRAS));
    response += ", thystame " + std::to_string(player.getInventory(THYSTAME));
    response += "]\n";
    return response;
}

std::string executeTake(Player &player, Map &map, const std::string &arg)
{
    Resource r = nameToResource(arg);

    if (r == RESOURCE_COUNT)
        return "ko\n";

    Tile &tile = map.getTile(player.getX(), player.getY());
    if (tile.getResource(r) <= 0)
        return "ko\n";
    tile.removeResource(r, 1);
    player.addToInv(r, 1);
    return "ok\n";
}

std::string executeSet(Player &player, Map &map, const std::string &arg)
{
    Resource r = nameToResource(arg);

    if (r == RESOURCE_COUNT)
        return "ko\n";
    if (player.getInventory(r) <= 0)
        return "ko\n";
    player.removeFromInv(r, 1);
    Tile &tile = map.getTile(player.getX(), player.getY());
    tile.addResource(r, 1);
    return "ok\n";
}

std::string executeConnectNbr(Player &player, std::vector<Team> &teams)
{
    for (auto &team: teams) {
        if (team.getName() == player.getTeamName())
            return std::to_string(team.getAvailableSlots()) + "\n";
    }
    return "0\n";
}

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

static std::string TileContents(Tile &tile, int x, int y, std::vector<std::shared_ptr<Client>> &clients)
{
    std::string contents;

    for (auto &client : clients) {
        auto player = std::dynamic_pointer_cast<Player>(client);
        if (player == nullptr)
            continue;
        if (player->getX() == x && player->getY() == y) {
            contents += "player ";
        }
    }
    for (int r = 0; r < RESOURCE_COUNT; r++) {
        int amount = tile.getResource(static_cast<Resource>(r));
        for (int j = 0; j < amount; j++)
            contents += resourceToName(static_cast<Resource>(r)) + " ";
    }
    //we have to remove trailing spaces as per the instructions
    if (!contents.empty())
        contents.pop_back();
    return contents;
}

std::string executeLook(Player &player, Map &map, std::vector<std::shared_ptr<Client>> &clients)
{
    int px = player.getX();
    int py = player.getY();
    int lvl = player.getLevel();
    int w = map.getWidth();
    int h = map.getHeight();
    orientation_t orient = player.getOrientation();
    std::vector<std::pair<int, int>> tilesToLook;

    tilesToLook.push_back({px, py});
    for (int i = 1; i <= lvl; i++) {
        for (int j = 1; j <= i; j++) {
            int tx;
            int ty;
            switch (orient) {
                case NORTH:
                    tx = ((px + j) % w + w) % w;
                    ty = ((py - i) % h + h) % h;
                    break;
                case EAST:
                    tx = ((px + i) % w + w) % w;
                    ty = ((py + j) % h + h) % h;
                    break;
                case SOUTH:
                    tx = ((px - j) % w + w) % w;
                    ty = ((py + i) % h + h) % h;
                    break;
                case WEST:
                    tx = ((px - i) % w + w) % w;
                    ty = ((py - j) % h + h) % h;
                    break;
                default:
                    tx = px;
                    ty = py;
                    break;
            }
            tilesToLook.push_back({tx, ty});
        }
    }

    std::string response = "[";

    for (size_t i = 0; i < tilesToLook.size(); i++) {
        int x = tilesToLook[i].first;
        int y = tilesToLook[i].second;
        response += TileContents(map.getTile(x, y), x, y, clients);
        if (i != tilesToLook.size() - 1)
            response += ", ";
    }
    response += "]\n";
    return response;
}

std::string resourceToName(Resource r)
{
    switch (r) {
        case FOOD: return "food";
        case LINEMATE: return "linemate";
        case DERAUMERE: return "deraumere";
        case SIBUR: return "sibur";
        case MENDIANE: return "mendiane";
        case PHIRAS: return "phiras";
        case THYSTAME: return "thystame";
        default: return "";
    }
}

Resource nameToResource(const std::string &name)
{
    if (name == "food")
        return FOOD;
    if (name == "linemate")
        return LINEMATE;
    if (name == "deraumere")
        return DERAUMERE;
    if (name == "sibur")
        return SIBUR;
    if (name == "mendiane")
        return MENDIANE;
    if (name == "phiras")
        return PHIRAS;
    if (name == "thystame")
        return THYSTAME;
    return RESOURCE_COUNT; //we'll return this if it's invalid
}
