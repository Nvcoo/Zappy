/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandHandler
*/

#include "../../include/command/CommandHandler.hpp"
#include "../../include/command/Actions.hpp"
#include "../../include/command/GuiNotify.hpp"
#include <memory>
#include <string>
#include <vector>

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
    cmd.started = false;
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

std::string executeCommand(std::shared_ptr<Player> player, const Command &cmd, Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients)
{
    switch (nameToType(cmd.name)) {
        case FORWARD:
            notifyForward(player, map, clients);
            return "ok\n";
        case LEFT:
            notifyLeft(player, clients);
            return "ok\n";
        case RIGHT:
            notifyRight(player, clients);
            return "ok\n";
        case INVENTORY:
            return executeInventory(*player);
        case TAKE:
            return notifyTake(player, map, cmd.arg, clients);
        case SET:
            return notifySet(player, map, cmd.arg, clients);
        case CONNECT_NBR:
            return executeConnectNbr(*player, teams);
        case BROADCAST:
            return notifyBroadcast(player, map, cmd.arg, clients);
        case LOOK:
            return executeLook(*player, map, clients);
        case FORK:
            return notifyFork(player, teams, clients);
        case EJECT:
            return notifyEject(player, map, teams, clients);
        case INCANTATION:
            if (cmd.started)
                return incantationEnd(player, map, clients);
            return "ko\n";
        case UNKNOWN:
            return "ko\n";
        default:
            return "ko\n";
    }
}

void processPlayerCommands(std::shared_ptr<Player> player, Clock &clock, Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients)
{
    if (!player->hasCommand())
        return;

    Command &cmd = player->frontCommand();
    bool isOwnIncantation = player->isInIncantation() && nameToType(cmd.name) == INCANTATION;

    if (player->isBusy() && !isOwnIncantation)
        return;

    if (cmd.executeAt == 0) {
        int costTicks = getCommandCost(nameToType(cmd.name));
        cmd.executeAt = clock.now() + clock.milliseconds(costTicks);
        if (nameToType(cmd.name) == INCANTATION) {
            incantationStart(*player, map, clients);
            cmd.started = true;
        }
        return;
    }
    if (clock.now() < cmd.executeAt)
        return;
    std::string response = executeCommand(player, cmd, map, teams, clients);
    if (!response.empty())
        player->sendMessage(response);
    player->popCommand();
}
