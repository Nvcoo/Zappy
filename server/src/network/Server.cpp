/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Server
*/

#include "../../include/network/Server.hpp"
#include "../../include/game/Player.hpp"
#include "../../include/command/CommandHandler.hpp"
#include <cmath>
#include <cstddef>
#include <memory>
#include <string>
#include <sys/poll.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <utility>
#include <cstdlib>

Server::Server(const Args &args) : _listenFd(-1), _args(args), _map(args.width, args.height), _clock(args.freq)
{
    for (const auto &name : args.teamNames)
        _teams.push_back(Team(name, args.clientsNb));
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

void Server::acceptNewClient()
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientFd = accept(_listenFd, (struct sockaddr *)&clientAddr, &clientAddrLen);

    if (clientFd < 0) {
        std::cerr << "the accept failed" << std::endl;
        return;
    }

    struct pollfd pfd;
    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    _pollFds.push_back(pfd);
    _clients.push_back(std::make_shared<Client>(clientFd, PLAYER));
    _clients.back()->sendMessage("WELCOME\n");
    std::cout << "New client connected on fd " << clientFd << std::endl;
}

void Server::handleClientData(size_t index)
{
    auto &client = _clients[index - 1];
    char buffer[1024];
    ssize_t bytesRead = read(client->getFd(), buffer, sizeof(buffer) - 1);

    if (bytesRead <= 0) {
        removeClient(index);
        return;
    }
    buffer[bytesRead] = '\0';
    client->appendToBuffer(std::string(buffer, bytesRead));
    if (client->hasOverflow()) {
        std::cerr << "Disconnecting client on fd " << client->getFd() << " due to buffer overflow" << std::endl;
        removeClient(index);
        return;
    }
    while (client->hasLine()) {
        std::string line = client->popLine();
        processLine(_clients[index - 1], line);
    }
}

void Server::removeClient(size_t index)
{
    std::cout << "Client on fd " << _pollFds[index].fd << " has been disconected" << std::endl;
    close(_pollFds[index].fd);
    _pollFds.erase(_pollFds.begin() + index);
    _clients.erase(_clients.begin() + (index - 1));
}

int Server::findClientIndex(int fd)
{
    for (size_t i = 0; i < _clients.size(); i++) {
        if (_clients[i]->getFd() == fd)
            return static_cast<int>(i);
    }
    return -1;
}

void Server::handleTeamName(Client &client, const std::string &teamName)
{
    Team *team = findTeam(teamName);
    if (team == nullptr || team->getAvailableSlots() <= 0) {
        client.sendMessage("ko\n");
        return;
    }

    int spawnX = std::rand() % _map.getWidth();
    int spawnY = std::rand() % _map.getHeight();
    if (team->hasEgg()) {
        Egg egg = team->popEgg();
        spawnX = egg.x;
        spawnY = egg.y;
    }

    auto new_player = std::make_shared<Player>(client.getFd(), teamName, spawnX, spawnY);
    team->addClient();

    int index = findClientIndex(client.getFd());
    _clients[index] = std::move(new_player);

    std::string response = std::to_string(team->getAvailableSlots()) + "\n";
    response += std::to_string(_map.getWidth()) + " " + std::to_string(_map.getHeight()) + "\n";
    _clients[index]->sendMessage(response);
}

void Server::processLine(std::shared_ptr<Client> client, const std::string &line)
{
    auto player = std::dynamic_pointer_cast<Player>(client);

    if (player == nullptr) {
        handleTeamName(*client, line);
        return;
    }

    Command cmd = parseCommand(line);
    if (!player->pushCommand(cmd))
        std::cout << "Command queue full for player on fd " << player->getFd() << std::endl;
}

Team *Server::findTeam(const std::string &name)
{
    for (auto &team : _teams) {
        if (team.getName() == name)
            return &team;
    }
    return nullptr;
}

void Server::updateGame()
{
    if (_clock.respawn()) {
        _map.spawnResources();
        _clock.resetSpawn();
        std::cout << "Resources respawned" << std::endl;
    }

    int elapsedTicks = _clock.elapsedTicks();
    if (elapsedTicks <= 0)
        return;

    for (size_t i = 0; i < _clients.size(); i++) {
        auto player = std::dynamic_pointer_cast<Player>(_clients[i]);
        if (player == nullptr)
            continue;
        player->decrementLife(elapsedTicks);
        if (player->isDead()) {
            std::cout << "Player on fd " << player->getFd() << " has died" << std::endl;
            player->sendMessage("dead\n");
            removeClient(i + 1);
            i--;
            continue;
        }
        processPlayerCommands(*player, _clock, _map, _teams, _clients);
    }
}
