#ifndef CLI_HPP
#define CLI_HPP

#include <string>

//the cli parser is slow to compile, hence this pointless src file
struct Args {
	const std::string input;
	const std::string output;
};

Args parseCLI(int, char**);

#endif
