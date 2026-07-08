/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Utilities
*/

#include "../../../include/command/Actions.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

std::string resourceToName(Resource r)
{
    switch (r) {
        case FOOD: return "food";
        case LINEMATE: return "linemate";
        case DERAUMERE: return "deraumere";
        case SIBUR: return "sibur";
        case MENDIANE: return "mendiane";
        case PHIRAS: return "phiras";
        case THYSTAME: return "thystame";
        default: return "";
    }
}

Resource nameToResource(const std::string &name)
{
    if (name == "food")
        return FOOD;
    if (name == "linemate")
        return LINEMATE;
    if (name == "deraumere")
        return DERAUMERE;
    if (name == "sibur")
        return SIBUR;
    if (name == "mendiane")
        return MENDIANE;
    if (name == "phiras")
        return PHIRAS;
    if (name == "thystame")
        return THYSTAME;
    return RESOURCE_COUNT; //we'll return this if it's invalid
}
