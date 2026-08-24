/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Brain
*/

#include "../../include/ai/Brain.hpp"
#include <cstring>
#include <sstream>
#include <vector>

static const int LOW_FOOD_THRESHOLD = 9;
static const int SAFE_FOOD_THRESHOLD = 13;

struct ElevationReq {
    int players;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
};

static const ElevationReq table[7] = {
    {1, 1, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 0, 0, 0},
    {2, 2, 0, 1, 0, 2, 0},
    {4, 1, 1, 2, 0, 1, 0},
    {4, 1, 2, 1, 3, 0, 0},
    {6, 1, 2, 3, 0, 1, 0},
    {6, 2, 2, 2, 2, 2, 1}
};

Resource nameToResource(const std::string &name)
{
    if (name == "food")
        return FOOD;
    if (name == "linemate")
        return LINEMATE;
    if (name == "deraumere")
        return DERAUMERE;
    if (name == "sibur")
        return SIBUR;
    if (name == "mendiane")
        return MENDIANE;
    if (name == "phiras")
        return PHIRAS;
    if (name == "thystame")
        return THYSTAME;
    return RESOURCE_COUNT;
}

std::string resourceToName(Resource r)
{
    switch (r) {
        case FOOD: return "food";
        case LINEMATE: return "linemate";
        case DERAUMERE: return "deraumere";
        case SIBUR: return "sibur";
        case MENDIANE: return "mendiane";
        case PHIRAS: return "phiras";
        case THYSTAME: return "thystame";
        default: return "";
    }
}

//specifically for the look command
static std::vector<std::string> splitTiles(const std::string &line)
{
    std::vector<std::string> tiles;
    size_t start = line.find('[');
    size_t end = line.find(']');

    if (start == std::string::npos || end == std::string::npos)
        return tiles;

    std::string inner = line.substr(start + 1, end - start - 1);
    size_t pos = 0;

    while (pos <= inner.size()) {
        size_t comma = inner.find(',', pos);
        std::string tile = (comma == std::string::npos) ? inner.substr(pos) : inner.substr(pos, comma - pos); //if there's no comman, grab everything

        while (!tile.empty() && tile.front() == ' ') //erase spaces after commas
            tile.erase(tile.begin());
        tiles.push_back(tile);
        if (comma == std::string::npos)
            break;
        pos = comma + 1; //continue if its not the last one.
    }
    return tiles;
}

//check if a specific thing is in a specific tile
static bool tileHas(const std::string &tile, const std::string &name)
{
    std::istringstream stream(tile);
    std::string token;

    while (stream >> token) {
        if (token == name)
            return true;
    }
    return false;
}

//counts players on a tile
static int countToken(const std::string &tile, const std::string &name)
{
    std::istringstream stream(tile);
    std::string token;
    int count = 0;

    while (stream >> token) {
        if (token == name)
            count++;
    }
    return count;
}

Brain::Brain() : _level(1), _seekingFood(false), _wanderTurns(0)
{
    std::memset(_inventory, 0, sizeof(_inventory));
}

int Brain::requiredAmount(Resource r) const
{
    const ElevationReq &req = table[_level - 1];

    switch (r) {
        case LINEMATE: return req.linemate;
        case DERAUMERE: return req.deraumere;
        case SIBUR: return req.sibur;
        case MENDIANE: return req.mendiane;
        case PHIRAS: return req.phiras;
        case THYSTAME: return req.thystame;
        default: return 0;
    }
}

Resource Brain::neededResource() const
{
    for (int r = LINEMATE; r <= THYSTAME; r++) {
        Resource res = static_cast<Resource>(r);

        if (_inventory[res] < requiredAmount(res))
            return res;
    }
    return RESOURCE_COUNT;
}

//Checks if a player is ready for incantation
bool Brain::readyForIncantation(const std::string &lookLine) const
{
    if (_level < 1 || _level > 7)
        return false;
    if (neededResource() != RESOURCE_COUNT)
        return false;

    std::vector<std::string> tiles = splitTiles(lookLine);

    if (tiles.empty())
        return false;
    return countToken(tiles[0], "player") >= table[_level - 1].players;
}

void Brain::applyInventory(const std::string &line)
{
    size_t start = line.find('[');
    size_t end = line.find(']');

    if (start == std::string::npos || end == std::string::npos)
        return;

    std::string inner = line.substr(start + 1, end - start - 1);
    std::istringstream stream(inner);
    std::string token;

    while (std::getline(stream, token, ',')) {
        std::istringstream pair(token);
        std::string name;
        int amount = 0;

        pair >> name >> amount;
        Resource r = nameToResource(name);
        if (r != RESOURCE_COUNT)
            _inventory[r] = amount;
    }

    if (_inventory[FOOD] <= LOW_FOOD_THRESHOLD)
        _seekingFood = true;
    else if (_inventory[FOOD] >= SAFE_FOOD_THRESHOLD)
        _seekingFood = false;
}

void Brain::buildPlan(int forward, int side, const std::string &target)
{
    _plan.clear();
    if (side > 0) {
        _plan.push_back("Right");
        for (int k = 0; k < side; k++)
            _plan.push_back("Forward");
        _plan.push_back("Left");
    } else if (side < 0) {
        _plan.push_back("Left");
        for (int k = 0; k < -side; k++)
            _plan.push_back("Forward");
        _plan.push_back("Right");
    }
    for (int k = 0; k < forward; k++)
        _plan.push_back("Forward");
    _plan.push_back("Take " + target);
}

std::string Brain::nextCommand(const std::string &lookLine)
{
    if (!_plan.empty()) {
        std::string cmd = _plan.front();
        _plan.pop_front();
        return cmd;
    }

    std::vector<std::string> tiles = splitTiles(lookLine);

    if (tiles.empty())
        return "Forward";

    if (tileHas(tiles[0], "food"))
        return "Take food";

    std::string target = _seekingFood ? "food" : resourceToName(neededResource());

    if (!target.empty() && tileHas(tiles[0], target))
        return "Take " + target;

    if (!target.empty()) {
        size_t idx = 1;

        for (int i = 1; idx < tiles.size(); i++) {
            for (int j = -i; j <= i && idx < tiles.size(); j++, idx++) {
                if (!tileHas(tiles[idx], target))
                    continue;
                buildPlan(i, j, target);
                std::string cmd = _plan.front();
                _plan.pop_front();
                return cmd;
            }
        }
    }

    _wanderTurns++;
    if (_wanderTurns % 5 == 0)
        return "Right";
    return "Forward";
}
