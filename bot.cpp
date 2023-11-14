#include<bits/stdc++.h>
using namespace std;
#define db double
#define ar array
#define all(x) x.begin(), x.end()
#define sz(x) (int)x.size()

struct Move
{
	int cell;
	int direction; // 0 cw, 1 ccw
	Move next_move()
	{
		return {(this->cell + (this->direction == 1 ? 1 : 11)) % 12, this->direction};
	}

	void print()
	{
		// printf("Move: %i %i \n", cell, direction);
		cout << cell << ' ' << direction << '\n';
	}
};
class Game
{
public:
	int turn;
	vector<ar<int, 2> > state;
	int P1points;
	int P2points;
	const int quanValue = 5;
	Game()
	{
		turn = 0;
		// state = {{0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}};
		state = {{7, 0}, {1, 0}, {2, 0}, {0, 0}, {7, 0}, {0, 0}, {4, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
		P1points = 0;
		P2points = 0;	
		/*
			11	10	9	8	7
		0						6
			1	2 	3	4	5
		*/
	}

	void check_borrow()
	{
		if (turn == 0)
		{
			bool out_of_rocks = 1;
			for (int cell = 1; cell < 6; cell++)
			if (state[cell][0])
			{
				out_of_rocks = 0;
				break;
			}
			if (out_of_rocks)
			{
				for (int cell = 1; cell < 6; cell++) state[cell][0] = 1;
				P1points -= 5;
			}
		}
		else
		{
			bool out_of_rocks = 1;
			for (int cell = 7; cell < 12; cell++)
			if (state[cell][0])
			{
				out_of_rocks = 0;
				break;
			}
			if (out_of_rocks)
			{
				for (int cell = 7; cell < 12; cell++) state[cell][0] = 1;
				P2points -= 5;
			}
		}
	}

	vector<Move> possible_move()
	{
		// this->print_table();
		vector<Move> res;
		if (turn == 0)
		{
			for (int cell = 1; cell < 6; cell++)
			if (state[cell][0]) 
			{
				res.push_back(Move{cell, 0});
				res.push_back(Move{cell, 1});
			}
		}
		else
		{
			for (int cell = 7; cell < 12; cell++)
			if (state[cell][0]) 
			{
				res.push_back(Move{cell, 0});
				res.push_back(Move{cell, 1});
			}
		}
		return res;
	}

	int check_ending()
	{
		if (state[0] == ar<int, 2>{0, 0} && state[6] == ar<int, 2>{0, 0}) 
		{
			for (int cell = 1; cell < 6; cell++) P1points += state[cell][0], state[cell][0] = 0;
			for (int cell = 7; cell < 12; cell++) P2points += state[cell][0], state[cell][0] = 0;
			if (P1points > P2points) return 1; // player 1 wins
			else if (P1points < P2points) return 2; // player 2 wins
			else return 3; // draw
		}
		return 0;
	}

	void make_move(Move move)
	{
		bool playerid = (move.cell > 6);
		int num_rocks = state[move.cell][0];
		state[move.cell][0] = 0;
		while(num_rocks)
		{
			// distributing rocks
			// cout << num_rocks << '\n';
			while(num_rocks)
			{
				move = move.next_move();
				// move.print();
				state[move.cell][0]++;
				num_rocks--;
			}
			int next_cell = move.next_move().cell;
			if (state[next_cell][0] != 0) // if next cell has dan
			{
				if (next_cell % 6 == 0) break; // if next cell is quan cell
				num_rocks = state[next_cell][0];
				state[next_cell][0] = 0;
				move = move.next_move();
				continue;
			}		
			
			// eating steps
			while(state[move.next_move().cell] == ar<int, 2>{0, 0} &&
				  state[move.next_move().next_move().cell] != ar<int, 2>{0, 0})
			{
				if (playerid == 0) P1points += state[move.next_move().next_move().cell][0] + state[move.next_move().next_move().cell][1] * quanValue;
				else P2points += state[move.next_move().next_move().cell][0] + state[move.next_move().next_move().cell][1] * quanValue;
				move = move.next_move().next_move();
				state[move.cell] = {0, 0};
			}
		}

		turn ^= 1;
		if (!this->check_ending()) this->check_borrow();
	}

	void print_table()
	{
		printf("    11 10  9  8  7  6 \n");
	    printf("+--+--------------+--+\n");
	    printf("|%2c|%2i|%2i|%2i|%2i|%2i|%2c|\n", state[0][1] ? '*' : ' ', state[11][0], state[10][0], state[9][0], state[8][0], state[7][0], state[6][1] ? '*' : ' ');
	    printf("|%2i|--------------|%2i|\n", state[0][0], state[6][0]);
	    printf("|  |%2i|%2i|%2i|%2i|%2i|  |\n", state[1][0], state[2][0], state[3][0], state[4][0], state[5][0]);
	    printf("+--+--------------+--+\n");
	    printf("  0  1  2  3  4  5\n");
	    cout << "P1: " << P1points << "		P2: " << P2points << '\n'; 

	}
};

class Strategy {
protected:
    Game* game;
public:
    Strategy(Game* game) : game(game) {}
    virtual Move calculate_move() = 0;
    void execute_move()
    {
    	game->make_move(calculate_move());
    }
};

class MinimaxStrategy : public Strategy {
public:
	const int INF = 7777;
    MinimaxStrategy(Game* game) : Strategy(game) {}

    bool optimize(int& best, int value, bool isMaximizingPlayer)
    {
    	if (isMaximizingPlayer) // if maximize player
    	{
    		if (best < value) 
    		{
    			best = value;
    			return 1;
    		}
    	}
    	else
    	{
    		if (best > value)
    		{
    			best = value;
    			return 1;
    		}
    	}
    	return 0;
    }
    Move calculate_move() override {
        Move bestMove = {-1, -1};
        int bestValue = (game->turn == 0) ? -INF : INF; 
        // cout << game->turn << ' ' << bestValue << '\n';
        // game->print_table();
        // vector<Move> wtf = game->possible_move();
        // cout << sz(wtf) << '\n';
        int cur_turn = game->turn;
        for (Move move : game->possible_move()) 
        {
        	// move.print();
            Game tempGame = *game;
            tempGame.make_move(move);
            
            int moveValue = minimax(tempGame, 0, cur_turn, -INF, INF);

            if (optimize(bestValue, moveValue, cur_turn ^ 1)) bestMove = move;
            // cout << moveValue << ' ' << bestValue << '\n';
            // move.print();
        }
        bestMove.print();
        return bestMove;
    }

private:
	const int MAX_DEPTH = 9;

	int ultility(Game& game, int isEnd)
	{
		if (isEnd == 1) return INF - 1;
		else if (isEnd == 2) return -INF + 1;
		return game.P1points - game.P2points;
	}

    int minimax(Game& game, int depth, bool isMaximizingPlayer, int alpha, int beta) {

    	int isEnd = game.check_ending();
        if (isEnd || depth == MAX_DEPTH) {
        	// game.print_table();
            return ultility(game, isEnd);
        }
        if (isMaximizingPlayer) {
            int bestValue = -INF;  

            for (Move move : game.possible_move()) 
           	{
	            Game tempGame = game;
	            tempGame.make_move(move);
	            // move.print();
	            
	            int moveValue = minimax(tempGame, depth + 1, false, alpha, beta);
	            // cout << moveValue << endl;
	            bestValue = max(bestValue, moveValue);
	            
	            // cout << moveValue << '\n';
	            alpha = max(alpha, bestValue);
	            if (beta <= alpha) break;
        	}	

            return bestValue;
        } else {
            int bestValue = INF;  
            for (Move move : game.possible_move()) 
           	{
	            Game tempGame = game;
	            tempGame.make_move(move);
	            
	            // move.print();
	            int moveValue = minimax(tempGame, depth + 1, true, alpha, beta);
	            
	            // cout << moveValue << '\n';
	            bestValue = min(bestValue, moveValue);

	            beta = min(beta, bestValue);
	            if (beta <= alpha) break;
        	}	

            return bestValue;
        }
    }
};

class Player {
private:
    Strategy* strategy;
public:
	// Player(Stategy* stategy) : strategy(strategy) {}

    void set_strategy(Strategy* strategy) {
        this->strategy = strategy;
    }
    void play() {
        strategy->execute_move();

    }
};

int main()
{    
    Game game;
    Player p1;
    MinimaxStrategy minimax(&game);
    p1.set_strategy(&minimax);
    Player p2;
    p2.set_strategy(&minimax);
    while(!game.check_ending())
    {
    	// game.check_borrow(game.turn);
    	game.print_table();
    	if (game.turn == 0) p1.play();
    	else p2.play();
    	// cout << game.turn << '\n';
    }
    game.print_table();
    int v = game.check_ending();
    if (v == 1) cout << "P1 wins";
    else if (v == 2) cout << "p2 wins";
    else if (v == 3) cout << "draw";
    return 0;
}
