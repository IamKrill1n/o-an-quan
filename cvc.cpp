#include "minimax.cpp"
#include "expectimax.cpp"
#include "human.cpp"
// #include "utility.cpp"

int ans[24][24]; // ans[i][j] = 1 if i wins j, 2 if i loses to j, 3 if draw
int main()
{    
    Game game;
	game.reset();
    // 1 7 0 0 0 0 0 1 0 0 0 0 0 3 0 0 0 0 0 1 0 0 0 0 0 0 3
    MinimaxStrategy minimax_bot1(&game);
    minimax_bot1.set_maxDepth(5);
    // minimax_bot1.set_weights(0.140251, -0.217708);
    minimax_bot1.set_weights(0.388364, 0.563261);
    // Human human(&game);
    MinimaxStrategy minimax_bot2(&game);
    minimax_bot2.set_maxDepth(5);
    minimax_bot2.set_weights(0, 0);
    // ExpectimaxStrategy expectimax_bot(&game);
    // expectimax_bot.set_maxDepth(7);
    // expectimax_bot.set_utility(utility1);
    Player p1;
    Player p2;
    p1.set_strategy(&minimax_bot1);
    p2.set_strategy(&minimax_bot2);
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
    game.print_table();
    return 0;
}