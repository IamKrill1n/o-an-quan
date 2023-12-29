#include "game.cpp"
#include "minimax.cpp"

const int POPULATION_SIZE = 30, MUTATION_RATE = 2;
class RandomGenerator {
public:
    RandomGenerator() : rng(chrono::steady_clock::now().time_since_epoch().count()) {}

    mt19937& getRng() { return rng; }

private:
    mt19937 rng;
};
struct Individual
{   
    double w1, w2;
    double fitness;
    Individual(double w1, double w2) : w1(w1), w2(w2) {}
    void set_state(double w1_, double w2_, int fitness_)
    {
        w1 = w1_;
        w2 = w2_;
        fitness = fitness_;
    }
};
Individual* round1_population[POPULATION_SIZE];
Individual* round2_population[10];
double evaluate_fitness(const double w1, const double w2)
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
    minimax2.set_weights(0.0, 0.0);
    Player p2;
    p2.set_strategy(&minimax2);
    int num_wins = 0, num_games = 0;
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
            if (game.check_ending()) num_games++;
        }
    }

    // The fitness is the win rate
    return (num_wins * 1.0) / num_games;
}

double coevolution_fitness(const double w1, const double w2)
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
    Player p2;
    p2.set_strategy(&minimax2);
    int num_wins = 0, num_games = 0;
    game.reset();
    vector<Move> openingMoves = game.possible_move();
    for (Individual* other : round2_population)
    {
        minimax2.set_weights(other->w1, other->w2);
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
                if (game.check_ending()) num_games++;
            }
        }
    }

    // The fitness is the win rate
    return (num_wins * 1.0) / num_games;
}

void create_OG_population()
{
    // Create the initial population
    mt19937& rng = RandomGenerator().getRng();
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        double w1 = uniform_real_distribution<double>(0, 1)(rng);
        double w2 = uniform_real_distribution<double>(0, 1)(rng);
        // cout << "Weights: " << weights[0] << ' ' << weights[1] << '\n';
        round1_population[i] = new Individual(w1, w2);
        // cout << "Individual " << i << ": " << population[i]->weights[0] << ' ' << population[i]->weights[1] << '\n';
    }
}

Individual* mutate(Individual& child)
{
    mt19937& rng = RandomGenerator().getRng();
    Individual* mutation = &child;
    if (rng()%2) mutation->w1 = uniform_real_distribution<double>(0, 1)(rng);
    else mutation->w2 = uniform_real_distribution<double>(0, 1)(rng);
    return mutation;
}

Individual* crossover(Individual* parent1, Individual* parent2)
{
    mt19937& rng = RandomGenerator().getRng();
    return new Individual(uniform_real_distribution<double>(min(parent1->w1, parent2->w1), 
                                                            max(parent1->w1, parent2->w1))(rng), 
                          uniform_real_distribution<double>(min(parent1->w2, parent2->w2), 
                                                            max(parent1->w2, parent2->w2))(rng));
}

void create_new_generation(Individual* population[], int round)
{
    mt19937& rng = RandomGenerator().getRng();
    int n = (round == 1) ? POPULATION_SIZE : 10;
    for (int i = 0; n - i - 1 >= n/2; i++)
    {
        // Select two parents from the top half of the population
        int parent1 = uniform_int_distribution<int>(0, n / 2 - 1)(rng);
        int parent2 = uniform_int_distribution<int>(0, n / 2 - 1)(rng);
        while (parent2 == parent1) parent2 = uniform_int_distribution<int>(0, n / 2 - 1)(rng);

        // Crossover and mutation
        Individual* child1 = crossover(population[parent1], population[parent2]);
        int mutation = uniform_int_distribution<int>(1, 10)(rng);
        if (mutation <= MUTATION_RATE) child1 = mutate(*child1);
        // Replace an individual from the bottom half of the population with the new child
        delete population[n - i - 1];
        population[n - i - 1] = child1;
    }
}

void GA()
{
    for (int i = 0; i < 5; i++)
    {
        cout << "AR shit here we go again " << i + 1 << '\n';
        create_OG_population();
        for (int generation = 1; generation <= 10; generation++)
        {
            cout << "Generation " << generation << '\n';
            for(Individual* ind : round1_population) ind->fitness = evaluate_fitness(ind->w1, ind->w2);
            sort(round1_population, round1_population + POPULATION_SIZE, [](const Individual* a, const Individual* b) {return a->fitness > b->fitness;});
            // for (int i = 0; i < POPULATION_SIZE; i++) cout << population[i]->fitness << ' ';
            cout << "Best fitness: " << round1_population[0]->fitness << '\n';
            cout << "Worst fitness: " << round1_population[POPULATION_SIZE - 1]->fitness << '\n';
            // if (generation > 100) break;
            cout << "Best weights: " << round1_population[0]->w1 << ", " << round1_population[0]->w2 << '\n';
            if (generation < 10) create_new_generation(round1_population, 1);
        }
        round2_population[i] = round1_population[0];
        round2_population[i + 5] = round1_population[1];
    }
}

void tournament_arc()
{
    for (int generation = 1; generation <= 10; generation++)
    {
        cout << "Generation " << generation << '\n';
        for(Individual* ind : round2_population) ind->fitness = coevolution_fitness(ind->w1, ind->w2);
        sort(round2_population, round2_population + 10, [](const Individual* a, const Individual* b) {return a->fitness > b->fitness;});
        // for (int i = 0; i < POPULATION_SIZE; i++) cout << population[i]->fitness << ' ';
        cout << "Best fitness: " << round2_population[0]->fitness << '\n';
        cout << "Worst fitness: " << round2_population[9]->fitness << '\n';
        // if (generation > 100) break;
        cout << "Best weights: " << round2_population[0]->w1 << ", " << round2_population[0]->w2 << '\n';
        if (generation < 10) create_new_generation(round2_population, 2);
        else 
        {
            cout << "FINAL STANDINGS\n";
            for (int i = 0; i < 10; i++) cout << round2_population[i]->w1 << ' ' << round2_population[i]->w2 << '\n';
        }
    }

}

int main()
{
    GA();
    // round2_population[0] = new Individual(0.433064, 0.588927);
    // round2_population[1] = new Individual(0.372005, 0.66842);
    // round2_population[2] = new Individual(0.347153, 0.695459);
    // round2_population[3] = new Individual(0.391176, 0.650489);
    // round2_population[4] = new Individual(0.4194, 0.667149);
    // round2_population[5] = new Individual(0.44585, 0.60462);
    // round2_population[6] = new Individual(0.350393, 0.707576);
    // round2_population[7] = new Individual(0.350553, 0.696475);
    // round2_population[8] = new Individual(0.382359, 0.685102);
    // round2_population[9] = new Individual(0.406681, 0.66417);
    tournament_arc();
    return 0;
}
