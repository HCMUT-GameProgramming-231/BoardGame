#include "Game2P_AI.h"
#include "MenuScene.h"
#include "AudioEngine.h"
#include "Minimax.h"

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

	//get screen size
	auto screen_size = Director::getInstance()->getVisibleSize();

	//BEGIN BACKGROUND -------------------------------------------------------------------------------------------
	//background
	auto bg = Sprite::create("Assets/GameScene/background.png");
	auto bg_size = bg->getContentSize();
	bg->setScale(screen_size.width / bg_size.width, screen_size.height / bg_size.height);
	bg->setPosition(screen_size.width / 2, screen_size.height / 2);
	this->addChild(bg);
	//END BACKGROUND --------------------------------------------------------------------------------------------

	//BEGIN BUTTONS ---------------------------------------------------------------------------------------------
	
	//back button
	auto back_btn = Sprite::create("Assets/GameScene/back.png");
	back_btn->setPosition(50, screen_size.height - 50);
	back_btn->setName("back");
	this->addChild(back_btn);


	//pause button
	pause_btn = Sprite::create("Assets/GameScene/pause_btn.png");
	pause_btn->setPosition(50, 50);
	pause_btn->setName("pause");
	pause_btn->retain();
	this->addChild(pause_btn);

	//resume button
	resume_btn = Sprite::create("Assets/GameScene/resume_btn.png");
	resume_btn->setPosition(50, 50);
	resume_btn->setName("resume");
	resume_btn->retain();

	//END BUTTONS -----------------------------------------------------------------------------------------------

	//BEGIN BOARD -----------------------------------------------------------------------------------------------
	auto board = Sprite::create("Assets/GameScene/2PlayerBoard.png");
	board->setPosition(screen_size.width / 2, screen_size.height / 2);
	board->setScale(2);
	this->addChild(board);

	//init board square
	float init_x = screen_size.width / 4.5 - 10;
	float init_y = screen_size.height / 1.75 + 30;
	auto sq_size = Size(screen_size.width / 7.5, screen_size.height / 5.5);
	float step_x = sq_size.width + 5;
	float step_y = sq_size.height + 20;
	int count = 0;
	for (int i = 0; i < 12; i++)
	{
		auto temp = Sprite::create();
		temp->setTextureRect(Rect(0, 0, sq_size.width, sq_size.height));
		temp->setTag(i);
		temp->retain();
		auto square = board_square();
		auto label = Label::createWithTTF("", "fonts/arial.ttf", 30);
		label->setTextColor((Color4B(0, 0, 0, 255)));
		if (i == 0)
		{
			temp->setPosition(init_x - step_x, screen_size.height / 2);
			label->setPosition(init_x - step_x + 50, screen_size.height / 2 - 50);
			stones_number_in_each_sq.push_back(label);
			//this->addChild(temp);
			square.square = temp;
			sq.push_back(square);
			continue;
		}
		
		if (i == 6)
		{
			temp->setPosition(init_x + 5 * step_x, screen_size.height / 2);
			label->setPosition(init_x + 5 * step_x + 25, screen_size.height / 2 - 50);
			stones_number_in_each_sq.push_back(label);
			//this->addChild(temp);
			square.square = temp;
			sq.push_back(square);
			init_y -= step_y;
			count--;
			continue;
		}

		temp->setPosition(init_x + count * step_x, init_y);
		label->setPosition(init_x + count * step_x + 50, init_y - 50);
		stones_number_in_each_sq.push_back(label);
		//this->addChild(temp);
		square.square = temp;
		sq.push_back(square);
		if (i >= 6)
		{
			count--;
		}
		else
		{
			count++;
		}
	}

	//END BOARD -------------------------------------------------------------------------------------------------

	//BEGIN INIT PIECES -----------------------------------------------------------------------------------------
	//preload sprite frame
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Assets/GameScene/stones_sheet.plist");

	//normal piece
	int sq_index = 1;
	for (int i = 0; i < 50; )
	{
		int rand = RandomHelper::random_int(1, 5);
		std::string sprite_path = "stone" + std::to_string(rand) + ".png";
		//log("%s", sprite_path.c_str());
		piece p;
		p.sq_num = i;
		p.point = 1;
		p.sprite = Sprite::createWithSpriteFrameName(sprite_path);
		p.sprite->retain();
		auto size = p.sprite->getContentSize();
		p.sprite->setScale(50 / size.width, 50 / size.height);
		//p.sprite->setPosition(RandomHelper::random_int(0, (int)screen_size.width), RandomHelper::random_int(0, (int)screen_size.height));
		//this->addChild(p.sprite);
		sq[sq_index].pieces.push_back(p);
		i++;
		if (i % 5 == 0) sq_index++;
		if (i == 25) sq_index++;
		//log("%i", sq_index);
	}
	
	//big pieces
	piece p1;
	p1.sprite = Sprite::createWithSpriteFrameName("stone6.png");
	p1.point = 5;
	p1.sprite->retain();
	auto size = p1.sprite->getContentSize();
	p1.sprite->setScale(100 / size.width, 100 / size.height);
	sq[0].pieces.push_back(p1);

	piece p2;
	p2.sprite = Sprite::createWithSpriteFrameName("stone6.png");
	p2.point = 5;
	p2.sprite->retain();
	p2.sprite->setScale(100 / size.width, 100 / size.height);
	sq[6].pieces.push_back(p2);

	//init position
	for (int i = 0; i < 12; i++)
	{
		auto box = sq[i].square->getBoundingBox();
		auto minX = box.getMinX() + 25, maxX = box.getMaxX() - 25, minY = box.getMinY() + 25, maxY = box.getMaxY() - 25;
		if (i == 0 || i == 6)
		{
			minX += 25;
			maxX -= 25;
		}
		for (int z = 0; z < sq[i].pieces.size(); z++)
		{
			auto pos_x = RandomHelper::random_real(minX, maxX);
			auto pos_y = RandomHelper::random_real (minY, maxY);
			sq[i].pieces[z].sprite->setPosition(pos_x, pos_y);
			this->addChild(sq[i].pieces[z].sprite);
		}
	}

	//END INIT PIECES -------------------------------------------------------------------------------------------

	//BEGIN LABEL
	myPoint = Label::createWithTTF("My point: 0", "fonts/arial.ttf", 40);
	myPoint->setTextColor(Color4B(0, 0, 0, 255));
	myPoint->setPosition(screen_size.width / 3, screen_size.height / 5);
	this->addChild(myPoint);

	opponentPoint = Label::createWithTTF("Opponent point: 0", "fonts/arial.ttf", 40);
	opponentPoint->setTextColor(Color4B(0, 0, 0, 255));
	opponentPoint->setPosition(2 * screen_size.width / 3, 4 * screen_size.height / 5);
	this->addChild(opponentPoint);

	for (int i = 0; i < stones_number_in_each_sq.size(); i++)
	{
		this->addChild(stones_number_in_each_sq[i]);
	}

	whoseTurn = Label::createWithTTF("Your turn", "fonts/arial.ttf", 40);
	whoseTurn->setTextColor(Color4B(0, 0, 0, 255));
	whoseTurn->setPosition (screen_size.width / 2, screen_size.height / 7);
	this->addChild(whoseTurn);

	holding_stone = Label::createWithTTF("Holding: ", "fonts/arial.ttf", 40);
	holding_stone->setTextColor(Color4B(0, 0, 0, 255));
	holding_stone->setPosition( screen_size.width / 5, screen_size.height - 100);
	this->addChild(holding_stone);

	//END LABEL

	//BEGIN INIT ARROW ------------------------------------------------------------------------------------------
	arrow = Sprite::create("Assets/GameScene/arrow.png");
	arrow->setScale(0.25);
	this->addChild(arrow);
	arrow->retain();
	arrow->setPosition(-50, -50);

	left_arrow = Sprite::create("Assets/GameScene/left_arrow.png");
	left_arrow->setScale(0.25);
	this->addChild(left_arrow);
	left_arrow->retain();
	left_arrow->setPosition(-50, -50);

	right_arrow = Sprite::create("Assets/GameScene/right_arrow.png");
	right_arrow->setScale(0.25);
	this->addChild(right_arrow);
	right_arrow->retain();
	right_arrow->setPosition(-50, -50);

	//arrow floating action
	auto float_up = MoveBy::create(1, Vec2(0, 50));
	auto float_down = MoveBy::create(1, Vec2(0, -50));
	auto seq = Sequence::create(float_up, float_down, nullptr);
	floating = RepeatForever::create(seq);
	floating->retain();


	//END INIT ARROW --------------------------------------------------------------------------------------------

	this->scheduleUpdate();
	return true;
}

void Game2P::update(float delta)
{
	time += delta;
	//log("%f", time);
	if (mode == "AI")
	{
		if (time - last_time >= 0.15) {
			last_time = time;
			this->update_point();
			this->update_stones_number();
			if (this->spreading)
			{
				if (this->direction == -1) this->move_stones_left();
			}
			else
			{
				if (myTurn) whoseTurn->setString("Your turn to move");
				else whoseTurn->setString("Opponent turn to move");
			}

			this->checkState();
		}

		if (!myTurn && AI_move == false && time - wait_time > 2)
		{
			wait_time = time;
			int state[12][1];
			for (int i = 0; i < sq.size(); i++)
			{
				state[i][0] = sq[i].pieces.size();
			}
			int move = select_move(state, -1, mp, opp, AI_mode);

			log("move AI: %i", move);
			if (move != -1)
			{
				auto sq_ = sq[move].square;
				arrow->setPosition(sq_->getPosition());
				arrow->setTag(move);
				this->move_stones_left(true);
				AI_move = true;
				whoseTurn->setString("Moving opponent soils...");
			}
			else
			{
				myTurn = true;
			}
			
		}
		
	}
	
}

void Game2P::run_AI_mode(int mode)
{
	this->mode = "AI";
	this->AI_mode = mode;
	this->myTurn = true;
	auto MouseEv = EventListenerMouse::create();
	MouseEv->onMouseDown = [&](EventMouse* event)
	{
		AudioEngine::play2d("Sound/click.mp3");
		auto MousePos = Vec2(event->getCursorX(), event->getCursorY());
		//get all buttons
		auto back = this->getChildByName("back");
		auto pause = this->getChildByName("pause");
		auto resume = this->getChildByName("resume");
		if (back->getBoundingBox().containsPoint(MousePos))
		{
			auto menu = MenuScene::createScene();
			Director::getInstance()->replaceScene(menu);
		}
		else if (pause && pause->getBoundingBox().containsPoint(MousePos))
		{
			this->addChild(resume_btn);
			this->removeChild(pause);
			Director::getInstance()->pause();
		}
		else if (resume && resume->getBoundingBox().containsPoint(MousePos))
		{
			this->addChild(pause_btn);
			this->removeChild(resume);
			Director::getInstance()->resume();
		}
		else if (left_arrow->getBoundingBox().containsPoint(MousePos))
		{
			int tag = arrow->getTag();
			if (tag >= 7 && tag <= 11) this->move_stones_left(true);
		}
		else if (right_arrow->getBoundingBox().containsPoint(MousePos))
		{
			int tag = arrow->getTag();
			if (tag >= 7 && tag <= 11) this->move_stones_left();
		}
		else
		{
			if (this->spreading || !myTurn) return;
			//check if click on board
			for (int i = 7; i < 12; i++)
			{
				auto cur_sq = sq[i];
				auto box = cur_sq.square->getBoundingBox();
				if (box.containsPoint(MousePos))
				{
					if (cur_sq.pieces.size() == 0) break;
					//draw arrow at this sq
					arrow->setPosition(box.getMidX(), box.getMidY());
					arrow->stopAllActions();
					arrow->runAction(floating);
					arrow->setTag(cur_sq.square->getTag());

					left_arrow->setPosition(box.getMidX() - 50, box.getMidY() - 30);
					//right_arrow->setPosition(box.getMidX() + 50, box.getMidY() - 30);
					break;
				}
			}
		}

	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseEv, this);
}

void Game2P::run_PvP_mode()
{
	
}

void Game2P::move_stones_left(bool first_move)
{
	int tag = arrow->getTag();
	if (first_move)
	{
		
		for (int i = 0; i < sq[tag].pieces.size(); i++)
		{
			hold.push_back(sq[tag].pieces[i]);
			sq[tag].pieces[i].sprite->removeFromParent();
		}
		sq[tag].pieces.clear();
		this->direction = -1;
		this->spreading = true;
		tag += 1;
		if (tag == 12) tag = 0;
		arrow->setTag(tag);
		auto pos = sq[tag].square->getPosition();
		arrow->setPosition(pos);
		left_arrow->setPosition(-50, -50);
		right_arrow->setPosition(-50, -50);
	}
	else 
	{
		if (hold.size() == 0)
		{
			this->spreading = false;
			arrow->stopAllActions();
			arrow->runAction(floating);
			if (this->check_next_sq()) this->move_stones_left(true);
			else
			{
				if (AI_move) AI_move = false;
			}
			return;
		}
		auto cur_sq = sq[tag];
		auto p = hold[0];
		sq[tag].pieces.push_back(p);
		
		auto box = cur_sq.square->getBoundingBox();
		float minX = box.getMinX() + 25, maxX = box.getMaxX() - 25, minY = box.getMinY() + 25, maxY = box.getMaxY() - 25;
		auto pos_x = RandomHelper::random_real(minX, maxX);
		auto pos_y = RandomHelper::random_real(minY, maxY);
		p.sprite->setPosition(pos_x, pos_y);
		this->addChild(p.sprite);
		
		hold.erase(hold.begin());
		tag += 1;
		if (tag == 12) tag = 0;
		arrow->setTag(tag);
		arrow->setPosition(sq[tag].square->getPosition());
		this->removeChild(arrow);
		this->removeChild(left_arrow);
		this->removeChild(right_arrow);
		this->addChild(right_arrow);
		this->addChild(left_arrow);
		this->addChild(arrow);
	}

	if (myTurn)
	{
		whoseTurn->setString("Moving your soils...");
	}
	else
	{
		whoseTurn->setString("Moving opponent soils...");
	}
	holding_stone->setString("Holding: " + std::to_string(hold.size()));
}


void Game2P::update_point()
{
	myPoint->setString("My point: " + std::to_string(mp));
	opponentPoint->setString("Opponent point: " + std::to_string(opp));
}

void Game2P::update_stones_number()
{
	for (int i = 0; i < stones_number_in_each_sq.size(); i++)
	{
		//log("tag: %i %i", i, sq[i].pieces.size());
		stones_number_in_each_sq[i]->setString(std::to_string(sq[i].pieces.size()));
	//	log("size: %i", sq[i].pieces.size());
	}

	
}

bool Game2P::check_next_sq()
{
	int tag = arrow->getTag();
	int next_tag;
	if (this->direction == -1)
	{
		next_tag = tag + 1;
	}
	else
	{
		next_tag = tag - 1;
	}
	if (next_tag == 12) next_tag = 0;
	else if (next_tag == -1) next_tag = 11;

	//case 1
	if (sq[tag].pieces.size() != 0 && tag != 0 && tag != 6)
	{
		return true;
	}
	else
	{
		auto sqr = sq[next_tag];
		if (sqr.pieces.size() != 0)
		{
			for (int i = 0; i < sqr.pieces.size(); i++)
			{
				const auto &p = sqr.pieces[i];
				if (myTurn) mp += p.point;
				else opp += p.point;
				this->removeChild(p.sprite);
				p.sprite->release();
			}
			sq[next_tag].pieces.clear();
			//log("%i %i", next_tag, sqr.pieces.size());
		}
	}

	myTurn = !myTurn;

	return false;
}

void Game2P::checkState()
{
	//case 1: 0 con nuoc di
	bool p1_moves = true, p2_moves = true;
	for (int i = 1; i < 6; i++)
	{
		if (sq[i].pieces.size() != 0)
		{
			p1_moves = false;
			break;
		}
	}

	for (int i = 7; i < 12; i++)
	{
		if (sq[i].pieces.size() != 0)
		{
			p2_moves = false;
			
			break;
		}
	}

	if (p1_moves && p2_moves)
	{
		this->showResult();
	}
	else if (p1_moves)
	{
		if (myTurn && side == "up") myTurn = false;
	}
	else if (p2_moves)
	{
		if (myTurn && side == "down") myTurn = false;
	}
}

void Game2P::showResult()
{
	std::string res;
	if (mp > opp) res = "You won !";
	else if (mp == opp) res = "Draw !";
	else res = "Opponent won |";

	auto size = Director::getInstance()->getVisibleSize();

	auto result_table = Sprite::create("Assets/MenuScene/board.png");
	result_table->setTextureRect(Rect(0, 0, 400, 300));
	result_table->setPosition(size.width / 2, size.height / 2);
	this->addChild(result_table);

	auto label = Label::createWithTTF(res, "fonts/arial.ttf", 40);
	label->setTextColor(Color4B(0, 0, 0, 255));
	label->setPosition(200, 150);
	result_table->addChild(label);
}