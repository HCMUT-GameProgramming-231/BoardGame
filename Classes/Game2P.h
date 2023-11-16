#pragma once
#include "cocos2d.h"
#include "network/SocketIO.h"



class Game2P : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Game2P);

	std::string game_mode;
	bool isMyTurn;
};

