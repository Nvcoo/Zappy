/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** main
*/

#include "include/network/Args.hpp"
#include "include/network/Server.hpp"
#include <exception>
#include <iostream>

int main(int ac, char **av)
{
    try {
        network::Args args = network::parseArgs(ac, av);
        network::Server server(args);
        server.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
