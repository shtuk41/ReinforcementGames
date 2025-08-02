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

constexpr int numberOfGames = 200;
using StateKey = std::tuple<int, int, bool>;
std::unordered_map<StateKey, std::vector<double>, StateKeyHash> returns;
double Value[11][10][2] = { 0 };

void updateValues(const std::vector<State>& episode) {
	std::unordered_set<StateKey, StateKeyHash> visited;

	for (size_t t = 0; t < episode.size(); ++t) {
		const auto& s = episode[t];
		StateKey key = { s.playerSum, s.dealerCard, s.usableAce };

		if (visited.count(key) == 0) {
			// Compute return G from t to end
			int G = 0;
			for (size_t k = t; k < episode.size(); ++k)
				G += static_cast<int>(episode[k].reward);

			// Update returns
			returns[key].push_back(G);

			// Update Value as average of returns
			double avg = std::accumulate(returns[key].begin(), returns[key].end(), 0.0) / returns[key].size();

			int playerIdx = s.playerSum - 12;
			int dealerIdx = s.dealerCard - 1;
			int aceIdx = s.usableAce ? 1 : 0;

			if (s.playerSum < 12 || s.playerSum > 21) continue;

			Value[playerIdx][dealerIdx][aceIdx] = avg;

			visited.insert(key);
		}
	}
}

int main()
{
	for (int episodeN = 0; episodeN < numberOfGames; episodeN++)
	{
		std::cout << std::format("Episode: {}\n", episodeN);

		std::vector<State> episode;

		deck gameDeck;
		gameDeck.Shuffle();

		game game(gameDeck, true);

		game.play(episode);

		updateValues(episode);
	}

	return 0;
}