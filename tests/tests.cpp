#include "tests.h"
#include <cctype>

tests::tests()
{
}


tests::~tests()
{
}

void tests::run_tests()
{
	try
	{
		bool show_partial_result = true;
		test_section_getter(show_partial_result);
		test_data_getter(show_partial_result);
	}
	catch (...)
	{
		std::cerr << "\n\n\nException throw\n\n\n";
	}
}

void tests::test_section_getter(bool show)
{
	std::size_t test_passed = 0;

	std::cout << "\nTest 1 start\n\n";

	std::cout << test_out("Test 1.1", show);
	std::string a = "[ \"nam e \"]";
	if (getSection(a) == "nam e ")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.2", show);
	a = "  [  n\\[ame  ]  ";
	if (getSection(a) == "n[ame")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.3", show);
	a = "s[name]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.4", show);
	a = "s [name]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.5", show);
	a = "[name] s";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.6", show);
	a = "[name]s";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.7", show);
	a = "[na me]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.8", show);
	a = "[na\\eme]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.9", show);
	a = "[\"name]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.10", show);
	a = "[name";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.11", show);
	a = "name]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.12", show);
	a = "[\"name\"\"fsf\"]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.13", show);
	a = "[\"na[]me\"]";
	if (getSection(a) == "na[]me")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.14", show);
	a = "[na[me]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.15", show);
	a = "[nam=e]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.16", show);
	a = "[\"na=me\"]";
	if (getSection(a) == "na=me")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.17", show);
	a = "[name]";
	if (getSection(a) == "name")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.18", show);
	a = "[]";
	try {
		getSection(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.19", show);
	a = "[name];fsfd";
	if (getSection(a) == "name")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.20", show);
	a = "[name] ;fsdf";
	if (getSection(a) == "name")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 1.21", show);
	a = "[nam\\\"e]";
	if (getSection(a) == "nam\"e")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << "Tests passed: " << (float)test_passed / 21 * 100 << "%\n";

	std::cout << "\nTest 1 end\n";
}

void tests::test_data_getter(bool show)
{
	std::size_t test_passed = 0;

	std::cout << "\nTest 2 start\n\n";

	std::string a = "data_name=data";
	std::pair < std::string, std::string> b;

	std::cout << test_out("Test 2.1", show);
	a = "data_name=data";
	b = getData(a);
	if (b.first == "data_name" && b.second == "data")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.2", show);
	a = "  data_name  =  data  ";
	b = getData(a);
	if (b.first == "data_name" && b.second == "data")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.3", show);
	a = "data name=data";
	try {
		getData(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.4", show);
	a = "data_name=dat a";
	try {
		getData(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.5", show);
	a = "data=name=data";
	try {
		getData(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.6", show);
	a = "\"data_name=data\"";
	try {
		getData(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.7", show);
	a = "data_n[ame=data";
	try {
		getData(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.8", show);
	a = "dat\"a_name=data";
	try {
		getData(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.9", show);
	a = "data_name=";
	try {
		getData(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.10", show);
	a = "=data";
	try {
		getData(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.11", show);
	a = "=";
	try {
		getData(a);
	}
	catch (std::runtime_error)
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.12", show);
	a = "data_name=\"da ta\"";
	b = getData(a);
	if (b.first == "data_name" && b.second == "da ta")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.13", show);
	a = "\"data name\"=data";
	b = getData(a);
	if (b.first == "data name" && b.second == "data")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.14", show);
	a = "\"data name\"=\"da ta\"";
	b = getData(a);
	if (b.first == "data name" && b.second == "da ta")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.15", show);
	a = "data_name=data;asdsad";
	b = getData(a);
	if (b.first == "data_name" && b.second == "data")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.16", show);
	a = "data_name=data ;asdsad";
	b = getData(a);
	if (b.first == "data_name" && b.second == "data")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << test_out("Test 2.17", show);
	a = "data\\\"name=data";
	b = getData(a);
	if (b.first == "data\"name" && b.second == "data")
	{
		std::cout << test_out("Passed", show);
		test_passed++;
	}

	std::cout << "Tests passed: " << (float)test_passed / 17 * 100 << "%\n";

	std::cout << "\nTest 2 end\n";
}

std::string tests::getSection(const std::string & line)
{
	enum class Parser_Section { Before, Name, After };

	Parser_Section parser_section = Parser_Section::Before;

	char character;
	std::string section_name;

	bool before_word = true;
	bool after_word = false;
	bool in_quotation_marks = false;

	try
	{
		for (std::string::size_type i = 0; i < line.size(); ++i)
		{
			character = line.at(i);

			switch (parser_section)
			{

			case Parser_Section::Before:
			{
				if (character == '[')
				{
					parser_section = Parser_Section::Name;
				}
				else if (!std::isspace(character))
				{
					throw std::runtime_error{ "Unexpected character: " + character };
				}
			}
				break;

			case Parser_Section::Name:
			{
				if (isspace(character))
				{
					if (in_quotation_marks)
					{
						section_name += character;
					}
					else
					{
						if (!(before_word || after_word))
						{
							after_word = true;
						}
					}
				}
				else
				{
					before_word = false;

					if (character == ']' && !in_quotation_marks)
					{
						if (section_name.size() == 0)
						{
							throw std::runtime_error{ "Unexpected character: " + character };
						}

						parser_section = Parser_Section::After;
						continue;
					}

					if (character == '\"')
					{
						if (!in_quotation_marks)
						{
							in_quotation_marks = true;
							before_word = false;
							continue;
						}
						else
						{
							in_quotation_marks = false;
							after_word = true;
							continue;
						}
					}

					if (character == '\\')
					{
						if (!in_quotation_marks)
						{
							++i;
							if (character = getSpecialCharacter(line.at(i)))
							{
								section_name += character;
								continue;
							}
							else
							{
								throw std::runtime_error{ "Unexpected character: " + character };
							}
						}
						else
						{
							if (character = getSpecialCharacter(line.at(i + 1)))
							{
								if (character == '\"')
								{
									i++;
									section_name += character;
									continue;
								}
								else
								{
									character = '\\';
								}
							}
						}
					}

					if (after_word)
					{
						throw std::runtime_error{ "Unexpected character: " + character };
					}

					if (!in_quotation_marks && !isValidCharacter(character))
					{
						throw std::runtime_error{ "Unexpected character: " + character };
					}

					section_name += character;
				}
			}
				break;

			case Parser_Section::After:
			{
				if (!std::isspace(character))
				{
					if (character == ';' || character == '#')
					{
						i = line.size();
						continue;
					}

					throw std::runtime_error{ "Unexpected character: " + character };
				}
			}
				break;
			default:
				break;
			}


		}
	}
	catch (std::out_of_range & excepion)
	{
		throw std::runtime_error{ "Unexpected end of line" };
	}

	if (parser_section != Parser_Section::After)
	{
		throw std::runtime_error{ "Unexpected end of line" };
	}

	return section_name;
}

std::pair<std::string, std::string> tests::getData(const std::string & line)
{
	enum class Parser_Section { Before, First_Name, Second_Name, After };

	Parser_Section parser_section = Parser_Section::Before;

	char character;
	std::string data_name;
	std::string data;

	bool before_word = true;
	bool after_word = false;
	bool in_quotation_marks = false;

	try
	{
		for (std::string::size_type i = 0; i < line.size(); ++i)
		{
			character = line.at(i);

			switch (parser_section)
			{

			case Parser_Section::Before:
			{
				if (!std::isspace(character))
				{
					parser_section = Parser_Section::First_Name;
					i--;
				}
			}
			break;

			case Parser_Section::First_Name:
			{
				if (isspace(character))
				{
					if (in_quotation_marks)
					{
						data_name += character;
					}
					else
					{
						if (!(before_word || after_word))
						{
							after_word = true;
						}
					}
				}
				else
				{
					before_word = false;

					if (character == '=' && !in_quotation_marks)
					{
						if (data_name.size() == 0)
						{
							throw std::runtime_error{ "Unexpected character: " + character };
						}

						parser_section = Parser_Section::Second_Name;
						before_word = true;
						after_word = false;
						in_quotation_marks = false;

						continue;
					}

					if (character == '\"')
					{
						if (!in_quotation_marks)
						{
							in_quotation_marks = true;
							before_word = false;
							continue;
						}
						else
						{
							in_quotation_marks = false;
							after_word = true;
							continue;
						}
					}

					if (character == '\\')
					{
						if (!in_quotation_marks)
						{
							++i;
							if (character = getSpecialCharacter(line.at(i)))
							{
								data_name += character;
								continue;
							}
							else
							{
								throw std::runtime_error{ "Unexpected character: " + character };
							}
						}
						else
						{
							if (character = getSpecialCharacter(line.at(i + 1)))
							{
								if (character == '\"')
								{
									i++;
									data_name += character;
									continue;
								}
								else
								{
									character = '\\';
								}
							}
						}
					}

					if (after_word)
					{
						throw std::runtime_error{ "Unexpected character: " + character };
					}

					if (!in_quotation_marks && !isValidCharacter(character))
					{
						throw std::runtime_error{ "Unexpected character: " + character };
					}

					data_name += character;
				}
			}
			break;

			case Parser_Section::Second_Name:
			{
				if (isspace(character))
				{
					if (in_quotation_marks)
					{
						data += character;
					}
					else
					{
						if (!(before_word || after_word))
						{
							if (data.size() == 0)
							{
								throw std::runtime_error{ "Unexpected character: " + character };
							}

							parser_section = Parser_Section::After;
						}
					}
				}
				else
				{
					before_word = false;


					if (character == '\"')
					{
						if (!in_quotation_marks)
						{
							in_quotation_marks = true;
							before_word = false;
							continue;
						}
						else
						{
							in_quotation_marks = false;
							after_word = true;

							if (i == line.size() - 1)
							{
								if (data.size() == 0)
								{
									throw std::runtime_error{ "Unexpected end of line" };
								}

								parser_section = Parser_Section::After;
							}

							continue;
						}
					}

					if (character == '\\')
					{
						if (!in_quotation_marks)
						{
							++i;
							if (character = getSpecialCharacter(line.at(i)))
							{
								data+= character;
								continue;
							}
							else
							{
								throw std::runtime_error{ "Unexpected character: " + character };
							}
						}
						else
						{
							if (character = getSpecialCharacter(line.at(i + 1)))
							{
								if (character == '\"')
								{
									i++;
									data += character;
									continue;
								}
								else
								{
									character = '\\';
								}
							}
						}
					}

					if (after_word)
					{
						throw std::runtime_error{ "Unexpected character: " + character };
					}

					if (!in_quotation_marks && !isValidCharacter(character))
					{
						if (character == ';' || character == '#')
						{
							parser_section = Parser_Section::After;
							i--;
							continue;
						}

						throw std::runtime_error{ "Unexpected character: " + character };
					}

					data += character;

					if (i == line.size() - 1)
					{
						if (!in_quotation_marks)
						{
							if (data.size() == 0)
							{
								throw std::runtime_error{ "Unexpected end of line" };
							}

							parser_section = Parser_Section::After;
						}
					}
				}
			}
			break;

			case Parser_Section::After:
			{
				if (!std::isspace(character))
				{
					if (character == ';' || character == '#')
					{
						i = line.size();
						continue;
					}

					throw std::runtime_error{ "Unexpected character: " + character };
				}
			}
			break;

			default:
				break;
			}


		}
	}
	catch (std::out_of_range & excepion)
	{
		throw std::runtime_error{ "Unexpected end of line" };
	}

	if (parser_section != Parser_Section::After)
	{
		throw std::runtime_error{ "Unexpected end of line" };
	}

	return { data_name, data };
}

char tests::getSpecialCharacter(char character)
{
	switch (character)
	{
	case ';':
		return ';';
		break;

	case '#':
		return '#';
		break;

	case '\\':
		return '\\';
		break;

	case '=':
		return '=';
		break;

	case '[':
		return '[';
		break;

	case ']':
		return ']';
		break;

	case '\"':
		return '\"';
		break;

	default:
		return 0;
		break;
	}
}

bool tests::isValidCharacter(char cahracter)
{
	return !(
		cahracter == ']' ||
		cahracter == '[' ||
		cahracter == '\\' ||
		cahracter == '=' ||
		cahracter == '#' ||
		cahracter == ';'
		);
}
