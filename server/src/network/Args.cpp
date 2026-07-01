/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#include <cstdlib>
#include <exception>
#include <iostream>
#include "../../include/network/Args.hpp"

namespace network {

static void printUsage()
{
    std::cerr << "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq" << std::endl;
    std::exit(84);
}

static int toPositiveInt(const std::string &value)
{
    try {
        int result = std::stoi(value);
        if (result <= 0)
            printUsage();
        return result;
    } catch (const std::exception &) {
        printUsage();
    }
    return 0;
}

Args parseArgs(int ac, char **av)
{
    Args args;

    args.port = 0;
    args.width = 0;
    args.height = 0;
    args.clientsNb = 0;
    args.freq = 100;

    for (int i = 1; i < ac; i++) {
        std::string arg = av[i];
        if (arg.size() != 2 || arg[0] != '-' || i + 1 >= ac)
            printUsage();
        switch (arg[1]) {
            case 'p':
                args.port = toPositiveInt(av[++i]);
                break;
            case 'x':
                args.width = toPositiveInt(av[++i]);
                break;
            case 'y':
                args.height = toPositiveInt(av[++i]);
                break;
            case 'c':
                args.clientsNb = toPositiveInt(av[++i]);
                break;
            case 'f':
                args.freq = toPositiveInt(av[++i]);
                break;
            case 'n':
                i++;
                while (i < ac && av[i][0] != '-') {
                    args.teamNames.push_back(av[i]);
                    i++;
                }
                i--;
                break;
            default:
                printUsage();
        }
    }
    if (args.port == 0 || args.width == 0 || args.height == 0 || args.clientsNb == 0 || args.teamNames.empty())
        printUsage();
    return args;
}

}
