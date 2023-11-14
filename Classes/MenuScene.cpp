#include "MenuScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = MenuScene::create();

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// connect to server
	_client = SocketIO::connect("http://127.0.0.1:3200", *this);

	_client->on("message", CC_CALLBACK_2(MenuScene::onReceiveEvent, this));
	auto mouseEv = EventListenerMouse::create();
	mouseEv->onMouseDown = [&](EventMouse* ev) {
		auto btn = ev->getMouseButton();
		switch (btn)
		{
		case cocos2d::EventMouse::MouseButton::BUTTON_LEFT:
			_client->emit("message", "hello");
			break;
		default:
			break;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEv, this);
	if (_client == nullptr) log("failed");
	else log("success");
	return true;
}

void MenuScene::onConnect(SIOClient* client) {
	// SocketIO::connect success
}
void MenuScene::onMessage(SIOClient* client, const std::string& data) {
	// SocketIO::receive
	log("%s",data.c_str());
}
void MenuScene::onClose(SIOClient* client) {
	// SocketIO::disconnect success
}
void MenuScene::onError(SIOClient* client, const std::string& data) {
	// SocketIO::failed
}

void MenuScene::onReceiveEvent(SIOClient* client, const std::string& data) {
	
};