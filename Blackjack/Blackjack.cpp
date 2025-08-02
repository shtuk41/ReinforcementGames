#include <algorithm>
#include <exception>
#include <format>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <numeric>

#include "common.h"
#include "deck.h"
#include "game.h"

constexpr int numberOfGamesTraining = 1000000;
constexpr int numberOfGamesTesting = 1000;
using StateKey = std::tuple<int, int, bool>;
std::unordered_map<StateKey, std::vector<double>, StateKeyHash> returns;
double Value[11][10][2] = { 0 };

void updateValues(const std::vector<State>& episode) {
	std::unordered_set<StateKey, StateKeyHash> visited;

	// Use terminal reward for all states
	int G = static_cast<int>(episode.back().reward);

	for (const auto& s : episode) {
		// Skip states outside value table range
		if (s.playerSum < 12 || s.playerSum > 21)
			continue;

		StateKey key = { s.playerSum, s.dealerCard, s.usableAce };

		// First-visit check and insert
		if (visited.insert(key).second) {
			// Store return
			returns[key].push_back(G);

			// Compute average return
			double avg = std::accumulate(returns[key].begin(), returns[key].end(), 0.0) / returns[key].size();

			// Update value table
			int playerIdx = s.playerSum - 12;
			int dealerIdx = s.dealerCard - 1;
			int aceIdx = s.usableAce ? 1 : 0;

			Value[playerIdx][dealerIdx][aceIdx] = avg;
		}
	}
}



int main()
{
	//deck gameDeck;

	//for (int ii = 0; ii < 52; ii++)
	//{
	//	int card = gameDeck.PrintCard(gameDeck.GetNextCard(), true);
	//	std::cout << std::format(" {}\n", card);
	//}

	for (int episodeN = 0; episodeN < numberOfGamesTraining; episodeN++)
	{
		//std::cout << std::format("Episode: {}\n", episodeN);

		std::vector<State> episode;

		deck gameDeck;
		gameDeck.Shuffle();

		int s[5] = { 1,2, 24, 3, 0};

		//gameDeck.SetFirstN(s, 5);

		game game(gameDeck, false);

		game.play(episode);

		updateValues(episode);
	}

	for (int kk = 0; kk < 11; kk++)
	{
		for (int jj = 0; jj < 10; jj++)
		{
			for (int ii = 0; ii < 2; ii++)
			{
				std::cout << std::format("{}, {}, {}: {}\n", kk + 12, jj + 1, ii, Value[kk][jj][ii]);
			}
		}
	}

	std::cout << "TEST GAMES\n";

	int winGames = 0;
	int drawGames = 0;
	int lostGames = 0;

	for (int g = 0; g < numberOfGamesTesting; g++)
	{
		deck gameDeck;
		gameDeck.Shuffle();
		game game(gameDeck, true);
		reward gameStatus = game.play(Value);
		
		if (gameStatus == reward::win)
		{
			winGames += 1;
		}
		else if (gameStatus == reward::draw)
		{
			drawGames += 1;
		}
		else if (gameStatus == reward::lose)
		{
			lostGames += 1;
		}
	}

	std::cout << std::format("Win {}, Draw {}, Lost {}", winGames, drawGames, lostGames);

	return 0;
}