#include "INIParser.h"

#include <cctype>
#include <sstream>
#include <fstream>


namespace ini::exception
{
	class INIParserError : public std::exception
	{
	public:
		INIParserError(std::string message) : message{ std::move(message) } {};
		const char* what() const override { return message.c_str(); };
	private:
		std::string message;
	};

	class INIParserWarnig : public std::exception
	{
	public:
		INIParserWarnig(std::string message) : message{ std::move(message) } {};
		const char* what() const override { return message.c_str(); };
	private:
		std::string message;
	};
}

template<typename F, typename ... Arg>
auto invokeInCLocale(F f, Arg && ... arg)
{
	auto locale = std::locale::global(std::locale("C"));
	auto val = std::invoke(f, std::forward<Arg>(arg)...);
	std::locale::global(locale);
	return val;
}


using namespace std::string_literals;

bool ini::INIParser::parse(const std::filesystem::path & path, INIFile & ini_file)
{
	std::fstream file{};
	file.open(path.string(), std::ios_base::in);

	if (!file.good())
	{
		std::string message{ "Cannot open file \""s + path.string() +"\""s };
		throw exception::INIParserException{ message };
	}

	auto success = parse(file, ini_file);

	file.close();

	return success;
}

bool ini::INIParser::parse(const std::string & data, INIFile & ini_file)
{
	std::stringstream ss;
	ss << data;
	return parse(ss, ini_file);
}

bool ini::INIParser::parse(const char * data, INIFile & ini_file)
{
	std::stringstream ss;
	ss << data;
	return parse(ss, ini_file);
}

std::vector<std::string> ini::INIParser::getErrorLog() const
{
	return error_log;
}

ini::INIParser::DATA_TYPE ini::INIParser::getDataType(char shortcut) const
{
	switch (shortcut)
	{
	case 'i':
		return DATA_TYPE::INT;
		break;

	case 'l':
		return DATA_TYPE::LONG;
		break;

	case 'd':
		return DATA_TYPE::DOUBLE;
		break;

	case 'b':
		return DATA_TYPE::BOOL;
		break;

	case 's':
		return DATA_TYPE::STRING;
		break;

	default:
		std::string message{ "Incorrect shortcut \""s + shortcut + "\""s };
		throw exception::INIParserError{ message };
		break;
	}
}

ini::INIParser::DATA_TYPE ini::INIParser::getDataType(std::string data) const
{
	if (data.size() < 6)
	{
		std::string data_copy{ data };
		std::transform(data_copy.begin(), data_copy.end(), data_copy.begin(), std::tolower);

		if (data_copy == "true" || data_copy == "false")
		{
			return DATA_TYPE::BOOL;
		}
	}

	std::size_t number_of_digits{ 0 }, number_of_dots{ 0 };
	std::for_each(data.begin(), data.end(), [&number_of_digits, &number_of_dots](char x) {
		if (x == '.') number_of_dots++;
		if (std::isdigit(static_cast<unsigned char>(x))) number_of_digits++; // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
	});

	if (number_of_digits == data.size())
	{
		try {
			::invokeInCLocale([](auto && x) { return std::stoi(x); }, data); // Invoke in "C" locale to ensure consistent conversion results

			return DATA_TYPE::INT;
		}
		catch ([[maybe_unused]] const std::out_of_range & exception) // Number is too big for an int
		{
			try {
				::invokeInCLocale([](auto && x) { return std::stoll(x); }, data); // Invoke in "C" locale to ensure consistent conversion results

				return DATA_TYPE::LONG;
			}
			catch (...) // Number is too big for a long long int
			{
				// return string
			}
		}
		catch (...) // String can't be converted to an int
		{
			// return string
		}
	}

	if (number_of_digits + number_of_dots == data.size())
	{
		try {
			::invokeInCLocale([](auto && x) { return std::stod(x); }, data); // Invoke in "C" locale to ensure consistent conversion results

			return DATA_TYPE::DOUBLE;
		}
		catch (...) // String can't be converted to a double
		{
			// return string
		}
	}

	return DATA_TYPE::STRING;
}

bool ini::INIParser::parse(std::istream & input, INIFile & ini_file)
{	
	main_section = &ini_file;
	section = &ini_file;
	auto last_section = section;

	error_log.clear();

	std::string line;
	std::size_t line_number{ 1 };

	while (std::getline(input, line))
	{
		try {
			auto type = getLineType(line);

			switch (type)
			{
			case ini::INIParser::TYPE::SECTION:
				parseSection(line);
				break;

			case ini::INIParser::TYPE::PROPERTY:
				parseProperty(line);
				break;

			case ini::INIParser::TYPE::INLINE_SECTION:
				parseInlineSection(line);
				break;

			default:
				break;
			}
		}
		catch (const ini::exception::INIParserError & exception)
		{
			std::string error_message = "Error in line " + std::to_string(line_number) + ": " + exception.what();

			error_log.push_back(error_message);
			section = last_section;
		}
		catch (const ini::exception::INIParserWarnig & exception)
		{
			std::string error_message = "Warning in line " + std::to_string(line_number) + ": " + exception.what();

			error_log.push_back(error_message);
		}

		line_number++;
		last_section = section;
	}

	return error_log.empty();
}

ini::INIParser::TYPE ini::INIParser::getLineType(std::string_view line) const
{
	auto first_non_blank_character = std::find_if(line.begin(), line.end(), [](char x) {return !std::isspace(static_cast<unsigned char>(x)); }); // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion

	if (first_non_blank_character == line.end())
	{
		return TYPE::BLANK;
	}

	if (*first_non_blank_character == '#' || *first_non_blank_character == ';')
	{
		return TYPE::COMMENT;
	}

	if (*first_non_blank_character == '[')
	{

		if (main_section->getSettings().inline_section)
		{
			struct inline_section_opening_bracket_finder
			{
				bool operator()(char x) {
					if (not_found) { return false; }
					if (expedct_bracket) {
						if (!std::isspace(static_cast<unsigned char>(x)))
						{
							if (x == '{') { return true; }
							not_found = true;
							return false;
						}
					}
					if (x == '\"') { if (last_character != '\\') { in_quotes = !in_quotes; } }
					if (x == '=' && !in_quotes) {
						if (last_character != '\\') { expedct_bracket = true; }
						return false;
					}
					last_character = x;

					return false;
				};

				bool in_quotes{ false };
				char last_character{ };
				bool expedct_bracket{ false };
				bool not_found{ false };
			};

			inline_section_opening_bracket_finder finder;

			auto inline_section_opening_bracket = std::find_if(line.begin(), line.end(), finder);  // Find the first '{' after = whitch isn't in quotes and ins't \=


			if (inline_section_opening_bracket != line.end())
			{
				return TYPE::INLINE_SECTION;
			}

		}

		return TYPE::SECTION;
	}

	return TYPE::PROPERTY;
}

void ini::INIParser::parseSection(std::string_view line)
{
	struct ending_brackets_finder
	{
		bool operator()(char x) {
			if (x == '\"') { if (last_character != '\\') { in_quotes = !in_quotes; } }
			if (x == ']' && !in_quotes) { return (last_character == '\\') ? false : true; }
			last_character = x;

			return false;
		};

		bool in_quotes{ false };
		char last_character{};
	};

	auto name_begin = std::find_if(line.begin(), line.end(), [](char x) { return !(std::isspace(static_cast<unsigned char>(x)) || x == '['); }); // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion

	ending_brackets_finder finder{};
	auto name_end = std::find_if(line.begin(), line.end(), finder); // Find the first ']' whitch isn't in quotes and isn't \]

	std::string_view begining_brackets{ line.substr(0, name_begin - line.begin()) };
	std::string_view raw_name{ line.substr(name_begin - line.begin(), name_end - name_begin) };
	std::string_view ending_brackets{ line.substr(name_end - line.begin(), line.end() - name_end) };

	auto level = getLevel(begining_brackets, '['); // Levvel of nesting

	if (level != getLevel(ending_brackets, ']'))
	{
		std::string message{ "Cannot determine level of nesting in line: \""s + std::string{line} + "\""s };
		throw exception::INIParserError{ message };
	}

	auto section_name = getName(raw_name);

	addAndSetSection(section_name, level);
}

void ini::INIParser::parseProperty(std::string_view line)
{
	struct equal_sign_finder
	{
		bool operator()(char x) {
			if (x == '\"') { if (last_character != '\\') { in_quotes = !in_quotes; } }
			if (x == '=' && !in_quotes) { return (last_character == '\\') ? false : true; }
			last_character = x;

			return false;
		};

		bool in_quotes{ false };
		char last_character{};
	};

	equal_sign_finder finder;
	auto equal_sign_pos = std::find_if(line.begin(), line.end(), finder);  // Find the first '=' whitch isn't in quotes and ins't \=

	if (equal_sign_pos == line.end())
	{
		std::string message{ "Unexepected end of line: \""s + std::string{line} +"\" in parseProperty function"s };
		throw exception::INIParserError{ message };
	}

	std::string_view raw_name{ line.substr(0, equal_sign_pos - line.begin()) };
	std::string_view raw_value{ line.substr(equal_sign_pos - line.begin() + 1, line.end() - equal_sign_pos) };

	auto property_name = getName(raw_name);
	auto property_value = getValue(raw_value);

	addProperty(property_name, std::move(property_value));

}

void ini::INIParser::parseInlineSection(std::string_view line)
{
	struct equal_sign_finder
	{
		bool operator()(char x) {
			if (x == '\"') { if (last_character != '\\') { in_quotes = !in_quotes; } }
			if (x == '=' && !in_quotes) { return (last_character == '\\') ? false : true; }
			last_character = x;

			return false;
		};

		bool in_quotes{ false };
		char last_character{};
	};

	equal_sign_finder finder;

	auto equal_sign_pos = std::find_if(line.begin(), line.end(), finder);  // Find the first '=' whitch isn't in quotes and ins't \=

	if (equal_sign_pos == line.end())
	{
		std::string message{ "Unexepected end of line: \""s + std::string{line} +"\" in parseInlineSection function"s };
		throw exception::INIParserError{ message };
	}

	auto parent_section = section;

	std::string_view raw_name{ line.substr(0, equal_sign_pos - line.begin()) };

	parseSection(raw_name);

	++equal_sign_pos;
	parseInlineSectionPropertyList({ &line[equal_sign_pos - line.begin()], static_cast<std::size_t>(line.end() - equal_sign_pos) });

	section = parent_section;
}

void ini::INIParser::parseInlineSectionPropertyList(std::string_view inline_section_property_list)
{
	auto current_pos= inline_section_property_list.begin();

	for (; current_pos!= inline_section_property_list.end(); current_pos++) // Blank characters may be in front of the name
	{
		if (!std::isspace(static_cast<unsigned char>(*current_pos))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
		{
			break;
		}
	}

	if (*current_pos!= '{') {
		std::string message{ "Unexepected character: \""s + *current_pos+ "\" in name: \""s + std::string{inline_section_property_list} +"\" on position: "s + std::to_string(current_pos- inline_section_property_list.begin()) + " in parseInlineSectionPropertyList function"s };
		throw exception::INIParserError{ message };
	}
	++current_pos;

	struct Finder
	{
		bool operator()(char x) {
			if (x == '\"') { if (last_character != '\\') { in_quotes = !in_quotes; } }
			if ((x == '}' || x == '|') && !in_quotes) { return (last_character == '\\') ? false : true; }
			last_character = x;

			return false;
		};

		bool in_quotes{ false };
		char last_character{};
	};
	std::size_t property_number{ 0 };

	Finder finder;

	for (auto property_end = std::find_if(current_pos, inline_section_property_list.end(), finder); property_end != inline_section_property_list.end(); property_number++)
	{
		auto property_value = getValue({ &inline_section_property_list[current_pos- inline_section_property_list.begin()], static_cast<std::size_t>(property_end - current_pos) });
		auto property_name = std::to_string(property_number);

		addProperty(property_name, std::move(property_value));

		++property_end;
		current_pos= property_end;
		if (*(current_pos- 1) == '}') { break; }

		property_end = std::find_if(current_pos, inline_section_property_list.end(), finder);
	}

	--current_pos;
	if (*current_pos!= '}') {
		std::string message{ "Unexepected character: \""s + *current_pos+ "\" in name: \""s + std::string{inline_section_property_list} +"\" on position: "s + std::to_string(current_pos- inline_section_property_list.begin()) + " in parseInlineSectionPropertyList function"s };
		throw exception::INIParserError{ message };
	}
	++current_pos;

	for (; current_pos!= inline_section_property_list.end(); current_pos++) // Blank characters and comments may be on back of the property
	{
		if (!std::isspace(static_cast<unsigned char>(*current_pos))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
		{
			if (*current_pos== ';' || *current_pos== '#')
			{
				break;
			}

			std::string message{ "Unexepected character: \""s + *current_pos+ "\" in value: \""s + std::string{inline_section_property_list} +"\" on position: "s + std::to_string(current_pos- inline_section_property_list.begin()) + " in parseInlineSectionPropertyList function"s };
			throw exception::INIParserError{ message };
		}
	}
}

std::string ini::INIParser::getName(std::string_view raw_name)
{
	auto current_pos= raw_name.begin();

	for (; current_pos!= raw_name.end(); current_pos++) // Blank characters may be in front of the name
	{
		if (!std::isspace(static_cast<unsigned char>(*current_pos))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
		{
			break;
		}
	}

	std::string name{};

	bool in_quotation_marks = false;
	try {
		for (; current_pos!= raw_name.end(); current_pos++)
		{
			auto character = *current_pos;

			if (std::isspace(static_cast<unsigned char>(character))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
			{
				if (in_quotation_marks)
				{
					name += character;

					continue;
				}
				else
				{
					break;
				}
			}

			if (character == '\"')
			{
				if (name.empty())
				{
					in_quotation_marks = true;

					continue;
				}
				else if (in_quotation_marks)
				{
					in_quotation_marks = false;

					continue;
				}
				else
				{
					std::string message{ "Unexepected character: \""s + character + "\" in name: \""s + std::string{raw_name} +"\" on position: "s + std::to_string(current_pos- raw_name.begin()) + " in getName function"s };
					throw exception::INIParserError{ message };
				}
			}

			if (character == '\\')
			{
				name += getSpecialCharacter(raw_name.at(current_pos - raw_name.begin() + 1));
				++current_pos;

				continue;
			}

			if (!in_quotation_marks && !isValidCharacter(character))
			{
				std::string message{ "Unexepected character: \""s + character + "\" in name: \""s + std::string{raw_name} + "\" on position: "s + std::to_string(current_pos- raw_name.begin()) + " in getName function"s };
				throw exception::INIParserError{ message };
			}

			name += character;

		}
	}
	catch ([[maybe_unused]] const std::out_of_range & exception)
	{
		std::string message{ "Unexepected end of line: \""s + std::string{raw_name} +"\" in getName function"s };
		throw exception::INIParserError{ message };
	}

	if (name.empty() || in_quotation_marks)
	{
		std::string message{ "Unexepected end of line: \""s + std::string{raw_name} + "\" in getName function"s };
		throw exception::INIParserError{ message };
	}


	for (; current_pos!= raw_name.end(); current_pos++) // Blank characters and comments may be on back of the name
	{
		if (!std::isspace(static_cast<unsigned char>(*current_pos))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
		{
			std::string message{ "Unexepected character: \""s + *current_pos+ "\" in name: \""s + std::string{raw_name} +"\" on position: "s + std::to_string(current_pos- raw_name.begin()) + " in getName function"s };
			throw exception::INIParserError{ message };
		}
	}

	return name;
}

std::size_t ini::INIParser::getLevel(std::string_view brackets, char bracket)
{
	auto current_pos= brackets.begin();

	for (; current_pos!= brackets.end(); current_pos++) // Blank characters may be in front of the brackets
	{
		if (!std::isspace(static_cast<unsigned char>(*current_pos))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
		{
			break;
		}
	}

	std::size_t level{};

	for (; current_pos!= brackets.end(); current_pos++)
	{
		if (*current_pos== bracket)
		{
			++level;
		}
		else
		{
			if (std::isspace(static_cast<unsigned char>(*current_pos))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
			{
				break;
			}
			else if(*current_pos== ';' || *current_pos== '#')
			{
				break;
			}
			else
			{
				std::string message{ "Unexepected character: \""s + *current_pos+ "\" in name: \""s + std::string{brackets} +"\" on position: "s + std::to_string(current_pos- brackets.begin()) + " in getLevel function"s };
				throw exception::INIParserError{ message };
			}
		}
	}

	for (; current_pos!= brackets.end(); current_pos++) // Blank characters and comments may be on back of the brackets
	{
		if (*current_pos== ';' || *current_pos== '#')
		{
			break;
		}

		if (!std::isspace(static_cast<unsigned char>(*current_pos))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
		{
			std::string message{ "Unexepected character: \""s + *current_pos+ "\" in name: \""s + std::string{brackets} +"\" on position: "s + std::to_string(current_pos- brackets.begin()) + " in getLevel function"s };
			throw exception::INIParserError{ message };
		}
	}

	if (level == 0)
	{
		std::string message{ "Unexepected end of line: \""s + std::string{brackets} + "\" in getLevel function"s };
		throw exception::INIParserError{ message };
	}

	return level;
}

std::pair<std::string, std::optional<ini::INIParser::DATA_TYPE>> ini::INIParser::getValue(std::string_view raw_value)
{
	auto current_pos= raw_value.begin();

	for (; current_pos!= raw_value.end(); current_pos++) // Blank characters may be in fron of the property
	{
		if (!std::isspace(static_cast<unsigned char>(*current_pos))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
		{
			break;
		}
	}

	std::string value{};

	bool in_quotation_marks = false;
	bool should_be_string = false;

	try {
		for (; current_pos!= raw_value.end(); current_pos++)
		{
			auto character = *current_pos;

			if (std::isspace(static_cast<unsigned char>(character))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
			{
				if (in_quotation_marks)
				{
					value += character;

					continue;
				}
				else
				{
					break;
				}
			}

			if (!in_quotation_marks && character == ':')
			{
				break;
			}


			if (character == '\"')
			{
				if (value.empty())
				{
					in_quotation_marks = true;
					should_be_string = true;

					continue;
				}
				else if (in_quotation_marks)
				{
					in_quotation_marks = false;

					continue;
				}
				else
				{
					std::string message{ "Unexepected character: \""s + character + "\" in value: \""s + std::string{raw_value} +"\" on position: "s + std::to_string(current_pos- raw_value.begin()) + " in getValue function"s };
					throw exception::INIParserError{ message };
				}
			}

			if (character == '\\')
			{
				value += getSpecialCharacter(raw_value.at(current_pos - raw_value.begin() + 1));
				++current_pos;

				continue;
			}
			
			if (!in_quotation_marks && character == ';' || character == '#')
			{
				current_pos= raw_value.end();

				break;
			}

			if (!in_quotation_marks && !isValidCharacter(character))
			{
				std::string message{ "Unexepected character: \""s + character + "\" in value: \""s + std::string{raw_value} +"\" on position: "s + std::to_string(current_pos- raw_value.begin()) + " in getValue function"s };
				throw exception::INIParserError{ message };
			}

			value += character;

		}
	}
	catch ([[maybe_unused]] const std::out_of_range & exception)
	{
		std::string message{ "Unexepected end of line: \""s + std::string{raw_value} +"\" in getValue function"s };
		throw exception::INIParserError{ message };
	}

	if (value.empty() || in_quotation_marks)
	{
		std::string message{ "Unexepected end of line: \""s + std::string{raw_value} +"\" in getValue function"s };
		throw exception::INIParserError{ message };
	}

	std::pair<std::string, std::optional<ini::INIParser::DATA_TYPE>> value_and_type{ value, {} };

	try {
		if (current_pos!= raw_value.end() && *current_pos== ':') // Check if the data type is specified
		{
			value_and_type.second.emplace(getDataType(raw_value.at(current_pos- raw_value.begin() + 1)));
			++current_pos;
			++current_pos;
		}
	}
	catch ([[maybe_unused]] const std::out_of_range & exception)
	{
		std::string message{ "Unexepected end of line: \""s + std::string{raw_value} +"\" in getValue function"s };
		throw exception::INIParserError{ message };
	}

	for (; current_pos!= raw_value.end(); current_pos++) // Blank characters and comments may be on back of the property
	{
		if (!std::isspace(static_cast<unsigned char>(*current_pos))) // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
		{
			if (*current_pos== ';' || *current_pos== '#')
			{
				break;
			}

			std::string message{ "Unexepected character: \""s + *current_pos+ "\" in value: \""s + std::string{raw_value} +"\" on position: "s + std::to_string(current_pos- raw_value.begin()) + " in getValue function"s };
			throw exception::INIParserError{ message };
		}
	}

	if (should_be_string && !value_and_type.second.has_value())
	{
		value_and_type.second.emplace(DATA_TYPE::STRING);

		return value_and_type;
	}

	return value_and_type;
}

ini::Property ini::INIParser::getProperty(std::string_view property_name, const std::string & data) const
{
	Property property{ std::string{property_name} };

	if (data.size() < 6) // If type is bool, the maximum size of the string is 5
	{
		std::string data_copy{ data };
		std::transform(data_copy.begin(), data_copy.end(), data_copy.begin(), std::tolower);

		if (data_copy == "true")
		{
			property.set(true);

			return property;
		}

		if (data_copy == "false")
		{
			property.set(false);

			return property;
		}
	}

	std::size_t number_of_digits{ 0 }, number_of_dots{ 0 };
	std::for_each(data.begin(), data.end(), [&number_of_digits, &number_of_dots](char x) {
		if (x == '.') number_of_dots++;
		if (std::isdigit(static_cast<unsigned char>(x))) number_of_digits++; // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion
	});

	if (number_of_digits == data.size())
	{
		try {
			property.set(::invokeInCLocale([](auto && x) { return std::stoi(x); }, data)); // Invoke in "C" locale to ensure consistent conversion results

			return property;
		}
		catch ([[maybe_unused]] const std::out_of_range & exception) // Number is too big for an int
		{
			try {
				property.set(::invokeInCLocale([](auto && x) { return std::stoll(x); }, data)); // Invoke in "C" locale to ensure consistent conversion results

				return property;
			}
			catch (...) // Number is too big for a long long int
			{
				// return string
			}
		}
		catch (...) // String can't be converted to an int
		{
			// return string
		}
	}

	if (number_of_digits + number_of_dots == data.size())
	{
		try {
			property.set(::invokeInCLocale([](auto && x) { return std::stod(x); }, data)); // Invoke in "C" locale to ensure consistent conversion results

			return property;
		}
		catch (...) // String can't be converted to a double
		{
			// return string
		}
	}

	property.set(data);

	return property;
}

ini::Property ini::INIParser::getProperty(std::string_view property_name, const std::string & data, ini::INIParser::DATA_TYPE data_type) const
{
	Property property{ std::string{property_name} };

	switch (data_type)
	{
	case ini::INIParser::DATA_TYPE::INT:
	{
		try {
			property.set(::invokeInCLocale([](auto && x) { return std::stoi(x); }, data)); // Invoke in "C" locale to ensure consistent conversion results

			return property;
		}
		catch (...) {}; // String can't be converted to an int
	}
		break;
	case ini::INIParser::DATA_TYPE::LONG:
	{
		try {
			property.set(::invokeInCLocale([](auto && x) { return std::stoll(x); }, data)); // Invoke in "C" locale to ensure consistent conversion results

			return property;
		}
		catch (...) {}; // String can't be converted to a long long int
	}
		break;
	case ini::INIParser::DATA_TYPE::BOOL:
	{
		std::string data_copy{ data };
		std::transform(data_copy.begin(), data_copy.end(), data_copy.begin(), std::tolower);

		if (data_copy == "false" || data_copy == "0") {
			property.set(false); return property;
		};

		if (data_copy == "true" || data_copy == "1") {
			property.set(true); return property;
		};
	}
		break;
	case ini::INIParser::DATA_TYPE::DOUBLE:
	{
		try {
			property.set(::invokeInCLocale([](auto && x) { return std::stod(x); }, data)); // Invoke in "C" locale to ensure consistent conversion results

			return property;
		}
		catch (...) {}; // String can't be converted to an double
	}
		break;
	case ini::INIParser::DATA_TYPE::STRING:
	{
		property.set(data);

		return property;
	}
		break;
	default:
		break;
	}

	std::string message{ "Cannot convert: \""s + data + "\" to: \""s + std::to_string(static_cast<int>(data_type)) + "\" in property: "s + std::string{property_name} +" in getProperty function"s };
	throw exception::INIParserError{ message };
}

void ini::INIParser::addAndSetSection(const std::string & section_name, std::size_t level)
{
	if (section->getLevel() + 1 == level)
		// Add this section to current section
	{
		if (!section->sectionExists({ section_name }))
		{
			section = &section->addSection(section_name);
		}
		else
		{
			section = &section->operator()(section_name);
		}
	}
	else if (section->getLevel() < level)
	{
		std::string message{ "Incorrect level of nesting"s };
		throw exception::INIParserError{ message };
	}
	else
	{
		// Return to section with level of nesting equal to this section level - 1

		while (section->getLevel() + 1 != level)
		{
			if (auto x = section->getParent(); x.has_value())
			{
				section = x.value();
			}
			else
			{
				std::string message{ "Incorrect level of nesting"s };
				throw exception::INIParserError{ message };
			}
		}

		if (!section->sectionExists({ section_name }))
		{
			section = &section->addSection(section_name);
		}
		else
		{
			section = &section->operator()(section_name);
		}
	}
}

void ini::INIParser::addProperty(const std::string & property_name, std::pair<std::string, std::optional<DATA_TYPE>> && property_value)
{
	auto[value, type] = property_value;

	Property property{ "temp" }; // Create empty property, name isn't important

	if (!main_section->getSettings().type_identification)
	{
		property = getProperty(property_name, value, ini::INIParser::DATA_TYPE::STRING);
	}
	else if (type.has_value())
	{
		property = getProperty(property_name, value, type.value());
	}
	else
	{
		property = getProperty(property_name, value);
	}

	if (!section->propertyExists({ property_name }))
	{
		section->addProperty(property);
	}
	else
	{
		section->operator[](property_name) = std::move(property);

		std::string message{ "Property \"" + property_name + "\" redefinition"s };
		throw exception::INIParserWarnig{ message };
	}
}


char ini::INIParser::getSpecialCharacter(char character) const
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

	case '{':
		return '{';
		break;

	case '}':
		return '}';
		break;

	case '\"':
		return '\"';
		break;


	case '|':
		return '|';
		break;

	case ':':
		return ':';
		break;

	case 'n':
		return '\n';
		break;

	case 't':
		return '\t';
		break;

	default:
		std::string message{ "Unexepected character: \""s + character + "\" in getSpecialCharacter function"s };
		throw exception::INIParserError{ message };
		break;
	}
}

bool ini::INIParser::isValidCharacter(char character) const
{
	return !(
		character == ']' ||
		character == '[' ||
		character == '{' ||
		character == '}' ||
		character == '\\' ||
		character == '=' ||
		character == ':' ||
		character == '#' ||
		character == '|' ||
		character == ';'
		);
}