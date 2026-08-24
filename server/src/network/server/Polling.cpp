/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Polling
*/

#include "../../../include/network/Server.hpp"
#include "../../../include/game/Player.hpp"
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>

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
        auto player = std::dynamic_pointer_cast<Player>(client);
        if (player != nullptr) {
            notifyGui([&player](GuiClient &gui) {
                gui.pdi(player);
            });
        }
        removeClient(index);
        return;
    }
    buffer[bytesRead] = '\0';
    client->appendToBuffer(std::string(buffer, bytesRead));
    if (client->hasOverflow()) {
        std::cerr << "Disconnecting client on fd " << client->getFd() << " due to buffer overflow" << std::endl;
        auto player = std::dynamic_pointer_cast<Player>(client);
        if (player != nullptr) {
            notifyGui([&player](GuiClient &gui) {
                gui.pdi(player);
            });
        }
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
