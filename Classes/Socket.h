#pragma once

#include "cocos2d.h"
#include "network/SocketIO.h"

USING_NS_CC;
using namespace network;

class Socket : public SocketIO::SIODelegate
{
public:
    static Socket* getInstance();
    void connect();
    void send(const std::string& message);
    void disconnect();
    virtual void onConnect(SIOClient* client) {}
    virtual void onMessage(SIOClient* client, const std::string& data) {}
    virtual void onClose(SIOClient* client) {}
    virtual void onError(SIOClient* client, const std::string& data) {}
private:
    Socket();
    static Socket* instance;
    SIOClient* socket;
};

