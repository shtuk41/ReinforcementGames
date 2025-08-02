#pragma once

#include "common.h"

struct deck
{
private:
	int counter = 0;
	int cards[52];

	int GetRankValue(ranks rank);
public:
	deck();

	void Shuffle();
	int PrintCard(int cardValue);
	int GetNextCard();
};