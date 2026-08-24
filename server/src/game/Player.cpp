/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#include "../../include/game/Player.hpp"

int Player::_nextId = 0;

void Player::addToInv(Resource r, int amount)
{
    _inventory[r] += amount;
}

void Player::removeFromInv(Resource r, int amount)
{
    _inventory[r] -= amount;
    if (_inventory[r] < 0)
        _inventory[r] = 0;
}

int Player::decrementLife(int amount)
{
    _lifeTimer -= amount;

    int eaten = 0;
    while (_lifeTimer <= 0 && _inventory[FOOD] > 0) {
        _inventory[FOOD]--;
        _lifeTimer += 126;
        eaten++;
    }
    if (_lifeTimer < 0)
        _lifeTimer = 0;
    return eaten;
}

bool Player::pushCommand(const Command &cmd)
{
    if ((int)_commandQueue.size() >= 10)
        return false;
    _commandQueue.push(cmd);
    return true;
}

bool Player::hasCommand() const
{
    return !_commandQueue.empty();
}

Command &Player::frontCommand()
{
    return _commandQueue.front();
}

void Player::popCommand()
{
    if (!_commandQueue.empty())
        _commandQueue.pop();
}
