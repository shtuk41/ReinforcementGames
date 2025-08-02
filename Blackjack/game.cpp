#include "game.h"

game::game(deck& deck, bool debug) : gameDeck(deck), printDebug(debug)
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

	if (printDebug) std::cout << std::format("Dealer Vis {}: ", dealerCounter++);
	int cardVisible = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
	dealer += cardVisible;
	if (printDebug) std::cout << dealer << '\n';
	if (printDebug) std::cout << std::format("Dealer {}: ", dealerCounter++);
	dealer += gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
	if (printDebug) std::cout << dealer << '\n';

	int playerCounter = 1;
	if (printDebug) std::cout << std::format("Player {}: ", playerCounter++);
	int card = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
	player += card;
	if (card == 11)
		aceCount++;

	if (player > 21 && aceCount > 0)
	{
		player -= 10;
		aceCount--;
	}

	aceUsable = (aceCount > 0);
	episode.push_back({ player, cardVisible, aceUsable, stateReward });
	if (printDebug) std::cout << player << '\n';

	if (printDebug) std::cout << std::format("Player {}: ", playerCounter++);
	card = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
	player += card;

	if (card == 11)
		aceCount++;

	if (player > 21 && (aceCount > 0))
	{
		player -= 10;
		aceCount--;
	}

	aceUsable = (aceCount > 0);

	episode.push_back({ player, cardVisible, aceUsable, stateReward });
	if (printDebug) std::cout << player << '\n';

	//while (player < 13)
	while (player < 20)
	{
		if (printDebug) std::cout << std::format("Player {}: ", playerCounter++);
		card = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
		player += card;

		if (card == 11)
			aceCount++;

		if (player > 21 && (aceCount > 0))
		{
			player -= 10;
			aceCount--;
		}

		aceUsable = (aceCount > 0);

		episode.push_back({ player, cardVisible, aceUsable, stateReward });
		if (printDebug) std::cout << player << '\n';
	}

	while (dealer < 17)
	{
		if (printDebug) std::cout << std::format("Dealer {}: ", dealerCounter++);
		card = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
		dealer += card;

		if (card == 11)
			dealerAceCount++;

		if (dealer > 21 && dealerAceCount > 0)
		{
			dealer -= 10;
			dealerAceCount--;
		}

		if (printDebug) std::cout << dealer << '\n';
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

	if (printDebug) printReward(stateReward);

	//State& last_state = episode.back();
	//last_state.reward = stateReward;

	for (auto& a : episode)
	{
		a.reward = stateReward;
	}
}

reward game::play(double value[11][10][2])
{
	int player = 0;
	int dealer = 0;

	bool aceUsable = false;
	int aceCount = 0;

	int dealerAceCount = 0;

	int dealerCounter = 1;
	if (printDebug) std::cout << std::format("Dealer Vis {}: ", dealerCounter++);
	int cardVisible = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
	dealer += cardVisible;
	if (printDebug) std::cout << dealer << '\n';
	if (printDebug) std::cout << std::format("Dealer {}: ", dealerCounter++);
	dealer += gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
	if (printDebug) std::cout << dealer << '\n';

	int playerCounter = 1;
	if (printDebug) std::cout << std::format("Player {}: ", playerCounter++);
	int card = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
	player += card;
	if (card == 11)
		aceCount++;

	if (player > 21 && aceCount > 0)
	{
		player -= 10;
		aceCount--;
	}

	aceUsable = (aceCount > 0);

	if (printDebug) std::cout << player << '\n';

	if (printDebug) std::cout << std::format("Player {}: ", playerCounter++);
	card = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
	player += card;

	if (card == 11)
		aceCount++;

	if (player > 21 && (aceCount > 0))
	{
		player -= 10;
		aceCount--;
	}

	aceUsable = (aceCount > 0);

	if (printDebug) std::cout << player << '\n';

	int playerIndex = std::max(player, 12) - 12;
	int dealerIndex = cardVisible - 1;
	int aceIndex = aceUsable ? 1 : 0;

	while (player <= 21)
	{
		playerIndex = std::max(player, 12) - 12;
		dealerIndex = cardVisible - 1;
		aceIndex = aceUsable ? 1 : 0;

		double v_stick = value[playerIndex][dealerIndex][aceIndex];
		double v_hit = 0;

		for (int card = 1; card <= 13; card++)
		{
			int cardValue = std::min(card, 10);
			if (card == 1)
				cardValue = 11; //ace initially

			int newSum = player + cardValue;
			bool newUsableAce = aceUsable;

			if (card == 1)
				newUsableAce = true;

			if (newSum > 21 && newUsableAce)
			{
				newSum -= 10;
				newUsableAce = false;
			}

			if (newSum >= 12 && newSum <= 21)
			{
				v_hit += value[newSum - 12][cardVisible - 1][newUsableAce] / 13.0;
			}
			else if (newSum < 12)
			{
				// Estimate value with lowest index (treat as 12)
				v_hit += 1;
			}
			else
			{
				v_hit += 2 * static_cast<double>(reward::lose) / 13.0;
			}
		}

		if (player < 12 || v_hit > v_stick)
		{
			if (printDebug) std::cout << std::format("Player {}: ", playerCounter++);
			card = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
			player += card;

			if (card == 11)
				aceCount++;

			if (player > 21 && (aceCount > 0))
			{
				player -= 10;
				aceCount--;
			}

			aceUsable = (aceCount > 0);

			if (printDebug) std::cout << player << '\n';
		}
		else
			break;
	}

	while (dealer < 17)
	{
		if (printDebug) std::cout << std::format("Dealer {}: ", dealerCounter++);
		card = gameDeck.PrintCard(gameDeck.GetNextCard(), printDebug);
		dealer += card;

		if (card == 11)
			dealerAceCount++;

		if (dealer > 21 && dealerAceCount > 0)
		{
			dealer -= 10;
			dealerAceCount--;
		}

		if (printDebug) std::cout << dealer << '\n';
	}

	reward stateReward;

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

	if (printDebug) printReward(stateReward);

	return stateReward;


}
