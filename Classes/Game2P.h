#pragma once
#include "cocos2d.h"
#include "network/SocketIO.h"

#define AI_MODE_HARD 15
#define AI_MODE_MEDIUM 10
#define AI_MODE_EASY 5

class piece
{
public:
	int point;
	int sq_num;
	cocos2d::Sprite* sprite;
};

class board_square
{
public:
	cocos2d::Sprite* square;
	std::vector<piece> pieces;
};

class Game2P : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	void run_AI_mode(int mode);
	void run_PvP_mode();
	virtual bool init();
	void move_stones_left(bool first_move = false);
	//void move_stones_right(bool first_move = false);
	void update(float fps) override;
	void update_point();
	void update_stones_number();
	bool check_next_sq();
	CREATE_FUNC(Game2P);

private:
	bool myTurn;
	std::string mode;
	bool spreading = false;
	int direction;
	std::vector<cocos2d::Label*> stones_number_in_each_sq;
	std::vector<board_square> sq;
	std::vector<piece> hold;
	int mp = 0, opp = 0;
	cocos2d::Label* myPoint;
	cocos2d::Label* opponentPoint;
	cocos2d::Sprite* pause_btn;
	cocos2d::Sprite* resume_btn;
	cocos2d::Sprite* arrow, *left_arrow, *right_arrow;
	cocos2d::ActionInterval* floating;
	double time;
	double last_time;
};

