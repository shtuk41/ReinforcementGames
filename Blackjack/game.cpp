#include "game.h"

game::game(deck& deck, bool training) : gameDeck(deck), training(training)
{

}

void game::play(std::vector<State>& episode)
{
	int player = 0;
	int dealer = 0;
	reward stateReward = reward::draw;

	bool aceUsable = false;
	int aceCount = 0;

	int dealerAceCount = 0;

	int dealerCounter = 1;
	std::cout << std::format("Dealer Vis {}: ", dealerCounter++);
	int cardVisible = gameDeck.PrintCard(gameDeck.GetNextCard());
	dealer += cardVisible;
	std::cout << dealer << '\n';
	std::cout << std::format("Dealer {}: ", dealerCounter++);
	dealer += gameDeck.PrintCard(gameDeck.GetNextCard());
	std::cout << dealer << '\n';

	int playerCounter = 1;
	std::cout << std::format("Player {}: ", playerCounter++);
	int card = gameDeck.PrintCard(gameDeck.GetNextCard());
	player += card;
	aceCount = (card == 11) ? 1 : 0;
	aceUsable = (aceCount > 0);
	episode.push_back({ player, cardVisible, aceUsable, stateReward });
	std::cout << player << '\n';

	std::cout << std::format("Player {}: ", playerCounter++);
	card = gameDeck.PrintCard(gameDeck.GetNextCard());
	player += card;

	if (card == 11)
		aceCount++;

	aceUsable = (aceCount > 0);

	if (player > 21 && aceUsable)
	{
		player -= 10;
		aceCount--;
	}

	episode.push_back({ player, cardVisible, aceUsable, stateReward });
	std::cout << player << '\n';

	while (player < 20)
	{
		std::cout << std::format("Player {}: ", playerCounter++);
		card = gameDeck.PrintCard(gameDeck.GetNextCard());
		player += card;

		if (card == 11)
			aceCount++;

		aceUsable = (aceCount > 0);

		if (player > 21 && aceUsable)
		{
			player -= 10;
			aceCount--;
		}

		aceUsable = (aceCount > 0);

		episode.push_back({ player, cardVisible, aceUsable, stateReward });
		std::cout << player << '\n';
	}

	while (dealer < 17)
	{
		std::cout << std::format("Dealer {}: ", dealerCounter++);
		card = gameDeck.PrintCard(gameDeck.GetNextCard());
		dealer += card;

		if (card == 11)
			dealerAceCount++;

		if (dealer > 21 && dealerAceCount > 0)
		{
			dealer -= 10;
			dealerAceCount--;
		}

		std::cout << dealer << '\n';
	}

	if (player > 21 && dealer > 21)
		stateReward = reward::draw;
	else if (player > 21)
		stateReward = reward::lose;
	else if (dealer > 21)
		stateReward = reward::win;
	else if (player > dealer)
		stateReward = reward::win;
	else if (dealer > player)
		stateReward = reward::lose;
	else if (dealer == player)
		stateReward = reward::draw;

	printReward(stateReward);

	State& last_state = episode.back();
	last_state.reward = stateReward;
}
