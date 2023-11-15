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

	auto background = Sprite::create("Assets/MenuScene/background.png");
	background->setAnchorPoint(Vec2(0, 0));
	background->setPosition(Vec2(0, 0));
	Size bgsize = background->getContentSize();
	background->setScale(visibleSize.width / bgsize.width, visibleSize.height / bgsize.height);
	this->addChild(background);
	// connect to server
	//_client = SocketIO::connect("192.168.1.8:3200", *this);
	
	//1v1 btn
	auto _1v1_btn = Label::createWithTTF("1vs1", "fonts/arial.ttf", 40);
	_1v1_btn->setTextColor(Color4B(255, 0, 0, 255));
	_1v1_btn->setPosition(130, 250);
	this->addChild(_1v1_btn);
	_1v1_btn->enableShadow();
	auto _1v1_btn_rect = _1v1_btn->getBoundingBox();

	//1v1vPC btn
	auto _1v1vPC_btn = Label::createWithTTF("1vs1vsPC", "fonts/arial.ttf", 40);
	_1v1vPC_btn->setTextColor(Color4B(255, 0, 0, 255));
	_1v1vPC_btn->setPosition(130, 200);
	this->addChild(_1v1vPC_btn);
	_1v1vPC_btn->enableShadow();
	auto _1v1vPC_btn_rect = _1v1vPC_btn->getBoundingBox();
	

	//how to play btn
	auto how_to_play_btn = Label::createWithTTF("How to play", "fonts/arial.ttf", 40);
	how_to_play_btn->setTextColor(Color4B(255, 0, 0, 255));
	how_to_play_btn->setPosition(130, 150);
	this->addChild(how_to_play_btn);
	how_to_play_btn->enableShadow();
	auto how_to_play_btn_rect = how_to_play_btn->getBoundingBox();

	//exit btn
	auto exit_btn = Label::createWithTTF("Exit", "fonts/arial.ttf", 40);
	exit_btn->setTextColor(Color4B(255, 0, 0, 255));
	exit_btn->setPosition(130, 100);
	this->addChild(exit_btn);
	exit_btn->enableShadow();
	auto exit_btn_rect = exit_btn->getBoundingBox();

	

	auto MouseEv = EventListenerMouse::create();

	//check if mouse is hovering on any button
	MouseEv->onMouseMove = [&, _1v1_btn_rect, _1v1_btn, _1v1vPC_btn_rect, _1v1vPC_btn,
								how_to_play_btn_rect, how_to_play_btn,
									exit_btn_rect, exit_btn](EventMouse* event)
	{
		auto MousePos = Vec2(event->getCursorX(), event->getCursorY());
		if (_1v1_btn_rect.containsPoint(MousePos))
		{
			_1v1_btn->disableEffect();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->enableShadow();
			exit_btn->enableShadow();
		}
		else if (_1v1vPC_btn_rect.containsPoint(MousePos))
		{
			_1v1_btn->enableShadow();
			_1v1vPC_btn->disableEffect();
			how_to_play_btn->enableShadow();
			exit_btn->enableShadow();
		}
		else if (how_to_play_btn_rect.containsPoint(MousePos))
		{
			_1v1_btn->enableShadow();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->disableEffect();
			exit_btn->enableShadow();
		}
		else if (exit_btn_rect.containsPoint(MousePos))
		{
			_1v1_btn->enableShadow();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->enableShadow();
			exit_btn->disableEffect();
		}
		else
		{
			_1v1_btn->enableShadow();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->enableShadow();
			exit_btn->enableShadow();
		}
	};

	//check which btn is clicked
	MouseEv->onMouseDown = [&, _1v1_btn_rect, _1v1_btn, _1v1vPC_btn_rect, _1v1vPC_btn,
								how_to_play_btn_rect, how_to_play_btn,
									exit_btn_rect, exit_btn](EventMouse* event)
	{
		auto MousePos = Vec2(event->getCursorX(), event->getCursorY());
		if (_1v1_btn_rect.containsPoint(MousePos))
		{
			log("1 vs 1 button pressed");
		}
		else if (_1v1vPC_btn_rect.containsPoint(MousePos))
		{
			log("1 vs 1 vs PC button pressed");
		}
		else if (how_to_play_btn_rect.containsPoint(MousePos))
		{
			log("How to play button pressed");
		}
		else if (exit_btn_rect.containsPoint(MousePos))
		{
			Director::getInstance()->end();
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseEv, this);

	
	

	//_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEv, this);
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
