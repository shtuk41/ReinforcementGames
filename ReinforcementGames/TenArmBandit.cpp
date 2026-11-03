#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

constexpr int NUMBER_BANDITS = 10;
constexpr int NUMBER_GAMES = 2000;
const double epsilon = 0.1;
const double INITIAL_Q_VALUE = 5.0;

struct Bandit
{
private:
	double mean;
	double variance;
	std::normal_distribution<double> distribution;

public:
	Bandit(double m, double v) : mean(m), variance(v), distribution(m, std::sqrt(v))
	{
	}

	double getReward(std::mt19937& gen)
	{
		return distribution(gen);
	}
};


int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	
	std::array<double, NUMBER_BANDITS> Q; //estimate
	Q.fill(INITIAL_Q_VALUE);
	std::array<double, NUMBER_BANDITS> N = { 0 }; //number of actions taken

	std::vector<Bandit> bandits;
	bandits.reserve(NUMBER_BANDITS);
	bandits.emplace_back(Bandit(0.2,1.0));
	bandits.emplace_back(Bandit(-0.8, 1.0));
	bandits.emplace_back(Bandit(1.6, 1.0));
	bandits.emplace_back(Bandit(0.4, 1.0));
	bandits.emplace_back(Bandit(1.2, 1.0));
	bandits.emplace_back(Bandit(-1.5, 1.0));
	bandits.emplace_back(Bandit(-0.2, 1.0));
	bandits.emplace_back(Bandit(-0.9, 1.0));
	bandits.emplace_back(Bandit(1, 1.0));
	bandits.emplace_back(Bandit(-0.5, 1.0));


	std::uniform_real_distribution explorationDistribution(0.0, 1.0);
	std::uniform_int_distribution banditDistribution(0, 9);

	for (int ii = 0; ii < NUMBER_GAMES; ii++)
	{
		int maxBanditIndex = -1;

		if (explorationDistribution(gen) > epsilon)
		{
			//eploitation
			double maxReward = -std::numeric_limits<double>::infinity();

			for (int jj = 0; jj < NUMBER_BANDITS; jj++)
			{
				if (Q[jj] > maxReward)
				{
					maxReward = Q[jj];
					maxBanditIndex = jj;
				}
			}

			std::cout << "Explotation selected: " << maxBanditIndex << "\n";
		}
		else
		{
			//exploration
			maxBanditIndex = banditDistribution(gen);

			std::cout << "Exploration selected: " << maxBanditIndex << "\n";
		}

		double R = bandits[maxBanditIndex].getReward(gen);
		N[maxBanditIndex] += 1;
		Q[maxBanditIndex] = Q[maxBanditIndex] + 1.0 / N[maxBanditIndex] * (R - Q[maxBanditIndex]);
	}

}