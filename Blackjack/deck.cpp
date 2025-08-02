#include "deck.h"

deck::deck()
{
	for (int ii = 0; ii < 52; ii++)
		cards[ii] = ii;
}

void deck::Shuffle()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 52 - 1; i > 0; --i) {
		std::uniform_int_distribution<> dis(0, i);
		int j = dis(gen);
		std::swap(cards[i], cards[j]);
	}
}

int deck::PrintCard(int cardValue)
{
	suits s = (suits)(cardValue / 13);
	ranks rank = (ranks)(cardValue % 13);

	std::cout << suitNames[s] << "-" << rankNames[rank] << '-';

	return GetRankValue(rank);
}

int deck::GetNextCard()
{
	if (counter < 52)
		return cards[counter++];

	throw std::exception("triedt to more than 52 cards");
}

int deck::GetRankValue(ranks r)
{
	int v = 10;
	switch (r)
	{
	case Ace:
		v = 11;
		break;
	case Two:
		v = 2;
		break;
	case Three:
		v = 3;
		break;
	case Four:
		v = 4;
		break;
	case Five:
		v = 5;
		break;
	case Six:
		v = 6;
		break;
	case Seven:
		v = 7;
		break;
	case Eight:
		v = 8;
		break;
	case Nine:
		v = 9;
		break;
	default: //Ten, Jack, Queen, King
		v = 10;
		break;
	}

	return v;
}