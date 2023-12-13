#ifndef MINIMAX_CPP
#define MINIMAX_CPP

#include "game.cpp"

typedef std::function<int(Game&, int)> UtilityFunc;

class MinimaxStrategy : public Strategy {
public:
	static const int INF = 7777;
	int maxDepth;
    UtilityFunc utility;
    MinimaxStrategy(Game* game, int maxDepth, UtilityFunc utility) : Strategy(game), maxDepth(maxDepth), utility(utility) {}

    Move calculate_move() override 
    {
        Move bestMove = {-1, -1};
        int bestValue = (game->turn == 0) ? -INF : INF; 
        int alpha = -INF, beta = INF;
        // cout << "CALCULATION: \n";
        // cout << "bestValue = " << bestValue << '\n';
        int isMaximizingPlayer = !(game->turn);
        for (Move move : game->possible_move()) 
        {
            Game tempGame = *game;
            tempGame.make_move(move);
            // move.print();
            int moveValue = minimax(tempGame, 0, !isMaximizingPlayer, alpha, beta, utility);
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
	bool optimize(int& best, int value, bool isMaximizingPlayer)
    {
    	if (isMaximizingPlayer) // if maximize player
    	{
    		if (best < value) return best = value, 1;
    	}
    	else if (best > value) return best = value, 1;
    	return 0;
    }

    int minimax(Game& game, int depth, bool isMaximizingPlayer, int alpha, int beta, UtilityFunc utility)
    {
    	int isEnd = game.check_ending();
        if (isEnd || depth == maxDepth) return utility(game, isEnd);
             
        int bestValue = isMaximizingPlayer ? -INF : INF;  
        // Move bestMove = {-1, -1};
        
        for (Move move : game.possible_move()) 
       	{
            Game tempGame = game;
            tempGame.make_move(move);     
            // cout << "P1 move: ";
            // move.print();
            // tempGame.print_table();
            int moveValue = minimax(tempGame, depth + 1, !isMaximizingPlayer, alpha, beta, utility);
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