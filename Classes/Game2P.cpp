#include "Game2P.h"
#include "Socket.h"
#include "MenuScene.h"
#include "AudioEngine.h"
#include <filesystem>
namespace fs = std::filesystem;

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
	this->state = "running";

	//background
	auto background = Sprite::create("Assets/GameScene/background.png");
	background->setAnchorPoint(Vec2(0, 0));
	background->setPosition(Vec2(0, 0));
	Size bgsize = background->getContentSize();
	background->setScale(visibleSize.width / bgsize.width, visibleSize.height / bgsize.height);
	this->addChild(background);

	//board
	auto board = Sprite::create("Assets/GameScene/2PlayerBoard_new.png");
	board->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	board->setScale(2);
	this->addChild(board);

	//Point
	auto your_point = Label::createWithTTF("Your point: 0", "fonts/arial.ttf", 50);
	your_point->setTextColor(Color4B(0, 0, 0, 255));
	your_point->setPosition(visibleSize.width * 2 / 5, visibleSize.height / 5);
	your_point->setName("your_point");
	this->addChild(your_point);

	auto opponent_point  = Label::createWithTTF("Opponen point: 0", "fonts/arial.ttf", 50);
	opponent_point->setTextColor(Color4B(0, 0, 0, 255));
	opponent_point->setPosition(visibleSize.width * 3 / 5, visibleSize.height * 4 / 5);
	opponent_point->setName("opponent_point");
	this->addChild(opponent_point);

	//pause button
	pause_btn = Sprite::create("Assets/GameScene/pause_btn.png");
	pause_btn->setPosition(50, 50);
	pause_btn->setName("pause");
	pause_btn->retain();
	this->addChild(pause_btn);

	//resume btn
	resume_btn = Sprite::create("Assets/GameScene/resume_btn.png");
	resume_btn->setPosition(50, 50);
	resume_btn->setName("resume");
	resume_btn->retain();

	//back to menu btn
	auto back_btn = Sprite::create("Assets/GameScene/back.png");
	back_btn->setPosition(50, visibleSize.height - 50);
	back_btn->setName("back");
	this->addChild(back_btn);

	//init  board squares
	Vec2 init_pos = Vec2(visibleSize.width / 4.8 + 5, visibleSize.height * 3 / 5 + 5);
	auto rect_size = Size(visibleSize.width / 7.5, visibleSize.height / 5 - 10);
	auto x_step = rect_size.width + 5;
	auto y_step = rect_size.height + 13;
	int count = 0;
	
	

	for (int i = 0; i < 10; i++, count++)
	{
		if (count == 5)
		{
			count = 0;
			init_pos.y -= y_step ;
		}
		auto r = Sprite::create();
		r->setContentSize(rect_size);
		//r->setTextureRect(Rect(0, 0, rect_size.width, rect_size.height));
		r->setPosition(init_pos.x + count * x_step, init_pos.y);
		r->setTag(i);
		r->retain();
		auto sq = board_square();
		sq.square = r;
		sq.current_stones = 5;
		squares.push_back(sq);
		//this->addChild(r);

		auto label = Label::createWithTTF(std::to_string(5), "fonts/arial.ttf", 20);
		label->setTextColor(Color4B(0, 0, 0, 255));
		label->setPosition(init_pos.x + count * x_step + 55, init_pos.y - 65);
		this->addChild(label);
		label->setName("stones at " + std::to_string(i));
	}

	//rect at 2 side
	rect_size = Size(visibleSize.width / 7.5, visibleSize.height / 5 - 10);
	auto rect1 = Sprite::create();
	rect1->setContentSize(rect_size);
	//rect1->setTextureRect(Rect(0, 0, rect_size.width, rect_size.height));
	rect1->setPosition(init_pos.x - rect_size.width, init_pos.y + y_step / 2);
	rect1->setTag(10);
	//this->addChild(rect1);
	rect1->retain();
	auto sq1 = board_square();
	sq1.square = rect1;
	sq1.current_stones = 5;
	squares.push_back(sq1);
	auto labelsq1 = Label::createWithTTF(std::to_string(5), "fonts/arial.ttf", 20);
	labelsq1->setTextColor(Color4B(0, 0, 0, 255));
	labelsq1->setPosition(init_pos.x - rect_size.width + 55, init_pos.y + y_step / 2 - 65);
	this->addChild(labelsq1);
	labelsq1->setName("stones at " + std::to_string(10));

	auto rect2 = Sprite::create();
	rect2->setContentSize(rect_size);
	//rect2->setTextureRect(Rect(0, 0, rect_size.width, rect_size.height));
	rect2->setPosition(init_pos.x + 5*rect_size.width + 30, init_pos.y + y_step / 2);
	//this->addChild(rect2);
	rect2->setTag(11);
	rect2->retain();
	auto sq2 = board_square();
	sq2.square = rect2;
	sq2.current_stones = 5;
	squares.push_back(sq2);
	auto labelsq2 = Label::createWithTTF(std::to_string(5), "fonts/arial.ttf", 20);
	labelsq2->setTextColor(Color4B(0, 0, 0, 255));
	labelsq2->setPosition(init_pos.x + 5 * rect_size.width + 30 + 55, init_pos.y + y_step / 2 - 65);
	this->addChild(labelsq2);
	labelsq2->setName("stones at " + std::to_string(10));
	
	//arrow
	arrow = Sprite::create("Assets/GameScene/arrow.png");
	arrow->setContentSize(Size(100, 150));
	arrow->retain();
	arrow->setName("arrow");
	
	left_arrow = Sprite::create("Assets/GameScene/left_arrow.png");
	left_arrow->setContentSize(Size(50, 70));
	left_arrow->retain();
	left_arrow->setName("left_arrow");

	right_arrow = Sprite::create("Assets/GameScene/right_arrow.png");
	right_arrow->setContentSize(Size(50, 70));
	right_arrow->retain();
	right_arrow->setName("right_arrow");

	//get stone sprite
	// load the Sprite Sheet
	auto spritecache = SpriteFrameCache::getInstance();
	// the .plist file can be generated with any of the tools mentioned below
	spritecache->addSpriteFramesWithFile("Assets/GameScene/stones_sheet.plist");

	//init board
	this->initBoard();

	//default listener
	auto MouseEv = EventListenerMouse::create();
	MouseEv->onMouseDown = [&, back_btn](EventMouse* event)
	{
		AudioEngine::play2d("Sound/click.mp3");
		auto MousePos = Vec2(event->getCursorX(), event->getCursorY());
		if (back_btn->getBoundingBox().containsPoint(MousePos))
		{
			//add confirm box

			//return to menu (temporary)
			auto menu = MenuScene::createScene();
			Director::getInstance()->replaceScene(menu);
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseEv, this);

	return true;
}

void Game2P::run_AI_mode(int mode)
{
	//event listener
	auto MouseEv = EventListenerMouse::create();
	MouseEv->onMouseDown = [&](EventMouse* event) {
		auto MousePos = Vec2(event->getCursorX(), event->getCursorY());
		if (auto btn = this->getChildByName("pause"))
		{
			auto r = btn->getBoundingBox();
			if (r.containsPoint(MousePos))
			{
				this->removeChild(btn);
				this->addChild(resume_btn);
				this->state = "pausing";
				Director::getInstance()->pause();
			}
		}
		else if (auto btn = this->getChildByName("resume"))
		{
			auto r = btn->getBoundingBox();
			if (r.containsPoint(MousePos))
			{
				this->removeChild(btn);
				this->addChild(pause_btn);
				this->state = "running";
				Director::getInstance()->resume();
			}
		}

		auto left_arrow = this->getChildByName("left_arrow");
		auto right_arrow = this->getChildByName("right_arrow");
		if (left_arrow && left_arrow->getBoundingBox().containsPoint(MousePos))
		{
			log("left arrow click");
		}
		else if (right_arrow && right_arrow->getBoundingBox().containsPoint(MousePos))
		{
			log("right arrow click");
		}
		else
		{
			for (int i = 5; i < 10; i++)
			{
				auto board_rect = this->squares[i].square->getBoundingBox();
				//log("%i", board_rect.containsPoint(MousePos));
				if (board_rect.containsPoint(MousePos))
				{
					float MidX = board_rect.getMidX(), MidY = board_rect.getMidY();
					this->arrow->setPosition(MidX, MidY + 30);
					this->removeChildByName("left_arrow");
					this->removeChildByName("right_arrow");
					this->right_arrow->setPosition(MidX + 40, MidY);
					this->left_arrow->setPosition(MidX - 40, MidY);
					this->addChild(this->right_arrow);
					this->addChild(this->left_arrow);
					if (this->getChildByName("arrow") == nullptr)
					{
						this->addChild(arrow);
					}
					arrow->stopAllActions();
					auto float_up = MoveBy::create(1, Vec2(0, 50));
					auto float_down = MoveBy::create(1, Vec2(0, -50));
					auto seq = Sequence::create(float_up, float_down, nullptr);
					auto repeat = RepeatForever::create(seq);
					//this->addChild(arrow);
					arrow->runAction(repeat);
				}
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseEv, this);
}

void Game2P::run_PvP_mode()
{
	
}

void Game2P::initBoard()
{
	this->drawStones(true);
}

void Game2P::drawStones(bool init)
{
	for (int i = 0; i < 10; i++)
	{
		auto r = squares[i].square->getBoundingBox();
		float minX = r.getMinX() + 25, maxX = r.getMaxX() - 25, minY = r.getMinY() + 25, maxY = r.getMaxY() - 25;
		for (int z = 0; z < squares[i].current_stones; z++)
		{
			float xPos = RandomHelper::random_real(minX, maxX);
			float yPos = RandomHelper::random_real(minY, maxY);
			int stone_index = RandomHelper::random_int(1, 5);
			auto stone = Sprite::createWithSpriteFrameName("stone" + std::to_string(stone_index) + ".png");
			auto size = stone->getContentSize();
			stone->setScale(50 / size.width, 50 / size.height);
			stone->setPosition(xPos, yPos);
			this->addChild(stone);
		}
	}
	if (init == true)
	{
		for (int i = 10; i < 12; i++)
		{
			auto r = squares[i].square->getBoundingBox();
			float minX = r.getMinX() + 25, maxX = r.getMaxX() - 25, minY = r.getMinY() + 25, maxY = r.getMaxY() - 25;
			float centerX = r.getMidX();
			float centerY = r.getMidY();
			auto big_stone = Sprite::createWithSpriteFrameName("stone" + std::to_string(6) + ".png");
			auto size = big_stone->getContentSize();
			big_stone->setScale(150 / size.width, 150 / size.height);
			big_stone->setPosition(centerX, centerY);
			this->addChild(big_stone);

			for (int z = 0; z < 4; z++)
			{

				float xPos = RandomHelper::random_real(minX, maxX);
				float yPos = RandomHelper::random_real(minY, maxY);
				int stone_index = RandomHelper::random_int(1, 5);
				auto stone = Sprite::createWithSpriteFrameName("stone" + std::to_string(stone_index) + ".png");
				auto size = stone->getContentSize();
				stone->setScale(50 / size.width, 50 / size.height);
				stone->setPosition(xPos, yPos);
				this->addChild(stone);
			}
		}
	}
	else
	{

	}
}

void Game2P::checkMousePos(Vec2 MousePos)
{
	
}
