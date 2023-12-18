#ifndef HUMAN_CPP
#define HUMAN_CPP

#include "game.cpp"

class Human : public Strategy {
public:
	Human(Game* game) : Strategy(game) {}

	Move calculate_move() override
	{
		Move move;
		cin >> move.cell >> move.direction;
		return move;
	}
};

#endif