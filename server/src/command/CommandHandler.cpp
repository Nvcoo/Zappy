/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandHandler
*/

#include "../../include/command/CommandHandler.hpp"
#include "../../include/command/Actions.hpp"

namespace command {

game::Command parseCommand(const std::string &line)
{
    game::Command cmd;
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

std::string executeCommand(game::Player &player, const game::Command &cmd, world::Map &map)
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
        case UNKNOWN:
            return "ko\n";
        default:
            return "ko\n";
    }
}

void processPlayerCommands(game::Player &player, game::Clock &clock, world::Map &map)
{
    if (!player.hasCommand())
        return;

    game::Command &cmd = player.frontCommand();

    if (cmd.executeAt == 0) {
        int costTicks = getCommandCost(nameToType(cmd.name));
        cmd.executeAt = clock.now() + clock.milliseconds(costTicks);
        return;
    }
    if (clock.now() < cmd.executeAt)
        return;
    std::string response = executeCommand(player, cmd, map);
    player.sendMessage(response);
    player.popCommand();
}

}
