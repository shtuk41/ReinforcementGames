#include <algorithm>
#include <iostream>
#include <random>
#include <string>

enum suits { Clubs = 0, Dimonds = 1, Hearts = 2, Spades = 3, S_Total = 4 };
enum ranks { Ace = 0, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, C_Total = 13 };
const std::string suitNames[] = { "Clubs", "Diamonds", "Hearts", "Spades" };
const std::string rankNames[] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven",
							"Eight", "Nine", "Ten", "Jack", "Queen", "King" };

struct deck
{
private:
	int cards[52];
public:
	deck()
	{
		for (int ii = 0; ii < 52; ii++)
			cards[ii] = ii;
	}

	void shuffle()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		for (int i = 52 - 1; i > 0; --i) {
			std::uniform_int_distribution<> dis(0, i);
			int j = dis(gen);
			std::swap(cards[i], cards[j]);
		}
	}

	void printCard(int cardNumber)
	{
		suits s = (suits)(cards[cardNumber] / 13);
		ranks rank = (ranks)(cards[cardNumber] % 13);

		std::cout << suitNames[s] << "-" << rankNames[rank] << ';';
	}
};

enum actopm
{
	stick = 0,
	hit = 1
};

enum class reward
{
	win = 1,
	draw = 0,
	lose = -1
};

int main()
{
	deck gameDeck;
	gameDeck.shuffle();
	gameDeck.printCard(0);
	std::cout << '\n';

	deck gameDeck1;
	gameDeck1.shuffle();
	gameDeck1.printCard(0);
	std::cout << '\n';



	return 0;
}