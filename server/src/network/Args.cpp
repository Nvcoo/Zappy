/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#include "../../include/network/Args.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <functional>

static void printUsage(const char *prog)
{
    std::cerr << "USAGE: " << prog << " -p port -x width -y height" " -n name1 name2 ... -c clientsNb -f freq" << std::endl;
}

static void checkArgs(const Args &args)
{
    if (args.port == 0 || args.width == 0 || args.height == 0 || args.clientsNb == 0 || args.teamNames.empty())
        throw std::invalid_argument("missing mandatory argument");

    std::unordered_set<std::string> parsed;
    for (const auto &name : args.teamNames) {
        if (name.empty())
            throw std::invalid_argument("team name can't be empty");
        if (name == "GRAPHIC")
            throw std::invalid_argument("'GRAPHIC' can't be used");
        if (!parsed.insert(name).second)
            throw std::invalid_argument("duplicate team: " + name);
    }
}

static std::vector<std::string> parseTeamNames(int &i, int ac, char **av)
{
    std::vector<std::string> names;
    i++;
    while (i < ac && av[i][0] != '-') {
        names.push_back(av[i]);
        i++;
    }
    i--;
    return names;
}

//We could use a normal map in this case since it's not too big, but apparently unordered_map is faster
Args parseArgs(int ac, char **av)
{
    using ArgHandler = std::function<void(Args &, const std::string &)>;
    static const std::unordered_map<std::string, ArgHandler> argMap = {
        {"-p", [](Args &a, const std::string &v) { a.port      = std::stoi(v); }},
        {"-x", [](Args &a, const std::string &v) { a.width     = std::stoi(v); }},
        {"-y", [](Args &a, const std::string &v) { a.height    = std::stoi(v); }},
        {"-c", [](Args &a, const std::string &v) { a.clientsNb = std::stoi(v); }},
        {"-f", [](Args &a, const std::string &v) { a.freq      = std::stoi(v); }},
    };
    Args args;

    args.port      = 0;
    args.width     = 0;
    args.height    = 0;
    args.clientsNb = 0;
    args.freq      = 100;
    try {
        for (int i = 1; i < ac; i++) {
            std::string flag = av[i];

            if (flag == "--help") {
                printUsage(av[0]);
                std::exit(0);
            }

            if (flag == "-n") {
                args.teamNames = parseTeamNames(i, ac, av);
                continue;
            }

            auto iterator = argMap.find(flag);
            if (iterator == argMap.end() || i + 1 >= ac) {
                throw std::invalid_argument("Incorrect flag writting: " + flag);
            }
            iterator->second(args, av[++i]);
        }
        checkArgs(args);
    } catch (const std::exception &e) {
        printUsage(av[0]);
        std::cerr << "Error: " << e.what() << std::endl;
        std::exit(84);
    }
    return args;
}