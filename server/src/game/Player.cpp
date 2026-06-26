/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#include "../../include/game/Player.hpp"

namespace game {

void Player::addToInv(world::Resource r, int amount)
{
    _inventory[r] += amount;
}

void Player::removeFromInv(world::Resource r, int amount)
{
    _inventory[r] -= amount;
}

void Player::decrementLife(int amount)
{
    _lifeTimer -= amount;
    if (_lifeTimer < 0)
        _lifeTimer = 0;
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

}
