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
	void SetFirstN(int* c, int number);
	int PrintCard(int cardValue, bool debug);
	int GetNextCard();
};