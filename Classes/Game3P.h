#pragma once
#include "cocos2d.h"
#include "Game2P_AI.h"

USING_NS_CC;

class Game3P : public Scene
{
public:
	static Scene* createScene();

	virtual bool init();
	void update(float delta) override;
	void setup(int opponent_id, int move_order);
	std::string state;
	void set_arrow(int tag);
	void spread_stones(bool first_spread = false);
	void opponent_out_of_move();
	void show_pause_request();
	void resume_from_pause();
	void show_pause();
	void opponent_run_away();
	void show_confirm_board();
	void wait_for_response(std::string type);
	void showResult();
	bool check_next_sq();
	void update_point();
	void update_stones_number();
	void checkState();
	void AI_move();
	CREATE_FUNC(Game3P);
private:
	double time;
	double last_frame_time;
	bool spreading;
	bool pausing;
	int opponent_id;
	int myTurn;
	int current_turn;
	int ap, mp, opp;
	board_square sq[18];
	std::vector<piece> hold;
	ActionInterval* floating;
	Sprite* pause_btn;
	Sprite* resume_btn;
	Sprite* back_btn;
	Sprite* bottom_arrow;
	Sprite* left_arrow;
	Sprite* right_arrow;
	Sprite* arrow;
	cocos2d::Label* myPoint;
	cocos2d::Label* opponentPoint;
	cocos2d::Label* AI_point;
	cocos2d::Label* whoseTurn;
	cocos2d::Label* holding_stone;
	cocos2d::Label* stones_number_in_each_sq[18];
};

