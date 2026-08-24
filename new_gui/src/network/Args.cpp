/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#include "../../include/network/Args.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

static void printUsage(const char *prog)
{
    std::cerr << "USAGE: " << prog << " -p port [-h machine]" << std::endl;
}

static void checkArgs(const Args &args)
{
    if (args.port <= 0)
        throw std::invalid_argument("missing or invalid port");

}

Args parseArgs(int ac, char **av)
{
    Args args;

    try {
        for (int i = 1; i < ac; i++) {
            std::string flag = av[i];
            if (flag == "--help") {
                printUsage(av[0]);
                exit(0);
            }
            if (flag == "-p") {
                if (i + 1 >= ac)
                    throw std::invalid_argument("missing port");
                args.port = std::stoi(av[++i]);
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
        exit(84);
    }
    return args;
}
