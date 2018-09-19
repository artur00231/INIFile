#ifndef INIPARSER_H
#define INIPARSER_H

#include <filesystem>
#include <string>
#include <string_view>
#include <istream>
#include <exception>

#include "INIFile.h"

namespace ini
{
	class INIParser
	{
	public:
		enum class DATA_TYPE { INT, LONG, BOOL, DOUBLE, STRING };

		INIParser() = default;
		INIParser(const INIParser&) = default;
		INIParser& operator=(const INIParser&) = default;
		INIParser(INIParser&&) = default;
		INIParser& operator=(INIParser&&) = default;

		bool parse(const std::filesystem::path & path, INIFile & ini_file);
		bool parse(const std::string & data, INIFile & ini_file);
		bool parse(const char * data, INIFile & ini_file);

		std::vector<std::string> getErrorLog() const;

		DATA_TYPE getDataType(char shortcut) const;
		DATA_TYPE getDataType(std::string data) const;

		~INIParser() = default;

	protected:
		bool parse(std::istream & input, INIFile & ini_file);

		enum class TYPE { COMMENT, SECTION, PROPERTY, BLANK, INLINE_SECTION };

		TYPE getLineType(std::string_view line) const;
		void parseSection(std::string_view line);
		void parseProperty(std::string_view line);
		void parseInlineSection(std::string_view line);
		void parseInlineSectionPropertyList(std::string_view inline_section_property_list);

		std::string getName(std::string_view raw_name);
		std::size_t getLevel(std::string_view brackets, char bracket);
		std::pair<std::string, std::optional<DATA_TYPE>> getValue(std::string_view raw_value);
		Property getProperty(std::string_view property_name, const std::string & data) const;
		Property getProperty(std::string_view property_name, const std::string & data, DATA_TYPE data_type) const;
		void addAndSetSection(const std::string & section_name, std::size_t level);
		void addProperty(const std::string & property_name, std::pair<std::string, std::optional<DATA_TYPE>> && property_value);

		char getSpecialCharacter(char character) const;
		bool isValidCharacter(char character) const;
	private:
		INIFile *main_section = nullptr;
		Section *section = nullptr;

		std::vector<std::string> error_log{};
	};

	namespace exception
	{
		class INIParserException : public std::exception
		{
		public:
			INIParserException(std::string message) : message{ std::move(message) } {};
			const char* what() const override { return message.c_str(); };
		private:
			std::string message;
		};

	}
}
#endif // !INIPARSER_H