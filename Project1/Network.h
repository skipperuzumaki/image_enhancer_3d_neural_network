#pragma once
#include "Matrix.h"
#include <vector>

class Network {
private:
	float delta = std::numeric_limits<float>::min();
	std::vector<Matrix> Layers;
	std::vector<Matrix> WeightsA;
	std::vector<Matrix> WeightsB;
	std::vector<Matrix> Biases;
	std::vector<std::vector<Matrix>> Cost;
public:
	int Nlayers;
	void instantialise(Matrix init);
	Network(std::vector<std::pair<int, int>> arrangement, float range);
	Matrix evaluate();
	void CalcCostDerivative(Matrix DesiredOutput, int stage);
	float CalcCost(Matrix DesiredOutput);
	std::pair<std::pair<int, int>, std::pair<int, int>> Getaw(std::pair<int, int> el1, std::pair<int, int> el2);
	std::vector<std::pair<int, int>> Bakval(bool A, int i, int j,int extntr,int extntc);
	void backprop(float cost);
};