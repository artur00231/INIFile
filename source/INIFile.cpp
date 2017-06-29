#include "INIFile.h"

INIFile::INIFile()
{
}


INIFile::~INIFile()
{
}


bool INIFile::read(const std::string& name)
{
	_file_name = name;
	std::ifstream input(name);
	if (!input.good())
	{
		_load = false;
		return false;
	}

	std::string line;
	std::string current_section = "global";

	while (std::getline(input, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), isblank));

		if (line != "")
		{
			if (*line.begin() == '[' && *(line.end() - 1) == ']' && line.size() > 2)
			{
				std::string section = std::string(line.begin() + 1, line.end() - 1);
				_ini_data.insert(std::pair<std::string, std::map<std::string, std::string>>{section, std::map<std::string, std::string>{}});
				current_section = section;
			}
			else if (*line.begin() != ';')
			{
				int poz = line.find('=');
				std::string::iterator it_poz = line.begin() + poz;

				if (poz != std::string::npos && line.begin() < it_poz && line.end() > it_poz + 1)
				{
					_ini_data.at(current_section).insert(std::pair<std::string, std::string>{std::string(line.begin(), it_poz), std::string(it_poz + 1, line.end())});
				}
			}

		}
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


bool INIFile::write()
{
	return write(_file_name);
}


bool INIFile::write(std::string name)
{
	if (name != "")
	{
		std::ofstream output(name);
		if (output.good())
		{
			for (const auto &x : _ini_data)
			{
				output << "[" + x.first + "]\n";

				for (const auto &a : x.second)
				{
					output << a.first << "=" << a.second << "\n";
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

