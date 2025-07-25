#include <array>
#include <format>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>

constexpr int numberOfStates = 101;
constexpr double discount = 1.0;
constexpr double convergence_epsilon = 0.05;
constexpr double p_win = 0.4;

int main()
{
	std::array <double, numberOfStates> states = { 0 };
	std::array <double, numberOfStates> policy = { 0 };
	std::array <double, numberOfStates> action;

	for (int ii = 1; ii < 100; ii++)
	{
		action[ii] = std::min(ii, 100 - ii);
	}

	bool converged = false;

	while (!converged)
	{
		converged = true;

		for (int ii = 1; ii < 100; ii++)
		{
			double bestValue = -1e9;
			int bestAction = 0;

			for (int a = 1; a <= action[ii]; a++)
			{
				double newValue = 
									p_win * ((ii + a == 100 ? 1.0 : 0.0) + states[ii + a]) +
									(1.0 - p_win) * states[ii - a];

				if (newValue > bestValue)
				{
					bestValue = newValue;
					bestAction = a;
				}
			}
	
			if (std::fabs(states[ii] - bestValue) > convergence_epsilon)
			{
				converged = false;
			}

			states[ii] = bestValue;
			policy[ii] = bestAction;
		}
	}

	std::cout << "State\tValue\tPolicy\n";
	for (int ii = 1; ii < 100; ++ii)
	{
		std::cout << std::format("{}\t{:.3f}\t{}\n", ii, states[ii], policy[ii]);
	}
}