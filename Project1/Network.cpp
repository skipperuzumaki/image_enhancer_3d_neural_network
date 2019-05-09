#include "Network.h"
#include <fstream>
#include <iostream>
#include <stdio.h>

void Network::instantialise(Matrix init)
{
	Layers.at(0) = init;
}

void Network::save()
{
	std::ofstream file;
	remove("train.bat");
	file.open("train.bat", std::ios::app);
	file.write((char*)this, sizeof(this));
}

void Network::load()
{
	std::vector<std::pair<int, int>> arr;
	arr.push_back(std::make_pair(0, 0));
	Network temp = Network(arr, 1);
	std::ifstream file;
	file.open("train.bat", std::ios::in);
	file.read((char*)&temp, sizeof(file));
	Nlayers = temp.Nlayers;
	Layers = temp.Layers;
	WeightsA = temp.WeightsA;
	WeightsB = temp.WeightsB;
	Biases = temp.Biases;
}

Network::Network(std::vector<std::pair<int, int>> arrangement,float range)
{
	Nlayers = int(arrangement.size());
	Layers.clear();
	for (int i = 0; i < Nlayers; i++) {
		Layers.push_back(Matrix(arrangement.at(i).first, arrangement.at(i).second));
	}
	Cost.push_back(Layers);
	Biases.push_back(Matrix(0, 0));
	for (int i = 1; i < Nlayers; i++) {
		Matrix T1 = Matrix(Layers.at(i - 1).getcoulmns(), Layers.at(i).getrows());
		T1.RandomlyInitialise(range);
		WeightsA.push_back(T1);
		Matrix T2 = Matrix(Layers.at(i).getrows(), Layers.at(i - 1).getcoulmns());
		T2.RandomlyInitialise(range);
		WeightsB.push_back(T2);
		Matrix T3 = Matrix(Layers.at(i).getrows(), Layers.at(i).getcoulmns());
		T3.RandomlyInitialise(range);
		Biases.push_back(T3);
	}
	Cost.push_back(WeightsA);
	Cost.push_back(WeightsB);
	Cost.push_back(Biases);
}

Network::Network(const Network & rhs)
{
	Nlayers = rhs.Nlayers;
	Layers = rhs.Layers;
	WeightsA = rhs.WeightsA;
	WeightsB = rhs.WeightsB;
	Biases = rhs.Biases;
}

Matrix Network::evaluate()
{
	for (int i = 0; i < Nlayers-1; i++) {
		Matrix T1 = Layers.at(i) * WeightsA.at(i);
		Matrix T2 = WeightsB.at(i) * T1;
		Matrix T3 = T2 + Biases.at(i + 1);
		T3.Sigmoid();
		Layers.at(i + 1) = T3;
	}
	return Layers.at(Nlayers - 1);
}

void Network::CalcCostDerivative(Matrix DesiredOutput, int stage)
{
	if (stage==1) {
		return;
	}
	Cost.at(0).at(stage) = Layers.at(stage).CalcVariance(DesiredOutput);
	Cost.at(0).at(stage).Percentise(Cost.at(0).at(stage).Maxval());
	Cost.at(1).at(stage-1).Setall(0.0f);
	Cost.at(2).at(stage-1).Setall(0.0f);
	for (int i = 0; i < Layers.at(stage - 1).getrows(); i++) {
		for (int j = 0; j < Layers.at(stage - 1).getcoulmns(); j++) {
			for (int k = 0; k < Cost.at(0).at(stage).getrows(); k++) {
				for (int l = 0; l < Cost.at(0).at(stage).getcoulmns(); l++) {
					auto W = Getaw(std::make_pair(i, j), std::make_pair(k, l));
					Cost.at(1).at(stage - 1).put(W.first.first, W.first.second, Cost.at(1).at(stage - 1).get(W.first.first, W.first.second)+Cost.at(0).at(stage).get(k, l));
					Cost.at(2).at(stage - 1).put(W.second.first, W.second.second, Cost.at(1).at(stage - 1).get(W.second.first, W.second.second)+Cost.at(0).at(stage).get(k, l));
					Cost.at(1).at(stage - 1).Percentise(Cost.at(1).at(stage - 1).Maxval());
					Cost.at(2).at(stage - 1).Percentise(Cost.at(2).at(stage - 1).Maxval());
				}
			}
		}
	}
	Cost.at(3).at(stage) = Cost.at(0).at(stage).Invert();
	Cost.at(0).at(stage - 1).Setall(0.0f);
	for (int i = 0; i < Cost.at(1).at(stage - 1).getrows(); i++) {
		for (int j = 0; j < Cost.at(1).at(stage - 1).getcoulmns(); j++) {
			auto k = Bakval(true, i, j, Cost.at(1).at(stage - 1).getrows(), Cost.at(1).at(stage - 1).getcoulmns());
			for (int l = 0; l < k.size(); l++) {
				Cost.at(0).at(stage - 1).put(k.at(i).first, k.at(i).second, Cost.at(0).at(stage - 1).get(k.at(i).first, k.at(i).second)+std::abs(Cost.at(1).at(stage - 1).get(i, j)));
			}
		}
	}
	for (int i = 0; i < Cost.at(2).at(stage - 1).getrows(); i++) {
		for (int j = 0; j < Cost.at(2).at(stage - 1).getcoulmns(); j++) {
			auto k = Bakval(true, i, j, Cost.at(2).at(stage - 1).getrows(), Cost.at(2).at(stage - 1).getcoulmns());
			for (int l = 0; l < k.size(); l++) {
				Cost.at(0).at(stage - 1).put(k.at(i).first, k.at(i).second, Cost.at(0).at(stage - 1).get(k.at(i).first, k.at(i).second)+std::abs(Cost.at(2).at(stage - 1).get(i, j)));
			}
		}
	}
	CalcCostDerivative(Cost.at(0).at(stage - 1), stage - 1);
}

float Network::CalcCost(Matrix DesiredOutput)
{
	float rtn = 0.0f;
	Matrix T = Layers.at(Nlayers-1).CalcVariance(DesiredOutput);
	for (int i = 0; i < T.getrows(); i++) {
		for (int j = 0; j < T.getcoulmns(); j++) {
			rtn = rtn + (T.get(i, j)*T.get(i, j));
		}
	}
	rtn = rtn / (T.getrows()*T.getcoulmns());
	return std::sqrt(rtn);
}

std::pair<std::pair<int, int>, std::pair<int, int>> Network::Getaw(std::pair<int, int> el1, std::pair<int, int> el2)
{
	return std::make_pair(std::make_pair(el1.second, el2.second), std::make_pair(el2.first, el1.first));
}

std::vector<std::pair<int, int>> Network::Bakval(bool A, int i, int j, int extntr,int extntc)
{
	std::vector<std::pair<int, int>> rtn;
	if (A) {
		for (int il = 0; il < extntr; il++) {
			rtn.push_back(std::make_pair(il, i));
		}
	}
	else {
		for (int il = 0; il < extntc; il++) {
			rtn.push_back(std::make_pair(j, il));
		}
	}
	return rtn;
}

void Network::backprop(float cost)
{
	delta = cost / 10;
	for (int l = 0; l < Nlayers; l++) {
		for (int i = 0; i < Biases.at(l).getrows(); i++) {
			for (int j = 0; j < Biases.at(l).getcoulmns(); j++) {
				Biases.at(l).put(i, j, Biases.at(l).get(i, j) + Cost.at(3).at(l).get(1, j)*delta);
			}
		}
		for (int i = 0; i < WeightsA.at(l).getrows(); i++) {
			for (int j = 0; j < WeightsA.at(l).getcoulmns(); j++) {
				WeightsA.at(l).put(i, j, WeightsA.at(l).get(i, j) + Cost.at(1).at(l).get(1, j)*delta);
			}
		}
		for (int i = 0; i < WeightsB.at(l).getrows(); i++) {
			for (int j = 0; j < WeightsB.at(l).getcoulmns(); j++) {
				WeightsB.at(l).put(i, j, WeightsB.at(l).get(i, j) + Cost.at(1).at(l).get(1, j)*delta);
			}
		}
	}
}
