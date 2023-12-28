#include "game.cpp"
#include "minimax.cpp"

const int POPULATION_SIZE = 30;
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
Individual* population[POPULATION_SIZE];

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
            num_games++;
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
        double w1 = uniform_real_distribution<double>(0, 0.5)(rng);
        double w2 = uniform_real_distribution<double>(0, 1)(rng);
        // cout << "Weights: " << weights[0] << ' ' << weights[1] << '\n';
        population[i] = new Individual(w1, w2);
        // cout << "Individual " << i << ": " << population[i]->weights[0] << ' ' << population[i]->weights[1] << '\n';
    }
}

Individual* mutate(Individual& child)
{
    mt19937& rng = RandomGenerator().getRng();
    Individual* mutation = &child;
    if (rng()%2) mutation->w1 = uniform_real_distribution<double>(0, 0.5)(rng);
    else mutation->w2 = uniform_real_distribution<double>(0, 1)(rng);
    return mutation;
}

Individual* crossover1(Individual* parent1, Individual* parent2)
{
    return new Individual(parent1->w1, parent2->w2);
}

Individual* crossover2(Individual* parent1, Individual* parent2)
{
    mt19937& rng = RandomGenerator().getRng();
    return new Individual(uniform_real_distribution<double>(min(parent1->w1, parent2->w1), 
                                                            max(parent1->w1, parent2->w1))(rng), 
                          uniform_real_distribution<double>(min(parent1->w2, parent2->w2), 
                                                            max(parent1->w2, parent2->w2))(rng));
}

void create_new_generation()
{
    mt19937& rng = RandomGenerator().getRng();
    // The bottom half of the population is replaced by the offspring of the top half
    for (int i = 0; POPULATION_SIZE - i - 1 >= POPULATION_SIZE/10; i++)
    {
        // Select two parents from the top half of the population
        int parent1 = uniform_int_distribution<int>(0, POPULATION_SIZE / 2 - 1)(rng);
        int parent2 = uniform_int_distribution<int>(0, POPULATION_SIZE / 2 - 1)(rng);
        while (parent2 == parent1) parent2 = uniform_int_distribution<int>(0, POPULATION_SIZE / 2 - 1)(rng);

        // Crossover and mutation
        Individual* child1 = crossover1(population[parent1], population[parent2]);
        int mutation = uniform_int_distribution<int>(1, 10)(rng);
        if (mutation <= 2) child1 = mutate(*child1);
        // Replace an individual from the bottom half of the population with the new child
        delete population[POPULATION_SIZE - i - 1];
        population[POPULATION_SIZE - i - 1] = child1;
    }
}
int main()
{
    create_OG_population();
    for (int generation = 1; generation <= 15; generation++)
    {
        cout << "Generation " << generation << '\n';
        for(Individual* ind : population) ind->fitness = evaluate_fitness(ind->w1, ind->w2);
        sort(population, population + POPULATION_SIZE, [](const Individual* a, const Individual* b) {return a->fitness > b->fitness;});
        // for (int i = 0; i < POPULATION_SIZE; i++) cout << population[i]->fitness << ' ';
        cout << "Best fitness: " << population[0]->fitness << '\n';
        cout << "Worst fitness: " << population[POPULATION_SIZE - 1]->fitness << '\n';
        // if (generation > 100) break;
        cout << "Best weights: " << population[0]->w1 << ", " << population[0]->w2 << '\n';
        create_new_generation();
    }
    return 0;
}
