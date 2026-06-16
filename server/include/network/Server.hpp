/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Server
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_

#include "Args.hpp"
#include "Client.hpp"
#include <cstddef>
#include <memory>
#include <vector>
#include <poll.h>

namespace network {

class Server {
    private:
        int _listenFd;
        Args _args;
        std::vector<struct pollfd> _pollFds;
        std::vector<std::unique_ptr<Client>> _clients;

        void acceptNewClient();
        void handleClientData(size_t index);
        void removeClient(size_t index);
        void processLine(Client &client, const std::string &line);
    protected:
    public:
        Server(const Args &args);
        ~Server();
        void run();
};

}

#endif
