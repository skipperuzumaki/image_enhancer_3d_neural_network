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

void Network::CalcCostLayers(Matrix DesiredOutput, int stage)
{
	if (stage==1) {
		return;
	}
	Cost.at(0).at(stage) = Layers.at(stage).CalcVariance(DesiredOutput);
	Cost.at(0).at(stage).Percentise(Cost.at(0).at(stage).Maxval());
	Cost.at(1).at(stage-1).Setall(1.0f);
	Cost.at(2).at(stage-1).Setall(1.0f);
	for (int i = 0; i < Layers.at(stage - 1).getrows(); i++) {
		for (int j = 0; j < Layers.at(stage - 1).getcoulmns(); j++) {
			for (int k = 0; k < Cost.at(0).at(stage).getrows; k++) {
				for (int l = 0; l < Cost.at(0).at(stage).getcoulmns(); l++) {
					auto W = Getaw(std::make_pair(i, j), std::make_pair(k, l));
					Cost.at(1).at(stage - 1).put(W.first.first, W.first.second, Cost.at(1).at(stage - 1).get(W.first.first, W.first.second)*Cost.at(0).at(stage).get(k, l));
					Cost.at(2).at(stage - 1).put(W.second.first, W.second.second, Cost.at(1).at(stage - 1).get(W.second.first, W.second.second)*Cost.at(0).at(stage).get(k, l));
					Cost.at(1).at(stage - 1).Percentise(Cost.at(1).at(stage - 1).Maxval());
					Cost.at(2).at(stage - 1).Percentise(Cost.at(2).at(stage - 1).Maxval());
				}
			}
		}
	}
}

std::pair<std::pair<int, int>, std::pair<int, int>> Network::Getaw(std::pair<int, int> el1, std::pair<int, int> el2)
{
	return std::make_pair(std::make_pair(el1.second, el2.second), std::make_pair(el2.first, el1.first));
}
