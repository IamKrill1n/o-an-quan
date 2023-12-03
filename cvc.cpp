#include "minimax.cpp"
#include "expectimax.cpp"
#include "human.cpp"

int ans[12][12]; // ans[i][j] = 1 if i wins j, 2 if j wins i, 3 if draw
int main()
{    
	Game game(0, {{0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}}, 0, 0);
    vector<MinimaxStrategy> minimax_bots;
    vector<ExpectimaxStrategy> expectimax_bots;
    for (int depth = 0; depth <= 11; depth++)
    {
    	MinimaxStrategy minimax(&game, depth);
        ExpectimaxStrategy expectimax(&game, depth);
    	minimax_bots.push_back(minimax);
        expectimax_bots.push_back(expectimax);
    }
    // // MinimaxStrategy minimax13(&game, 13);
    // MinimaxStrategy minimax9(&game, 9);
    // MinimaxStrategy minimax8(&game, 8);
    // // ExpectimaxStrategy expectimax2(&game, 6);
    // // ExpectimaxStrategy expectimax1(&game, 7);
    // Human human(&game);
    // // minimax9.calculate_move().print();

    // // 1 7 0 0 0 0 0 1 0 0 0 0 0 3 0 0 0 0 0 1 0 0 0 0 0 0 3

    for (MinimaxStrategy p1_strats : minimax_bots)
    for (MinimaxStrategy p2_strats : minimax_bots)
    {
        game.set_state(0, {{0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}}, 0, 0);
        Player p1;
        p1.set_strategy(&p1_strats);
        Player p2;
        p2.set_strategy(&p2_strats);

        int num_turn = 1;
        while(!game.check_ending())
        {
            // game.check_borrow(game.turn);
            // cout << "TURN: " << num_turn << '\n';
            // game.print_table();
            if (game.turn == 0) p1.play();
            else p2.play();
            num_turn++;
            // cout << game.turn << '\n';
        }
        // game.print_table();
        ans[p1_strats.maxDepth][p2_strats.maxDepth] = game.check_ending();
        // int v = game.check_ending();
        // if (v == 1) cout << "P1 wins";
        // else if (v == 2) cout << "P2 wins";
        // else if (v == 3) cout << "Draw";
    }
    // for (int i = 0; i < 12; i++) cout << i << ' ';
    for (int i = 0; i < 12; i++)
    {
        // cout << i << ' ';
        for (int j = 0; j < 12; j++) cout << ans[i][j] << ' ';
        cout << '\n';
    }
    return 0;
}