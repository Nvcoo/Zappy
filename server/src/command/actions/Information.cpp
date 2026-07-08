/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Information
*/

#include "../../../include/command/Actions.hpp"

std::string executeConnectNbr(Player &player, std::vector<Team> &teams)
{
    for (auto &team: teams) {
        if (team.getName() == player.getTeamName())
            return std::to_string(team.getAvailableSlots()) + "\n";
    }
    return "0\n";
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
