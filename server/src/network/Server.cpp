/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Server
*/

#include "../../include/network/Server.hpp"
#include "../../include/game/Player.hpp"
#include "../../include/command/CommandHandler.hpp"
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>

Server::Server(const Args &args) : _listenFd(-1), _args(args), _map(args.width, args.height), _clock(args.freq)
{
    for (const auto &name : args.teamNames)
        _teams.push_back(Team(name, args.clientsNb));
    for (auto &team : _teams) {
        for (int i = 0; i < args.clientsNb; i++) {
            int x = std::rand() % args.width;
            int y = std::rand() % args.height;
            team.addEgg(x, y);
        }
    }
    _listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenFd < 0)
        throw std::runtime_error("Failed to create socket");

    int opt = 1;
    setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(_args.port);
    if (bind(_listenFd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Failed to bind socket");
    if (listen(_listenFd, 10) < 0)
        throw std::runtime_error("Failed to listen to socket");

    struct pollfd pfd;
    pfd.fd = _listenFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    _pollFds.push_back(pfd);
    std::cout << "Server listening on port " << _args.port << std::endl;
}

Server::~Server()
{
    for (auto &pfd : _pollFds)
        close(pfd.fd);
}

//we iterate the other way around cuz
//if 2 or more clients disconnect during the same poll cycle
//we will get a crash cuz the second removal operates in a shifted vector.
void Server::run()
{
    while (true) {
        int ready = poll(_pollFds.data(), _pollFds.size(), 100);

        if (ready < 0)
            throw std::runtime_error("poll failed");
        if (ready > 0) {
            if (_pollFds[0].revents & POLLIN)
                acceptNewClient();
            for (size_t i = _pollFds.size() - 1; i >= 1; i--) {
                if (_pollFds[i].revents & POLLIN)
                    handleClientData(i); //here
            }
        }
        updateGame();
    }
}

void Server::processLine(std::shared_ptr<Client> client, const std::string &line)
{
    auto gui = std::dynamic_pointer_cast<GuiClient>(client);
    if (gui != nullptr) {
        gui->parseCommand(line, _map, _teams, _clients, _args.freq);
        return;
    }

    auto player = std::dynamic_pointer_cast<Player>(client);
    if (player == nullptr) {
        handleTeamName(*client, line);
        return;
    }

    Command cmd = parseCommand(line);
    if (!player->pushCommand(cmd))
        std::cout << "Command queue full for player on fd " << player->getFd() << std::endl;
}

void Server::notifyGui(std::function<void(GuiClient &)> fn)
{
    //for (auto &client : _clients) {
    //    auto gui = std::dynamic_pointer_cast<GuiClient>(client);
    //    if (gui != nullptr)
    //        fn(*gui);
    //}
    broadcastGui(_clients, fn);
}
