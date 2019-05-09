#include "Network.h"
#include "VideoHandler.h"

int main()
{
	char stringi[128];
	printf("enter input video location");
	scanf_s("s", &stringi);
	char stringo[128];
	printf("enter output video location");
	scanf_s("%s", &stringo);
	auto input = read_video(std::string(stringi), std::make_pair(720, 480));
	auto output = read_video(std::string(stringo), std::make_pair(1920, 1080));
	std::vector<std::pair<int, int>> arrangement;
	arrangement.push_back(std::make_pair(720, 480));
	arrangement.push_back(std::make_pair(480, 360));
	arrangement.push_back(std::make_pair(720, 480));
	arrangement.push_back(std::make_pair(1280, 720));
	arrangement.push_back(std::make_pair(1920, 1080));
	Network net = Network(arrangement, 50.0f);
	for (int i_ = 0; i_ < 500; i_++) {
		for (int i = 0; i < input.size(); i++) {
			net.instantialise(input.at(i));
			net.evaluate();
			net.CalcCostDerivative(output.at(i), net.Nlayers);
			net.backprop(net.CalcCost(output.at(i)));
		}
		printf("epoch number %d done", i_);
	}
	return 0;
}