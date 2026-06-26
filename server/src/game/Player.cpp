/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#include "../../include/game/Player.hpp"

namespace game {

int Player::getX() const
{
    return _x;
}

int Player::getY() const
{
    return _y;
}

orientation_t Player::getOrientation() const
{
    return _orientation;
}

int Player::getLevel() const
{
    return _level;
}

int Player::getLifeTimer() const
{
    return _lifeTimer;
}

const std::string &Player::getTeamName() const
{
    return _teamName;
}

bool Player::isBusy() const
{
    return _busy;
}

int Player::getInventory(world::Resource r) const
{
    return _inventory[r];
}

void Player::setPos(int x, int y)
{
    _x = x;
    _y = y;
}

void Player::setOrientation(orientation_t o)
{
    _orientation = o;
}

void Player::setLevel(int level)
{
    _level = level;
}

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

bool Player::isDead() const
{
    return _lifeTimer <= 0;
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

void Player::setBusy(bool busy)
{
    _busy = busy;
}

}
