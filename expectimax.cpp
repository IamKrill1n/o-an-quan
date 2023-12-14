#ifndef EXPECTIMAX_CPP
#define EXPECTIMAX_CPP

#include "game.cpp"

typedef function<int(Game&, int)> UtilityFunc;

class ExpectimaxStrategy : public Strategy {
public:
	const int INF = 7777;
	int maxDepth;
	UtilityFunc utility;

    ExpectimaxStrategy(Game* game) : Strategy(game) {};

	void set_maxDepth(int maxDepth_)
	{
		this->maxDepth = maxDepth_;
	}

	void set_utility(UtilityFunc utility_)
	{
		this->utility = utility_;
	}

    Move calculate_move() override 
    {
        Move bestMove = {-1, -1};
        int bestValue = (game->turn == 0) ? -INF : INF; 
        // cout << "CALCULATION: \n";
        // cout << "bestValue = " << bestValue << '\n';
        int isMaximizingPlayer = !(game->turn);
        for (Move move : game->possible_move()) 
        {
            Game tempGame = *game;
            tempGame.make_move(move);
            int moveValue = expectimax(tempGame, 0, !isMaximizingPlayer, game->turn, utility);
            // cout << moveValue << '\n';
            if (optimize(bestValue, moveValue, isMaximizingPlayer)) bestMove = move;
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

    int expectimax(Game& game, int depth, bool isMaximizingPlayer, int agentId, UtilityFunc utility)
    {
    	int isEnd = game.check_ending();
        if (isEnd || depth == maxDepth) return utility(game, isEnd);
		
        if (isMaximizingPlayer ^ agentId)
        {
        	int bestValue = isMaximizingPlayer ? -INF : INF; 
        	Move bestMove = {-1, -1};

        	for (Move move : game.possible_move()) 
		   	{
		        Game tempGame = game;
		        tempGame.make_move(move);     
		        // cout << "P1 move: ";
		        // move.print();
		        // tempGame.print_table();
		        int moveValue = expectimax(tempGame, depth + 1, !isMaximizingPlayer, agentId, utility);
		        optimize(bestValue, moveValue, isMaximizingPlayer);
			}

			return bestValue;
        } 
        else
        {
        	int avgValue = 0;
        	vector<Move> possibleMove = game.possible_move();
        	for (Move move : possibleMove)
        	{
        		Game tempGame = game;
        		tempGame.make_move(move);

        		int moveValue = expectimax(tempGame, depth + 1, !isMaximizingPlayer, agentId, utility);

        		avgValue += moveValue;
        	}

        	avgValue /= sz(possibleMove);

        	return avgValue;
        }
    }
};

#endif