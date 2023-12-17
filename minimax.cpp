#ifndef MINIMAX_CPP
#define MINIMAX_CPP

#include "game.cpp"


class MinimaxStrategy : public Strategy {
public:
	const double INF = 7777777;
	int maxDepth;
    double gamma;
    MinimaxStrategy(Game* game) : Strategy(game){};

    void set_maxDepth(int maxDepth_)
    {
    	this->maxDepth = maxDepth_;
    }

    void set_gamma(double gamma_)
    {
        this->gamma = gamma_;
    }

    Move calculate_move() override 
    {
        Move bestMove = {-1, -1};
        double bestValue = (game->turn == 0) ? -INF : INF; 
        double alpha = -INF, beta = INF;
        // cout << "CALCULATION: \n";
        // cout << "bestValue = " << bestValue << '\n';
        int isMaximizingPlayer = !(game->turn);
        for (Move move : game->possible_move()) 
        {
            Game tempGame = *game;
            tempGame.make_move(move);
            // move.print();
            double moveValue = minimax(tempGame, 0, !isMaximizingPlayer, alpha, beta);
            // cout << "Move value = " << moveValue << '\n';
            if (optimize(bestValue, moveValue, isMaximizingPlayer)) bestMove = move;
            if (isMaximizingPlayer) alpha = max(alpha, moveValue);
            else beta = min(beta, moveValue);
            if (beta <= alpha) break;
        }
        // cout << "MOVE PLAYED: ";
    	// bestMove.print();
        return bestMove;
    }

private:
	bool optimize(double& best, double value, bool isMaximizingPlayer)
    {
    	if (isMaximizingPlayer) // if maximize player
    	{
    		if (best < value) return best = value, 1;
    	}
    	else if (best > value) return best = value, 1;
    	return 0;
    }

    double utility(Game& game, int isEnd)
    {
        if (isEnd == 1) return INF - 1;
        else if (isEnd == 2) return -INF + 1;
        int p1_stones = 0, p2_stones = 0;
        for (int cell = 1; cell < 6; cell++) p1_stones += game.state[cell][0];
    	for (int cell = 7; cell < 12; cell++) p2_stones += game.state[cell][0];

        return game.P1points - game.P2points + gamma * (p1_stones - p2_stones);
    }

    double minimax(Game& game, int depth, bool isMaximizingPlayer, double alpha, double beta)
    {
    	int isEnd = game.check_ending();
        if (isEnd || depth == maxDepth) return utility(game, isEnd);
             
        double bestValue = isMaximizingPlayer ? -INF : INF;  
        // Move bestMove = {-1, -1};
        
        for (Move move : game.possible_move()) 
       	{
            Game tempGame = game;
            tempGame.make_move(move);     
            // cout << "P1 move: ";
            // move.print();
            // tempGame.print_table();
            double moveValue = minimax(tempGame, depth + 1, !isMaximizingPlayer, alpha, beta);
            // cout << moveValue << endl;
            optimize(bestValue, moveValue, isMaximizingPlayer);
            
            if (isMaximizingPlayer) alpha = max(alpha, bestValue);
            else beta = min(beta, bestValue);
            if (beta <= alpha) break;
    	}	

        return bestValue;
    }
};

#endif