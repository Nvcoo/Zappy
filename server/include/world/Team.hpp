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
        static int _nextEggId;
    protected:
    public:
        Team(const std::string &name, int maxClients);
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
