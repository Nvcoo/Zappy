/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Team
*/

#include "../../include/world/Team.hpp"
#include <cstdlib>
#include <string>

namespace world {

int Team::_nextEggId = 0; //must be defined exactly here 

const std::string &Team::getName() const
{
    return _name;
}

int Team::getMaxClients() const
{
    return _maxClients;
}

int Team::getConnectedClients() const
{
    return _connectedClients;
}

int Team::getAvailableSlots() const
{
    return _maxClients - _connectedClients;
}

void Team::addEgg(int x, int y)
{
    Egg egg;

    egg.id = _nextEggId++;
    egg.x = x;
    egg.y = y;
    _eggs.push_back(egg);
}

bool Team::hasEgg()
{
    return !_eggs.empty();
}

Egg Team::popEgg()
{
    int index = std::rand() % _eggs.size();
    Egg egg = _eggs[index];
    _eggs.erase(_eggs.begin() + index);
    return egg;
}


void Team::addClient()
{
    _connectedClients++;
}

void Team::removeClient()
{
    if (_connectedClients > 0)
        _connectedClients--;
}

}
