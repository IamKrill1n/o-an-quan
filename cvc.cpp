#include "minimax.cpp"
#include "expectimax.cpp"
#include "human.cpp"
#include "utility.cpp"

int ans[24][24]; // ans[i][j] = 1 if i wins j, 2 if i loses to j, 3 if draw
int main()
{    
	Game game(0, {{0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}}, 0, 0);
    // vector<MinimaxStrategy> minimax_bots;
    // vector<ExpectimaxStrategy> expectimax_bots;
    // for (int depth = 4; depth <= 10; depth++)
    // {
    // 	MinimaxStrategy minimax(&game, depth, utility1);
    //     ExpectimaxStrategy expectimax(&game, depth);
    // 	minimax_bots.push_back(minimax);
    //     if (depth <= 7) expectimax_bots.push_back(expectimax);
    // }
    // 1 7 0 0 0 0 0 1 0 0 0 0 0 3 0 0 0 0 0 1 0 0 0 0 0 0 3
    MinimaxStrategy minimax_bots(&game);
    minimax_bots.set_maxDepth(8);
    minimax_bots.set_utility(utility1);
    ExpectimaxStrategy expectimax_bots(&game);
    expectimax_bots.set_maxDepth(5);
    expectimax_bots.set_utility(utility1);
    Player p1;
    Player p2;
    p1.set_strategy(&expectimax_bots);
    p2.set_strategy(&minimax_bots);
    while(!game.check_ending())
    {
        // game.check_borrow(game.turn);
        // cout << "TURN: " << num_turn << '\n';
        game.print_table();
        // if (game.turn > 200) break;
        if (game.turn == 0) p1.play();
        else p2.play();
        // num_turn++;
        // cout << game.turn << '\n';
    }
    // for (int d1 = 0; d1 <= 18; d1++)
    // for (int d2 = 0; d2 <= 18; d2++)
    // {
    //     game.set_state(0, {{0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {0, 1}, {5, 0}, {5, 0}, {5, 0}, {5, 0}, {5, 0}}, 0, 0);
    //     Player p1;
    //     if (d1 <= 10) p1.set_strategy(&minimax_bots[d1]);
    //     else p1.set_strategy(&expectimax_bots[d1 - 11]);
    //     Player p2;
    //     if (d2 <= 10) p2.set_strategy(&minimax_bots[d2]);
    //     else p2.set_strategy(&expectimax_bots[d2 - 11]);
    //     cout << d1 << ' ' << d2 << '\n';
    //     int num_turn = 1;
    //     while(!game.check_ending())
    //     {
    //         // game.check_borrow(game.turn);
    //         // cout << "TURN: " << num_turn << '\n';
    //         // game.print_table();
    //         if (num_turn > 200) break;
    //         if (game.turn == 0) p1.play();
    //         else p2.play();
    //         num_turn++;
    //         // cout << game.turn << '\n';
    //     }
    //     // game.print_table();
    //     ans[d1][d2] = game.check_ending();
    //     // int v = game.check_ending();
    //     // if (v == 1) cout << "P1 wins";
    //     // else if (v == 2) cout << "P2 wins";
    //     // else if (v == 3) cout << "Draw";
    // }
    // freopen("stats.csv", "w", stdout);
    // cout << "\"                P2 strats\n\n\nP1 strats\"";
    // for (int i = 0; i <= 10; i++) cout << ",Minimax depth " << i;
    // for (int i = 0; i <= 7; i++) cout << ",Expectimax depth " << i;
    // cout << '\n';
    // for (int i = 0; i <= 10; i++)
    // {
    //     cout << "Minimax depth " << i << ',';
    //     for (int j = 0; j <= 18; j++) cout << ans[i][j] << ',';
    //     cout << '\n';
    // }
    // for (int i = 0; i <= 7; i++)
    // {
    //     cout << "Expectimax depth " << i << ',';
    //     for (int j = 0; j <= 18; j++) cout << ans[i + 11][j] << ',';
    //     cout << '\n';
    // }
    return 0;
}