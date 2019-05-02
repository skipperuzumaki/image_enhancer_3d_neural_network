#include "Network.h"

Network::Network(std::vector<std::pair<int, int>> arrangement)
{
	Nlayers = arrangement.size();
	Layers.clear();
	for (int i = 0; i < Nlayers; i++) {
		Layers.push_back(Matrix(arrangement.at(i).first, arrangement.at(i).second));
	}
	Biases.push_back(Matrix(0, 0));
	for (int i = 1; i < Nlayers; i++) {
		WeightsA.push_back(Matrix(Layers.at(i - 1).getcoulmns(), Layers.at(i).getrows()));
		WeightsB.push_back(Matrix(Layers.at(i).getrows(), Layers.at(i - 1).getcoulmns()));
		Biases.push_back(Matrix(Layers.at(i).getrows(), Layers.at(i).getcoulmns));
	}
}
