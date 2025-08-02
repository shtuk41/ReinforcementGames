#pragma once

#include "deck.h"

class game
{
private:
	deck gameDeck;
	bool printDebug;

public:
	game(deck& deck, bool debug);
	void play(std::vector<State> &episode);
	reward play(double value[11][10][2]);
};
