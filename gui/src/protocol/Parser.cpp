#include "Parser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

static inline std::string trim(const std::string &s) {
    size_t a = 0;
    while (a < s.size() && std::isspace((unsigned char)s[a])) ++a;
    size_t b = s.size();
    while (b > a && std::isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

std::vector<std::string> Parser::splitTokens(const std::string &s) {
    // split only by spaces
    std::vector<std::string> out;
    std::istringstream iss(s);
    std::string tok;
    while (iss >> tok) out.push_back(tok);
    return out;
}

int Parser::safeStoi(const std::string &s, int fallback) {
    try {
        size_t idx = 0;
        int v = std::stoi(s, &idx);
        if (idx != s.size()) return fallback;
        return v;
    } catch (...) {
        return fallback;
    }
}

int Parser::parseHashInt(const std::string &s, int fallback) {
    if (s.empty()) return fallback;
    if (s[0] == '#') return safeStoi(s.substr(1), fallback);
    return safeStoi(s, fallback);
}

std::string Parser::joinFrom(const std::vector<std::string> &toks, size_t start) {
    if (start >= toks.size()) return "";
    std::string out = toks[start];
    for (size_t i = start + 1; i < toks.size(); ++i) {
        out += " ";
        out += toks[i];
    }
    return out;
}

bool Parser::parse(const std::string &lineRaw, World &world) {
    std::string line = trim(lineRaw);
    if (line.empty()) return true;

    // ignore server replies that are not part of protocol
    if (line == "ok" || line == "ko" || line == "WELCOME") return true;

    auto toks = splitTokens(line);
    if (toks.empty()) return true;

    const std::string &cmd = toks[0];

    // msz X Y
    if (cmd == "msz" && toks.size() >= 3) {
        int x = safeStoi(toks[1], 0);
        int y = safeStoi(toks[2], 0);
        world.setMapSize(x, y);
        return true;
    }

    // bct X Y q0 q1 q2 q3 q4 q5 q6
    if (cmd == "bct" && toks.size() >= 10) {
        int x = safeStoi(toks[1], 0);
        int y = safeStoi(toks[2], 0);
        int q[7];
        for (int i = 0; i < 7; ++i) q[i] = safeStoi(toks[3 + i], 0);
        world.updateTile(x, y, q);
        return true;
    }

    // mct
    //  client request
    if (cmd == "mct") {
        // server sends many bct lines
        // nothing to do here
        return true;
    }

    // tna N (team name) repeated
    if (cmd == "tna" && toks.size() >= 2) {
        world.addTeam(toks[1]);
        return true;
    }

    // pnw #n X Y O L N
    if (cmd == "pnw" && toks.size() >= 6) {
        int id = parseHashInt(toks[1], -1);
        int x = safeStoi(toks[2], 0);
        int y = safeStoi(toks[3], 0);
        int o = safeStoi(toks[4], 1);
        int l = safeStoi(toks[5], 1);
        std::string team = (toks.size() >= 7) ? toks[6] : std::string();
        world.addPlayer(id, x, y, o, l, team);
        return true;
    }

    // ppo #n X Y O
    if (cmd == "ppo" && toks.size() >= 5) {
        int id = parseHashInt(toks[1], -1);
        int x = safeStoi(toks[2], 0);
        int y = safeStoi(toks[3], 0);
        int o = safeStoi(toks[4], 1);
        world.movePlayer(id, x, y, o);
        return true;
    }

    // plv #n L
    if (cmd == "plv" && toks.size() >= 3) {
        int id = parseHashInt(toks[1], -1);
        int level = safeStoi(toks[2], 1);
        world.setPlayerLevel(id, level);
        return true;
    }

    // pin #n X Y q0 q1 q2 q3 q4 q5 q6
    if (cmd == "pin" && toks.size() >= 11) {
        int id = parseHashInt(toks[1], -1);
        int x = safeStoi(toks[2], 0);
        int y = safeStoi(toks[3], 0);
        int q[7];
        for (int i = 0; i < 7; ++i) q[i] = safeStoi(toks[4 + i], 0);
        world.setPlayerInventory(id, x, y, q);
        return true;
    }

    // pex #n
    if (cmd == "pex" && toks.size() >= 2) {
        int id = parseHashInt(toks[1], -1);
        world.playerExpelled(id);
        return true;
    }

    // pbc #n M  (broadcast) -> player message
    if (cmd == "pbc" && toks.size() >= 3) {
        int id = parseHashInt(toks[1], -1);
        std::string msg = joinFrom(toks, 2);
        world.playerBroadcast(id, msg);
        return true;
    }

    // pic X Y L #n #n ...
    if (cmd == "pic" && toks.size() >= 4) {
        int x = safeStoi(toks[1], 0);
        int y = safeStoi(toks[2], 0);
        int level = safeStoi(toks[3], 1);
        std::vector<int> players;
        for (size_t i = 4; i < toks.size(); ++i) players.push_back(parseHashInt(toks[i], -1));
        world.startIncantation(x, y, level, players);
        return true;
    }

    // pie X Y R
    if (cmd == "pie" && toks.size() >= 4) {
        int x = safeStoi(toks[1], 0);
        int y = safeStoi(toks[2], 0);
        int result = safeStoi(toks[3], 0);
        world.endIncantation(x, y, result);
        return true;
    }

    // pfk #n (player laid egg)
    if (cmd == "pfk" && toks.size() >= 2) {
        int id = parseHashInt(toks[1], -1);
        world.playerLaidEgg(id);
        return true;
    }

    // pdr #n i (drop resource)
    if (cmd == "pdr" && toks.size() >= 3) {
        int id = parseHashInt(toks[1], -1);
        int i = safeStoi(toks[2], 0);
        world.playerDropped(id, i);
        return true;
    }

    // pgt #n i (get resource)
    if (cmd == "pgt" && toks.size() >= 3) {
        int id = parseHashInt(toks[1], -1);
        int i = safeStoi(toks[2], 0);
        world.playerCollected(id, i);
        return true;
    }

    // pdi #n (player died)
    if (cmd == "pdi" && toks.size() >= 2) {
        int id = parseHashInt(toks[1], -1);
        world.playerDied(id);
        return true;
    }

    // enw #e #n X Y (egg laid)
    if (cmd == "enw" && toks.size() >= 5) {
        int e = parseHashInt(toks[1], -1);
        int n = parseHashInt(toks[2], -1);
        int x = safeStoi(toks[3], 0);
        int y = safeStoi(toks[4], 0);
        world.addEgg(e, n, x, y);
        return true;
    }

    // ebo #e (egg used to connect)
    if (cmd == "ebo" && toks.size() >= 2) {
        int e = parseHashInt(toks[1], -1);
        world.eggHatched(e);
        return true;
    }

    // edi #e (egg died)
    if (cmd == "edi" && toks.size() >= 2) {
        int e = parseHashInt(toks[1], -1);
        world.eggDied(e);
        return true;
    }

    // sgt T (server get time unit)
    if (cmd == "sgt" && toks.size() >= 2) {
        int t = safeStoi(toks[1], 0);
        world.setTimeUnit(t);
        return true;
    }

    // sst T (server set time unit)
    if (cmd == "sst" && toks.size() >= 2) {
        int t = safeStoi(toks[1], 0);
        world.setTimeUnit(t);
        return true;
    }

    // seg N (end of game)
    if (cmd == "seg" && toks.size() >= 2) {
        std::string team = toks[1];
        world.endGame(team);
        return true;
    }

    // smg M (server message)
    if (cmd == "smg" && toks.size() >= 2) {
        std::string msg = joinFrom(toks, 1);
        world.serverMessage(msg);
        return true;
    }

    // suc (unknown command)
    if (cmd == "suc") {
        world.unknownCommand();
        return true;
    }

    // sbp (bad parameter)
    if (cmd == "sbp") {
        world.badParameter();
        return true;
    }

    // unknown command
    world.unknownLine(line);
    return true;
}
