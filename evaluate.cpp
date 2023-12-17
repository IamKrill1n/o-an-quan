#include "game.cpp"
#include "minimax.cpp"

int eval1(const double w1, const double w2)
{    
    // Set the weights in the evaluation function
	cout << "Weights: " << w1 << ' ' << w2 << '\n';
    Game game;
    MinimaxStrategy minimax(&game);
    minimax.set_maxDepth(5);
    minimax.set_weights(w1, w2);
    Player p1;
    p1.set_strategy(&minimax);
    MinimaxStrategy minimax2(&game);
    minimax2.set_maxDepth(5);
    // Player p2;
    // p2.set_strategy(&minimax2);
	int num_wins = 0;
	for (double w1_ = 0; w1_ <= 0.5; w1_ += 0.1)
    {
        for (double w2_ = 0; w2_ <= 1; w2_ += 0.2)
        {
            minimax2.set_weights(w1_, w2_);
            Player p2;
            p2.set_strategy(&minimax2);
            game.reset();
            vector<Move> openingMoves = game.possible_move();
            for (int ai_player = 0; ai_player < 2; ai_player++)
            {
                for (Move firstMove : openingMoves)
                {
                    game.reset();
                    game.make_move(firstMove);
                    int num_turns = 1;
                    while(!game.check_ending())    
                    {
                        if (num_turns > 100) break;
                        if (game.turn == ai_player) p1.play(); // AI player
                        else p2.play();
                        num_turns++;
                    }
                    if (game.check_ending() == ai_player + 1) num_wins++;         
                }
            }
        }
		
	}
    // The fitness is the win rate
	cout << "Win rate: " << db(num_wins) / 500 << '\n';
    return num_wins;
}

// int eval2(const double gamma_)
// {    
//     // Set the weights in the evaluation function
// 	cout << "Gamma: " << gamma_ << '\t';
//     Game game;
//     MinimaxStrategy minimax(&game);
//     minimax.set_maxDepth(5);
//     minimax.set_gamma(gamma_);
//     Player p1;
//     p1.set_strategy(&minimax);
// 	int total_points = 0;
// 	for (double gamma = -0.5; gamma <= 0.5; gamma += 0.1)
//     {
// 		MinimaxStrategy minimax2(&game);
// 		minimax2.set_maxDepth(5);
// 		minimax2.set_gamma(gamma);
// 		Player p2;
// 		p2.set_strategy(&minimax2);
// 		game.reset();
// 		vector<Move> openingMoves = game.possible_move();
// 		for (int ai_player = 0; ai_player < 2; ai_player++)
// 		{
// 			for (Move firstMove : openingMoves)
// 			{
// 				game.reset();
// 				game.make_move(firstMove);
// 				int num_turns = 1;
// 				while(!game.check_ending())    
// 				{
// 					if (num_turns > 100) break;
// 					if (game.turn == ai_player) p1.play(); // AI player
// 					else p2.play();
// 					num_turns++;
// 				}
// 				if (game.check_ending())  
// 				{
// 					if (ai_player == 0) total_points += game.P1points;
// 					else total_points += game.P2points;
// 				}       
// 			}
// 		}
// 	}
// 	cout << total_points << '\n';
// 	// The fitness is the win rate
// 	return total_points;
// }
int main()
{
	cout << eval1(0.0690323, 0.60825);
}