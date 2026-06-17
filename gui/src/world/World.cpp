// TEST WORLD FOR DEBUGGING

#include "World.hpp"
#include <iostream>

void World::setMapSize(int w, int h) {
    width = w;
    height = h;
    map.assign(h, std::vector<Tile>(w));
    std::cout << "[WORLD] Map size set to " << w << "x" << h << "\n";
}

void World::updateTile(int x, int y, int q[7]) {
    if (y >= 0 && y < (int)map.size() && x >= 0 && x < (int)map[y].size()) {
        for (int i = 0; i < 7; ++i) map[y][x].q[i] = q[i];
    }
    std::cout << "[WORLD] Tile (" << x << "," << y << ") updated: "
              << q[0] << " " << q[1] << " " << q[2] << " "
              << q[3] << " " << q[4] << " " << q[5] << " " << q[6] << "\n";
}

void World::addTeam(const std::string &name) {
    teams.push_back(name);
    std::cout << "[WORLD] Team added: " << name << "\n";
}

void World::addPlayer(int id, int x, int y, int o, int level, const std::string &team) {
    Player p;
    p.id = id;
    p.x = x;
    p.y = y;
    p.orientation = o;
    p.level = level;
    p.team = team;
    players[id] = p;
    std::cout << "[WORLD] Player added: #" << id
              << " pos(" << x << "," << y << ")"
              << " o=" << o << " lvl=" << level
              << " team=" << team << "\n";
}

void World::movePlayer(int id, int x, int y, int o) {
    auto it = players.find(id);
    if (it != players.end()) {
        it->second.x = x;
        it->second.y = y;
        it->second.orientation = o;
    }
    std::cout << "[WORLD] Player #" << id << " moved to ("
              << x << "," << y << ") o=" << o << "\n";
}

void World::setPlayerLevel(int id, int level) {
    players[id].level = level;
    std::cout << "[WORLD] Player #" << id << " level set to " << level << "\n";
}

void World::setPlayerInventory(int id, int x, int y, int q[7]) {
    std::cout << "[WORLD] Player #" << id << " inventory at ("
              << x << "," << y << "): ";
    for (int i = 0; i < 7; ++i) std::cout << q[i] << " ";
    std::cout << "\n";
}

void World::playerExpelled(int id) {
    std::cout << "[WORLD] Player #" << id << " expelled\n";
}

void World::playerBroadcast(int id, const std::string &msg) {
    std::cout << "[WORLD] Player #" << id << " broadcast: " << msg << "\n";
}

void World::startIncantation(int x, int y, int level, const std::vector<int> &players) {
    std::cout << "[WORLD] Incantation start at (" << x << "," << y
              << ") level " << level << " players:";
    for (int p : players) std::cout << " #" << p;
    std::cout << "\n";
}

void World::endIncantation(int x, int y, int result) {
    std::cout << "[WORLD] Incantation end at (" << x << "," << y
              << ") result=" << result << "\n";
}

void World::playerLaidEgg(int id) {
    std::cout << "[WORLD] Player #" << id << " laid an egg\n";
}

void World::playerDropped(int id, int resource) {
    std::cout << "[WORLD] Player #" << id << " dropped resource " << resource << "\n";
}

void World::playerCollected(int id, int resource) {
    std::cout << "[WORLD] Player #" << id << " collected resource " << resource << "\n";
}

void World::playerDied(int id) {
    std::cout << "[WORLD] Player #" << id << " died\n";
}

void World::addEgg(int egg, int player, int x, int y) {
    std::cout << "[WORLD] Egg #" << egg << " laid by player #" << player
              << " at (" << x << "," << y << ")\n";
}

void World::eggHatched(int egg) {
    std::cout << "[WORLD] Egg #" << egg << " hatched\n";
}

void World::eggDied(int egg) {
    std::cout << "[WORLD] Egg #" << egg << " died\n";
}

void World::setTimeUnit(int t) {
    std::cout << "[WORLD] Time unit set to " << t << "\n";
}

void World::endGame(const std::string &team) {
    std::cout << "[WORLD] Game ended. Winner: " << team << "\n";
}

void World::serverMessage(const std::string &msg) {
    std::cout << "[WORLD] Server message: " << msg << "\n";
}

void World::unknownCommand() {
    std::cout << "[WORLD] Unknown command received\n";
}

void World::badParameter() {
    std::cout << "[WORLD] Bad parameter received\n";
}

void World::unknownLine(const std::string &line) {
    std::cout << "[WORLD] Unknown line: " << line << "\n";
}
