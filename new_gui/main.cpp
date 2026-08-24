/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** main
*/

#include <iostream>
#include <unistd.h>
#include "include/network/Args.hpp"
#include "include/network/TcpClient.hpp"
#include "include/protocol/Parser.hpp"
#include "include/world/World.hpp"
#include "include/renderer/Renderer.hpp"

static bool waitForMapSize(TcpClient &client, Parser &parser, World &world)
{
    std::string line;

    while (world.getWidth() == 0) {
        if (!client.isConnected())
            return false;
        if (client.receiveLine(line))
            parser.parseLine(line, world);
        else
            usleep(1000); //we NEED this line otherwise your compute will slowly cook itself
    }
    return true;
}

int main(int ac, char **av)
{
    Args args = parseArgs(ac, av);
    TcpClient client;

    if (!client.connectTo(args.host, args.port)) {
        std::cerr << "Failed to connect to " << args.host << ":" << args.port << std::endl;
        return 84;
    }
    client.sendMessage("GRAPHIC\n");

    World world;
    Parser parser;

    if (!waitForMapSize(client, parser, world)) {
        std::cerr << "Server closed the connection before sending map data" << std::endl;
        return 84;
    }

    Renderer renderer(world.getWidth(), world.getHeight(), 40);
    std::string line;

    while (renderer.isOpen()) {
        while (client.receiveLine(line))
            parser.parseLine(line, world);
        if (!client.isConnected()) {
            std::cerr << "Lost connection to server" << std::endl;
            break;
        }
        renderer.handleEvents();
        renderer.draw(world);
    }
    return 0;
}
