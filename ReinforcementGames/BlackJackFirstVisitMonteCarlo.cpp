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

constexpr int numberOfGames = 200;

struct StateKeyHash {
	std::size_t operator()(const std::tuple<int, int, bool>& key) const noexcept {
		auto h1 = std::hash<int>{}(std::get<0>(key));
		auto h2 = std::hash<int>{}(std::get<1>(key));
		auto h3 = std::hash<bool>{}(std::get<2>(key));

		// Combine hashes (this is a common simple way)
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

using StateKey = std::tuple<int, int, bool>;
std::unordered_map<StateKey, std::vector<double>, StateKeyHash> returns;
double Value[11][10][2] = { 0 };

struct deck
{
private:
	int counter = 0;
	int cards[52];

	int value(ranks r)
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

	int printCard(int cardValue)
	{
		suits s = (suits)(cardValue / 13);
		ranks rank = (ranks)(cardValue % 13);

		std::cout << suitNames[s] << "-" << rankNames[rank] << '-';

		return value(rank);
	}

	int getNextCard()
	{
		if (counter < 52)
			return cards[counter++];

		throw std::exception("triedt to more than 52 cards");
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

void printReward(const reward&r)
{
	switch (r)
	{
	case reward::draw:
		std::cout << "Draw\n";
		break;
	case reward::lose:
		std::cout << "Lost\n";
		break;
	case reward::win:
		std::cout << "Win\n";
		break;
	default:
		throw std::exception("unknown reward");
	};

	enum class reward
	{
		win = 1,
		draw = 0,
		lose = -1
	};
}

int main()
{
	for (int episodeN = 0; episodeN < numberOfGames; episodeN++)
	{
		std::cout << std::format("Episode: {}\n", episodeN);

		std::vector<State> episode;

		deck gameDeck;
		gameDeck.shuffle();

		int player = 0;
		int dealer = 0;
		reward stateReward = reward::draw;

		bool aceUsable = false;
		int aceCount = 0;

		int dealerAceCount = 0;

		int dealerCounter = 1;
		std::cout << std::format("Dealer Vis {}: ", dealerCounter++);
		int cardVisible = gameDeck.printCard(gameDeck.getNextCard());
		dealer += cardVisible;
		std::cout << dealer << '\n';
		std::cout << std::format("Dealer {}: ", dealerCounter++);
		dealer += gameDeck.printCard(gameDeck.getNextCard());
		std::cout << dealer << '\n';

		int playerCounter = 1;
		std::cout << std::format("Player {}: ", playerCounter++);
		int card = gameDeck.printCard(gameDeck.getNextCard());
		player += card;
		aceCount = (card == 11) ? 1 : 0;
		aceUsable = (aceCount > 0);
		episode.push_back({player, cardVisible, aceUsable, stateReward});
		std::cout << player << '\n';

		std::cout << std::format("Player {}: ", playerCounter++);
		card = gameDeck.printCard(gameDeck.getNextCard());
		player += card;

		if (card == 11)
			aceCount++;

		aceUsable = (aceCount > 0);

		if (player > 21 && aceUsable)
		{
			player -= 10;
			aceCount--;
		}

		episode.push_back({player, cardVisible, aceUsable, stateReward});
		std::cout << player << '\n';

		while (player < 20)
		{
			std::cout << std::format("Player {}: ", playerCounter++);
			card = gameDeck.printCard(gameDeck.getNextCard());
			player += card;

			if(card == 11)
				aceCount++;

			aceUsable = (aceCount > 0);

			if (player > 21 && aceUsable)
			{
				player -= 10;
				aceCount--;
			}

			aceUsable = (aceCount > 0);

			episode.push_back({player, cardVisible, aceUsable, stateReward});
			std::cout << player << '\n';
		}

		while (dealer < 17)
		{
			std::cout << std::format("Dealer {}: ", dealerCounter++);
			card = gameDeck.printCard(gameDeck.getNextCard());
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

		State & last_state = episode.back();
		last_state.reward = stateReward;

		updateValues(episode);

		std::cout << '\n';
	}

	return 0;
}