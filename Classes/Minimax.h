//select move
//alpha beta

#include <vector>

class state_eval
{
public:
	int best_move;
	int p1_score;
	int p2_score;
};

int select_move(int cur_state[12][1], int player_to_move, int player1_score, int player2_score, int depth);

state_eval minimax_alpha_beta(int cur_state[12][1], int player_to_move, std::vector<int> validMove, int depth, int cur_depth, state_eval eval, int alpha, int beta);

std::vector<int> find_valid_move(int cur_state[12][1], int player_to_move);

int** make_move(int state[12][1], int move, state_eval &eval, int player);

