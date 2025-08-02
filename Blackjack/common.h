#pragma once

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

enum suits { Clubs = 0, Dimonds = 1, Hearts = 2, Spades = 3, S_Total = 4 };
enum ranks { Ace = 0, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, C_Total = 13 };
const std::string suitNames[] = { "Clubs", "Diamonds", "Hearts", "Spades" };
const std::string rankNames[] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven",
							"Eight", "Nine", "Ten", "Jack", "Queen", "King" };

struct StateKeyHash {
	std::size_t operator()(const std::tuple<int, int, bool>& key) const noexcept {
		auto h1 = std::hash<int>{}(std::get<0>(key));
		auto h2 = std::hash<int>{}(std::get<1>(key));
		auto h3 = std::hash<bool>{}(std::get<2>(key));

		// Combine hashes (this is a common simple way)
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

enum actopm
{
	stick = 0,
	twist = 1
};

enum class reward
{
	win = 1,
	draw = 0,
	lose = -1
};

struct State
{
	int playerSum;
	int dealerCard;
	bool usableAce;
	reward reward;
};

#pragma region functions
void printReward(const reward& r);
