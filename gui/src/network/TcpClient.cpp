#include "TcpClient.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <cstring>
#include <cerrno>
#include <iostream>

TcpClient::TcpClient(const std::string &host_, int port_)
    : sockfd(-1), host(host_), port(port_), buffer(), connected(false)
{
}

TcpClient::~TcpClient() {
    closeSocket();
}

// non-blocking so that the gui doesnt freeze waiting for the connection to succced
void TcpClient::setNonBlocking(bool on) {
    if (sockfd < 0) return;
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0) return;
    if (on) flags |= O_NONBLOCK;
    else flags &= ~O_NONBLOCK;
    fcntl(sockfd, F_SETFL, flags);
}

bool TcpClient::connectToServer(int timeoutSeconds) {
    if (connected) return true;

    // create IPv4 socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket() failed: " << strerror(errno) << "\n";
        return false;
    }

    // prepare IPv4 sockaddr
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    // convert numeric IPv4 string to binary
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
        std::cerr << "inet_pton() failed for host " << host << "\n";
        closeSocket();
        return false;
    }

    setNonBlocking(true);

    int c = connect(sockfd, (sockaddr*)&addr, sizeof(addr));
    if (c == 0) {
        // cnnected immediately
        connected = true;
        return true;
    }

    if (c < 0 && errno == EINPROGRESS) {
        // wait for connection to complete
        fd_set wfds; //write file descs
        FD_ZERO(&wfds);
        FD_SET(sockfd, &wfds);

        struct timeval tv;
        tv.tv_sec = timeoutSeconds;
        tv.tv_usec = 0;

        // select waits until socket is ready
        int sel = select(sockfd + 1, nullptr, &wfds, nullptr, &tv);
        if (sel > 0 && FD_ISSET(sockfd, &wfds)) { // file desc is set
            int err = 0;
            socklen_t len = sizeof(err);
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &err, &len) < 0 || err != 0) {
                std::cerr << "connect() failed: " << strerror(err) << "\n";
                closeSocket();
                return false;
            }
            connected = true;
            return true;
        }

        std::cerr << "connect() timeout or select() error\n";
        closeSocket();
        return false;
    }

    // Immediate error
    std::cerr << "connect() failed: " << strerror(errno) << "\n";
    closeSocket();
    return false;
}

// loops until all is sentt
bool TcpClient::sendAll(const std::string &msg) {
    if (!connected || sockfd < 0) return false;

    size_t total = 0;
    const char *data = msg.c_str();
    size_t len = msg.size();

    while (total < len) {
        ssize_t n = ::send(sockfd, data + total, len - total, 0);
        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                usleep(1000);
                continue;
            }
            std::cerr << "send() failed: " << strerror(errno) << "\n";
            closeSocket();
            return false;
        }
        total += static_cast<size_t>(n);
    }
    return true;
}

// stores mssg in buff until /n is found
// vector cus server can send multiple complete mssgs in a single read
std::vector<std::string> TcpClient::pollMessages() {
    std::vector<std::string> out;
    if (!connected || sockfd < 0) return out;

    char buf[2048];
    while (true) {
        ssize_t n = recv(sockfd, buf, sizeof(buf), 0);
        if (n > 0) {
            buffer.append(buf, static_cast<size_t>(n));

            size_t pos;
            while ((pos = buffer.find('\n')) != std::string::npos) {
                std::string line = buffer.substr(0, pos);
                if (!line.empty() && line.back() == '\r')
                    line.pop_back();
                out.push_back(line);
                buffer.erase(0, pos + 1);
            }
            continue;
        } else if (n == 0) {
            std::cerr << "Server closed connection\n";
            closeSocket();
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            if (errno == EINTR)
                continue;

            std::cerr << "recv() error: " << strerror(errno) << "\n";
            closeSocket();
            break;
        }
    }
    return out;
}

void TcpClient::closeSocket() {
    if (sockfd >= 0) {
        close(sockfd);
        sockfd = -1;
    }
    connected = false;
}
