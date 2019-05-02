#include "Network.h"
#include <cassert>

Network::Network(std::vector<std::pair<int, int>> arrangement)
{
	Nlayers = arrangement.size();
	Layers.clear();
	for (int i = 0; i < Nlayers; i++) {
		Layers.push_back(Matrix(arrangement.at(i).first, arrangement.at(i).second));
	}
	Cost.push_back(Layers);
	Biases.push_back(Matrix(0, 0));
	for (int i = 1; i < Nlayers; i++) {
		WeightsA.push_back(Matrix(Layers.at(i - 1).getcoulmns(), Layers.at(i).getrows()));
		WeightsB.push_back(Matrix(Layers.at(i).getrows(), Layers.at(i - 1).getcoulmns()));
		Biases.push_back(Matrix(Layers.at(i).getrows(), Layers.at(i).getcoulmns));
	}
	Cost.push_back(WeightsA);
	Cost.push_back(WeightsB);
	Cost.push_back(Biases);
}

Matrix Network::evaluate()
{
	for (int i = 0; i < Nlayers-1; i++) {
		Matrix T1 = Layers.at(i) * WeightsA.at(i);
		Matrix T2 = WeightsB.at(i) * T1;
		Matrix T3 = T2 + Biases.at(i + 1);
		Layers.at(i + 1) = T3;
	}
	return Layers.at(Nlayers - 1);
}

void Network::CalcCost(Matrix DesiredOutput)
{
	assert(DesiredOutput.getcoulmns() == Layers.at(Nlayers - 1).getcoulmns());
	assert(DesiredOutput.getrows() == Layers.at(Nlayers - 1).getrows());
	for (int r = 0; r < DesiredOutput.getrows(); r++) {
		for (int c = 0; c < DesiredOutput.getcoulmns(); c++) {

		}
	}
}
