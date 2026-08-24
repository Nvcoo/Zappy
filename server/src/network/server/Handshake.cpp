/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Handshake
*/

#include "../../../include/network/Server.hpp"
#include "../../../include/game/Player.hpp"

void Server::handleTeamName(Client &client, const std::string &teamName)
{
    if (teamName == "GRAPHIC") {
        int index = findClientIndex(client.getFd());
        auto gui = std::make_shared<GuiClient>(client.getFd());
        _clients[index] = gui;
        gui->setInitialState(_map, _teams, _clients, _args.freq);
        return;
    }

    Team *team = findTeam(teamName);
    if (team == nullptr || team->getAvailableSlots() <= 0) {
        client.sendMessage("ko\n");
        return;
    }

    Egg egg = team->popEgg();
    team->addClient();

    int index = findClientIndex(client.getFd());
    _clients[index] = std::make_shared<Player>(client.getFd(), teamName, egg.x, egg.y);

    auto newPlayer = std::dynamic_pointer_cast<Player>(_clients[index]);
    notifyGui([&newPlayer](GuiClient &gui) {
        gui.pnw(newPlayer);
    });

    std::string response = std::to_string(team->getAvailableSlots()) + "\n";
    response += std::to_string(_map.getWidth()) + " " + std::to_string(_map.getHeight()) + "\n";
    _clients[index]->sendMessage(response);
}

int Server::findClientIndex(int fd)
{
    for (size_t i = 0; i < _clients.size(); i++) {
        if (_clients[i]->getFd() == fd)
            return static_cast<int>(i);
    }
    return -1;
}

Team *Server::findTeam(const std::string &name)
{
    for (auto &team : _teams) {
        if (team.getName() == name)
            return &team;
    }
    return nullptr;
}
