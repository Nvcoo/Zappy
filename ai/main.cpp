/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** main
*/

#include <iostream>
#include "include/network/Args.hpp"
#include "include/network/TcpClient.hpp"
#include "include/ai/Brain.hpp"

//Makes sure that our information is correct throughout the game
static bool readResponse(TcpClient &client, std::string &line, Brain &brain, bool expectIncantation = false)
{
    while (true) {
        if (!client.receiveLine(line))
            return false;
        if (line.rfind("message ", 0) == 0)
            continue;
        if (line.rfind("eject: ", 0) == 0)
            continue;
        if (line.rfind("seg ", 0) == 0)
            continue;
        if (line.rfind("Current level: ", 0) == 0) {
            if (expectIncantation)
                return true;
            brain.applyLevel(std::stoi(line.substr(15)));
            std::cerr << "swept into another player's incantation, now level " << brain.getLevel() << std::endl;
            continue;
        }
        if (line == "Elevation underway") {
            if (expectIncantation)
                return true;
            continue;
        }
        if (line == "dead")
            return false;
        return true;
    }
}

static void doIncantation(TcpClient &client, Brain &brain, bool &alive)
{
    std::string line;

    std::cerr << "starting incantation attempt (level " << brain.getLevel()
        << " -> " << brain.getLevel() + 1 << ")" << std::endl;

    for (int r = LINEMATE; r <= THYSTAME; r++) {
        Resource res = static_cast<Resource>(r);
        int need = brain.requiredAmount(res);

        for (int n = 0; n < need; n++) {
            client.sendMessage("Set " + resourceToName(res) + "\n");
            if (!readResponse(client, line, brain)) {
                alive = false;
                return;
            }
        }
    }

    client.sendMessage("Incantation\n");
    if (!readResponse(client, line, brain, true)) {
        alive = false;
        return;
    }
    if (line == "Elevation underway") {
        std::cerr << "incantation underway, waiting for result..." << std::endl;
        if (!readResponse(client, line, brain, true)) {
            alive = false;
            return;
        }
        if (line.rfind("Current level: ", 0) == 0) {
            brain.applyLevel(std::stoi(line.substr(15)));
            std::cerr << "LEVEL UP! now level " << brain.getLevel() << std::endl;
        } else {
            std::cerr << "incantation failed (response: " << line << ")" << std::endl;
        }
    } else {
        std::cerr << "incantation refused (response: " << line << ")" << std::endl;
    }
}

//number of players a team needs for the win condition (subject: "at least 6 players reach the maximum elevation")
static const int TARGET_TEAM_SIZE = 6;

static void autoFork(TcpClient &client, Brain &brain, int availableSlots, bool &alive)
{
    int neededForks = TARGET_TEAM_SIZE - 1 - availableSlots;
    std::string line;

    if (neededForks <= 0)
        return;
    std::cerr << "auto-forking " << neededForks << " time(s) to fill the team" << std::endl;
    for (int i = 0; i < neededForks; i++) {
        client.sendMessage("Fork\n");
        if (!readResponse(client, line, brain)) {
            alive = false;
            return;
        }
    }
}

int main(int ac, char **av)
{
    Args args = parseArgs(ac, av);
    TcpClient client;

    if (!client.connectTo(args.host, args.port)) {
        std::cerr << "Failed to connect to " << args.host << ":" << args.port << std::endl;
        return 84;
    }

    std::string line;

    if (!client.receiveLine(line))
        return 84;
    client.sendMessage(args.name + "\n");
    if (!client.receiveLine(line))
        return 84;

    int availableSlots = std::stoi(line);

    if (!client.receiveLine(line))
        return 84;

    Brain brain;
    bool alive = true;

    autoFork(client, brain, availableSlots, alive);

    while (alive) {
        if (brain.hasPlan()) {
            std::string cmd = brain.popPlan();

            client.sendMessage(cmd + "\n");
            if (!readResponse(client, line, brain))
                break;
            continue;
        }

        client.sendMessage("Inventory\n");
        if (!readResponse(client, line, brain))
            break;
        brain.applyInventory(line);
        //std::cerr << "food level: " << line << std::endl;

        client.sendMessage("Look\n");
        if (!readResponse(client, line, brain))
            break;

        if (brain.readyForIncantation(line)) {
            doIncantation(client, brain, alive);
            continue;
        }

        std::string cmd = brain.nextCommand(line);

        client.sendMessage(cmd + "\n");
        if (!readResponse(client, line, brain))
            break;
        if (cmd == "Take food" && line == "ok")
            std::cerr << "food collected" << std::endl;
    }
    std::cerr << "dead" << std::endl;
    return 0;
}
