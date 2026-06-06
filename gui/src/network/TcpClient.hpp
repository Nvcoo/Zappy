#pragma once
#include <string>
#include <vector>

class TcpClient {
public:
    TcpClient(const std::string &host, int port);

    bool connectToServer();
    void send(const std::string &msg);
    std::vector<std::string> pollMessages();

private:
    int sockfd;
    bool connected;
    std::string buffer;
};
