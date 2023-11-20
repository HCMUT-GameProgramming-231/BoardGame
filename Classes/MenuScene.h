#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <network/SocketIO.h>
#include "Socket.h"

using namespace cocos2d::network;


class MenuScene : public cocos2d::Scene
{
public:


	static cocos2d::Scene* createScene();
	void show_matching_board();
	void run_1v1_PvP(int id, bool move_first);
	void run_1v1v1(int id, int move_order);
	void update(float delta) override;

	virtual bool init();
	CREATE_FUNC(MenuScene);


	int AI_mode;
	double time;
	std::string state;
};

#endif // __HELLOWORLD_SCENE_H__
