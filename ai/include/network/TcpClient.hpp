/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TcpClient
*/

#ifndef TCPCLIENT_HPP_
    #define TCPCLIENT_HPP_

#include <iostream>
#include <string>
#include <unistd.h>

class TcpClient {
    private:
        int _socket;
        std::string _buffer;
    protected:
    public:
        TcpClient() : _socket(-1)
        {
        }
        ~TcpClient()
        {
            if (_socket >= 0)
                close(_socket);
        }
        void sendMessage(const std::string &msg)
        {
            if (write(_socket, msg.c_str(), msg.size()) < 0)
                std::cerr << "Failed to send message" << std::endl;
        }
        bool connectTo(const std::string &host, int port);
        bool receiveLine(std::string &line);
};

#endif
