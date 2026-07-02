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
#include "../world/Map.hpp"
#include "../world/Team.hpp"
#include "../game/Clock.hpp"
#include <cstddef>
#include <memory>
#include <vector>
#include <poll.h>

class Server {
    private:
        int _listenFd;
        Args _args;
        std::vector<struct pollfd> _pollFds;
        std::vector<std::shared_ptr<Client>> _clients;
        void acceptNewClient();
        void handleClientData(size_t index);
        void removeClient(size_t index);
        void processLine(std::shared_ptr<Client> client, const std::string &line);
        void handleTeamName(Client &client, const std::string &teamName);
        int findClientIndex(int fd);

        Map _map;
        std::vector<Team> _teams;
        Clock _clock;
        Team *findTeam(const std::string &name);
        void updateGame();
    protected:
    public:
        Server(const Args &args);
        ~Server();
        void run();
};

#endif
