#include "MenuScene.h"
#include "Game2P_AI.h"
#include "Game2P_PVP.h"
#include "Game3P.h"
#include "Socket.h"
#include "AudioEngine.h"
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

	this->AI_mode = AI_MODE_EASY;
	this->state = "";
	if (Socket::getInstance() == nullptr) log("NULL");



	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("Assets/MenuScene/background.png");
	background->setAnchorPoint(Vec2(0, 0));
	background->setPosition(Vec2(0, 0));
	Size bgsize = background->getContentSize();
	background->setScale(visibleSize.width / bgsize.width, visibleSize.height / bgsize.height);
	this->addChild(background);
	
	//BEGIN BUTTONS------------------------------------------------------------------

	//1v1 btn (AI)
	auto _1v1_AI_btn = Label::createWithTTF("1vs1 (AI)", "fonts/arial.ttf", 40);
	_1v1_AI_btn->setTextColor(Color4B(255, 0, 0, 255));
	_1v1_AI_btn->setPosition(130, 300);
	this->addChild(_1v1_AI_btn);
	_1v1_AI_btn->enableShadow();
	auto _1v1_AI_btn_rect = _1v1_AI_btn->getBoundingBox();

	//1v1 (pvp)
	auto _1v1_PVP_btn = Label::createWithTTF("1vs1 (PvP)", "fonts/arial.ttf", 40);
	_1v1_PVP_btn->setTextColor(Color4B(255, 0, 0, 255));
	_1v1_PVP_btn->setPosition(130, 250);
	this->addChild(_1v1_PVP_btn);
	_1v1_PVP_btn->enableShadow();
	auto _1v1_PVP_btn_rect = _1v1_PVP_btn->getBoundingBox();

	//1v1vPC btn
	auto _1v1vPC_btn = Label::createWithTTF("1vs1vsAI", "fonts/arial.ttf", 40);
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

	//setting btn
	auto setting_btn = Label::createWithTTF("Setting", "fonts/arial.ttf", 40);
	setting_btn->setTextColor(Color4B(255, 0, 0, 255));
	setting_btn->setPosition(130, 100);
	this->addChild(setting_btn);
	setting_btn->enableShadow();
	auto setting_btn_rect = setting_btn->getBoundingBox();

	//exit btn
	auto exit_btn = Label::createWithTTF("Exit", "fonts/arial.ttf", 40);
	exit_btn->setTextColor(Color4B(255, 0, 0, 255));
	exit_btn->setPosition(130, 50);
	this->addChild(exit_btn);
	exit_btn->enableShadow();
	auto exit_btn_rect = exit_btn->getBoundingBox();

	//END BUTTONS--------------------------------------------------------------------------

	//BEGIN MOUSE LISTENER-----------------------------------------------------------------
	auto MouseEv = EventListenerMouse::create();

	//check if mouse is hovering on any button
	MouseEv->onMouseMove = [&, _1v1_AI_btn_rect, _1v1_AI_btn, _1v1vPC_btn_rect, _1v1vPC_btn,
								_1v1_PVP_btn_rect, _1v1_PVP_btn,
									how_to_play_btn_rect, how_to_play_btn,
										setting_btn_rect, setting_btn,
											exit_btn_rect, exit_btn](EventMouse* event)
	{
		auto MousePos = Vec2(event->getCursorX(), event->getCursorY());
		if (_1v1_AI_btn_rect.containsPoint(MousePos))
		{
			_1v1_AI_btn->disableEffect();
			_1v1_PVP_btn->enableShadow();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->enableShadow();
			setting_btn->enableShadow();
			exit_btn->enableShadow();
		}
		else if (_1v1_PVP_btn_rect.containsPoint(MousePos))
		{
			_1v1_AI_btn->enableShadow();
			_1v1_PVP_btn->disableEffect();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->enableShadow();
			setting_btn->enableShadow();
			exit_btn->enableShadow();
		}
		else if (_1v1vPC_btn_rect.containsPoint(MousePos))
		{
			_1v1_AI_btn->enableShadow();
			_1v1_PVP_btn->enableShadow();
			_1v1vPC_btn->disableEffect();
			how_to_play_btn->enableShadow();
			setting_btn->enableShadow();
			exit_btn->enableShadow();
		}
		else if (how_to_play_btn_rect.containsPoint(MousePos))
		{
			_1v1_AI_btn->enableShadow();
			_1v1_PVP_btn->enableShadow();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->disableEffect();
			setting_btn->enableShadow();
			exit_btn->enableShadow();
		}
		else if (setting_btn_rect.containsPoint(MousePos))
		{
			_1v1_AI_btn->enableShadow();
			_1v1_PVP_btn->enableShadow();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->enableShadow();
			setting_btn->disableEffect();
			exit_btn->enableShadow();
		}
		else if (exit_btn_rect.containsPoint(MousePos))
		{
			_1v1_AI_btn->enableShadow();
			_1v1_PVP_btn->enableShadow();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->enableShadow();
			setting_btn->enableShadow();
			exit_btn->disableEffect();
		}
		else
		{
			_1v1_AI_btn->enableShadow();
			_1v1_PVP_btn->enableShadow();
			_1v1vPC_btn->enableShadow();
			how_to_play_btn->enableShadow();
			setting_btn->enableShadow();
			exit_btn->enableShadow();
		}
	};

	//check which btn is clicked
	MouseEv->onMouseDown = [&, _1v1_AI_btn_rect, _1v1vPC_btn_rect, _1v1_PVP_btn_rect,
								how_to_play_btn_rect, setting_btn_rect, exit_btn_rect](EventMouse* event)
	{
		AudioEngine::play2d("Sound/click.mp3");
		auto MousePos = Vec2(event->getCursorX(), event->getCursorY());
		Socket::getInstance()->send("hello");
		if (_1v1_AI_btn_rect.containsPoint(MousePos))
		{
			auto GameScene = (Game2P*) Game2P::createScene();
			GameScene->run_AI_mode(this->AI_mode);
			Director::getInstance()->replaceScene(GameScene);
		}
		else if (_1v1_PVP_btn_rect.containsPoint(MousePos) && this->state == "")
		{
			Socket::getInstance()->send("match 1v1");
			this->state = "matching";
			this->show_matching_board();
		}
		else if (_1v1vPC_btn_rect.containsPoint(MousePos))
		{
			//socket send matching 1v1v1
			Socket::getInstance()->send("match 1v1v1");
			this->state = "matching";
			this->show_matching_board();
		}
		else if (how_to_play_btn_rect.containsPoint(MousePos) && this->state=="")
		{
			auto howtoplay_board = this->getChildByName("setting board");
			if (howtoplay_board == nullptr)
			{
				auto frameSize = this->getContentSize();
				//how to play board
				auto howtoplay_board = Sprite::create("Assets/MenuScene/board.png");
				howtoplay_board->setPosition(frameSize.width / 2, frameSize.height / 2);
				//howtoplay_board->setScale(1.1);
				howtoplay_board->setName("how to play board");
				this->addChild(howtoplay_board);

				//how to play board label
				Size setting_size = howtoplay_board->getContentSize();
				auto label = Label::createWithTTF("How To Play", "fonts/arial.ttf", 40);
				label->setTextColor(Color4B(0, 0, 0, 255));
				//label->enableShadow();
				label->setPosition(setting_size.width / 2, setting_size.height - 40);
				label->setName("label");
				howtoplay_board->addChild(label);

				//close button
				auto close_btn = Sprite::create("Assets/MenuScene/close_btn.png");
				auto close_org_size = close_btn->getContentSize();
				close_btn->setScale(30 / close_org_size.width);
				close_btn->setPosition(setting_size.width - 25, setting_size.height - 25);
				close_btn->setName("close");
				howtoplay_board->addChild(close_btn);

				//content
				std::string content = "   Moi ben 5 o, moi luot chon 1 trong 5 o ben minh,\n   lay tat ca soi o o do rai theo chieu kim dong ho \
\n   (hoac nguoc chieu kim dong ho) moi o 1 vien, \n   rai het thi lay soi o o tiep theo rai tiep \
 cho toi khi \n   cham 1 o trong thi dung, neu o tiep theo o \n   trong do la 1 o chua soi thi an tat \
ca soi trong o\n   do. Tro choi ket thuc khi 2 quan bi an hoac \n   khong con soi tren ban co. \
1 quan = 5 diem, \n   1 dan = 1 diem";;
				auto content_label = Label::createWithTTF(content, "fonts/arial.ttf", 18);
				content_label->setTextColor(Color4B(0, 0, 0, 255));
				//label->enableShadow();
				content_label->setPosition(setting_size.width / 2, setting_size.height - 155);
				content_label->setName("label");
				howtoplay_board->addChild(content_label);

				this->state = "how to play opening";
			}
		}
		else if (setting_btn_rect.containsPoint(MousePos) && this->state=="")
		{
			auto setting_board = this->getChildByName("setting board");
			if (setting_board == nullptr)
			{
				auto frameSize = this->getContentSize();

				//setting board
				auto setting_board = Sprite::create("Assets/MenuScene/board.png");
				setting_board->setPosition(frameSize.width / 2, frameSize.height / 2);
				setting_board->setName("setting board");
				this->addChild(setting_board);

				//setting board label
				Size setting_size = setting_board->getContentSize();
				auto label = Label::createWithTTF("AI MODE", "fonts/arial.ttf", 40);
				label->setTextColor(Color4B(0, 0, 0, 255));
				//label->enableShadow();
				label->setPosition(setting_size.width / 2, setting_size.height - 40);
				label->setName("label");
				setting_board->addChild(label);

				//close button
				auto close_btn = Sprite::create("Assets/MenuScene/close_btn.png");
				auto close_org_size = close_btn->getContentSize();
				close_btn->setScale(30 / close_org_size.width);
				close_btn->setPosition(setting_size.width - 25, setting_size.height - 25);
				close_btn->setName("close");
				setting_board->addChild(close_btn);

				//Easy mode
				auto easy_label = Label::createWithTTF("Easy", "fonts/arial.ttf", 35);
				easy_label->setTextColor(Color4B(0, 0, 0, 255));
				easy_label->setPosition(setting_size.width / 2, setting_size.height - 100);
				easy_label->setName("easy");
				setting_board->addChild(easy_label);

				//Medium mode
				auto medium_label = Label::createWithTTF("Medium", "fonts/arial.ttf", 35);
				medium_label->setTextColor(Color4B(0, 0, 0, 255));
				medium_label->setPosition(setting_size.width / 2, setting_size.height - 150);
				medium_label->setName("medium");
				setting_board->addChild(medium_label);

				//Hard mode
				auto hard_label = Label::createWithTTF("Hard", "fonts/arial.ttf", 35);
				hard_label->setTextColor(Color4B(0, 0, 0, 255));
				hard_label->setPosition(setting_size.width / 2, setting_size.height - 200);
				hard_label->setName("hard");
				setting_board->addChild(hard_label);

				if (this->AI_mode == AI_MODE_EASY) easy_label->enableShadow();
				else if (this->AI_mode == AI_MODE_HARD) medium_label->enableShadow();
				else hard_label->enableShadow();

				this->state = "setting opening";
			}
		}
		else if (exit_btn_rect.containsPoint(MousePos))
		{
			Director::getInstance()->end();
		}
		else
		{
			//setting panel 
			if (this->state == "setting opening")
			{
				auto setting_board = this->getChildByName("setting board");
				auto setting_board_pos = setting_board->getBoundingBox().origin;
				MousePos.x -= setting_board_pos.x;
				MousePos.y -= setting_board_pos.y;
				auto children = setting_board->getChildren();
				int children_num = setting_board->getChildrenCount();
				for (const auto child : children)
				{
					auto rect = child->getBoundingBox();
					if (rect.containsPoint(MousePos))
					{
						auto name = child->getName();
						if (name == "close")
						{
							this->state = "";
							this->removeChild(setting_board);
						}
						else if (name == "easy")
						{
							this->AI_mode = AI_MODE_EASY;
							((Label*)child)->enableShadow();
							((Label*)setting_board->getChildByName("medium"))->disableEffect();
							((Label*)setting_board->getChildByName("hard"))->disableEffect();
						}
						else if (name == "medium")
						{
							this->AI_mode = AI_MODE_MEDIUM;
							((Label*)child)->enableShadow();
							((Label*)setting_board->getChildByName("easy"))->disableEffect();
							((Label*)setting_board->getChildByName("hard"))->disableEffect();
						}
						else
						{
							this->AI_mode = AI_MODE_HARD;
							((Label*)child)->enableShadow();
							((Label*)setting_board->getChildByName("medium"))->disableEffect();
							((Label*)setting_board->getChildByName("easy"))->disableEffect();
							
						}
					}
				}
				
			}
			else if (this->state == "how to play opening")
			{
				auto howtoplay_board = this->getChildByName("how to play board");
				auto howtoplay_board_pos = howtoplay_board->getBoundingBox().origin;
				MousePos.x -= howtoplay_board_pos.x;
				MousePos.y -= howtoplay_board_pos.y;
				auto close_rect = howtoplay_board->getChildByName("close")->getBoundingBox();
				if (close_rect.containsPoint(MousePos))
				{
					this->state = "";
					this->removeChild(howtoplay_board);
				}
			}
			else if (this->state == "matching")
			{
				auto matching_board = this->getChildByName("matching board");
				auto board_pos = matching_board->getBoundingBox().origin;
				MousePos.x -= board_pos.x;
				MousePos.y -= board_pos.y;
				auto close_rect = matching_board->getChildByName("close")->getBoundingBox();
				if (close_rect.containsPoint(MousePos))
				{
					Socket::getInstance()->send("Unmatch");
					this->state = "";
					this->removeChild(matching_board);
				}
			}
		}
	};

	

	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseEv, this);

	//END MOUSE LISTENER ------------------------------------------------------------------
	this->setName("MenuScene");

	this->scheduleUpdate();

	return true;
}


void MenuScene::show_matching_board()
{
	auto size = Director::getInstance()->getVisibleSize();
	auto board = Sprite::create("Assets/MenuScene/board.png");
	this->addChild(board);
	board->setPosition(size.width / 2, size.height / 2);
	board->setName("matching board");

	//close button
	Size setting_size = board->getContentSize();
	auto close_btn = Sprite::create("Assets/MenuScene/close_btn.png");
	auto close_org_size = close_btn->getContentSize();
	close_btn->setScale(30 / close_org_size.width);
	close_btn->setPosition(setting_size.width - 25, setting_size.height - 25);
	close_btn->setName("close");
	board->addChild(close_btn);

	//label
	auto label = Label::createWithTTF("Matching", "fonts/arial.ttf", 40);
	label->setTextColor(Color4B(0, 0, 0, 255));
	label->setPosition(setting_size.width / 2, setting_size.height / 2);
	label->setName("matching label");
	board->addChild(label);
	
}

void MenuScene::update(float delta)
{
	time += delta;
	if (state == "matching")
	{
		std::string dot;
		int num = (int)time;
		for (int i  = 0; i < num % 5; i++) dot += ".";
		((Label*)this->getChildByName("matching board")->getChildByName("matching label"))->setString("Matching" + dot);
	}
}

void MenuScene::run_1v1_PvP(int id, bool move_first)
{
	auto game = (Game2P_PvP *)Game2P_PvP::createScene();
	game->set_opponent_info(id, !move_first);
	Director::getInstance()->replaceScene(game);
}
void MenuScene::run_1v1v1(int id, int move_order)
{
	auto game = (Game3P*)Game3P::createScene();
	game->setup(id, move_order);
	Director::getInstance()->replaceScene(game);
}