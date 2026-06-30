/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Clock
*/

#include "../../include/game/Clock.hpp"

namespace game {

int Clock::now() const
{
    auto current = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current.time_since_epoch());

    return static_cast<int>(ms.count());
}

bool Clock::respawn()
{
    auto current = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current - _lastSpawn);
    int respawnInterval = milliseconds(20);

    return elapsed.count() >= respawnInterval;
}

}
