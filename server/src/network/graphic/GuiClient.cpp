/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiClient
*/

#include "../../../include/network/GuiClient.hpp"
#include <sstream>

std::string GuiClient::buildTileContent(int x, int y, Map &map) const
{
    Tile &tile = map.getTile(x, y);
    std::string result = "bct " + std::to_string(x) + " " + std::to_string(y);

    result += " " + std::to_string(tile.getResource(FOOD));
    result += " " + std::to_string(tile.getResource(LINEMATE));
    result += " " + std::to_string(tile.getResource(DERAUMERE));
    result += " " + std::to_string(tile.getResource(SIBUR));
    result += " " + std::to_string(tile.getResource(MENDIANE));
    result += " " + std::to_string(tile.getResource(PHIRAS));
    result += " " + std::to_string(tile.getResource(THYSTAME));
    result += "\n";
    return result;
}

void GuiClient::setInitialState(Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients, int freq)
{
    msz(map);
    sgt(freq);
    mct(map);
    tna(teams);
    for (auto &client : clients) {
        auto player = std::dynamic_pointer_cast<Player>(client);
        if (player == nullptr)
            continue;
        pnw(player);
        pin(player);
        plv(player);
    }
    for (auto &team : teams) {
        for (auto &egg : team.getEggs())
            enw(egg.id, -1, egg.x, egg.y);
    }
}

void GuiClient::parseCommand(const std::string &line, Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients, int &freq)
{
    std::istringstream ss(line);
    std::string cmd;
    ss >> cmd;
    std::vector<std::string> args;
    std::string arg;

    while (ss >> arg)
        args.push_back(arg);
    auto it = GuiCommandMap.find(cmd);
    if (it == GuiCommandMap.end()) {
        sendMessage("suc\n");
        return;
    }
    switch (it->second) {
        case MSZ: msz(map); break;
        case MCT: mct(map); break;
        case BCT: bct(map, args); break;
        case TNA: tna(teams); break;
        case SGT: sgt(freq); break;
        case SST: sst(args, freq); break;
        case PPO: ppo(clients, args); break;
        case PLV: plv(clients, args); break;
        case PIN: pin(clients, args); break;
        default: sendMessage("suc\n"); break;
    }
}
