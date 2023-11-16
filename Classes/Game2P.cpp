#include "Game2P.h"
#include "Socket.h"

USING_NS_CC;
using namespace network;
Scene* Game2P::createScene()
{
	return Game2P::create();
}

bool Game2P::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//background
	auto background = Sprite::create("Assets/GameScene/background.png");
	background->setAnchorPoint(Vec2(0, 0));
	background->setPosition(Vec2(0, 0));
	Size bgsize = background->getContentSize();
	background->setScale(visibleSize.width / bgsize.width, visibleSize.height / bgsize.height);
	this->addChild(background);

	//board
	auto board = Sprite::create("Assets/GameScene/2PlayerBoard.png");
	board->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	board->setScale(2);
	this->addChild(board);

	//event listener
	auto MouseEv = EventListenerMouse::create();
	MouseEv->onMouseDown = [&](EventMouse* event) {
		Socket::getInstance()->send("Clicked from Game scene");
	};
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseEv, this);
	return true;
}
