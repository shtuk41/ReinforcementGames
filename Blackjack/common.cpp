#include "common.h"


void printReward(const reward& r)
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
}