#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct Tile {
    int q[7] = {0,0,0,0,0,0,0};
};

struct Player {
    int id = -1;
    int x = 0;
    int y = 0;
    int orientation = 1;
    int level = 1;
    std::string team;
    int inventory[7] = {0,0,0,0,0,0,0};
};

struct Egg {
    int id = -1;
    int owner = -1;
    int x = 0;
    int y = 0;
};

struct Incant {
    int x = 0;
    int y = 0;
    int level = 1;
    std::vector<int> players;
};

class World {
public:
    // map
    void setMapSize(int w, int h);
    void updateTile(int x, int y, int q[7]);

    // teams
    void addTeam(const std::string &name);

    // players
    void addPlayer(int id, int x, int y, int o, int level, const std::string &team);
    void movePlayer(int id, int x, int y, int o);
    void setPlayerLevel(int id, int level);
    void setPlayerInventory(int id, int x, int y, int q[7]);
    void playerExpelled(int id);
    void playerBroadcast(int id, const std::string &msg);
    void startIncantation(int x, int y, int level, const std::vector<int> &players);
    void endIncantation(int x, int y, int result);
    void playerLaidEgg(int id);
    void playerDropped(int id, int resource);
    void playerCollected(int id, int resource);
    void playerDied(int id);

    // eggs
    void addEgg(int egg, int player, int x, int y);
    void eggHatched(int egg);
    void eggDied(int egg);

    // time / server messages
    void setTimeUnit(int t);
    void endGame(const std::string &team);
    void serverMessage(const std::string &msg);
    void unknownCommand();
    void badParameter();

    // unknown
    void unknownLine(const std::string &line);

    // for renderer
    std::vector<int> getPlayersInTile(int x, int y) const;

    // public data
    int width = 0;
    int height = 0;
    int timeUnit = 0;
    bool gameEnded = false;
    std::string winningTeam;

    std::vector<std::vector<Tile>> map;
    std::unordered_map<int, Player> players;
    std::vector<std::string> teams;

    std::unordered_map<int, Egg> eggs;
    std::vector<Incant> incantations;
};
