#include <iostream>

#include "INIFile/INIFile.h"

int main()
{
	ini::INIFile ini{};

	if (!ini.load(std::filesystem::path{ "example.txt" }))
	{
		for (auto && x : ini.getLastParseErrorLog())
		{
			std::cerr << x << std::endl;
		}

		std::exit(EXIT_FAILURE);
	}

	std::cout << "data::y " << ini("data")["y"].get<int>() << std::endl;
	std::cout << "lib info::ver " << ini("lib info")["ver"].get<std::string>() << std::endl;
	std::cout << "data::a " << ini("data")["a"].get<std::string>() << std::endl;

	auto section = ini("data"); // set current section

	std::cout << "data::x " << section["x"].get<int>() << std::endl;
	std::cout << "data::dimensions::length " << ini("data")("dimensions")["length"].get<double>() << std::endl;
	std::cout << "data::z::0 " << ini("data")("z")["0"].get<int>() << std::endl;
	std::cout << "data::dimensions::width::precision  " << ini("data")("dimensions")("width")["precision"].get<double>() << std::endl;

	auto ini_file = ini.getProperty({ "ini_file", "ini_file" }).get<std::string>();

	if (!ini.load(ini_file))
	{
		for (auto && x : ini.getLastParseErrorLog())
		{
			std::cerr << x << std::endl;
		}

		std::exit(EXIT_FAILURE);
	}


	std::cout << "section::b " << ini("section")["b"].get<int>() << std::endl;

	ini.save("file.ini");

	return 0;
}
