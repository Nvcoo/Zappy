/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Clock
*/

#ifndef CLOCK_HPP_
    #define CLOCK_HPP_

#include <chrono>

namespace game {

class Clock {
    private:
        int _freq;
        std::chrono::steady_clock::time_point _lastSpawn; //apparently this clock is the best one for measuring elapsed time.
    protected:
    public:
        Clock(int freq) : _freq(freq)
        {
            _lastSpawn = std::chrono::steady_clock::now();
        }
        int milliseconds(int ticks) const
        {
            return (1000 * ticks) / _freq;
        }
        void resetSpawn()
        {
            _lastSpawn = std::chrono::steady_clock::now();
        }
        bool respawn();
        int now() const;
};

}

#endif
