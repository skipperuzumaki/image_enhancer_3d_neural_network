#include "VideoHandler.h"
#include "Network.h"
#include "Matrix.h"
#include <string>
#include <vector>
#include <cassert>

int main()
{
	std::vector<std::pair<int, int>> arrangement;
	arrangement.push_back(std::make_pair(720, 480));
	arrangement.push_back(std::make_pair(480, 360));
	arrangement.push_back(std::make_pair(720, 480));
	arrangement.push_back(std::make_pair(1280, 720));
	arrangement.push_back(std::make_pair(1920, 1080));
	Network net = Network(arrangement, 50.0f);
}