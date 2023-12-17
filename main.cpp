#include "minimax.cpp"
#include "expectimax.cpp"
#include "human.cpp"
// #include "utility.cpp"

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
        minimax.set_weights({1, 0});
        minimax.calculate_move().print();
    }
    // else if (strategy == "expectimax")
    // {
    //     ExpectimaxStrategy expectimax(&game);
    //     expectimax.set_maxDepth(depth);
    //     if (utility == 1) expectimax.set_utility(utility1);
    //     else expectimax.set_utility(utility2);
    //     expectimax.calculate_move().print();
    // }
    // MinimaxStrategy minimax10(&game, 5);
    // ExpectimaxStrategy expectimax2(&game, 6);
    // ExpectimaxStrategy expectimax1(&game, 7);
    // Human human(&game);
}