/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Client
*/

#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

#include <string>

typedef enum ClientType{
    PLAYER,
    GUI
} client_type_t;

class Client {
    private:
        int _fd;
        client_type_t _type;
        std::string _buffer;
        bool _overflow;
    protected:
    public:
        Client(int fd, client_type_t type) : _fd(fd), _type(type), _overflow(false) {};
        virtual ~Client() = default;
        int getFd() const;
        client_type_t getType() const;
        void appendToBuffer(const std::string &data);
        bool hasLine() const;
        bool hasOverflow() const;
        std::string popLine();
        void sendMessage(const std::string &message) const;
};

#endif
