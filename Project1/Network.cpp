#include "Network.h"
#include <fstream>
#include <iostream>
#include <stdio.h>

void Network::instantialise(Matrix* init)
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
		Layers.push_back(new Matrix(arrangement.at(i).first, arrangement.at(i).second));
	}
	Cost.push_back(Layers);
	Biases.push_back(new Matrix(0, 0));
	for (int i = 1; i < Nlayers; i++) {
		Matrix* T1 = new Matrix(Layers[(i - 1)]->getcoulmns(), Layers[(i)]->getrows());
		T1->RandomlyInitialise(range);
		WeightsA.push_back(T1);
		Matrix* T2 = new Matrix(Layers[(i)]->getrows(), Layers[(i - 1)]->getcoulmns());
		T2->RandomlyInitialise(range);
		WeightsB.push_back(T2);
		Matrix* T3 = new Matrix(Layers[(i)]->getrows(), Layers[(i)]->getcoulmns());
		T3->RandomlyInitialise(range);
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

Matrix* Network::evaluate()
{
	for (int i = 0; i < Nlayers-1; i++) {
		Matrix* T1 = (*Layers[(i)]) * (*WeightsA[(i)]);
		Matrix* T2 = (*WeightsB[(i)]) * (*T1);
		Matrix* T3 = (*T2) + (*Biases[(i + 1)]);
		T3->Sigmoid();
		Layers[(i + 1)] = T3;
	}
	return Layers[(Nlayers - 1)];
}

void Network::CalcCostDerivative(Matrix* DesiredOutput, int stage)
{
	if (stage==1) {
		return;
	}
	Cost[(0)][(stage)] = Layers[(stage)]->CalcVariance(DesiredOutput);
	Cost[(0)][(stage)]->Percentise(Cost[(0)][(stage)]->Maxval());
	Cost[(1)][(stage-1)]->Setall(0.0f);
	Cost[(2)][(stage-1)]->Setall(0.0f);
	for (int i = 0; i < Layers[(stage - 1)]->getrows(); i++) {
		for (int j = 0; j < Layers[(stage - 1)]->getcoulmns(); j++) {
			for (int k = 0; k < Cost[(0)][(stage)]->getrows(); k++) {
				for (int l = 0; l < Cost[(0)][(stage)]->getcoulmns(); l++) {
					auto W = Getaw(std::make_pair(i, j), std::make_pair(k, l));
					Cost[(1)][(stage - 1)]->put(W.first.first, W.first.second, Cost[(1)][(stage - 1)]->get(W.first.first, W.first.second)+Cost[(0)][(stage)]->get(k, l));
					Cost[(2)][(stage - 1)]->put(W.second.first, W.second.second, Cost[(1)][(stage - 1)]->get(W.second.first, W.second.second)+Cost[(0)][(stage)]->get(k, l));
					Cost[(1)][(stage - 1)]->Percentise(Cost[(1)][(stage - 1)]->Maxval());
					Cost[(2)][(stage - 1)]->Percentise(Cost[(2)][(stage - 1)]->Maxval());
				}
			}
		}
	}
	Cost[(3)][(stage)] = Cost[(0)][(stage)]->Invert();
	Cost[(0)][(stage - 1)]->Setall(0.0f);
	for (int i = 0; i < Cost[(1)][(stage - 1)]->getrows(); i++) {
		for (int j = 0; j < Cost[(1)][(stage - 1)]->getcoulmns(); j++) {
			auto k = Bakval(true, i, j, Cost[(1)][(stage - 1)]->getrows(), Cost[(1)][(stage - 1)]->getcoulmns());
			for (int l = 0; l < k.size(); l++) {
				Cost[(0)][(stage - 1)]->put(k.at(i).first, k.at(i).second, Cost[(0)][(stage - 1)]->get(k.at(i).first, k.at(i).second)+std::abs(Cost[(1)][(stage - 1)]->get(i, j)));
			}
		}
	}
	for (int i = 0; i < Cost[(2)][(stage - 1)]->getrows(); i++) {
		for (int j = 0; j < Cost[(2)][(stage - 1)]->getcoulmns(); j++) {
			auto k = Bakval(true, i, j, Cost[(2)][(stage - 1)]->getrows(), Cost[(2)][(stage - 1)]->getcoulmns());
			for (int l = 0; l < k.size(); l++) {
				Cost[(0)][(stage - 1)]->put(k.at(i).first, k.at(i).second, Cost[(0)][(stage - 1)]->get(k.at(i).first, k.at(i).second)+std::abs(Cost[(2)][(stage - 1)]->get(i, j)));
			}
		}
	}
	CalcCostDerivative(Cost.at(0).at(stage - 1), stage - 1);
}

float Network::CalcCost(Matrix* DesiredOutput)
{
	float rtn = 0.0f;
	Matrix* T = Layers[(Nlayers-1)]->CalcVariance(DesiredOutput);
	for (int i = 0; i < T->getrows(); i++) {
		for (int j = 0; j < T->getcoulmns(); j++) {
			rtn = rtn + (T->get(i, j)*T->get(i, j));
		}
	}
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
	for (int l = 0; l < Nlayers-1; l++) {
		for (int i = 0; i < Biases[(l)]->getrows(); i++) {
			for (int j = 0; j < Biases[(l)]->getcoulmns(); j++) {
				Biases[(l)]->put(i, j, Biases[(l)]->get(i, j) + Cost[(3)][(l)]->get(1, j)*delta);
			}
		}
		for (int i = 0; i < WeightsA[(l)]->getrows(); i++) {
			for (int j = 0; j < WeightsA[(l)]->getcoulmns(); j++) {
				WeightsA[(l)]->put(i, j, WeightsA[(l)]->get(i, j) + Cost[(1)][(l)]->get(1, j)*delta);
			}
		}
		for (int i = 0; i < WeightsB[(l)]->getrows(); i++) {
			for (int j = 0; j < WeightsB[(l)]->getcoulmns(); j++) {
				WeightsB[(l)]->put(i, j, WeightsB[(l)]->get(i, j) + Cost[(1)][(l)]->get(1, j)*delta);
			}
		}
	}
	for (int i = 0; i < Biases[(Nlayers-1)]->getrows(); i++) {
		for (int j = 0; j < Biases[(Nlayers-1)]->getcoulmns(); j++) {
			Biases[(Nlayers-1)]->put(i, j, Biases[(Nlayers-1)]->get(i, j) + Cost[(3)][(Nlayers-1)]->get(1, j)*delta);
		}
	}
}
