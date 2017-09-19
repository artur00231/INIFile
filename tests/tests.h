#pragma once

#include <iostream>
#include <string>

class test_out
{
public:
	test_out(const std::string & text, bool use_cout) : _text{ text }, _use_cout{ use_cout } {};

private:
	bool _use_cout;
	const std::string & _text;

	friend std::ostream& operator<<(std::ostream & out, const test_out & to_out);
};

inline std::ostream& operator<<(std::ostream & out, const test_out & to_out)
{
	if (to_out._use_cout)
	{
		out << to_out._text << std::endl;
	}

	return out;
}

class tests
{
public:
	tests();
	~tests();

	static void run_tests();

protected:
	static void test_section_getter(bool show);
	static void test_data_getter(bool show);

	// To test
	static std::string getSection(const std::string & line);
	static std::pair<std::string, std::string> getData(const std::string & line);
	static char getSpecialCharacter(char cahracter);
	static bool isValidCharacter(char cahracter);
};

