#pragma once
#include <string>
#include <vector>

class TcpClient {
public:
    TcpClient(const std::string &host, int port);
    ~TcpClient();

    bool connectToServer(int timeoutSeconds = 2);
    bool sendAll(const std::string &msg);
    std::vector<std::string> pollMessages();
    void closeSocket();

private:
    int sockfd;
    std::string host;
    int port;
    std::string buffer;
    bool connected;

    void setNonBlocking(bool on);
};
