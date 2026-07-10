/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandHandler
*/

#include "../../include/command/CommandHandler.hpp"
#include "../../include/command/Actions.hpp"

Command parseCommand(const std::string &line)
{
    Command cmd;
    size_t spacePos = line.find(' ');

    if (spacePos == std::string::npos) {
        cmd.name = line;
        cmd.arg = "";
    } else {
        cmd.name = line.substr(0, spacePos);
        cmd.arg = line.substr(spacePos + 1);
    }
    cmd.executeAt = 0;
    return cmd;
}

int getCommandCost(CommandType type)
{
    switch (type) {
        case FORWARD:
            return 7;
        case LEFT:
            return 7;
        case RIGHT:
            return 7;
        case LOOK:
            return 7;
        case INVENTORY:
            return 1;
        case BROADCAST:
            return 7;
        case CONNECT_NBR:
            return 0;
        case FORK:
            return 42;
        case EJECT:
            return 7;
        case TAKE:
            return 7;
        case SET:
            return 7;
        case INCANTATION:
            return 300;
        default:
            return 7;
    }
}

std::string executeCommand(Player &player, const Command &cmd, Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients)
{
    switch (nameToType(cmd.name)) {
        case FORWARD:
            moveForward(player, map);
            return "ok\n";
        case LEFT:
            rotateLeft(player);
            return "ok\n";
        case RIGHT:
            rotateRight(player);
            return "ok\n";
        case INVENTORY:
            return executeInventory(player);
        case TAKE:
            return executeTake(player, map, cmd.arg);
        case SET:
            return executeSet(player, map, cmd.arg);
        case CONNECT_NBR:
            return executeConnectNbr(player, teams);
        case BROADCAST:
            return executeBroadcast(player, cmd.arg, clients, map);
        case LOOK:
            return executeLook(player, map, clients);
        case FORK:
            return executeFork(player, teams);
        case UNKNOWN:
            return "ko\n";
        default:
            return "ko\n";
    }
}

void processPlayerCommands(Player &player, Clock &clock, Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients)
{
    if (!player.hasCommand())
        return;

    Command &cmd = player.frontCommand();

    if (cmd.executeAt == 0) {
        int costTicks = getCommandCost(nameToType(cmd.name));
        cmd.executeAt = clock.now() + clock.milliseconds(costTicks);
        return;
    }
    if (clock.now() < cmd.executeAt)
        return;
    std::string response = executeCommand(player, cmd, map, teams, clients);
    player.sendMessage(response);
    player.popCommand();
}
