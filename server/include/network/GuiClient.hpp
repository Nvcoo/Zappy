/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiClient
*/

#ifndef GUICLIENT_HPP_
    #define GUICLIENT_HPP_

#include "Client.hpp"
#include "../world/Map.hpp"
#include "../world/Team.hpp"
#include "../game/Player.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

enum GuiCommands {
    MSZ,
    BCT,
    MCT,
    TNA,
    PPO,
    PLV,
    PIN,
    SGT,
    SST
};

inline const std::unordered_map<std::string, GuiCommands> GuiCommandMap = {
    {"msz", MSZ},
    {"bct", BCT},
    {"mct", MCT},
    {"tna", TNA},
    {"ppo", PPO},
    {"plv", PLV},
    {"pin", PIN},
    {"sgt", SGT},
    {"sst", SST}
};

class GuiClient : public Client
{
    private:
        std::string buildTileContent(int x, int y, Map &map) const;
    protected:
    public:
        GuiClient(int fd) : Client(fd, GUI) {};
        void parseCommand(const std::string &line, Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients, int &freq);
        void setInitialState(Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients, int freq);

        //active
        void msz(Map &map);
        void mct(Map &map);
        void bct(Map &map, std::vector<std::string> &args);
        void tna(std::vector<Team> &teams);
        void sgt(int freq);
        void sst(std::vector<std::string> &args, int &freq);
        void ppo(std::vector<std::shared_ptr<Client>> &clients, std::vector<std::string> &args);
        void plv(std::vector<std::shared_ptr<Client>> &clients, std::vector<std::string> &args);
        void pin(std::vector<std::shared_ptr<Client>> &clients, std::vector<std::string> &args);

        //passive
        void pnw(std::shared_ptr<Player> player);
        void ppo(std::shared_ptr<Player> player);
        void plv(std::shared_ptr<Player> player);
        void pin(std::shared_ptr<Player> player);
        void pex(std::shared_ptr<Player> player);
        void pbc(std::shared_ptr<Player> player, const std::string &msg);
        void pic(int lvl, int x, int y, std::vector<std::shared_ptr<Player>> &participants);
        void pie(int x, int y, bool success);
        void bct (int x, int y, Map &map);
        void pfk(std::shared_ptr<Player> player);
        void pgt(std::shared_ptr<Player> player, int resourceType);
        void pdr(std::shared_ptr<Player> player, int resourceType);
        void pdi(std::shared_ptr<Player> player);
        void enw(int eggId, int playerId, int x, int y);
        void ebo(int eggId);
        void edi(int eggId);
        void seg(const std::string &teamName);
        void smg(const std::string &msg);
};

#endif
