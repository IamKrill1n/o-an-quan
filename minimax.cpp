#include "game.cpp"

class MinimaxStrategy : public Strategy {
public:
	const int INF = 7777;
	vector<Move> bestLine;
	int MAX_DEPTH;
    MinimaxStrategy(Game* game, int MAX_DEPTH) : Strategy(game), MAX_DEPTH(MAX_DEPTH) {}

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
            int moveValue = minimax(tempGame, 1, !isMaximizingPlayer, alpha, beta);
            // cout << moveValue << ' ' << isMaximizingPlayer << '\n';
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
	int num_state = 0;
	map<int, Move> track;
	bool optimize(int& best, int value, bool isMaximizingPlayer)
    {
    	if (isMaximizingPlayer) // if maximize player
    	{
    		if (best < value) return best = value, 1;
    	}
    	else if (best > value) return best = value, 1;
    	return 0;
    }

	int ultility(Game& game, int isEnd)
	{
		if (isEnd == 1) return INF - 1;
		else if (isEnd == 2) return -INF + 1;
		return game.P1points - game.P2points;
	}

    int minimax(Game& game, int depth, bool isMaximizingPlayer, int alpha, int beta)
    {
    	int isEnd = game.check_ending();
        if (isEnd || depth == MAX_DEPTH) return ultility(game, isEnd);
             
        int bestValue = isMaximizingPlayer ? -INF : INF;  
        Move bestMove = {-1, -1};
        
        for (Move move : game.possible_move()) 
       	{
            Game tempGame = game;
            tempGame.make_move(move);     
            // cout << "P1 move: ";
            // move.print();
            // tempGame.print_table();
            int moveValue = minimax(tempGame, depth + 1, !isMaximizingPlayer, alpha, beta);
            // cout << moveValue << endl;
            if (optimize(bestValue, moveValue, isMaximizingPlayer)) bestMove = move;
            
            if (isMaximizingPlayer) alpha = max(alpha, bestValue);
            else beta = min(beta, bestValue);
            if (beta <= alpha) break;
    	}	

        return bestValue;
    }
};