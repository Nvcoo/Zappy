/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiNotify
*/

#include "../../../include/command/GuiNotify.hpp"
#include "../../../include/network/GuiClient.hpp"
#include "../../../include/command/Actions.hpp"

void broadcastGui(std::vector<std::shared_ptr<Client>> &clients, std::function<void(GuiClient &)> fn)
{
    for (auto &client : clients) {
        auto gui = std::dynamic_pointer_cast<GuiClient>(client);
        if (gui)
            fn(*gui);
    }
}

void notifyForward(std::shared_ptr<Player> player, Map &map, std::vector<std::shared_ptr<Client>> &clients)
{
    moveForward(*player, map);
    broadcastGui(clients, [&player](GuiClient &gui) {
        gui.ppo(player);
    });
}

void notifyLeft(std::shared_ptr<Player> player, std::vector<std::shared_ptr<Client>> &clients)
{
    rotateLeft(*player);
    broadcastGui(clients, [&player](GuiClient &gui) {
        gui.ppo(player);
    });
}

void notifyRight(std::shared_ptr<Player> player, std::vector<std::shared_ptr<Client>> &clients)
{
    rotateRight(*player);
    broadcastGui(clients, [&player](GuiClient &gui) {
        gui.ppo(player);
    });
}

std::string notifyTake(std::shared_ptr<Player> player, Map &map, const std::string &arg, std::vector<std::shared_ptr<Client>> &clients)
{
    std::string result = executeTake(*player, map, arg);
    if (result == "ok\n") {
        Resource r = nameToResource(arg);
        broadcastGui(clients, [&player, r](GuiClient &gui) {
            gui.pgt(player, static_cast<int>(r));
        });
        broadcastGui(clients, [&player](GuiClient &gui) {
            gui.pin(player);
        });
    }
    return result;
}

std::string notifySet(std::shared_ptr<Player> player, Map &map, const std::string &arg, std::vector<std::shared_ptr<Client>> &clients)
{
    std::string result = executeSet(*player, map, arg);
    if (result == "ok\n") {
        Resource r = nameToResource(arg);
        broadcastGui(clients, [&player, r](GuiClient &gui) {
            gui.pdr(player, static_cast<int>(r));
        });
        broadcastGui(clients, [&player](GuiClient &gui) {
            gui.pin(player);
        });
    }
    return result;
}

std::string notifyBroadcast(std::shared_ptr<Player> player, Map &map, const std::string &arg, std::vector<std::shared_ptr<Client>> &clients)
{
    broadcastGui(clients, [&player, &arg](GuiClient &gui) {
        gui.pbc(player, arg);
    });
    return executeBroadcast(*player, arg, clients, map);
}

static void notifyEgg(std::shared_ptr<Player> player, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients)
{
    for (auto &team : teams) {
        if (team.getName() != player->getTeamName())
            continue;
        auto &eggs = team.getEggs();
        if (eggs.empty())
            break;
        auto &egg = eggs.back();
        broadcastGui(clients, [&egg, &player](GuiClient &gui) {
            gui.enw(egg.id, player->getId(), egg.x, egg.y);
        });
        break;
    }
}

std::string notifyFork(std::shared_ptr<Player> player, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients)
{
    std::string result = executeFork(*player, teams);
    if (result == "ok\n") {
        broadcastGui(clients, [&player](GuiClient &gui) { gui.pfk(player); });
        notifyEgg(player, teams, clients);
    }
    return result;
}

std::string notifyEject(std::shared_ptr<Player> player, Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients)
{
    EjectResult result = executeEject(*player, map, teams, clients);
    if (result.response == "ok\n") {
        for (auto &victim : result.ejected) {
            broadcastGui(clients, [&victim](GuiClient &gui) { gui.pex(victim); });
            broadcastGui(clients, [&victim](GuiClient &gui) { gui.ppo(victim); });
        }
        for (auto &egg : result.destroyedEggs) {
            broadcastGui(clients, [&egg](GuiClient &gui) { gui.edi(egg.id); });
        }
    }
    return result.response;
}
