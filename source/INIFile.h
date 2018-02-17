#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <cctype>
#include <exception>
#include <vector>

#ifndef INIFILE_H
#define INIFILE_H

class INIFile
{
public:
	using string = const std::string&;

	INIFile();

	INIFile(const INIFile&) = delete;
	INIFile(INIFile&&) = default;

	INIFile& operator=(const INIFile&) = delete;
	INIFile& operator=(INIFile&&) = default;

	~INIFile();

	bool read(const std::string& name); // read file
	void set_section(const std::string&); // set current section
	bool good() const; // is INIFile ready?
	void clear(); // claer all INIFile data
	bool check(std::vector<std::pair<std::string, std::string>> & names); // check if all data is loaded


	// get bool/int/long long/double/const std::string& from INIFile
	// throw out_of_range if value don't exist
	template<typename T>
	inline T get(const std::string&, const std::string&) const;

	template<typename T>
	inline T get(const std::string&) const;


	// set bool/int/long long/double/const std::string& to INIFile
	template<typename T>
	inline void set(const std::string&, const std::string&, const T&);

	template<typename T>
	inline void set(const std::string&, const T&);


	// change existing bool/int/long long/double/const std::string& in INIFile
	template<typename T>
	inline bool change(const std::string&, const std::string&, const T&);

	template<typename T>
	inline bool change(const std::string&, const T&);


	// write to file
	bool write();
	bool write(std::string);

	bool exist(const std::string&, const std::string&) const;

protected:
	enum class Type {COMMENT, SECTION, DATA, BLANK};
	Type getLineType(const std::string & line);
	std::string getSection(const std::string & line);
	std::pair<std::string, std::string> getData(const std::string & line);

	char getSpecialCharacter(char cahracter);
	bool isValidCharacter(char cahracter);

private:
	std::map<std::string, std::map<std::string, std::string>> _ini_data;
	std::string _current_section = "Global";

	bool _load = false;
	std::string _file_name{};

};



template<typename T>
inline T INIFile::get(const std::string&, const std::string&) const
{
	static_assert(std::is_same<T, int>, "Incorrect type");
	return T{};
}

template<>
inline int INIFile::get(const std::string& section, const std::string& property) const
{
	return atoi(_ini_data.at(section).at(property).c_str());
}

template<>
inline double INIFile::get(const std::string& section, const std::string& property) const
{
	return atof(_ini_data.at(section).at(property).c_str());
}

template<>
inline long long INIFile::get(const std::string& section, const std::string& property) const
{
	return atoll(_ini_data.at(section).at(property).c_str());
}

template<>
inline bool INIFile::get(const std::string& section, const std::string& property) const
{
	std::string val = _ini_data.at(section).at(property);

	if (val == "1" || val == "true" || val == "True") return true;
	if (val == "0" || val == "false" || val == "False") return false;

	throw std::range_error{ "Incorrect value" };
}

template<>
inline const std::string& INIFile::get(const std::string& section, const std::string& property) const
{
	return _ini_data.at(section).at(property);
}


template<typename T>
inline T INIFile::get(const std::string& property) const
{
	return get<T>(_current_section, property);
}


template<typename T>
inline void INIFile::set(const std::string &, const std::string &, const T &)
{
	static_assert(0, "Incorrect type");
	return T{};
}

template<>
inline void INIFile::set(const std::string & section, const std::string & property, const int & value)
{
	_ini_data[section][property] = std::to_string(value);
}

template<>
inline void INIFile::set(const std::string & section, const std::string & property, const long long & value)
{
	_ini_data[section][property] = std::to_string(value);
}

template<>
inline void INIFile::set(const std::string & section, const std::string & property, const double & value)
{
	_ini_data[section][property] = std::to_string(value);
}

template<>
inline void INIFile::set(const std::string & section, const std::string & property, const bool & value)
{
	_ini_data[section][property] = std::to_string(int{ value });
}

template<>
inline void INIFile::set(const std::string & section, const std::string & property, const std::string & value)
{
	_ini_data[section][property] = value;
}

template<typename T>
inline void INIFile::set(const std::string& property, const T& value)
{
	set<T>(_current_section, property, val);
}

template<typename T>
inline bool INIFile::change(const std::string & section, const std::string & property, const T & value)
{
	if (!exist(section, property))
		return false;

	set(section, property, value);
	return true;
}

template<typename T>
inline bool INIFile::change(const std::string & property, const T & value)
{
	return change(_current_section, property, value);
}



#endif // !INIFILE_H