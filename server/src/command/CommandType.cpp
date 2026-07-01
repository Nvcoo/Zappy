/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandType
*/

#include "../../include/command/CommandType.hpp"

namespace command {

CommandType nameToType(const std::string &name)
{
    if (name == "Forward")
        return FORWARD;
    if (name == "Left")
        return LEFT;
    if (name == "Right")
        return RIGHT;
    if (name == "Look")
        return LOOK;
    if (name == "Inventory")
        return INVENTORY;
    if (name == "Broadcast")
        return BROADCAST;
    if (name == "Connect_nbr")
        return CONNECT_NBR;
    if (name == "Fork")
        return FORK;
    if (name == "Eject")
        return EJECT;
    if (name == "Take")
        return TAKE;
    if (name == "Set")
        return SET;
    if (name == "Incantation")
        return INCANTATION;
    return UNKNOWN;
}

}
