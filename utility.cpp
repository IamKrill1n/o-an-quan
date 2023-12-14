#ifndef UTILITY_CPP
#define UTILITY_CPP

#include "game.cpp"

const int INF = 77777;

int utility1(Game& game, int isEnd)
{
    if (isEnd == 1) return INF - 1;
    else if (isEnd == 2) return -INF + 1;
    return 120 * (game.P1points - game.P2points);
}

int utility2(Game& game, int isEnd)
{
    const int ackerman = 5;
    if (isEnd == 1) return INF - 1;
    else if (isEnd == 2) return -INF + 1;
    int p1_stones = 0, p2_stones = 0;
    for (int cell = 1; cell < 6; cell++) p1_stones += game.state[cell][0];
	for (int cell = 7; cell < 12; cell++) p2_stones += game.state[cell][0];
    return 120 * ((game.P1points - game.P2points) * ackerman + p1_stones - p2_stones);
}

#endif