#include <iostream>

#define STD_LOG(MSG) (std::cout << MSG << std::endl)
#define STD_ERR(MSG) (std::cerr << MSG << std::endl)
#define QUIT(MSG) \
	STD_ERR(MSG); \
	exit(EXIT_FAILURE)

int main(void) {
	QUIT("yolo");
}
