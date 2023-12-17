#include "game.cpp"
#include "minimax.cpp"

const int POPULATION_SIZE = 10;
class RandomGenerator {
public:
    RandomGenerator() : rng(chrono::steady_clock::now().time_since_epoch().count()) {}

    mt19937& getRng() { return rng; }

private:
    mt19937 rng;
};
struct Individual
{
    vector<double> weights;
    int fitness;
    Individual(vector<double> weights_) : weights(weights_) {}
    bool operator < (const Individual& other) const
    {
        return fitness > other.fitness;
    }
    void set_state(vector<double> weights_, int fitness_)
    {
        weights = weights_;
        fitness = fitness_;
    }
};
Individual* population[POPULATION_SIZE];

int evaluate_fitness(const vector<double>& weights)
{    
    // Set the weights in the evaluation function
    cout << "Weights: " << weights[0] << ' ' << weights[1] << '\n';
    Game game;
    MinimaxStrategy minimax(&game);
    minimax.set_maxDepth(5);
    minimax.set_weights(weights);
    Player p1;
    p1.set_strategy(&minimax);
    MinimaxStrategy minimax2(&game);
    minimax2.set_maxDepth(5);
    minimax2.set_weights({1.0, 0,0});
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

            if (game.check_ending() == ai_player + 1) 
            {
                num_wins++;
                cout << "WIN\n";
            }
            else cout << "LOSE\n";   
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
        vector<double> weights;
        weights.push_back(1.0);
        weights.push_back(uniform_real_distribution<double>(0, 1)(rng));
        // cout << "Weights: " << weights[0] << ' ' << weights[1] << '\n';
        population[i] = new Individual(weights);
        // cout << "Individual " << i << ": " << population[i]->weights[0] << ' ' << population[i]->weights[1] << '\n';
    }
}

Individual* mutate(Individual& child)
{
    mt19937& rng = RandomGenerator().getRng();
    Individual* mutation = &child;
    int index = uniform_int_distribution<int>(0, child.weights.size()-1)(rng);
    mutation->weights[index] = uniform_real_distribution<double>(0, 5)(rng);
    return mutation;
}

Individual* crossover(Individual* parent1, Individual* parent2)
{
    mt19937& rng = RandomGenerator().getRng();
    vector<double> w;
    int mark = uniform_int_distribution<int>(1, parent1->weights.size()-1)(rng); // mark is the index of the crossover point
    for (int i = 0; i < mark; i++) w.push_back(parent1->weights[i]);
    for(int i = mark; i<parent1->weights.size(); i++) w.push_back(parent2->weights[i]);
    return new Individual(w);
}

void create_new_generation()
{
    mt19937& rng = RandomGenerator().getRng();
    // Sort the population by fitness
    sort(population, population + POPULATION_SIZE);
    cout << "Best fitness: " << population[0]->fitness << '\n';
    cout << "Worst fitness: " << population[POPULATION_SIZE - 1]->fitness << '\n';
    // The bottom half of the population is replaced by the offspring of the top half
    for (int i = 0; i < POPULATION_SIZE / 2; i += 2)
    {
        // Select two parents from the top half of the population
        int parent1 = uniform_int_distribution<int>(0, POPULATION_SIZE / 2 - 1)(rng);
        int parent2 = uniform_int_distribution<int>(0, POPULATION_SIZE / 2 - 1)(rng);
        while (parent2 == parent1) parent2 = uniform_int_distribution<int>(0, POPULATION_SIZE / 2 - 1)(rng);

        // Crossover and mutation
        Individual* child1 = crossover(population[parent1], population[parent2]);
        Individual* child2 = crossover(population[parent2], population[parent1]);

        // Replace an individual from the bottom half of the population with the new child
        population[POPULATION_SIZE / 2 + i] = child1;
        population[POPULATION_SIZE / 2 + i + 1] = child2;
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
            ind->fitness = evaluate_fitness(ind->weights);
        }
        if (generation > 10) break;
        create_new_generation();
        cout << "Generation " << generation << ": " << population[0]->fitness << '\n';
    }
    return 0;
}
