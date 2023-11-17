#pragma once
#include "cocos2d.h"
#include "network/SocketIO.h"

#define AI_MODE_HARD 15
#define AI_MODE_MEDIUM 10
#define AI_MODE_EASY 5

class board_square
{
public:
	cocos2d::Sprite* square;
	int current_stones;
};

class Game2P : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	void run_AI_mode(int mode);
	void run_PvP_mode();
	//void checkMousePos(cocos2d::Vec2 MousePos, std::vector<Node*> squares);
	void checkMousePos(cocos2d::Vec2 MousePos);
	void initBoard();
	void drawStones(bool init = false);
	virtual bool init();

	CREATE_FUNC(Game2P);

private:
	std::vector<board_square> squares;
	int AI_mode;
	bool isMyTurn;
	std::string state;
	cocos2d::Sprite* pause_btn;
	cocos2d::Sprite* resume_btn;
	cocos2d::Sprite* arrow;
	cocos2d::Sprite* left_arrow;
	cocos2d::Sprite* right_arrow;
};

