#include "minimax.cpp"
#include "expectimax.cpp"
#include "human.cpp"

int main()
{    
	vector<ar<int, 2> > state(12);
	int turn, P1points, P2points;
	cin >> turn;
	for (int i = 0; i < 12; i++) cin >> state[i][0] >> state[i][1];
	cin >> P1points >> P2points;

    Game game(turn, state, P1points, P2points);
    MinimaxStrategy minimax9(&game, 9);
    // MinimaxStrategy minimax10(&game, 5);
    // ExpectimaxStrategy expectimax2(&game, 6);
    // ExpectimaxStrategy expectimax1(&game, 7);
    // Human human(&game);
    minimax9.calculate_move().print();
}