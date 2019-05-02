#pragma once
#include "Matrix.h"
#include <vector>

class Network {
private:
	int Nlayers;
	std::vector<Matrix> Layers;
	std::vector<Matrix> WeightsA;
	std::vector<Matrix> WeightsB;
	std::vector<Matrix> Biases;
	std::vector<std::vector<Matrix>> Cost;
public:
	Network(std::vector<std::pair<int,int>> arrangement);
	Matrix evaluate();
	void CalcCostLayers(Matrix DesiredOutput, int stage);
	std::pair<std::pair<int, int>, std::pair<int, int>> Getaw(std::pair<int, int> el1, std::pair<int, int> el2);
};