#include "INIFile.h"
#include "INIParser.h"

#include <sstream>
#include <fstream>
#include <cctype>


ini::INIFile::INIFile(INIFileSettings settings) : Section{ nullptr, 0, "global", nullptr }, main_settings{ settings }
{
	this->settings = &main_settings;
}


ini::INIFile::~INIFile()
{
}

bool ini::INIFile::load(const std::filesystem::path & path)
{
	clear();
	file_path.reset(new std::filesystem::path{ path });

	ini::INIParser ini_parser;
	try {
		auto success = ini_parser.parse(path, *this);

		last_parse_error_log = std::move(ini_parser.getErrorLog());

		return success;
	}
	catch (const ini::exception::INIParserException & exception)
	{
		throw ini::exception::FileDoesntExist{ exception.what() };
	}
}

bool ini::INIFile::load(const std::string & data)
{
	clear();

	ini::INIParser ini_parser;
	auto success = ini_parser.parse(data, *this);
	last_parse_error_log = std::move(ini_parser.getErrorLog());

	return success;
}

bool ini::INIFile::load(const char * data)
{
	clear();


	ini::INIParser ini_parser;
	auto success = ini_parser.parse(data, *this);
	last_parse_error_log = std::move(ini_parser.getErrorLog());

	return success;
}

void saveSection(const ini::Section & section, std::ostream & out, const ini::INIFileSettings & settings);

void ini::INIFile::save(std::string & buffer) const
{
	buffer.clear();
	std::stringstream ss;

	saveSection(*this, ss, main_settings);
	ss.flush();

	buffer = ss.str();
}

void ini::INIFile::save(const std::filesystem::path & path) const
{
	using namespace std::string_literals;

	std::fstream file{};
	file.open(path.string(), std::ios_base::out);

	if (!file.good())
	{
		std::string message{ "Cannot open file \""s + path.string() + "\""s };
		throw exception::FileDoesntExist{ message };
	}

	saveSection(*this, file, main_settings);


	file.close();
}

void ini::INIFile::save() const
{
	using namespace std::string_literals;

	if (!file_path)
	{
		std::string message{ "Save path is not set"s };
		throw exception::FileDoesntExist{ message };
	}

	save(*file_path);
}

const std::vector<std::string>& ini::INIFile::getLastParseErrorLog() const
{
	return last_parse_error_log;
}

ini::INIFileSettings ini::INIFile::getSettings() const
{
	return main_settings;
}



bool isSpecialCharacter(char character);

std::string getNameWithSpecialCharacters(std::string_view name);

bool haveSpecialCharacters(std::string_view text);

void saveProperty(const ini::Property & property, std::ostream & out, bool type_identification);

void saveInlineSection(const ini::Section & section, std::ostream & out);

bool isInlineSection(const ini::Section & section);

struct DataTypeIdentifier
{
	void operator()(int x) { data_type = ini::INIParser::DATA_TYPE::INT; };
	void operator()(std::string x) { data_type = ini::INIParser::DATA_TYPE::STRING; }
	void operator()(double x) { data_type = ini::INIParser::DATA_TYPE::DOUBLE; };
	void operator()(long long x) { data_type = ini::INIParser::DATA_TYPE::LONG; };
	void operator()(bool x) { data_type = ini::INIParser::DATA_TYPE::BOOL; };

	ini::INIParser::DATA_TYPE data_type{};
};

bool isSpecialCharacter(char character)
{
	return (
		character == ']' ||
		character == '[' ||
		character == '{' ||
		character == '}' ||
		character == '\\' ||
		character == '\"' ||
		character == '=' ||
		character == ':' ||
		character == '#' ||
		character == ';' ||
		character == '|' ||
		character == ':' ||
		character == '\n' ||
		character == '\t'
		);
}

std::string getNameWithSpecialCharacters(std::string_view name) 
{
	std::string name_with_special_characters{};
	for (auto && character : name) // ", \n and \t have to be save as a special character
	{
		switch (character)
		{
		case '\"':
			name_with_special_characters.append("\\\"");
			break;

		case '\n':
			name_with_special_characters.append("\\n");
			break;

		case '\t':
			name_with_special_characters.append("\\t");
			break;

		default:
			name_with_special_characters += character;
			break;
		}
	}

	return name_with_special_characters;
}

bool haveSpectianCharacters(std::string_view text)
{
	for (auto && x : text)
	{
		if (std::isspace(static_cast<unsigned char>(x))
			|| isSpecialCharacter(x)
			)
		{
			return true;
		}
	}

	return false;
}

void saveSection(const ini::Section & section, std::ostream & out, const ini::INIFileSettings & settings)
{
	if (section.getLevel() != 0)
	{
		if (settings.inline_section)
		{
			if (isInlineSection(section))
			{
				if (settings.advanced_save_layout) { out << std::string(section.getLevel() - 1, '\t'); }

				saveInlineSection(section, out);

				return;
			}
		}

		std::string opening_brackets(section.getLevel(), '[');
		std::string closing_brackets(section.getLevel(), ']');

		if (!haveSpectianCharacters(section.getName()))
		{
			if (settings.advanced_save_layout) { out << std::string(section.getLevel() - 1, '\t'); }
			out << opening_brackets << section.getName() << closing_brackets << "\n";
		}
		else
		{
			auto section_name = getNameWithSpecialCharacters(section.getName());

			out << opening_brackets << '\"' << section_name << '\"' << closing_brackets << "\n";
		}
	}

	for (auto && x : section)
	{
		if (settings.advanced_save_layout) { 
			auto level = (section.getLevel() ? section.getLevel() - 1: section.getLevel()); // Do not subtract if level is equal to 0
			out << std::string(level, '\t');
		}
		saveProperty(x, out, settings.type_identification);
	}

	for (auto it = ini::const_SectionIterator{ section }; it != ini::const_SectionIterator{}; it++)
	{
		saveSection(*it, out, settings);
	}
}

void saveProperty(const ini::Property & property, std::ostream & out, bool type_identification)
{
	if (!haveSpectianCharacters(property.getName()))
	{
		out << property.getName() << " = ";
	}
	else
	{
		std::string property_name = getNameWithSpecialCharacters(property.getName());

		out << '\"' << property_name << "\" = ";
	}

	auto property_value = property.get<std::string>();

	if (!haveSpectianCharacters(property_value))
	{
		out << property_value;
	}
	else
	{
		property_value = getNameWithSpecialCharacters(property_value);

		out << '\"' << property_value << '\"';
	}

	if (type_identification)
	{
		ini::INIParser ini_parser;
		DataTypeIdentifier data_type_indentifier{};
		property.visit(data_type_indentifier);

		if (ini_parser.getDataType(property_value) != data_type_indentifier.data_type) // If parser incorrectly deduce the proerty type, save a proper type
		{
			out << ":";

			switch (data_type_indentifier.data_type)
			{
			case ini::INIParser::DATA_TYPE::INT:
				out << 'i';
				break;

			case ini::INIParser::DATA_TYPE::BOOL:
				out << 'b';
				break;

			case ini::INIParser::DATA_TYPE::LONG:
				out << 'l';
				break;

			case ini::INIParser::DATA_TYPE::DOUBLE:
				out << 'd';
				break;

			case ini::INIParser::DATA_TYPE::STRING:
				out << 's';
				break;
			default:
				break;
			}
		}
	}

	out << "\n";
}

void saveInlineSection(const ini::Section & section, std::ostream & out)
{
	std::string opening_brackets(section.getLevel(), '[');
	std::string closing_brackets(section.getLevel(), ']');

	if (!haveSpectianCharacters(section.getName()))
	{
		out << opening_brackets << section.getName() << closing_brackets << " = { ";
	}
	else
	{
		auto section_name = getNameWithSpecialCharacters(section.getName());

		out << '\"' << opening_brackets << section.getName() << closing_brackets << "\" = { ";
	}

	std::vector<ini::Property> properties{ section.begin(), section.end() };

	std::sort(properties.begin(), properties.end(), [](const ini::Property & p1, const ini::Property & p2){
		return std::stoul(p1.getName()) < std::stoul(p2.getName());
	});

	for (auto property = properties.begin(); property != properties.end(); property++)
	{
		if (property != properties.begin())
		{
			out << " | ";
		}

		auto property_value = property->get<std::string>();

		if (!haveSpectianCharacters(property_value))
		{
			out << property_value;
		}
		else
		{
			property_value = getNameWithSpecialCharacters(property_value);

			out << '\"' << property_value << '\"';
		}

		ini::INIParser ini_parser;
		DataTypeIdentifier data_type_indentifier{};
		property->visit(data_type_indentifier);

		if (ini_parser.getDataType(property_value) != data_type_indentifier.data_type) // If parser incorrectly deduce the proerty type, save a proper type
		{
			out << ":";

			switch (data_type_indentifier.data_type)
			{
			case ini::INIParser::DATA_TYPE::INT:
				out << 'i';
				break;

			case ini::INIParser::DATA_TYPE::BOOL:
				out << 'b';
				break;

			case ini::INIParser::DATA_TYPE::LONG:
				out << 'l';
				break;

			case ini::INIParser::DATA_TYPE::DOUBLE:
				out << 'd';
				break;

			case ini::INIParser::DATA_TYPE::STRING:
				out << 's';
				break;
			default:
				break;
			}
		}
	}

	out << " }\n";
}

bool isInlineSection(const ini::Section & section)
{
	std::vector<std::size_t> numbers{};

	if (ini::const_SectionIterator{ section } != ini::const_SectionIterator{})
	{
		return false;
	}

	for (auto && property : section)
	{
		auto name = property.getName();

		if (std::count_if(name.begin(), name.end(), std::isdigit) != name.size())
		{
			return false;
		}

		numbers.emplace_back(static_cast<std::size_t>(std::stoul(name)));
	}

	if (numbers.empty())
	{
		return false;
	}

	std::sort(numbers.begin(), numbers.end());

	if (numbers.front() != 0)
	{
		return false;
	}

	for (auto it = numbers.begin() + 1; it != numbers.end(); it++)
	{
		if (*(it - 1) != *it - 1)
		{
			return false;
		}
	}

	return true;
}