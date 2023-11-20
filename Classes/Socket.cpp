#include "Socket.h"
#include "MenuScene.h"
#include "Game2P_PVP.h"
#include <vector>
#include <sstream>

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

void Socket::onMessage(SIOClient* client, const std::string& msg)
{
    std::string data = msg.substr(1, msg.length() - 2);
    Scene* scene = Director::getInstance()->getRunningScene();
    std::string scene_name = scene->getName();
    log("%s", data.c_str());
    if (scene_name == "MenuScene")
    {
        if (data.find("finding") != std::string::npos)
        {
            Sleep(100);
            int first_sep = data.find_first_of(":");
            std::string game_type = data.substr(first_sep + 1);
            game_type.pop_back();
            Socket::getInstance()->send("match " + game_type);
        }
        else if (data.find("ready") != std::string::npos)
        {
            std::vector<std::string> split_str = split(data);
            std::string game_mode = split_str[1];
           //log("%s", split_str[2].substr(split_str[2].find_last_of("=")).c_str());
            int move_first = std::stoi(split_str[2].substr(split_str[2].find_last_of("=") + 1));
            int opponent_id = std::stoi(split_str[3].substr(split_str[3].find_last_of("=") + 1));

            //log("opponent id: %i", opponent_id);
            ((MenuScene*)scene)->run_1v1_PvP(opponent_id, move_first);
        }
    }
    else if (scene_name == "1v1 PvP")
    {
        std::vector<std::string> split_str = split(data);
        if (split_str[0].find("arrow") != std::string::npos)
        {
            int tag = stoi(split_str[1].substr(split_str[1].find("=") + 1));
            log("%i tag from opp", tag);
            ((Game2P_PvP*)scene)->set_arrow(tag);
        }
        else if (split_str[0].find("move") != std::string::npos)
        {
            ((Game2P_PvP*)scene)->spread_stones(true);
        }
        else if (split_str[0].find("state") != std::string::npos)
        {
            if (split_str[1].find("out_of_move") != std::string::npos)
            {
                ((Game2P_PvP*)scene)->opponent_out_of_move();
            }
            else if (split_str[1].find("end") != std::string::npos) ((Game2P_PvP*)scene)->showResult();
        }
        else if (split_str[0].find("request") != std::string::npos)
        {
            if (split_str[1].find("pause_request") != std::string::npos)
            {
                ((Game2P_PvP*)scene)->show_pause_request();
            }
            else if (split_str[1].find("resume") != std::string::npos)
            {
                ((Game2P_PvP*)scene)->resume_from_pause();
            }
            else if (split_str[1].find("pause_accept") != std::string::npos)
            {
                ((Game2P_PvP*)scene)->show_pause();
            }
            else if (split_str[1].find("run_away") != std::string::npos)
            {
                ((Game2P_PvP*)scene)->opponent_run_away();
            }
        }
        ((Game2P_PvP*)scene)->state = "";
    }
    
}

std::vector<std::string> split(std::string str)
{
    std::stringstream test(str);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(test, segment, ':'))
    {
        seglist.push_back(segment);
    }

    return seglist;
}