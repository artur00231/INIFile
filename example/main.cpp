#include "INIFile.h"

#include <iostream>

#include "tests.h"

#define TEST false

int main()
{
#if !TEST
	INIFile ini;

	try
	{
		ini.read("example.cfg");
	}
	catch (std::runtime_error & err)
	{
		std::cout << err.what() << std::endl;
		system("pause >> null");
		exit(EXIT_FAILURE);
	}
	if (!ini.good()) exit(1);

	try {

		std::cout << "data::y " << ini.get<int>("data", "y") << std::endl;
		std::cout << "info::ver " << ini.get<INIFile::string>("info", "ver") << std::endl; // INIFile::string == const std::string&
		std::cout << "sec::ban " << ini.get<bool>("sec", "ban") << std::endl;

		ini.set_section("data"); // set current section

		std::cout << "data::x " << ini.get<int>("x") << std::endl; // == ini.get<int>("data", "x")
		std::cout << "data::length " << ini.get<double>("data", "length") << std::endl;
		std::cout << "new::val " << ini.get<double>("new", "val") << std::endl;
		std::cout << "info::autor " << ini.get<INIFile::string>("info", "autor") << std::endl;

	}
	catch (std::out_of_range &e)
	{
		std::cout << e.what() << std::endl; // some values don't exist
	}

	if (ini.change("new", "val2", 55.5))
	{
		// val2 exist
	}
	else
	{
		// val2 don't exist
		ini.set("new", "val2", 55.5); // set val2
	}


	ini.write(); // == ini.write("example.cfg");

#else

	tests::run_tests();

#endif

	system("pause");

	return 0;
}

