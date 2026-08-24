/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#ifndef ARGS_HPP_
    #define ARGS_HPP_

#include <string>

struct Args {
    int port = 0;
    std::string name;
    std::string host = "localhost";
};

Args parseArgs(int ac, char **av);

#endif
