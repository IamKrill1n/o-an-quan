#include "minimax.cpp"
#include "expectimax.cpp"
#include "human.cpp"

int main()
{    
	vector<ar<int, 2> > state(12);
	int turn, P1points, P2points, depth;
    string strategy;
	cin >> turn;
	for (int i = 0; i < 12; i++) cin >> state[i][0] >> state[i][1];
	cin >> P1points >> P2points;
    cin >> strategy >> depth;
    Game game;
    game.set_state(turn, state, P1points, P2points);
    if (strategy == "minimax")
    {
        MinimaxStrategy minimax(&game);
        minimax.set_maxDepth(depth);
        minimax.set_weights(0, 0);
        minimax.calculate_move().print();
    }
    else if (strategy == "expectimax")
    {
        ExpectimaxStrategy expectimax(&game);
        expectimax.set_maxDepth(depth);
        expectimax.calculate_move().print();
    }
}