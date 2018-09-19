#include "Section.h"


ini::Section::~Section()
{
}

ini::Section & ini::Section::operator()(std::string section_name)
{
	using namespace std::string_literals;

	section_name = getProperName(section_name);

	if (sections.count(section_name)) // Check if the section exists
	{
		return sections.at(section_name);
	}

	std::string message = "Section: \""s + section_name + "\" doesn't exist."s;
	throw exception::SectionDoesntExist{ message };
}

const ini::Section & ini::Section::operator()(std::string section_name) const
{
	using namespace std::string_literals;

	section_name = getProperName(section_name);

	if (sections.count(section_name)) // Check if the section exists
	{
		return sections.at(section_name);
	}

	std::string message = "Section: \""s + section_name + "\" doesn't exist."s;
	throw exception::SectionDoesntExist{ message };
}

ini::Section & ini::Section::getSection(const std::vector<std::string>& path)
{
	Section *section = this;

	for (auto x : path)
	{
		x = getProperName(x);

		section = &(section->operator()(x));
	}

	return *section;
}

const ini::Section & ini::Section::getSection(const std::vector<std::string>& path) const
{
	const Section *section = this;

	for (auto x : path)
	{
		x = getProperName(x);

		section = &(section->operator()(x));
	}

	return *section;
}

ini::Property & ini::Section::operator[](std::string property_name)
{
	using namespace std::string_literals;

	property_name = getProperName(property_name);

	if (propertes.count(property_name)) // Check if the property exists
	{
		return propertes.at(property_name);
	}

	std::string message = "Property: \""s + property_name + "\" doesn't exist."s;
	throw exception::PropertyDoesntExist{ message };
}

const ini::Property & ini::Section::operator[](std::string property_name) const
{
	using namespace std::string_literals;

	property_name = getProperName(property_name);

	if (propertes.count(property_name)) // Check if the property exists
	{
		return propertes.at(property_name);
	}

	std::string message = "Property: \""s + property_name + "\" doesn't exist."s;
	throw exception::PropertyDoesntExist{ message };
}

ini::Property& ini::Section::getProperty(const std::vector<std::string> & path)
{
	if (path.size() == 0)
	{
		std::string message = "Property: \"\" doesn't exist.";
		throw exception::PropertyDoesntExist{ message };
	}

	if (path.size() == 1)
	{
		return getPropertyByName(path.back());
	}

	Section *section = this;

	for (auto x = path.begin(); x != path.end() - 1; x++)
	{
		auto section_name = getProperName(*x);
		section = &(section->operator()(section_name));
	}

	return section->getPropertyByName(path.back());
}

const ini::Property& ini::Section::getProperty(const std::vector<std::string> & path) const
{
	if (path.size() == 0)
	{
		std::string message = "Property: \"\" doesn't exist.";
		throw exception::PropertyDoesntExist{ message };
	}

	if (path.size() == 1)
	{
		return getPropertyByName(path.back());
	}

	const Section *section = this;

	for (auto x = path.begin(); x != path.end() - 1; x++)
	{
		auto section_name = getProperName(*x);
		section = &(section->operator()(section_name));
	}

	return section->getPropertyByName(path.back());
}

ini::Property & ini::Section::addProperty(const Property & property)
{
	using namespace std::string_literals;

	auto[it, created] = propertes.insert(std::make_pair(getProperName(property.getName()), property));

	if (created)
	{
		return it->second;
	}

	std::string message = "Property: \""s + property.getName() + "\" already exist."s;
	throw exception::PropertyAlreadyExist{ message };
}

ini::Property & ini::Section::addProperty(Property && property)
{
	using namespace std::string_literals;

	auto[it, created] = propertes.insert(std::make_pair(getProperName(property.getName()), std::move(property)));

	if (created)
	{
		return it->second;
	}

	std::string message = "Section: \""s + property.getName() + "\" already exist."s;
	throw exception::SectionAlreadyExist{ message };
}

ini::Section & ini::Section::addSection(std::string section_name)
{
	using namespace std::string_literals;

	section_name = getProperName(section_name);

	Section section{ this, level + 1, section_name, settings };
	auto[it, created] = sections.insert(std::make_pair(section_name, std::move(section)));

	if (created)
	{
		return it->second;
	}
	
	std::string message = "Section: \""s + section_name + "\" already exist."s;
	throw exception::SectionAlreadyExist{ message };
}

std::string ini::Section::getName() const
{
	return name;
}

std::size_t ini::Section::getLevel() const
{
	return level;
}

std::optional<ini::Section*> ini::Section::getParent() const
{
	if (!parent)
	{
		return {};
	}

	return { parent };
}

void ini::Section::clear()
{
	sections.clear();
	propertes.clear();
}

bool ini::Section::propertyExists(const std::vector<std::string>& path) const
{
	if (path.size() == 0)
	{
		return false;
	}

	if (path.size() == 1)
	{
		return propertes.count(getProperName(path.back())); // Check if the property exists
	}

	if (sections.count(getProperName(path.front()))) // Check if the section exists
	{
		const Section * section = this;

		for (auto x = path.begin(); x != path.end() - 1; x++)
		{
			auto section_name = getProperName(*x);
			if (section->sections.count(section_name)) // Check if the section exists
			{

				section = &(section->sections.at(section_name));
			}
			else
			{
				return false;
			}
		}

		return section->propertes.count(getProperName(path.back())); // Check if the property exists
	}

	return false;
}

bool ini::Section::sectionExists(const std::vector<std::string>& path) const
{
	if (path.size() == 0)
	{
		return false;
	}

	if (path.size() == 1)
	{
		return sections.count(getProperName(path.back())); // Check if the section exists
	}

	if (sections.count(getProperName(path.front()))) // Check if the property exists
	{
		const Section * section = this;

		for (auto x = path.begin(); x != path.end() - 1; x++)
		{
			auto section_name = getProperName(*x);
			if (section->sections.count(section_name)) // Check if the section exists
			{
				section = &(section->sections.at(section_name));
			}
			else
			{
				return false;
			}
		}

		return section->sections.count(getProperName(path.back())); // Check if the property exists
	}

	return false;
}

ini::PropertyIterator ini::Section::begin()
{
	return PropertyIterator{ *this };
}

ini::const_PropertyIterator ini::Section::begin() const
{
	return const_PropertyIterator{ *this };
}

ini::const_PropertyIterator ini::Section::cbegin() const
{
	return const_PropertyIterator{ *this };
}

ini::PropertyIterator ini::Section::end()
{
	return PropertyIterator{};
}

ini::const_PropertyIterator ini::Section::end() const
{
	return const_PropertyIterator{};
}

ini::const_PropertyIterator ini::Section::cend() const 
{
	return const_PropertyIterator{};
}

ini::Section::Section(Section * parent, std::size_t level, std::string name, const ini::INIFileSettings * settings)
	: parent{ parent },
	level{ level },
	name{ std::move(name) },
	settings{ settings }
{
}

ini::Property & ini::Section::getPropertyByName(std::string property_name)
{
	using namespace std::string_literals;

	property_name = getProperName(property_name);

	if (propertes.count(property_name)) // Check if the property exists
	{
		return propertes.at(property_name);
	}

	std::string message = "Property: \""s + property_name + "\" doesn't exist."s;
	throw exception::PropertyDoesntExist{ message };
}

const ini::Property & ini::Section::getPropertyByName(std::string property_name) const
{
	using namespace std::string_literals;

	property_name = getProperName(property_name);

	if (propertes.count(property_name)) // Check if the property exists
	{
		return propertes.at(property_name);
	}

	std::string message = "Property: \""s + property_name + "\" doesn't exist."s;
	throw exception::PropertyDoesntExist{ message };
}

std::string ini::Section::getProperName(std::string_view name) const
{
	if (!settings->case_sensitivity)
	{
		return settings->tolower(name);
	}

	return std::string(name);
}
