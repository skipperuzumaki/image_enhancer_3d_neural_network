#include "Network.h"
#include "VideoHandler.h"
#include <string>
#include <iostream>

int main()
{
	std::string stringi;
	printf("enter input video location >>> ");
	std::getline(std::cin, stringi);
	std::string stringo;
	printf("enter output video location >>> ");
	std::getline(std::cin, stringo);
	printf("started");
	for (int i_ = 0; i_ < 500; i_++) {
		train(stringi, stringo, std::make_pair(640, 480), std::make_pair(1920, 1080));
		printf("epoch no %d done", i_);
	}
	return 0;
}