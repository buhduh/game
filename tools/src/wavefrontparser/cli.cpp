#include <iostream>
#include <stdexcept>

#include "wavefrontparser/cli.hpp"
#include "cxxopts/cxxopts.hpp"

Args parseCLI(int argc, char* argv[]) {
	cxxopts::Options options("modelparser", "a model parser.");
	options
		.add_options()
		("i,input", "Input", cxxopts::value<std::string>())
		("o,output", "Output file", cxxopts::value<std::string>());
	options.parse_positional({"input", "output"});
	auto result = options.parse(argc, argv);
	if(!result.count("input") || !result.count("output")) {
		std::cout << "help not implemented!" << std::endl;
		throw std::runtime_error("positional args, input and output, are required!");
	}
	return Args{
		result["input"].as<std::string>(),
		result["output"].as<std::string>()
	};
}
