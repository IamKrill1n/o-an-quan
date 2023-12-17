#include "game.cpp"
#include "minimax.cpp"

int eval1(const double gamma_)
{    
    // Set the weights in the evaluation function
	cout << "Gamma: " << gamma_ << '\t';
    Game game;
    MinimaxStrategy minimax(&game);
    minimax.set_maxDepth(5);
    minimax.set_gamma(gamma_);
    Player p1;
    p1.set_strategy(&minimax);
	int num_wins = 0;
	for (double gamma = -0.5; gamma <= 0.5; gamma += 0.1)
    {
		MinimaxStrategy minimax2(&game);
		minimax2.set_maxDepth(5);
		minimax2.set_gamma(gamma);
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
    // The fitness is the win rate
	cout << "Num wins: " << num_wins << '\n';
    return num_wins;
}

int eval2(const double gamma_)
{    
    // Set the weights in the evaluation function
	cout << "Gamma: " << gamma_ << '\t';
    Game game;
    MinimaxStrategy minimax(&game);
    minimax.set_maxDepth(5);
    minimax.set_gamma(gamma_);
    Player p1;
    p1.set_strategy(&minimax);
	int total_points = 0;
	for (double gamma = -0.5; gamma <= 0.5; gamma += 0.1)
    {
		MinimaxStrategy minimax2(&game);
		minimax2.set_maxDepth(5);
		minimax2.set_gamma(gamma);
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
				if (game.check_ending())  
				{
					if (ai_player == 0) total_points += game.P1points;
					else total_points += game.P2points;
				}       
			}
		}
	}
	cout << total_points << '\n';
	// The fitness is the win rate
	return total_points;
}
// void gradient_descent()
// {
// 	double gamma = 0;
// 	double learning_rate = 1e-3;
// 	double eps = 1e-2;
// 	double fitness = eval2(gamma);
// 	for (int i = 0; i < 1000; i++)
// 	{
// 		double new_fitness = eval2(gamma + eps);
// 		double diff = new_fitness - fitness;
// 		fitness = new_fitness;
// 		// cout << diff << '\n';
// 		gamma += learning_rate * diff;
// 	}
// 	cout << "Optimal gamma: " << gamma << '\n';
// }
int main()
{
	// gradient_descent();
	for (double gamma = -0.5; gamma <= 0.5; gamma += 0.1)
	{
		eval1(gamma);
	}
}