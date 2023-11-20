#include "Minimax.h"
#include <algorithm>
#include <iterator>
#include "cocos2d.h"

int select_move(int cur_state[12][1], int player_to_move, int player1_score, int player2_score, int depth)
{
	state_eval eval_cur_state = state_eval();
	eval_cur_state.best_move = -1;
	eval_cur_state.p1_score = player1_score;
	eval_cur_state.p2_score = player2_score;

	std::vector<int> valid_move = find_valid_move(cur_state, player_to_move);

	int new_cur_state[12][1];
	std::copy(&cur_state[0][0], &cur_state[0][0] + 12 * 1, &new_cur_state[0][0]);

	if (valid_move.size() == 0) return -1;
	state_eval move = minimax_alpha_beta(new_cur_state, player_to_move, valid_move, depth, 0, eval_cur_state, -100, 100);

	for(int i = 0; i < valid_move.size(); i++)
		cocos2d::log("valid Move: %i", valid_move[i]);
	cocos2d::log("Move: %i", move.best_move);
	if (move.best_move == -1 && valid_move.size() != 0) move.best_move = valid_move.back();

	return move.best_move;
}

std::vector<int> find_valid_move(int cur_state[12][1], int player_to_move)
{
	std::vector<int> valid_move;
	if (player_to_move == 1)
	{
		for (int i = 7; i < 12; i++)
		{
			if (cur_state[i][0] != 0) valid_move.push_back(i);
		}
	}
	else
	{
		for (int i = 1; i < 6; i++)
		{
			if (cur_state[i][0] != 0) valid_move.push_back(i);
		}
	}

	return valid_move;
}

state_eval minimax_alpha_beta(int cur_state[12][1], int player_to_move, std::vector<int> validMove, int depth, int cur_depth, state_eval eval, int alpha, int beta)
{
	if (cur_depth == depth || validMove.size() == 0) return eval;

	int new_cur_state[12][1];
	std::copy(&cur_state[0][0], &cur_state[0][0] + 12 * 1, &new_cur_state[0][0]);
	//cocos2d::log("depth: %i ,player: %i, move: %i", cur_depth, player_to_move, eval.best_move);

	state_eval return_state;
	
	return_state.best_move = -1;
	for (int i = 0; i < validMove.size(); i++)
	{
		state_eval new_eval;
		new_eval.best_move = eval.best_move;
		new_eval.p1_score = eval.p1_score;
		new_eval.p2_score = eval.p2_score;
		//cocos2d::log("eval: %i %i move: %i", eval.p1_score, eval.p2_score, validMove[i]);
		int ** new_state = make_move(new_cur_state, validMove[i], new_eval, player_to_move);
		//cocos2d::log("eval %i %i", eval.p1_score, eval.p2_score);
		
		int new_state_array[12][1];

		for (int i = 0; i < 12; i++)
		{
			new_state_array[i][0] = new_state[i][0];
		}
		for (int i = 0; i < 12; i++) delete[] new_state[i];
		delete[] new_state;

		std::vector<int> new_valid_move = find_valid_move(new_state_array, -player_to_move);
		

		new_eval.best_move = validMove[i];
		state_eval res = minimax_alpha_beta(new_state_array, -player_to_move, new_valid_move, depth, cur_depth + 1, new_eval, -beta, -alpha);
		int new_val;
		if (player_to_move == -1) new_val = res.p2_score - res.p1_score;
		else new_val = res.p1_score - res.p2_score;

		new_val = -new_val;
		//cocos2d::log("alpha: %i, beta: %i", alpha, beta);

		if (new_val > alpha)
		{
			alpha = new_val;
			return_state.best_move = new_eval.best_move;
			return_state.p1_score = new_eval.p1_score;
			return_state.p2_score = new_eval.p2_score;
		}

		if (alpha >= beta)
		{
			return new_eval;
		}
	}
	//cocos2d::log("%i", return_state.best_move);
	return return_state;

}

int** make_move(int state[12][1], int move, state_eval &eval, int player)
{
	int** copied_state = new int* [12];
	for (int i = 0; i < 12; i++)
	{
		copied_state[i] = new int[1];
	}
	
	for (int i = 0; i < 12; i++)
	{
		copied_state[i][0] = state[i][0];
	}

	int pos = move + 1;
	bool flag = true;
	int holding;
	holding = copied_state[move][0];
	copied_state[move][0] = 0;
	while (flag)
	{
		
		if (pos == 12) pos = 0;
		if (holding == 0)
		{
			//cocos2d::log("%i %i", pos, copied_state[pos][0]);
			int next_pos = pos + 1;
			if (next_pos == 12) next_pos = 0;
			
			if (copied_state[pos][0] != 0)
			{
				holding = copied_state[pos][0];
				copied_state[pos][0] = 0;
				pos = next_pos;
			}
			else
			{
				//cocos2d::log("%i", copied_state[pos][0]);
				if (copied_state[next_pos][0] != 0)
				{
					if (player == 1)
					{
						eval.p1_score += copied_state[next_pos][0];
						
					}
					else
					{
						eval.p2_score += copied_state[next_pos][0];
					}
					copied_state[next_pos][0] = 0;
				}
				break;
			}
		}
		copied_state[pos][0] += 1;
		holding -= 1;
		pos += 1;
	}

	return copied_state;
}