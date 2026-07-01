/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Team
*/

#ifndef TEAM_HPP_
    #define TEAM_HPP_

#include <string>
#include <vector>

namespace world {

struct Egg {
    int id;
    int x;
    int y;
};

class Team {
    private:
        std::string _name;
        int _maxClients;
        int _connectedClients;
        std::vector<Egg> _eggs;
        static int _nextEggId; //it needs to be static to prevent 2 teams's eggs having the same ID
    protected:
    public:
        Team(const std::string &name, int maxClients) : _name(name), _maxClients(maxClients), _connectedClients(0) {};
        const std::string &getName() const;
        int getMaxClients() const;
        int getConnectedClients() const;
        int getAvailableSlots() const;
        void addEgg(int x, int y);
        bool hasEgg();
        Egg popEgg();
        void addClient();
        void removeClient();
};

}

#endif
