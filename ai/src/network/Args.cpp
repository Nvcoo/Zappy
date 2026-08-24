/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#include "../../include/network/Args.hpp"
#include <iostream>
#include <stdexcept>

static void printUsage(const char *prog)
{
    std::cerr << "USAGE: " << prog << " -p port -n name [-h machine]" << std::endl;
}

static void checkArgs(const Args &args)
{
    if (args.port <= 0)
        throw std::invalid_argument("missing or invalid port");
    if (args.name.empty())
        throw std::invalid_argument("missing team name");
    if (args.name == "GRAPHIC")
        throw std::invalid_argument("'GRAPHIC' can't be used as a team name");
}

Args parseArgs(int ac, char **av)
{
    Args args;

    try {
        for (int i = 1; i < ac; i++) {
            std::string flag = av[i];

            if (flag == "--help") {
                printUsage(av[0]);
                std::exit(0);
            }
            if (flag == "-p") {
                if (i + 1 >= ac)
                    throw std::invalid_argument("missing port");
                args.port = std::stoi(av[++i]);
                continue;
            }
            if (flag == "-n") {
                if (i + 1 >= ac)
                    throw std::invalid_argument("missing name");
                args.name = av[++i];
                continue;
            }
            if (flag == "-h") {
                if (i + 1 >= ac)
                    throw std::invalid_argument("missing host");
                args.host = av[++i];
                continue;
            }
            throw std::invalid_argument("unknown flag: " + flag);
        }
        checkArgs(args);
    } catch (const std::exception &e) {
        printUsage(av[0]);
        std::cerr << "Error: " << e.what() << std::endl;
        std::exit(84);
    }
    return args;
}
