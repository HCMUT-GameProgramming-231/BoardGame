#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <network/SocketIO.h>

using namespace cocos2d::network;


class MenuScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();


	// SIODelegate
	CREATE_FUNC(MenuScene);

	std::string AI_mode;
	std::string state;
};

#endif // __HELLOWORLD_SCENE_H__
