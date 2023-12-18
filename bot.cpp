#include<bits/stdc++.h>
using namespace std;
#define db double
#define ar array
#define all(x) x.begin(), x.end()
#define sz(x) (int)x.size()
struct Move {
	int cell;
	int direction; // 0 cw, 1 ccw
	Move next_move()
	{
		return {(this->cell + (this->direction == 1 ? 1 : 11)) % 12, this->direction};
	}

	void print()
	{
		cout << cell << ' ' << direction << '\n';
	}
};
class Game {
public:
	int turn;
	vector<ar<int, 2> > state;
	int P1points;
	int P2points;
	const int quanValue = 5;
	Game(int turn_, vector<ar<int, 2> > state_, int P1points_, int P2points_)
	{
		turn = turn_;
		// state = {{0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}};
		state = state_; // {{7, 0}, {1, 0}, {2, 0}, {0, 0}, {7, 0}, {0, 0}, {4, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
		P1points = P1points_;
		P2points = P2points_;	
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
	vector<Move> bestLine;
	int MAX_DEPTH;
    MinimaxStrategy(Game* game, int MAX_DEPTH) : Strategy(game), MAX_DEPTH(MAX_DEPTH) {}

    Move calculate_move() override 
    {
        Move bestMove = {-1, -1};
        int bestValue = (game->turn == 0) ? -INF : INF; 
        int alpha = -INF, beta = INF;
        // cout << game->turn << ' ' << bestValue << '\n';
        // game->print_table();
        // vector<Move> wtf = game->possible_move();
        // cout << "CALCULATION: \n";
        int isMaximizingPlayer = !(game->turn);
        for (Move move : game->possible_move()) 
        {
        	// move.print();
            Game tempGame = *game;
            tempGame.make_move(move);
            int moveValue = minimax(tempGame, 0, !isMaximizingPlayer, alpha, beta);

            if (optimize(bestValue, moveValue, isMaximizingPlayer)) bestMove = move;
            // cout << moveValue << ' ' << bestValue << '\n';
            // move.print();
            if (isMaximizingPlayer) alpha = max(alpha, moveValue);
            else beta = min(beta, moveValue);
            if (beta <= alpha) break;
        }
        // bestMove.print();
        // for (Move move : bestLine) move.print();
        cout << "MOVE PLAYED: ";
    	bestMove.print();
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

    int minimax(Game& game, int depth, bool isMaximizingPlayer, int& alpha, int& beta)
    {
    	int isEnd = game.check_ending();
        if (isEnd || depth == MAX_DEPTH) {
        	// game.print_table();
            return ultility(game, isEnd);
        }
        
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

// int main(int argc, char* argv[])
int main()
{    
	// vector<ar<int, 2> > state(12);
	// int turn, P1points, P2points;
	// turn = stoi(argv[1]);
	// for (int i = 2; i < argc; i++)
	// {
	// 	if (i <= 25) state[i / 2 - 1][i & 1] = stoi(argv[i]);
	// 	else if (i == 26) P1points = stoi(argv[i]);
	// 	else P2points = stoi(argv[i]);
	// }
	Game game(0, {{0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}}, 0, 0);
    // Game game(turn, state, P1points, P2points);
    Player p1;
    MinimaxStrategy minimax9(&game, 9);
    MinimaxStrategy minimax1(&game, 1);
    // minimax.calculate_move().print();
    p1.set_strategy(&minimax1);


    Player p2;
    p2.set_strategy(&minimax1);

    int num_turn = 1;
    while(!game.check_ending())
    {
    	// game.check_borrow(game.turn);
    	cout << "TURN: " << num_turn << '\n';
    	game.print_table();
    	if (game.turn == 0) p1.play();
    	else p2.play();
    	num_turn++;
    	// cout << game.turn << '\n';
    }
    game.print_table();
    int v = game.check_ending();
    if (v == 1) cout << "P1 wins";
    else if (v == 2) cout << "p2 wins";
    else if (v == 3) cout << "draw";
    return 0;
}
