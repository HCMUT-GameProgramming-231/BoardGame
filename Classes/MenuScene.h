#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/cocosGUI.h"
//^^^^^^^^^^^^^^^^^^^^^^^^
#include <network/SocketIO.h>

using namespace cocos2d::network;
using namespace cocos2d::ui;
//VVVVVVVVVVVVVVVVVVVVVVVV

class MenuScene : public cocos2d::Scene, public SocketIO::SIODelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();


	// SIODelegate
	virtual void onConnect(SIOClient* client);
	virtual void onMessage(SIOClient* client, const std::string& data);
	virtual void onClose(SIOClient* client);
	virtual void onError(SIOClient* client, const std::string& data);

	CREATE_FUNC(MenuScene);

private:
	SIOClient* _client;
};

#endif // __HELLOWORLD_SCENE_H__
