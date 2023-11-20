#pragma once

#include "cocos2d.h"
#include "Socket.h"
#include "Game2P_AI.h"

USING_NS_CC;



class Game2P_PvP : public Scene
{
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Game2P_PvP);

	void update_point();
	void update_stones_number();
	void update(float delta) override;
	void set_opponent_info(int id, bool move_first);
	void set_arrow(int tag);
	void spread_stones(bool first_spread = false);
	bool check_next_sq();
	void checkState();
	void showResult();
	void opponent_out_of_move();
	void show_pause_request();
	void wait_for_response(std::string type);
	void show_confirm_board();
	void opponent_run_away();
	void show_pause();
	void resume_from_pause();
	int opponent_id;
	std::string state;
	std::string opponent_state;
private:
	bool pausing;
	double time, last_update_time;
	bool spreading;
	std::string side;
	int mp, opp;
	bool myTurn;
	Sprite* pause_btn;
	Sprite* resume_btn;
	Sprite* back_btn;
	std::vector<cocos2d::Label*> stones_number_in_each_sq;
	std::vector<board_square> sq;
	std::vector<piece> hold;
	cocos2d::ActionInterval* floating;
	cocos2d::Label* myPoint;
	cocos2d::Label* opponentPoint;
	cocos2d::Label* whoseTurn;
	cocos2d::Label* holding_stone;
	cocos2d::Sprite* arrow, * left_arrow, * right_arrow;
};