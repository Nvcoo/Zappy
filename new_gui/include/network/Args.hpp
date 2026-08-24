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
    std::string host = "127.0.0.1";
};

Args parseArgs(int ac, char **av);

#endif
