#include "game.cpp"
#include "minimax.cpp"

const int POPULATION_SIZE = 100;
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
    int fitness;
    Individual(double w1, double w2) : w1(w1), w2(w2) {}
    bool operator < (const Individual& other) const
    {
        return fitness > other.fitness;
    }
    void set_state(double w1_, double w2_, int fitness_)
    {
        w1 = w1_;
        w2 = w2_;
        fitness = fitness_;
    }
};
Individual* population[POPULATION_SIZE];

int evaluate_fitness(const double w1, const double w2)
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
    minimax2.set_weights(1.0, 0.0);
    Player p2;
    p2.set_strategy(&minimax2);
    int num_wins = 0;
    game.reset();
    vector<Move> openingMoves = game.possible_move();
    for (int ai_player = 0; ai_player < 2; ai_player++)
    {
        for (Move firstMove : openingMoves)
        {
            game.reset();
            game.make_move(firstMove);
            while(!game.check_ending())    
            {
                if (game.turn == ai_player) p1.play(); // AI player
                else p2.play();
            }

            if (game.check_ending() == ai_player + 1) num_wins++;
        }
    }

    // The fitness is the win rate
    return num_wins;
}
void create_OG_population()
{
    // Create the initial population
    mt19937& rng = RandomGenerator().getRng();
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        double w1 = uniform_real_distribution<double>(-0.5, 0.5)(rng);
        double w2 = uniform_real_distribution<double>(-0.5, 0.5)(rng);
        // cout << "Weights: " << weights[0] << ' ' << weights[1] << '\n';
        population[i] = new Individual(w1, w2);
        // cout << "Individual " << i << ": " << population[i]->weights[0] << ' ' << population[i]->weights[1] << '\n';
    }
}

Individual* mutate(Individual& child)
{
    mt19937& rng = RandomGenerator().getRng();
    Individual* mutation = &child;
    if (rng()%2) mutation->w1 = uniform_real_distribution<double>(-0.5, 0.5)(rng);
    else mutation->w2 = uniform_real_distribution<double>(-0.5, 0.5)(rng);
    return mutation;
}

Individual* crossover(Individual* parent1, Individual* parent2)
{
    mt19937& rng = RandomGenerator().getRng();
    return new Individual(parent1->w1, parent2->w2);;
}

void create_new_generation()
{
    mt19937& rng = RandomGenerator().getRng();
    // Sort the population by fitness
    sort(population, population + POPULATION_SIZE);
    cout << "Best fitness: " << population[0]->fitness << '\n';
    cout << "Worst fitness: " << population[POPULATION_SIZE - 1]->fitness << '\n';
    // The bottom half of the population is replaced by the offspring of the top half
    for (int i = 0; i < POPULATION_SIZE / 2; i++)
    {
        // Select two parents from the top half of the population
        int parent1 = uniform_int_distribution<int>(0, POPULATION_SIZE / 2 - 1)(rng);
        int parent2 = uniform_int_distribution<int>(0, POPULATION_SIZE / 2 - 1)(rng);
        while (parent2 == parent1) parent2 = uniform_int_distribution<int>(0, POPULATION_SIZE / 2 - 1)(rng);

        // Crossover and mutation
        Individual* child1 = crossover(population[parent1], population[parent2]);
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
    int generation = 0;
    while(1)
    {
        generation++;
        for(Individual* ind : population) //cout << ind->weights[0] << ' ' << ind->weights[1] << '\n';
        {
            ind->fitness = evaluate_fitness(ind->w1, ind->w2);
        }
        if (generation > 100) break;
        create_new_generation();
        cout << "Generation " << generation << ": " << population[0]->fitness << '\n';
    }
    cout << "Best weights: " << population[0]->w1 << ' ' << population[0]->w2 << '\n';
    return 0;
}
