/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TcpClient
*/

#include "../../include/network/TcpClient.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

bool TcpClient::connectTo(const std::string &host, int port)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    struct sockaddr_in serverAddr;

    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        struct hostent *server = gethostbyname(host.c_str()); //owned by the C library's internal static buffer, not us: must not be freed/wrapped in a smart pointer

        if (server == nullptr) {
            std::cerr << "Unknown host: " << host << std::endl;
            return false;
        }
        std::memcpy(&serverAddr.sin_addr, server->h_addr, server->h_length);
    }

    if (connect(_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to connect to " << host << ":" << port << std::endl;
        return false;
    }

    std::cout << "Connected to " << host << ":" << port << std::endl;
    _connected = true;
    return true;
}

//there's no need to implement buffer overflow policy for the GUI.
bool TcpClient::receiveLine(std::string &line)
{
    size_t pos = _buffer.find('\n');

    if (pos == std::string::npos) {
        char buf[4096];
        ssize_t n = recv(_socket, buf, sizeof(buf), MSG_DONTWAIT); //we make this call non-blocking

        if (n > 0)
            _buffer.append(buf, n);
        else if (n == 0 || (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) //closed connection
            _connected = false;
        pos = _buffer.find('\n');
        if (pos == std::string::npos)
            return false;
    }
    line = _buffer.substr(0, pos);
    _buffer.erase(0, pos + 1);
    return true;
}
