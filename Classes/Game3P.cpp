#include "Game3P.h"
#include "Socket.h"
#include "MenuScene.h"
#include "AudioEngine.h"

Scene* Game3P::createScene()
{
	return Game3P::create();
}

bool Game3P::init()
{
	if (!Scene::init())
	{
		return false;
	}

	this->setName("1v1v1");
	std::srand(0);
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
	auto board = Sprite::create("Assets/GameScene/3PlayerBoard.png");
	board->setPosition(screen_size.width / 2, screen_size.height / 2);
	//board->setScale(2);
	auto board_size = board->getContentSize();
	board->setScale(1000 / board_size.width, 700 / board_size.height);
	this->addChild(board);


	//board square
	//bottom row
	int init_x = screen_size.width / 4 + 15 ;
	int init_y = screen_size.width / 5 + 20;
	auto sq_size = Size(screen_size.width / 9 - 15, screen_size.height / 8);
	int step_x = sq_size.width + 13 ;
	int count = 0;
	for (int i = 1; i < 6; i++, count++)
	{
		auto temp = Sprite::create();
		temp->setTextureRect(Rect(0, 0, sq_size.width, sq_size.height));
		temp->setTag(i);
		temp->retain();
		temp->setColor(Color3B(255, 0, 0));
		auto square = board_square();
		auto label = Label::createWithTTF(std::to_string(i), "fonts/arial.ttf", 20);

		temp->setPosition(init_x + count*step_x, init_y);
		//this->addChild(temp);

		label->setPosition(init_x + count * step_x + 40, init_y - 40);
		label->setTextColor(Color4B(0, 0, 0, 255));
		stones_number_in_each_sq[i] = label;
		//this->addChild(temp);
		square.square = temp;
		sq[i] = square;
	}
	//left row
	init_x = screen_size.width / 5 ;
	init_y = screen_size.width / 3;
	sq_size = Size(screen_size.width / 9 - 15, screen_size.height / 9);
	step_x = 55;
	int step_y = 70;
	count = 0;
	for (int i = 17; i > 12; i--, count++)
	{
		auto temp = Sprite::create();
		temp->setTextureRect(Rect(0, 0, sq_size.width, sq_size.height - i));
		temp->setTag(i);
		temp->retain();
		temp->setColor(Color3B(255, 0, 0));
		temp->setRotation(31.5);
		auto square = board_square();
		auto label = Label::createWithTTF(std::to_string(i), "fonts/arial.ttf", 20);

		temp->setPosition(init_x + count * step_x, init_y + count * step_y);
		//this->addChild(temp);

		label->setPosition(init_x + count * step_x + 20, init_y + count * step_y - 30);
		label->setTextColor(Color4B(0, 0, 0, 255));
		stones_number_in_each_sq[i] = label;
		//this->addChild(temp);
		square.square = temp;
		sq[i] = square;
	}

	//right row
	init_x = screen_size.width * 3 / 4 + 15;
	init_y = screen_size.width / 3;
	sq_size = Size(screen_size.width / 9 - 15, screen_size.height / 9);
	step_x = 55;
	step_y = 70;
	count = 0;
	for (int i = 7; i < 12; i++, count++)
	{
		auto temp = Sprite::create();
		temp->setTextureRect(Rect(0, 0, sq_size.width, sq_size.height - i));
		temp->setTag(i);
		temp->retain();
		temp->setColor(Color3B(255, 0, 0));
		temp->setRotation(-32);
		auto square = board_square();
		auto label = Label::createWithTTF(std::to_string(i), "fonts/arial.ttf", 20);

		temp->setPosition(init_x - count * step_x, init_y + count * step_y);
		//this->addChild(temp);

		label->setPosition(init_x - count * step_x - 20, init_y + count * step_y - 30);
		label->setTextColor(Color4B(0, 0, 0, 255));
		stones_number_in_each_sq[i] = label;
		//this->addChild(temp);
		square.square = temp;
		sq[i] = square;
	}

	//top  12
	sq_size = Size(screen_size.width / 9 - 25, screen_size.height / 8 - 30);
	auto top_sq = Sprite::create();
	top_sq->setTextureRect(Rect(0, 0, sq_size.width, sq_size.height));
	top_sq->setTag(12);
	top_sq->retain();
	top_sq->setColor(Color3B(255, 0, 0));
	auto board_sq_top = board_square();
	auto label_top = Label::createWithTTF(std::to_string(12), "fonts/arial.ttf", 20);

	top_sq->setPosition(screen_size.width / 2 - 10, screen_size.height - 90);
	//this->addChild(top_sq);

	label_top->setPosition(screen_size.width / 2 + 30, screen_size.height - 70 - 30);
	label_top->setTextColor(Color4B(0, 0, 0, 255));
	stones_number_in_each_sq[12] = label_top;
	//this->addChild(temp);
	board_sq_top.square = top_sq;
	sq[12] = board_sq_top;

	//bottom left = 0
	sq_size = Size(screen_size.width / 9 - 25, screen_size.height / 8 - 30);
	auto btl_sq = Sprite::create();
	btl_sq->setTextureRect(Rect(0, 0, sq_size.width, sq_size.height));
	btl_sq->setTag(0);
	btl_sq->setRotation(31);
	btl_sq->retain();
	btl_sq->setColor(Color3B(255, 0, 0));
	auto board_sq_btl = board_square();
	auto label_btl = Label::createWithTTF(std::to_string(0), "fonts/arial.ttf", 20);

	btl_sq->setPosition(screen_size.width / 5 - 25, screen_size.height / 4 + 55);
	//this->addChild(btl_sq);

	label_btl->setPosition(screen_size.width / 5 - 25 + 30, screen_size.height / 4 + 55 - 30);
	label_btl->setTextColor(Color4B(0, 0, 0, 255));
	stones_number_in_each_sq[0] = label_btl;
	//this->addChild(temp);
	board_sq_btl.square = btl_sq;
	sq[0] = board_sq_btl;

	//bottom right = 6
	sq_size = Size(screen_size.width / 9 - 25, screen_size.height / 8 - 30);
	auto btr_sq = Sprite::create();
	btr_sq->setTextureRect(Rect(0, 0, sq_size.width, sq_size.height));
	btr_sq->setTag(6);
	btr_sq->setRotation(-32);
	btr_sq->retain();
	btr_sq->setColor(Color3B(255, 0, 0));
	auto board_sq_btr = board_square();
	auto label_btr = Label::createWithTTF(std::to_string(6), "fonts/arial.ttf", 20);

	btr_sq->setPosition(screen_size.width * 3 / 4 + 50, screen_size.height / 4 + 55);
	//this->addChild(btr_sq);

	label_btr->setPosition(screen_size.width * 3 / 4 + 20, screen_size.height / 4 + 55 - 30);
	label_btr->setTextColor(Color4B(0, 0, 0, 255));
	stones_number_in_each_sq[6] = label_btr;
	//this->addChild(temp);
	board_sq_btr.square = btr_sq;
	sq[6] = board_sq_btr;

	//END BOARD -------------------------------------------------------------------------------------------------

	//BEGIN INIT PIECES -----------------------------------------------------------------------------------------
	//preload sprite frame
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Assets/GameScene/stones_sheet.plist");

	//normal piece
	int sq_index = 1;
	for (int i = 0; i < 75; )
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
		p.sprite->setScale(25 / size.width, 25 / size.height);
		//p.sprite->setPosition(RandomHelper::random_int(0, (int)screen_size.width), RandomHelper::random_int(0, (int)screen_size.height));
		//this->addChild(p.sprite);
		sq[sq_index].pieces.push_back(p);
		i++;
		if (i % 5 == 0) sq_index++;
		if (i == 25) sq_index++;
		if (i == 50) sq_index++;
		//log("%i", sq_index);
	}

	//big pieces
	piece p1;
	p1.sprite = Sprite::createWithSpriteFrameName("stone6.png");
	p1.point = 5;
	p1.sprite->retain();
	auto size = p1.sprite->getContentSize();
	p1.sprite->setScale(50 / size.width, 50 / size.height);
	sq[0].pieces.push_back(p1);

	piece p2;
	p2.sprite = Sprite::createWithSpriteFrameName("stone6.png");
	p2.point = 5;
	p2.sprite->retain();
	p2.sprite->setScale(50 / size.width, 50 / size.height);
	sq[6].pieces.push_back(p2);

	piece p3;
	p3.sprite = Sprite::createWithSpriteFrameName("stone6.png");
	p3.point = 5;
	p3.sprite->retain();
	p3.sprite->setScale(50 / size.width, 50 / size.height);
	sq[12].pieces.push_back(p3);

	//init position
	for (int i = 0; i < 18; i++)
	{
		auto box = sq[i].square->getBoundingBox();
		auto minX = box.getMinX() + 30, maxX = box.getMaxX() - 30, minY = box.getMinY() + 30, maxY = box.getMaxY() - 30;
		if (i > 0 && i < 7)
		{
			minY -= 10;
			maxY += 10;
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


	//BEGIN LABEL -----------------------------------------------------------------------------------------------

	//BEGIN LABEL
	myPoint = Label::createWithTTF("My point: 0", "fonts/arial.ttf", 40);
	myPoint->setTextColor(Color4B(0, 0, 0, 255));
	this->addChild(myPoint);

	opponentPoint = Label::createWithTTF("Opponent point: 0", "fonts/arial.ttf", 40);
	opponentPoint->setTextColor(Color4B(0, 0, 0, 255));
	opponentPoint->setPosition(2 * screen_size.width / 3 + 150, 4 * screen_size.height / 5);
	this->addChild(opponentPoint);

	AI_point = Label::createWithTTF("AI point: 0", "fonts/arial.ttf", 40);
	AI_point->setTextColor(Color4B(0, 0, 0, 255));
	AI_point->setPosition(150, 4 * screen_size.height / 5);
	this->addChild(AI_point);

	for (int i = 0; i < 18; i++)
	{
		this->addChild(stones_number_in_each_sq[i]);
	}

	whoseTurn = Label::createWithTTF("Your turn", "fonts/arial.ttf", 40);
	whoseTurn->setTextColor(Color4B(0, 0, 0, 255));
	whoseTurn->setPosition(screen_size.width / 2, 50);
	this->addChild(whoseTurn);

	holding_stone = Label::createWithTTF("Holding: 0", "fonts/arial.ttf", 40);
	holding_stone->setTextColor(Color4B(0, 0, 0, 255));
	holding_stone->setPosition(screen_size.width / 5, screen_size.height - 50);
	this->addChild(holding_stone);

	

	//END LABEL
	//END LABEL -------------------------------------------------------------------------------------------------

	//BEGIN ARROW -----------------------------------------------------------------------------------------------
	arrow = Sprite::create("Assets/GameScene/arrow.png");
	arrow->setScale(0.2);
	this->addChild(arrow);
	arrow->retain();
	arrow->setPosition(-50, -50);

	right_arrow = Sprite::create("Assets/GameScene/left_arrow.png");
	right_arrow->setRotation(32);
	right_arrow->setScale(0.2);
	this->addChild(right_arrow);
	right_arrow->retain();
	right_arrow->setPosition(-50, -50);

	left_arrow = Sprite::create("Assets/GameScene/left_arrow.png");
	left_arrow->setScale(0.2);
	left_arrow->setRotation(-32);
	this->addChild(left_arrow);
	left_arrow->retain();
	left_arrow->setPosition(-50, -50);

	bottom_arrow = Sprite::create("Assets/GameScene/right_arrow.png");
	bottom_arrow->setScale(0.2);
	this->addChild(bottom_arrow);
	bottom_arrow->retain();
	bottom_arrow->setPosition(-50, -50);

	//arrow floating action
	auto float_up = MoveBy::create(1, Vec2(0, 50));
	auto float_down = MoveBy::create(1, Vec2(0, -50));
	auto seq = Sequence::create(float_up, float_down, nullptr);
	floating = RepeatForever::create(seq);
	floating->retain();

	//END ARROW -------------------------------------------------------------------------------------------------

	//event listener
	auto MouseEv = EventListenerMouse::create();
	MouseEv->onMouseDown = [&](EventMouse* event)
	{
		AudioEngine::play2d("Sound/click.mp3");
		if (this->getChildByName("end"));
		{
			auto menu = MenuScene::createScene();
			Director::getInstance()->replaceScene(menu);
			Socket::getInstance()->send("end");
			return;
		}
		auto MousePos = Vec2(event->getCursorX(), event->getCursorY());
		std::string msg = "1v1v1MatchWith=" + std::to_string(opponent_id);
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
		else if (myTurn == current_turn && !pausing)
		{
			int begin, end;
			Sprite* move_arrow;
			if (current_turn == 1)
			{
				begin = 7;
				end = 12;
				move_arrow = right_arrow;
			}
			else if (current_turn == 0)
			{
				begin = 1;
				end = 6;
				move_arrow = bottom_arrow;
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
						//if (side == "down") left_arrow->setPosition(pos.x - 40, pos.y - 30);
						//else right_arrow->setPosition(pos.x + 40, pos.y - 30);
						if (current_turn == 0)
						{
							bottom_arrow->setPosition(pos.x + 40, pos.y - 20);
						}
						else
						{
							right_arrow->setPosition(pos.x -50, pos.y + 20);
						}
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

void Game3P::update(float delta)
{
	time += delta;
	if (time - last_frame_time > 0.25)
	{
		//log("%s", state.c_str());
		last_frame_time = time;
		update_point();
		update_stones_number();

		if (!spreading)
		{
			checkState();
			if (myTurn == current_turn) whoseTurn->setString("Your turn to move");
			else if (current_turn == 2) whoseTurn->setString("AI turn to move");
			else	whoseTurn->setString("Opponent turn to move");

			if (current_turn == 2)
			{
				this->AI_move();
			}
		}
		else
		{
			this->spread_stones();
		}

		
	}
}

void Game3P::setup(int opponent_id, int move_order)
{
	this->opponent_id = opponent_id;
	myTurn = move_order;

	auto screen_size = Director::getInstance()->getVisibleSize();
	Vec2 myPointpos;
	Vec2 opPointpos;
	if (myTurn == 0)
	{
		myPointpos = Vec2(screen_size.width / 3, screen_size.height / 5 - 30);
		opPointpos = Vec2(2 * screen_size.width / 3 + 150, 4 * screen_size.height / 5);
	}
	else
	{
		myPointpos = Vec2(2 * screen_size.width / 3 + 150, 4 * screen_size.height / 5);
		opPointpos = Vec2(screen_size.width / 3, screen_size.height / 5 - 30);
	}
	myPoint->setPosition(myPointpos);
	opponentPoint->setPosition(opPointpos);

	current_turn = 0;
	if (current_turn == myTurn) whoseTurn->setString("MyTurn");
	else whoseTurn->setString("Opponent turn");
}

void Game3P::set_arrow(int tag)
{
	auto box = sq[tag].square;
	arrow->setPosition(box->getPosition());
	arrow->setTag(tag);
	arrow->stopAllActions();
	arrow->runAction(floating);
}

void Game3P::spread_stones(bool first_spread)
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
		if (tag == 18) tag = 0;
		arrow->setTag(tag);
		auto pos = sq[tag].square->getPosition();
		arrow->setPosition(pos);
		left_arrow->setPosition(-50, -50);
		right_arrow->setPosition(-50, -50);
		bottom_arrow->setPosition(-50, -50);
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
		if (tag == 18) tag = 0;
		arrow->setTag(tag);
		arrow->setPosition(sq[tag].square->getPosition());
		this->removeChild(arrow);
		this->removeChild(left_arrow);
		this->removeChild(right_arrow);
		this->addChild(right_arrow);
		this->addChild(left_arrow);
		this->addChild(arrow);
	}

	if (myTurn == current_turn)
	{
		whoseTurn->setString("Moving your soils...");
	}
	else if(current_turn == 2)
	{
		whoseTurn->setString("Moving AI soils...");
	}
	else
	{
		whoseTurn->setString("Moving opponent soils...");
	}
	holding_stone->setString("Holding: " + std::to_string(hold.size()));
}

void Game3P::opponent_out_of_move()
{

}

void Game3P::show_pause_request()
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

void Game3P::resume_from_pause()
{
	pausing = false;
	this->removeChildByName("pausing board");
	this->removeChild(resume_btn);
	this->addChild(pause_btn);
}

void Game3P::show_pause()
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

void Game3P::wait_for_response(std::string type)
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

void Game3P::opponent_run_away()
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

void Game3P::show_confirm_board()
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

void Game3P::showResult()
{
	std::string res;
	if (mp > opp && mp > ap) res = "You won !";
	else if (mp == opp && ap == mp) res = "Draw !";
	else if (opp > mp && opp > ap) res = "Opponent won !";
	else res = "AI won !";

	auto size = Director::getInstance()->getVisibleSize();

	auto result_table = Sprite::create("Assets/MenuScene/board.png");
	result_table->setTextureRect(Rect(0, 0, 400, 300));
	result_table->setPosition(size.width / 2, size.height / 2);
	result_table->setName("end");
	this->addChild(result_table);

	auto label = Label::createWithTTF(res, "fonts/arial.ttf", 40);
	label->setTextColor(Color4B(0, 0, 0, 255));
	label->setPosition(200, 150);
	result_table->addChild(label);

	this->state = "end";
}

bool Game3P::check_next_sq()
{
	int tag = arrow->getTag();
	int next_tag;

	next_tag = tag + 1;

	if (next_tag == 18) next_tag = 0;

	//case 1
	if (sq[tag].pieces.size() != 0 && tag != 0 && tag != 6 && tag != 12)
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
				if (myTurn == current_turn) mp += p.point;
				else if (current_turn == 2) ap += p.point;
				else opp += p.point;
				this->removeChild(p.sprite);
				p.sprite->release();
			}
			sq[next_tag].pieces.clear();
			//log("%i %i", next_tag, sqr.pieces.size());
		}
	}


	current_turn += 1;
	if (current_turn == 3) current_turn = 0;


	return false;
}

void Game3P::update_point()
{
	myPoint->setString("My point: " + std::to_string(mp));
	opponentPoint->setString("Opponent point: " + std::to_string(opp));
	AI_point->setString("AI point: " + std::to_string(ap));
}

void Game3P::update_stones_number()
{
	for (int i = 0; i < 18; i++)
	{
		//log("tag: %i %i", i, sq[i].pieces.size());
		stones_number_in_each_sq[i]->setString(std::to_string(sq[i].pieces.size()));
		//	log("size: %i", sq[i].pieces.size());
	}
}

void Game3P::AI_move()
{
	std::vector<int> ai_valid_move;
	for(int i = 13; i < 18; i++)
	{
		if (sq[i].pieces.size() != 0)  ai_valid_move.push_back(i);
	}

	
	int tag = std::rand() % ai_valid_move.size();
	this->set_arrow(ai_valid_move[tag]);
	this->spread_stones(true);
}

void Game3P::checkState()
{
	int begin, end;
	int op_begin, op_end;
	int ai_begin = 13, ai_end = 18;
	if (myTurn == 0)
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
	bool have_move_ai = false;

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

	for (int i = ai_begin; i < ai_end; i++)
	{
		if (sq[i].pieces.size() > 0)
		{
			have_move_ai = true;
			break;
		}
	}


	if (!have_move && !have_move_op && !have_move_ai) this->showResult();
	else if (myTurn == current_turn && !have_move) current_turn += 1;
	else if (current_turn == 2 && !have_move_ai) current_turn = 0;
	else if (myTurn != current_turn && current_turn != 2 && !have_move_op) current_turn += 1;
}