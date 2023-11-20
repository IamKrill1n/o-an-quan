#include "minimax.cpp"
#include "expectimax.cpp"
#include "human.cpp"

int main()
{    
	// vector<ar<int, 2> > state(12);
	// int turn, P1points, P2points;
	// cin >> turn;
	// for (int i = 0; i < 12; i++) cin >> state[i][0] >> state[i][1];
	// cin >> P1points >> P2points;

	Game game(0, {{0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}}, 0, 0);
    // Game game(turn, state, P1points, P2points);
    // MinimaxStrategy minimax13(&game, 13);
    MinimaxStrategy minimax9(&game, 9);
    // ExpectimaxStrategy expectimax(&game, 3);
    // Human human(&game);
    minimax9.calculate_move().print();

    // 1 7 0 0 0 0 0 1 0 0 0 0 0 3 0 0 0 0 0 1 0 0 0 0 0 0 3
    // Player p1;
    // p1.set_strategy(&minimax13);
    // Player p2;
    // p2.set_strategy(&minimax9);

    // int num_turn = 1;
    // while(!game.check_ending())
    // {
    // 	// game.check_borrow(game.turn);
    // 	cout << "TURN: " << num_turn << '\n';
    // 	game.print_table();
    // 	if (game.turn == 0) p1.play();
    // 	else p2.play();
    // 	num_turn++;
    // 	// cout << game.turn << '\n';
    // }
    // game.print_table();
    // int v = game.check_ending();
    // if (v == 1) cout << "P1 wins";
    // else if (v == 2) cout << "P2 wins";
    // else if (v == 3) cout << "Draw";
    // return 0;
}