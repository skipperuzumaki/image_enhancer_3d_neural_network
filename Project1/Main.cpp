#include "VideoHandler.h"
#include "Network.h"
#include "Matrix.h"
#include <string>
#include <vector>
#include <cassert>

int main()
{
	char stringi[128];
	printf("enter input video location");
	scanf("s", &stringi);
	char stringo[128];
	printf("enter output video location");
	scanf("%s", &stringo);
	int iterations;
	printf("enter number of iterations");
	scanf("%d", iterations);
	auto input = read_video(std::string(stringi), std::make_pair(720, 480));
	auto output = read_video(std::string(stringo), std::make_pair(1920, 1080));
	std::vector<std::pair<int, int>> arrangement;
	arrangement.push_back(std::make_pair(720, 480));
	arrangement.push_back(std::make_pair(480, 360));
	arrangement.push_back(std::make_pair(720, 480));
	arrangement.push_back(std::make_pair(1280, 720));
	arrangement.push_back(std::make_pair(1920, 1080));
	Network net = Network(arrangement, 50.0f);
	assert(input.second == output.second);
	for (int i_ = 0; i_ < iterations; i_++) {
		for (int i = 0; i < input.second; i++) {
			net.instantialise(input.first.at(i));
			net.evaluate();
			net.CalcCostDerivative(output.first.at(i), net.Nlayers);
			net.backprop(net.CalcCost(output.first.at(i)));
		}
		printf("epoch number %d done", i_);
	}
	return 0;
}