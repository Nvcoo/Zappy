/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Incantation
*/

#include "../../../include/command/Actions.hpp"
#include "../../../include/world/Elevation.hpp"
#include "../../../include/command/GuiNotify.hpp"
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
    broadcastGui(clients, [&participants, &player](GuiClient &gui) {
        gui.pic(player.getLevel(), player.getX(), player.getY(), participants);
    });
    for (auto &p : participants) {
        p->setBusy(true);
        p->setInIncantation(true);
        p->sendMessage("Elevation underway\n");
    }
}

static void checkWin(int newLvl, std::vector<std::shared_ptr<Client>> &clients)
{
    if (newLvl == 8) {
        for (auto &client : clients) {
            auto p = std::dynamic_pointer_cast<Player>(client);
            if (p == nullptr)
                continue;
            int count = 0;
            std::string winningTeam = p->getTeamName();
            for (auto &c : clients) {
                auto pp = std::dynamic_pointer_cast<Player>(c);
                if (pp && pp->getTeamName() == winningTeam && pp->getLevel() == 8)
                    count++;
            }
            if (count >= 6) {
                for (auto &c : clients)
                    c->sendMessage("seg " + winningTeam + "\n");
                return;
            }
        }
    }
}

std::string incantationEnd(std::shared_ptr<Player> player, Map &map, std::vector<std::shared_ptr<Client>> &clients)
{
    if (!player->isInIncantation())
        return "";
    Tile &tile = map.getTile(player->getX(), player->getY());
    auto participants = getParticipants(*player, clients);
    std::vector<std::shared_ptr<Player>> stillIn;

    for (auto &p : participants) {
        if (p->isInIncantation())
            stillIn.push_back(p);
    }
    if (!checkRequirements(player->getLevel(), stillIn.size(), tile)) {
        broadcastGui(clients, [&player](GuiClient &gui) {
            gui.pie(player->getX(), player->getY(), false);
        });
        for (auto &p : stillIn) {
            p->setBusy(false);
            p->setInIncantation(false);
            p->sendMessage("ko\n");
        }
        return "";
    }

    int newLvl = player->getLevel() + 1;

    broadcastGui(clients, [&player](GuiClient &gui) {
        gui.pie(player->getX(), player->getY(), true);
    });
    consumeResources(player->getLevel(), tile);
    std::string response = "Current level: " + std::to_string(newLvl) + "\n";
    for (auto &p : stillIn) {
        p->setLevel(newLvl);
        p->setBusy(false);
        p->setInIncantation(false);
        p->sendMessage(response);
    }
    checkWin(newLvl, clients);
    return "";
}
