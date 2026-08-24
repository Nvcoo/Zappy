/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiEvents
*/

#include "../../../include/network/GuiClient.hpp"

void GuiClient::pnw(std::shared_ptr<Player> player)
{
    std::string msg = "pnw #" + std::to_string(player->getId());

    msg += " " + std::to_string(player->getX());
    msg += " " + std::to_string(player->getY());
    msg += " " + std::to_string((player->getOrientation()));
    msg += " " + std::to_string(player->getLevel());
    msg += " " + player->getTeamName();
    msg += "\n";
    sendMessage(msg);
}

void GuiClient::ppo(std::shared_ptr<Player> player)
{
    std::string msg = "ppo #" + std::to_string(player->getId());

    msg += " " + std::to_string(player->getX());
    msg += " " + std::to_string(player->getY());
    msg += " " + std::to_string((player->getOrientation()));
    msg += "\n";
    sendMessage(msg);
}

void GuiClient::plv(std::shared_ptr<Player> player)
{
    std::string msg = "plv #" + std::to_string(player->getId());

    msg += " " + std::to_string(player->getLevel());
    msg += "\n";
    sendMessage(msg);
}

void GuiClient::pin(std::shared_ptr<Player> player)
{
    std::string msg = "pin #" + std::to_string(player->getId());

    msg += " " + std::to_string(player->getX());
    msg += " " + std::to_string(player->getY());
    msg += " " + std::to_string(player->getInventory(FOOD));
    msg += " " + std::to_string(player->getInventory(LINEMATE));
    msg += " " + std::to_string(player->getInventory(DERAUMERE));
    msg += " " + std::to_string(player->getInventory(SIBUR));
    msg += " " + std::to_string(player->getInventory(MENDIANE));
    msg += " " + std::to_string(player->getInventory(PHIRAS));
    msg += " " + std::to_string(player->getInventory(THYSTAME));
    msg += "\n";
    sendMessage(msg);
}

void GuiClient::pex(std::shared_ptr<Player> player)
{
    sendMessage("pex #" + std::to_string(player->getId()) + "\n");
}

void GuiClient::pbc(std::shared_ptr<Player> player, const std::string &msg)
{
    sendMessage("pbc #" + std::to_string(player->getId()) + " " + msg + "\n");
}

void GuiClient::pic(int lvl, int x, int y, std::vector<std::shared_ptr<Player>> &participants)
{
    std::string msg = "pic " + std::to_string(x) + " " + std::to_string(y);

    msg += " " + std::to_string(lvl);
    for (auto &p : participants)
        msg += " #" + std::to_string(p->getId());
    msg += "\n";
    sendMessage(msg);
}

void GuiClient::pie(int x, int y, bool success)
{
    std::string msg = "pie " + std::to_string(x) + " " + std::to_string(y);

    msg += " " + std::to_string(success ? 1 : 0) + "\n";
    sendMessage(msg);
}

void GuiClient::bct(int x, int y, Map &map)
{
    sendMessage(buildTileContent(x, y, map));
}

void GuiClient::pfk(std::shared_ptr<Player> player)
{
    sendMessage("pfk #" + std::to_string(player->getId()) + "\n");
}

void GuiClient::pgt(std::shared_ptr<Player> player, int resourceType)
{
    sendMessage("pgt #" + std::to_string(player->getId()) + " " + std::to_string(resourceType) + "\n");
}

void GuiClient::pdr(std::shared_ptr<Player> player, int resourceType)
{
    sendMessage("pdr #" + std::to_string(player->getId()) + " " + std::to_string(resourceType) + "\n");
}

void GuiClient::pdi(std::shared_ptr<Player> player)
{
    sendMessage("pdi #" + std::to_string(player->getId()) + "\n");
}

void GuiClient::enw(int eggId, int playerId, int x, int y)
{
    std::string msg = "enw #" + std::to_string(eggId);

    msg += " #" + std::to_string(playerId);
    msg += " " + std::to_string(x);
    msg += " " + std::to_string(y) + "\n";
    sendMessage(msg);
}

void GuiClient::ebo(int eggId)
{
    sendMessage("ebo #" + std::to_string(eggId) + "\n");
}

void GuiClient::edi(int eggId)
{
    sendMessage("edi #" + std::to_string(eggId) + "\n");
}

void GuiClient::seg(const std::string &teamName)
{
    sendMessage("seg " + teamName + "\n");
}

void GuiClient::smg(const std::string &msg)
{
    sendMessage("smg " + msg + "\n");
}
