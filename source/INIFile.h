#ifndef INIFILE_H
#define INIFILE_H

#include <filesystem>

#include "Section.h"
#include "SectionIterator.h"
#include "INIFileSettings.h"

namespace ini
{
	class INIFile : public Section
	{
	public:
		explicit INIFile(INIFileSettings settings = {});
		~INIFile();

		bool load(const std::filesystem::path & path);
		bool load(const std::string & data);
		bool load(const char * data);

		void save(std::string & buffer) const;
		void save(const std::filesystem::path & path) const;
		void save() const; // Save to the file which was used in a load function

		const std::vector<std::string>& getLastParseErrorLog() const;

		INIFileSettings getSettings() const;

	protected:

	private:
		std::vector<std::string> last_parse_error_log{};
		INIFileSettings main_settings;

		std::unique_ptr<std::filesystem::path> file_path;
	};

	namespace exception
	{
		class FileDoesntExist : public std::exception
		{
		public:
			FileDoesntExist(std::string message) : message{ std::move(message) } {};
			const char* what() const override { return message.c_str(); };
		private:
			std::string message;
		};
	}

}

#endif // !INIFILE_H