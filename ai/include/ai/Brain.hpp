/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Brain
*/

#ifndef BRAIN_HPP_
    #define BRAIN_HPP_

#include <string>
#include <deque>

enum Resource {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    RESOURCE_COUNT
};

Resource nameToResource(const std::string &name);
std::string resourceToName(Resource r);

class Brain {
    private:
        int _level;
        int _inventory[RESOURCE_COUNT];
        bool _seekingFood;
        int _wanderTurns; //chilling counter; helps with "randomness"
        std::deque<std::string> _plan;
        Resource neededResource() const;
        void buildPlan(int forward, int side, const std::string &target);
    protected:
    public:
        Brain();
        int getLevel() const
        {
            return _level;
        }
        void applyLevel(int level)
        {
            _level = level;
        }
        void applyInventory(const std::string &line);
        bool readyForIncantation(const std::string &lookLine) const;
        int requiredAmount(Resource r) const;
        std::string nextCommand(const std::string &lookLine);
        bool hasPlan() const
        {
            return !_plan.empty();
        }
        std::string popPlan() //consumes the plan
        {
            std::string cmd = _plan.front();
            _plan.pop_front();
            return cmd;
        }
};

#endif
