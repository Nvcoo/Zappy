/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Client
*/

#include <iostream>
#include <unistd.h>
#include "../../include/network/Client.hpp"

int Client::getFd() const
{
    return _fd;
}

client_type_t Client::getType() const
{
    return _type;
}

void Client::appendToBuffer(const std::string &data)
{
    _buffer += data;
    if (_buffer.size() > 1024) {
        std::cerr << "Buffer overflow on fd " << _fd << std::endl;
        _buffer.clear();
    }
}

bool Client::hasLine() const
{
    return _buffer.find('\n') != std::string::npos;
}

std::string Client::popLine()
{
    size_t pos = _buffer.find('\n');
    std::string line = _buffer.substr(0, pos);

    _buffer.erase(0, pos + 1);
    return line;
}

void Client::sendMessage(const std::string &message) const
{
    write(_fd, message.c_str(), message.size());
}
