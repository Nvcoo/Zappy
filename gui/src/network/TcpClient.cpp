#include "TcpClient.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

TcpClient::TcpClient(const std::string &host, int port)
    : sockfd(-1), connected(false)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket() failed: " << strerror(errno) << "\n";
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
        std::cerr << "inet_pton() failed for host " << host << "\n";
        return;
    }

    if (connect(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "connect() failed: " << strerror(errno) << "\n";
        return;
    }

    connected = true;

    // Make socket non-blocking
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
}

bool TcpClient::connectToServer() {
    return connected;
}

void TcpClient::send(const std::string &msg) {
    ::send(sockfd, msg.c_str(), msg.size(), 0);
}

std::vector<std::string> TcpClient::pollMessages() {
    std::vector<std::string> out;
    char buf[1024];

    int n = recv(sockfd, buf, sizeof(buf), 0);
    if (n <= 0) return out;

    buffer.append(buf, n);

    size_t pos;
    while ((pos = buffer.find('\n')) != std::string::npos) {
        std::string line = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);
        out.push_back(line);
    }

    return out;
}
