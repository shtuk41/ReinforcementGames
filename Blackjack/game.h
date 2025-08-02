#pragma once

#include "deck.h"

class game
{
private:
	bool training;
	deck gameDeck;

public:
	game(deck& deck, bool training = false);
	void play(std::vector<State> &episode);
};
