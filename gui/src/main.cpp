#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "network/TcpClient.hpp"

static void print_usage(const char *prog) {
    std::cout << "USAGE: " << prog << " -p port -h machine\n"
              << "  -p port     : server port\n"
              << "  -h machine  : server hostname or IP\n"
              << "  --help      : show this message\n";
}

static bool parse_port(const char *s, int &out) {
    if (!s) return false;
    char *end = nullptr;
    long v = std::strtol(s, &end, 10);
    if (end == s || *end != '\0') return false;
    if (v < 1 || v > 65535) return false;
    out = static_cast<int>(v);
    return true;
}

static bool parse_arguments(int argc, char **argv, std::string &host, int &port) {
    bool have_host = false;
    bool have_port = false;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-?") == 0) {
            print_usage(argv[0]);
            std::exit(0);
        }

        if (std::strcmp(argv[i], "-p") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Error: -p requires an argument\n";
                return false;
            }
            if (!parse_port(argv[++i], port)) {
                std::cerr << "Error: invalid port: " << argv[i] << "\n";
                return false;
            }
            have_port = true;
            continue;
        }

        if (std::strcmp(argv[i], "-h") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Error: -h requires an argument\n";
                return false;
            }
            host = argv[++i];
            have_host = true;
            continue;
        }

        std::cerr << "Unknown option: " << argv[i] << "\n";
        return false;
    }

    if (!have_host || !have_port) {
        std::cerr << "Error: both -p and -h are required\n";
        print_usage(argv[0]);
        return false;
    }

    return true;
}

int main(int argc, char **argv) {
    std::string host;
    int port = 0;

    if (!parse_arguments(argc, argv, host, port)) {
        return 1;
    }

    std::cout << "Connecting to " << host << ":" << port << " ...\n";

    TcpClient client(host, port);
    if (!client.connectToServer()) {
        std::cerr << "Failed to create socket or connect\n";
        return 1;
    }

    // identify as GUI
    client.send("GRAPHIC\n");

    // minimal loop to show messages
    while (true) {
        auto messages = client.pollMessages();
        for (auto &m : messages) {
            std::cout << "SERVER: " << m << std::endl;
        }
        usleep(10 * 1000); // 10 ms
    }

    return 0;
}
