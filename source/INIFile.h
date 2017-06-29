#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <cctype>
#include <exception>

#ifndef INIFILE_H
#define INIFILE_H

class INIFile
{
public:
	INIFile();
	~INIFile();

	bool read(const std::string& name);
	void set_section(const std::string&);
	bool good() const;
	void clear();

	template<typename T>
	inline T get(const std::string&, const std::string&) const;

	template<typename T>
	inline T get(const std::string&) const;

	template<typename T>
	inline void set(const std::string&, const std::string&, const T&);

	template<typename T>
	inline void set(const std::string&, const T&);

	template<typename T>
	inline bool change(const std::string&, const std::string&, const T&);

	template<typename T>
	inline bool change(const std::string&, const T&);

	bool write();
	bool write(std::string);

	bool exist(const std::string&, const std::string&) const;

private:
	std::map<std::string, std::map<std::string, std::string>> _ini_data;
	std::string _current_section = "Global";

	bool _load = false;
	std::string _file_name{};

};



template<typename T>
inline T INIFile::get(const std::string&, const std::string&) const
{
	static_assert(0, "Incorrect type");
	return T{};
}

template<>
inline int INIFile::get(const std::string& section, const std::string& property) const
{
	if (!exist(section, property))
		throw std::out_of_range {"Data is not accessible"};

	return atoi(_ini_data.at(section).at(property).c_str());
}

template<>
inline double INIFile::get(const std::string& section, const std::string& property) const
{
	if (!exist(section, property))
		throw std::out_of_range {"Data is not accessible"};

	return atof(_ini_data.at(section).at(property).c_str());
}

template<>
inline long long INIFile::get(const std::string& section, const std::string& property) const
{
	if (!exist(section, property))
		throw std::out_of_range {"Data is not accessible"};

	return atoll(_ini_data.at(section).at(property).c_str());
}

template<>
inline bool INIFile::get(const std::string& section, const std::string& property) const
{
	if (!exist(section, property))
		throw std::out_of_range {"Data is not accessible"};

	std::string val = _ini_data.at(section).at(property);

	if (val == "1" || val == "true" || val == "True") return true;
	if (val == "0" || val == "false" || val == "False") return false;

	throw std::range_error{ "Incorrect value" };
}

template<>
inline const std::string& INIFile::get(const std::string& section, const std::string& property) const
{
	if (!exist(section, property))
		throw std::out_of_range {"Data is not accessible"};

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