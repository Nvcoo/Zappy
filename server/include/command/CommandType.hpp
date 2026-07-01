/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandType
*/

#ifndef COMMANDTYPE_HPP_
    #define COMMANDTYPE_HPP_

#include <string>

namespace command {

enum CommandType {
    FORWARD,
    LEFT,
    RIGHT,
    LOOK,
    INVENTORY,
    BROADCAST,
    CONNECT_NBR,
    FORK,
    EJECT,
    TAKE,
    SET,
    INCANTATION,
    UNKNOWN
};

CommandType nameToType(const std::string &name);

}

#endif
