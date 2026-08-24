/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiRequests
*/

#include "../../../include/network/GuiClient.hpp"
#include <memory>
#include <string>
#include <vector>

static int parseId(const std::string &s)
{
    if (s.empty())
        return -1;
    if (s[0] == '#')
        return std::stoi(s.substr(1));
    return std::stoi(s);
}

static std::shared_ptr<Player> findPlayer(std::vector<std::shared_ptr<Client>> &clients, int id)
{
    for (auto &client : clients) {
        auto p = std::dynamic_pointer_cast<Player>(client);
        if (p && p->getId() == id)
            return p;
    }
    return nullptr;
}

void GuiClient::msz(Map &map)
{
    sendMessage("msz " + std::to_string(map.getWidth()) + " " + std::to_string(map.getHeight()) + "\n");
}

void GuiClient::mct(Map &map)
{
    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++)
            sendMessage(buildTileContent(x, y, map));
    }
}

void GuiClient::bct(Map &map, std::vector<std::string> &args)
{
    if (args.size() < 2) { sendMessage("suc\n"); return; }
    int x = std::stoi(args[0]);
    int y = std::stoi(args[1]);
    if (x < 0 || x >= map.getWidth() || y < 0 || y >= map.getHeight()) {
        sendMessage("suc\n");
        return;
    }
    sendMessage(buildTileContent(x, y, map));
}

void GuiClient::tna(std::vector<Team> &teams)
{
    for (auto &team : teams)
        sendMessage("tna " + team.getName() + "\n");
}

void GuiClient::sgt(int freq)
{
    sendMessage("sgt " + std::to_string(freq) + "\n");
}

void GuiClient::sst(std::vector<std::string> &args, int &freq)
{
    if (args.empty()) {
        sendMessage("suc\n");
        return;
    }

    int newFreq = std::stoi(args[0]);
    if (newFreq < 1) {
        sendMessage("suc\n");
        return;
    }

    freq = newFreq;
    sendMessage("sst " + std::to_string(freq) + "\n");
}

void GuiClient::ppo(std::vector<std::shared_ptr<Client>> &clients, std::vector<std::string> &args)
{
    if (args.empty()) {
        sendMessage("suc\n");
        return;
    }

    auto p = findPlayer(clients, parseId(args[0]));
    if (!p) {
        sendMessage("suc\n");
        return;
    }
    ppo(p);
}

void GuiClient::plv(std::vector<std::shared_ptr<Client>> &clients, std::vector<std::string> &args)
{
    if (args.empty()) {
        sendMessage("suc\n");
        return;
    }

    auto p = findPlayer(clients, parseId(args[0]));
    if (!p) {
        sendMessage("suc\n");
        return;
    }
    plv(p);
}

void GuiClient::pin(std::vector<std::shared_ptr<Client>> &clients, std::vector<std::string> &args)
{
    if (args.empty()) {
        sendMessage("suc\n");
        return;
    }

    auto p = findPlayer(clients, parseId(args[0]));
    if (!p) {
        sendMessage("suc\n");
        return;
    }
    pin(p);
}