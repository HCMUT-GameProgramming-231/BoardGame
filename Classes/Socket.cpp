#include "Socket.h"

Socket* Socket::instance = nullptr;

Socket* Socket::getInstance() {
    if (instance == nullptr) {
        instance = new Socket();
    }
    return instance;
}

void Socket::connect() {
    socket = SocketIO::connect("127.0.0.1:3200", *this);
}

void Socket::send(const std::string& message) {
    socket->emit("message", message);
}

void Socket::disconnect() {
    socket->disconnect();
}

Socket::Socket() {
    socket = nullptr;
}