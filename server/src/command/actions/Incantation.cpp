/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Incantation
*/

#include "../../../include/command/Actions.hpp"
#include "../../../include/world/Elevation.hpp"
#include <memory>
#include <string>
#include <vector>

static std::vector<std::shared_ptr<Player>> getParticipants(Player &initiator, std::vector<std::shared_ptr<Client>> &clients)
{
    std::vector<std::shared_ptr<Player>> participants;

    for (auto &client : clients) {
        auto p = std::dynamic_pointer_cast<Player>(client);
        if (p == nullptr)
            continue;
        if (p->getX() != initiator.getX() || p->getY() != initiator.getY() || p->getLevel() != initiator.getLevel())
            continue;
        participants.push_back(p);
    }
    return participants;
}

void incantationStart(Player &player, Map &map, std::vector<std::shared_ptr<Client>> &clients)
{
    Tile &tile = map.getTile(player.getX(), player.getY());
    auto participants = getParticipants(player, clients);

    if (!checkRequirements(player.getLevel(), participants.size(), tile)) {
        player.sendMessage("ko\n");
        player.setBusy(false);
        return;
    }
    for (auto &p : participants) {
        p->setBusy(true);
        p->setInIncantation(true);
        p->sendMessage("Elevation underway\n");
    }
}

std::string incantationEnd(Player &player, Map &map, std::vector<std::shared_ptr<Client>> &clients)
{
    if (!player.isInIncantation())
        return "";
    Tile &tile = map.getTile(player.getX(), player.getY());
    auto participants = getParticipants(player, clients);
    std::vector<std::shared_ptr<Player>> stillIn;

    for (auto &p : participants) {
        if (p->isInIncantation())
            stillIn.push_back(p);
    }
    if (!checkRequirements(player.getLevel(), stillIn.size(), tile)) {
        for (auto &p : stillIn) {
            p->setBusy(false);
            p->setInIncantation(false);
            p->sendMessage("ko\n");
        }
        return "";
    }

    int newLvl = player.getLevel() + 1;

    consumeResources(player.getLevel(), tile);
    std::string response = "Current level: " + std::to_string(newLvl) + "\n";
    for (auto &p : stillIn) {
        p->setLevel(newLvl);
        p->setBusy(false);
        p->setInIncantation(false);
        p->sendMessage(response);
    }
    return "";
}
