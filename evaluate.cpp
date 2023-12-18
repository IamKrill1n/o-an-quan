#include "game.cpp"
#include "minimax.cpp"
#include "expectimax.cpp"

int eval1(int depth, const double w1, const double w2)
{    
    // Set the weights in the evaluation function
	cout << "Weights: " << w1 << ", " << w2 << '\n';
    Game game;
    MinimaxStrategy minimax(&game);
    minimax.set_maxDepth(depth);
    minimax.set_weights(w1, w2);
    Player p1;
    p1.set_strategy(&minimax);
    MinimaxStrategy minimax2(&game);
    minimax2.set_maxDepth(5);
    // Player p2;
    // p2.set_strategy(&minimax2);
    vector<int> num_points_won;
	int num_wins = 0, num_games = 0;
    double avg_points = 0.0;
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
                    if (!game.check_ending()) continue;
                    num_games++;
                    if (game.check_ending() == ai_player + 1) num_wins++;     
                    num_points_won.push_back((ai_player == 0) ? game.P1points : game.P2points);
                    avg_points += (ai_player == 0) ? game.P1points : game.P2points;
                }
            }
        }
		
	}
    avg_points /= num_games;
    double variance = 0.0;
    for (int i = 0; i < num_games; i++) {
        variance += (num_points_won[i] - avg_points) * (num_points_won[i] - avg_points);
    }
    variance /= num_games;
    // The fitness is the win rate
	cout << "Win rate: " << db(num_wins) / num_games << "\t" << "Average points: " << avg_points << '\t' << "Variance: " << variance << '\n';
    return num_wins;
}

int eval2(int depth_, int depth)
{    
    // Set the weights in the evaluation function
	// cout << "Weights: " << w1 << ", " << w2 << '\n';
    Game game;
    MinimaxStrategy minimax(&game);
    minimax.set_maxDepth(depth_);
    minimax.set_weights(0, 0);
    Player p1;
    p1.set_strategy(&minimax);
    MinimaxStrategy minimax2(&game);
    minimax2.set_weights(0, 0);
    // Player p2;
    // p2.set_strategy(&minimax2);
    vector<int> num_points_won;
	int num_wins = 0, num_games = 0;
    double avg_points = 0.0;
	minimax2.set_maxDepth(depth);
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
            if (!game.check_ending()) continue;
            num_games++;
            if (game.check_ending() == ai_player + 1) num_wins++;     
            num_points_won.push_back((ai_player == 0) ? game.P1points : game.P2points);
            avg_points += (ai_player == 0) ? game.P1points : game.P2points;
        }
    }
    avg_points /= num_games;
    double variance = 0.0;
    for (int i = 0; i < num_games; i++) {
        variance += (num_points_won[i] - avg_points) * (num_points_won[i] - avg_points);
    }
    variance /= num_games;
    // The fitness is the win rate
	cout << "Win rate: " << db(num_wins) / num_games << "\t" << "Average points: " << avg_points << '\t' << "Variance: " << variance << '\n';
    return num_wins;
}

int eval3(int depth_, const double w1, const double w2)
{    
    // Set the weights in the evaluation function
	cout << "Weights: " << w1 << ", " << w2 << '\n';
    Game game;
    MinimaxStrategy minimax(&game);
    minimax.set_maxDepth(depth_);
    minimax.set_weights(w1, w2);
    Player p1;
    p1.set_strategy(&minimax);
    ExpectimaxStrategy expectimax2(&game);
    // Player p2;
    // p2.set_strategy(&minimax2);
    vector<int> num_points_won;
	int num_wins = 0, num_games = 0;
    double avg_points = 0.0;
	for (int depth = 1; depth <= 7; depth++)
    {
        expectimax2.set_maxDepth(depth);
        Player p2;
        p2.set_strategy(&expectimax2);
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
                if (!game.check_ending()) continue;
                num_games++;
                if (game.check_ending() == ai_player + 1) num_wins++;     
                num_points_won.push_back((ai_player == 0) ? game.P1points : game.P2points);
                avg_points += (ai_player == 0) ? game.P1points : game.P2points;
            }
        }
	}
    avg_points /= num_games;
    double variance = 0.0;
    for (int i = 0; i < num_games; i++) {
        variance += (num_points_won[i] - avg_points) * (num_points_won[i] - avg_points);
    }
    variance /= num_games;
    // The fitness is the win rate
	cout << "Win rate: " << db(num_wins) / num_games << "\t" << "Average points: " << avg_points << '\t' << "Variance: " << variance << '\n';
    return num_wins;
}

int main()
{
    for (int i = 1; i <= 9; i++)
    {
        eval2(9, i);
    }
    // eval1(0.34908, 0.665755);
    // eval2(9, 0, 0);
    // cout << eval1(0, 0) << '\n';
    // cout << eval2(0, 0);
}