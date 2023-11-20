#include "Game2P_PVP.h"
#include "AudioEngine.h"
#include "MenuScene.h"

Scene* Game2P_PvP::createScene()
{
	return Game2P_PvP::create();
}

bool Game2P_PvP::init()
{
	if (!Scene::init())
	{
		return false;
	}

	this->setName("1v1 PvP");

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
	auto board = Sprite::create("Assets/GameScene/2PlayerBoard_new.png");
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
			auto pos_y = RandomHelper::random_real(minY, maxY);
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
	whoseTurn->setPosition(screen_size.width / 2, screen_size.height / 7);
	this->addChild(whoseTurn);

	holding_stone = Label::createWithTTF("Holding: 0", "fonts/arial.ttf", 40);
	holding_stone->setTextColor(Color4B(0, 0, 0, 255));
	holding_stone->setPosition(screen_size.width / 5, screen_size.height - 100);
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

	//Event listener
	auto MouseEv = EventListenerMouse::create();
	MouseEv->onMouseDown = [&](EventMouse* event)
	{
		AudioEngine::play2d("Sound/click.mp3");
		if (this->getChildByName ( "end" ))
		{
			auto menu = MenuScene::createScene();
			Director::getInstance()->replaceScene(menu);
			Socket::getInstance()->send("end");
			return;
		}
		auto MousePos = Vec2(event->getCursorX(), event->getCursorY());
		std::string msg = "inMatchWith=" + std::to_string(opponent_id);
		std::string data;
		log("state: %s", state.c_str());
		if (state == "wait for server response") return;
		auto pause = this->getChildByName("pause");
		auto resume = this->getChildByName("resume");
		auto pause_request = this->getChildByName("pause request");
		auto back_btn = this->getChildByName("back");
		auto confirm_board = this->getChildByName("confirm board");
		if (confirm_board)
		{
			auto board_pos = confirm_board->getBoundingBox().origin;
			MousePos.x -= board_pos.x;
			MousePos.y -= board_pos.y;

			if (confirm_board->getChildByName("accept")->getBoundingBox().containsPoint(MousePos))
			{
				data = "type=request:content=run_away";
				auto menu = MenuScene::createScene();
				Director::getInstance()->replaceScene(menu);
			}
			else if (confirm_board->getChildByName("close")->getBoundingBox().containsPoint(MousePos))
			{
				this->removeChild(confirm_board);
			}

		}
		else if (back_btn->getBoundingBox().containsPoint(MousePos) && !spreading)
		{
			show_confirm_board();
		}
		else if (pause_request)
		{
			auto board_pos = pause_request->getBoundingBox().origin;
			MousePos.x -= board_pos.x;
			MousePos.y -= board_pos.y;

			if (pause_request->getChildByName("accept")->getBoundingBox().containsPoint(MousePos))
			{
				data = "type=request:content=pause_accept";
				this->state = "wait for server response";
			}
			else if (pause_request->getChildByName("close")->getBoundingBox().containsPoint(MousePos))
			{
				data = "type=request:content=pause_decline";
				this->state = "wait for server response";
			}
		}
		else if (pause && pause->getBoundingBox().containsPoint(MousePos) && !spreading)
		{
			data = "type=request:content=pause_request";
			this->wait_for_response("pause");
			this->state = "wait for server response";
		}
		else if (resume && resume->getBoundingBox().containsPoint(MousePos))
		{
			data = "type=request:content=resume";
			this->state = "wait for server response";
		}
		else if (myTurn && !pausing)
		{
			int begin, end;
			Sprite* move_arrow;
			if (side == "down")
			{
				begin = 7;
				end = 12;
				move_arrow = left_arrow;
			}
			else
			{
				begin = 1;
				end = 6;
				move_arrow = right_arrow;
			}
			//click on arrow
			if (move_arrow->getBoundingBox().containsPoint(MousePos))
			{
				data = "type=move:tag=" + std::to_string(arrow->getTag());
			}
			//click on square
			else
			{
				for (int i = begin; i < end; i++)
				{
					auto box = sq[i].square;
					auto rect = box->getBoundingBox();
					if (rect.containsPoint(MousePos) && sq[i].pieces.size() != 0)
					{
						auto pos = box->getPosition();
						arrow->setPosition(pos);
						if (side == "down") left_arrow->setPosition(pos.x - 40, pos.y - 30);
						else right_arrow->setPosition(pos.x + 40, pos.y - 30);
						arrow->stopAllActions();
						arrow->runAction(floating);
						arrow->setTag(box->getTag());
						data = "type=arrow:tag=" + std::to_string(box->getTag());
						break;
					}
				}
			}
			
		}
		
		

		if (data != "")
		{
			Socket::getInstance()->send(msg + ":" + data);
			state = "wait for server response";
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseEv, this);

	this->scheduleUpdate();

	return true;
}

void Game2P_PvP::set_opponent_info(int id, bool move_first)
{
	this->opponent_id = id;
	if (move_first) myTurn = false;
	else myTurn = true;

	if (myTurn)
	{
		this->side = "down";
	}
	else
	{
		this->side = "up";
	}
}

void Game2P_PvP::update(float delta)
{
	time += delta;
	if (time - last_update_time > 0.25)
	{
		//log("%s", state.c_str());
		last_update_time = time;
		update_point();
		update_stones_number();
		if (!spreading)
		{
			if (myTurn) whoseTurn->setString("Your turn to move");
			else whoseTurn->setString("Opponent turn to move");
		}
		else
		{
			this->spread_stones();
		}
	}
}

void Game2P_PvP::update_point()
{
	myPoint->setString("My point: " + std::to_string(mp));
	opponentPoint->setString("Opponent point: " + std::to_string(opp));
}

void Game2P_PvP::update_stones_number()
{
	for (int i = 0; i < stones_number_in_each_sq.size(); i++)
	{
		//log("tag: %i %i", i, sq[i].pieces.size());
		stones_number_in_each_sq[i]->setString(std::to_string(sq[i].pieces.size()));
		//	log("size: %i", sq[i].pieces.size());
	}
}

void Game2P_PvP::set_arrow(int tag)
{
	auto box = sq[tag].square;
	arrow->setPosition(box->getPosition());
	arrow->setTag(tag);
	arrow->stopAllActions();
	arrow->runAction(floating);
}

void Game2P_PvP::spread_stones(bool first_spread)
{
	int tag = arrow->getTag();
	if (first_spread)
	{

		for (int i = 0; i < sq[tag].pieces.size(); i++)
		{
			hold.push_back(sq[tag].pieces[i]);
			sq[tag].pieces[i].sprite->removeFromParent();
		}
		sq[tag].pieces.clear();
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
			if (this->check_next_sq()) this->spread_stones(true);
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

bool Game2P_PvP::check_next_sq()
{
	int tag = arrow->getTag();
	int next_tag;

	next_tag = tag + 1;

	if (next_tag == 12) next_tag = 0;

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
				const auto& p = sqr.pieces[i];
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
	checkState();

	return false;
}

void Game2P_PvP::checkState()
{
	int begin, end;
	int op_begin, op_end;
	if (side == "up")
	{
		begin = 1;
		end = 6;
		op_begin = 7;
		op_end = 12;
	}
	else
	{
		begin = 7;
		end = 12;
		op_begin = 1;
		op_end = 6;
	}

	bool have_move = false;
	bool have_move_op = false;
	for (int i = begin; i < end; i++)
	{
		
		if (sq[i].pieces.size() > 0)
		{
			have_move = true;
			break;
		}

	}
	
	for (int i = op_begin; i < op_end; i++)
	{
		if (sq[i].pieces.size() > 0)
		{
			have_move_op = true;
			break;
		}
	}

	log("opponent move: %i, my move: %i", have_move_op, have_move);

	if (!have_move && !have_move_op) showResult();
	else if (!myTurn && !have_move_op)  myTurn = true;
	else if (myTurn && !have_move) myTurn = false;
}

void Game2P_PvP::showResult()
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
	
	this->state = "end";
}

void Game2P_PvP::opponent_out_of_move()
{
	opponent_state = "out_of_move";
	myTurn = true;
}

void Game2P_PvP::show_pause_request()
{
	std::string res = "Opponent want to\n pause. Accept?";

	auto size = Director::getInstance()->getVisibleSize();

	auto result_table = Sprite::create("Assets/MenuScene/board.png");
	result_table->setTextureRect(Rect(0, 0, 400, 300));
	result_table->setPosition(size.width / 2, size.height / 2);
	this->addChild(result_table);
	result_table->setName("pause request");

	auto label = Label::createWithTTF(res, "fonts/arial.ttf", 40);
	label->setTextColor(Color4B(0, 0, 0, 255));
	label->setPosition(200, 150);
	result_table->addChild(label);

	auto board_size = result_table->getContentSize();

	//close button
	auto close_btn = Sprite::create("Assets/MenuScene/close_btn.png");
	auto close_org_size = close_btn->getContentSize();
	close_btn->setScale(30 / close_org_size.width);
	close_btn->setPosition(board_size.width - 25, board_size.height - 25);
	close_btn->setName("close");
	result_table->addChild(close_btn);

	//accept buttn
	auto accept_btn = Sprite::create("Assets/GameScene/accept.png");
	auto accept_org_size = accept_btn->getContentSize();
	accept_btn->setScale(50 / accept_org_size.width);
	accept_btn->setPosition(board_size.width / 2, 35);
	accept_btn->setName("accept");
	result_table->addChild(accept_btn);

}

void Game2P_PvP::wait_for_response(std::string type)
{
	auto size = Director::getInstance()->getVisibleSize();

	auto result_table = Sprite::create("Assets/MenuScene/board.png");
	result_table->setTextureRect(Rect(0, 0, 400, 300));
	result_table->setPosition(size.width / 2, size.height / 2);
	this->addChild(result_table);
	result_table->setName("waiting board");
	
	std::string res;
	if (type == "pause") res = "Wait for opponent\n response";

	auto label = Label::createWithTTF(res, "fonts/arial.ttf", 40);
	label->setAlignment(TextHAlignment::CENTER);
	label->setTextColor(Color4B(0, 0, 0, 255));
	label->setPosition(200, 150);
	result_table->addChild(label);
}

void Game2P_PvP::show_pause()
{
	this->removeChildByName("waiting board");
	this->removeChildByName("pause request");
	this->removeChild(pause_btn);
	this->addChild(resume_btn);

	pausing = true;
	auto size = Director::getInstance()->getVisibleSize();

	auto result_table = Sprite::create("Assets/MenuScene/board.png");
	result_table->setTextureRect(Rect(0, 0, 400, 300));
	result_table->setPosition(size.width / 2, size.height / 2);
	this->addChild(result_table);
	result_table->setName("pausing board");

	auto label = Label::createWithTTF("Pausing", "fonts/arial.ttf", 40);
	label->setAlignment(TextHAlignment::CENTER);
	label->setTextColor(Color4B(0, 0, 0, 255));
	label->setPosition(200, 150);
	result_table->addChild(label);
}

void Game2P_PvP::resume_from_pause()
{
	pausing = false;
	this->removeChildByName("pausing board");
	this->removeChild(resume_btn);
	this->addChild(pause_btn);
}

void Game2P_PvP::show_confirm_board()
{

	this->removeChildByName("waiting board");
	this->removeChildByName("pause request");

	auto size = Director::getInstance()->getVisibleSize();

	auto result_table = Sprite::create("Assets/MenuScene/board.png");
	result_table->setTextureRect(Rect(0, 0, 400, 300));
	result_table->setPosition(size.width / 2, size.height / 2);
	this->addChild(result_table);
	result_table->setName("confirm board");

	auto label = Label::createWithTTF("Run away?", "fonts/arial.ttf", 40);
	label->setAlignment(TextHAlignment::CENTER);
	label->setTextColor(Color4B(0, 0, 0, 255));
	label->setPosition(200, 150);
	result_table->addChild(label);

	auto board_size = result_table->getContentSize();

	auto close_btn = Sprite::create("Assets/MenuScene/close_btn.png");
	auto close_org_size = close_btn->getContentSize();
	close_btn->setScale(30 / close_org_size.width);
	close_btn->setPosition(board_size.width - 25, board_size.height - 25);
	close_btn->setName("close");
	result_table->addChild(close_btn);

	auto accept_btn = Sprite::create("Assets/GameScene/accept.png");
	auto accept_org_size = accept_btn->getContentSize();
	accept_btn->setScale(50 / accept_org_size.width);
	accept_btn->setPosition(board_size.width / 2, 35);
	accept_btn->setName("accept");
	result_table->addChild(accept_btn);
}

void Game2P_PvP::opponent_run_away()
{
	auto size = Director::getInstance()->getVisibleSize();

	auto result_table = Sprite::create("Assets/MenuScene/board.png");
	result_table->setTextureRect(Rect(0, 0, 400, 300));
	result_table->setPosition(size.width / 2, size.height / 2);
	this->addChild(result_table);
	result_table->setName("end");

	auto label = Label::createWithTTF("Opponent run away", "fonts/arial.ttf", 40);
	label->setAlignment(TextHAlignment::CENTER);
	label->setTextColor(Color4B(0, 0, 0, 255));
	label->setPosition(200, 150);
	result_table->addChild(label);
}