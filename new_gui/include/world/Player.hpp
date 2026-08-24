/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#ifndef PLAYER_HPP_
    #define PLAYER_HPP_

#include <string>

typedef enum orientation {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
} orientation_t;

class Player {
    private:
        int _id;
        int _x;
        int _y;
        orientation_t _orientation;
        int _level;
        std::string _team;
    protected:
    public:
        Player(int id, int x, int y, orientation_t o, int level, const std::string &team) : _id(id), _x(x), _y(y), _orientation(o), _level(level), _team(team) {}
        int getId() const
        {
            return _id;
        }
        int getX() const
        {
            return _x;
        }
        int getY() const
        {
            return _y;
        }
        orientation_t getOrientation() const
        {
            return _orientation;
        }
        int getLevel() const
        {
            return _level;
        }
        const std::string &getTeam() const
        {
            return _team;
        }
        void setPos(int x, int y, orientation_t o)
        {
            _x = x;
            _y = y;
            _orientation = o;
        }
        void setLevel(int level)
        {
            _level = level;
        }
};

#endif
