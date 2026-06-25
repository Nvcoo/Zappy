#include "World.hpp"
#include <iostream>
#include <algorithm>

// helper
static bool inBounds(const World &w, int x, int y) {
    return (y >= 0 && y < (int)w.map.size() && x >= 0 && x < (int)w.map[y].size());
}

// map & teams
void World::setMapSize(int w, int h) {
    width = w;
    height = h;
    // map is stored as map[y][x]
    map.assign(h, std::vector<Tile>(w));
    std::cout << "[WORLD] Map size set to " << width << "x" << height << "\n";
}

void World::updateTile(int x, int y, int q[7]) {
    if (!inBounds(*this, x, y)) {
        std::cout << "[WORLD] updateTile: out of bounds (" << x << "," << y << ")\n";
        return;
    }
    for (int i = 0; i < 7; ++i) map[y][x].q[i] = q[i];
    std::cout << "[WORLD] Tile (" << x << "," << y << ") = [";
    for (int i = 0; i < 7; ++i) {
        if (i) std::cout << " ";
        std::cout << map[y][x].q[i];
    }
    std::cout << "]\n";
}


void World::addTeam(const std::string &name) {
    teams.push_back(name);
    std::cout << "[WORLD] Team added: " << name << "\n";
}

//player & actions
void World::addPlayer(int id, int x, int y, int o, int level, const std::string &team) {
    Player p;
    p.id = id;
    p.x = x;
    p.y = y;
    p.orientation = o;
    p.level = level;
    p.team = team;
    for (int i = 0; i < 7; ++i) p.inventory[i] = 0;
    players[id] = p;
    std::cout << "[WORLD] Player added: #" << id
              << " pos(" << x << "," << y << ") o=" << o
              << " lvl=" << level << " team=" << team << "\n";
}

void World::movePlayer(int id, int x, int y, int o) {
    auto it = players.find(id);
    if (it == players.end()) {
        std::cout << "[WORLD] movePlayer: unknown player #" << id << "\n";
        return;
    }
    // wrap positions if map is set
    if (width > 0 && height > 0) {
        x = (x % width + width) % width;
        y = (y % height + height) % height;
    }
    it->second.x = x;
    it->second.y = y;
    it->second.orientation = o;
    std::cout << "[WORLD] Player #" << id << " moved to (" << x << "," << y << ") o=" << o << "\n";
}

void World::setPlayerLevel(int id, int level) {
    auto it = players.find(id);
    if (it == players.end()) {
        std::cout << "[WORLD] setPlayerLevel: unknown player #" << id << "\n";
        return;
    }
    it->second.level = level;
    std::cout << "[WORLD] Player #" << id << " level -> " << level << "\n";
}

void World::setPlayerInventory(int id, int x, int y, int q[7]) {
    auto it = players.find(id);
    if (it == players.end()) {
        if (inBounds(*this, x, y)) {
            for (int i = 0; i < 7; ++i) map[y][x].q[i] = q[i];
            std::cout << "[WORLD] setPlayerInventory: unknown player #" << id << " (tile updated)\n";
        } else {
            std::cout << "[WORLD] setPlayerInventory: unknown player #" << id << " and tile OOB\n";
        }
        return;
    }

    for (int i = 0; i < 7; ++i) it->second.inventory[i] = q[i];
    it->second.x = x;
    it->second.y = y;
    std::cout << "[WORLD] Player #" << id << " inventory at (" << x << "," << y << "): ";
    for (int i = 0; i < 7; ++i) {
        if (i) std::cout << " ";
        std::cout << it->second.inventory[i];
    }
    std::cout << "\n";
}

void World::playerExpelled(int id) {
    std::cout << "[WORLD] Player #" << id << " expelled\n";
}

void World::playerBroadcast(int id, const std::string &msg) {
    std::cout << "[WORLD] Player #" << id << " broadcast: " << msg << "\n";
}

void World::startIncantation(int x, int y, int level, const std::vector<int> &pls) {
    Incant ic;
    ic.x = x; ic.y = y; ic.level = level; ic.players = pls;
    incantations.push_back(ic);
    std::cout << "[WORLD] Incantation started at (" << x << "," << y << ") level " << level << " players:";
    for (int p : pls) std::cout << " #" << p;
    std::cout << "\n";
}

void World::endIncantation(int x, int y, int result) {
    incantations.erase(std::remove_if(incantations.begin(), incantations.end(),
        [&](const Incant &ic){ return ic.x == x && ic.y == y; }), incantations.end());
    std::cout << "[WORLD] Incantation ended at (" << x << "," << y << ") result=" << result << "\n";
}

void World::playerLaidEgg(int id) {
    std::cout << "[WORLD] Player #" << id << " laid an egg (pfk)\n";
}

void World::playerDropped(int id, int resource) {
    if (resource < 0 || resource >= 7) {
        std::cout << "[WORLD] playerDropped: invalid resource " << resource << "\n";
        return;
    }
    auto it = players.find(id);
    if (it == players.end()) {
        std::cout << "[WORLD] playerDropped: unknown player #" << id << "\n";
        return;
    }
    if (it->second.inventory[resource] > 0) it->second.inventory[resource]--;
    int x = it->second.x, y = it->second.y;
    if (inBounds(*this, x, y)) map[y][x].q[resource]++;
    std::cout << "[WORLD] Player #" << id << " dropped resource " << resource << " at (" << x << "," << y << ")\n";
}

void World::playerCollected(int id, int resource) {
    if (resource < 0 || resource >= 7) {
        std::cout << "[WORLD] playerCollected: invalid resource " << resource << "\n";
        return;
    }
    auto it = players.find(id);
    if (it == players.end()) {
        std::cout << "[WORLD] playerCollected: unknown player #" << id << "\n";
        return;
    }
    int x = it->second.x, y = it->second.y;
    if (inBounds(*this, x, y) && map[y][x].q[resource] > 0) {
        map[y][x].q[resource]--;
        it->second.inventory[resource]++;
        std::cout << "[WORLD] Player #" << id << " collected resource " << resource << " from (" << x << "," << y << ")\n";
    } else {
        it->second.inventory[resource]++;
        std::cout << "[WORLD] Player #" << id << " collected resource " << resource << " (tile had none or OOB)\n";
    }
}

void World::playerDied(int id) {
    auto it = players.find(id);
    if (it != players.end()) {
        std::cout << "[WORLD] Player #" << id << " died (removed)\n";
        players.erase(it);
    } else {
        std::cout << "[WORLD] Player #" << id << " died (unknown)\n";
    }
}

// eggs
void World::addEgg(int egg, int player, int x, int y) {
    Egg e;
    e.id = egg;
    e.owner = player;
    e.x = x;
    e.y = y;
    eggs[egg] = e;
    std::cout << "[WORLD] Egg #" << egg << " laid by player #" << player << " at (" << x << "," << y << ")\n";
}

void World::eggHatched(int egg) {
    auto it = eggs.find(egg);
    if (it != eggs.end()) {
        std::cout << "[WORLD] Egg #" << egg << " hatched\n";
        eggs.erase(it);
    } else {
        std::cout << "[WORLD] Egg #" << egg << " hatched (unknown)\n";
    }
}

void World::eggDied(int egg) {
    auto it = eggs.find(egg);
    if (it != eggs.end()) {
        std::cout << "[WORLD] Egg #" << egg << " died\n";
        eggs.erase(it);
    } else {
        std::cout << "[WORLD] Egg #" << egg << " died (unknown)\n";
    }
}


void World::setTimeUnit(int t) {
    timeUnit = t;
    std::cout << "[WORLD] Time unit set to " << t << "\n";
}

void World::endGame(const std::string &team) {
    std::cout << "[WORLD] Game ended. Winner: " << team << "\n";
    gameEnded = true;
    winningTeam = team;
}

void World::serverMessage(const std::string &msg) {
    std::cout << "[WORLD] Server message: " << msg << "\n";
}

void World::unknownCommand() {
    std::cout << "[WORLD] Server: unknown command (suc)\n";
}

void World::badParameter() {
    std::cout << "[WORLD] Server: bad parameter (sbp)\n";
}

void World::unknownLine(const std::string &line) {
    std::cout << "[WORLD] Unknown line: " << line << "\n";
}

// help for rendering
std::vector<int> World::getPlayersInTile(int x, int y) const {
    std::vector<int> out;
    if (!inBounds(*this, x, y)) return out;
    for (const auto &kv : players) {
        const Player &p = kv.second;
        if (p.x == x && p.y == y) out.push_back(p.id);
    }
    return out;
}
