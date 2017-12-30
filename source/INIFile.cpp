#include "INIFile.h"
#include <regex>

INIFile::INIFile()
{
}


INIFile::~INIFile()
{
}


bool INIFile::read(const std::string& name)
{
	clear();

	_file_name = name;

	std::ifstream input(name);
	if (!input.good())
	{
		_file_name = "";
		return false;
	}

	std::string line;
	std::string current_section = "global";

	try
	{
		while (std::getline(input, line))
		{
			auto x = getLineType(line);

			if (x == Type::SECTION)
			{
				current_section = getSection(line);
				_ini_data[current_section];
			}

			if (x == Type::DATA)
			{
				auto data = getData(line);
				_ini_data[current_section][data.first] = data.second;
			}

		}
	}
	catch (...)
	{
		input.close();

		_ini_data.clear();

		throw;
	}

	input.close();

	_load = true;
	return true;
}


void INIFile::set_section(const std::string& section)
{
	_current_section = section;
}


bool INIFile::good() const
{
	return _load;
}


void INIFile::clear()
{
	_load = false;
	_ini_data.clear();
	_file_name = "";
	_current_section = "Global";
}

bool INIFile::check(std::vector<std::pair<std::string, std::string>>& names)
{
	for (auto && data : names)
	{
		if (!_ini_data.count(data.first))
		{
			return false;
		}

		auto map = _ini_data.at(data.first);

		if (!map.count(data.second))
		{
			return false;
		}
	}

	return true;
}


bool INIFile::write()
{
	return write(_file_name);
}


bool INIFile::write(std::string name)
{
	if (!_load)
	{
		return false;
	}

	if (name != "")
	{
		std::ofstream output(name);
		if (output.good())
		{
			for (const auto &x : _ini_data)
			{
				output << "[";

				auto section_name = x.first;

				section_name = std::regex_replace(section_name, std::regex("\""), "\\\"");

				if (std::find_if(section_name.begin(), section_name.end(), [](char a) -> bool {
					return std::isspace(a) ||
						a == ']' ||
						a == '[' ||
						a == '\\' ||
						a == '=' ||
						a == '#' ||
						a == ';'; }) != section_name.end())
				{
					output << '\"' << section_name << '\"';
				}
				else
				{
					output << section_name;
				}

				output << "]\n";

				for (const auto &a : x.second)
				{

					auto data_name = a.first;

					data_name = std::regex_replace(data_name, std::regex("\""), "\\\"");

					if (std::find_if(data_name.begin(), data_name.end(), [](char a) -> bool {
						return std::isspace(a) ||
							a == ']' ||
							a == '[' ||
							a == '\\' ||
							a == '=' ||
							a == '#' ||
							a == ';'; }) != data_name.end())
					{
						output << '\"' << data_name << '\"';
					}
					else
					{
						output << data_name;
					}

					output << "=" ;

					auto data = a.second;

					data = std::regex_replace(data, std::regex("\""), "\\\"");

					if (std::find_if(data.begin(), data.end(), [](char a) -> bool {
						return std::isspace(a) ||
							a == ']' ||
							a == '[' ||
							a == '\\' ||
							a == '=' ||
							a == '#' ||
							a == ';'; }) != data.end())
					{
						output << '\"' << data << '\"';
					}
					else
					{
						output << data;
					}

					output << '\n';
				}
			}

			output.close();
			return true;
		}

		output.close();
	}
	return false;
}

bool INIFile::exist(const std::string & section, const std::string & property) const
{
	if (_ini_data.count(section) == 1)
	{

		if (_ini_data.at(section).count(property) == 1)
		{
			return true;
		}
	}
	return false;
}

INIFile::Type INIFile::getLineType(const std::string & line)
{
	char first_non_blank_character = ' ';

	for (auto &x : line)
	{
		if (!std::isspace(x))
		{
			first_non_blank_character = x;
			break;
		}
	}

	if (first_non_blank_character == ';' || first_non_blank_character == '#')
	{
		return Type::COMMENT;
	}

	if (first_non_blank_character == '[')
	{
		return Type::SECTION;
	}

	if (!std::isspace(first_non_blank_character))
	{
		return Type::DATA;
	}

	return Type::BLANK;
}

std::string INIFile::getSection(const std::string & line)
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

std::pair<std::string, std::string> INIFile::getData(const std::string & line)
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
								data += character;
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

char INIFile::getSpecialCharacter(char character)
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

bool INIFile::isValidCharacter(char cahracter)
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
