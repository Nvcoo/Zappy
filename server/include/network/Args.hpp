/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#ifndef ARGS_HPP_
    #define ARGS_HPP_

#include <string>
#include <vector>

struct Args {
    int port;
    int width;
    int height;
    std::vector<std::string> teamNames;
    int clientsNb;
    int freq;
};

Args parseArgs(int ac, char **av);

#endif
