/*
# define STB_IMAGE_IMPLEMENTATION
*/
#include "stb/stb_image.h"

#include <iostream>
#include <string>
#include <fstream>

int main(void) {

	int x,y,n;
	std::string filename("/home/dale/local/game/models/textures/2k_earth_daymap.jpgadsfa");
	//std::ifstream inFile(filename, std::ifstream::in|std::ifstream::binary);
	std::ifstream inFile(filename);
	if(!inFile.good()) {
		std::cout << "could not find " << filename << std::endl;
		return 1;
	}
	unsigned char *data = stbi_load(filename.c_str(), &x, &y, &n, STBI_rgb_alpha);
	stbi_image_free(data);
	std::cout << "x: " << x << std::endl;
	std::cout << "y: " << y << std::endl;
	std::cout << "n: " << n << std::endl;
}
