#ifndef SECTION_H
#define SECTION_H

#include "Property.h"
#include "INIFileSettings.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <exception>
#include <optional>

namespace ini
{
	class SectionIterator;
	class const_SectionIterator;
	class PropertyIterator;
	class const_PropertyIterator;

	class Section
	{
	public:
		Section(const Section&) = default;
		Section& operator=(const Section&) = default;
		Section(Section&&) = default;
		Section& operator=(Section&&) = default;
		~Section();

		Section& operator()(std::string section_name);
		const Section& operator()(std::string section_name) const;
		Section& getSection(const std::vector<std::string> & path);
		const Section& getSection(const std::vector<std::string> & path) const;

		Section& addSection(std::string section_name);

		Property& operator[](std::string property_name);
		const Property& operator[](std::string property_name) const;
		Property& getProperty(const std::vector<std::string> & path);
		const Property& getProperty(const std::vector<std::string> & path) const;

		Property& addProperty(const Property & property);
		Property& addProperty(Property && property);
		template<typename T> // Possible types: int, long long, bool, double, std::string
		Property& addProperty(std::string property_name, T && val );

		std::string getName() const;
		std::size_t getLevel() const; // Get level of nesting
		std::optional<Section*> getParent() const;

		void clear();

		bool propertyExists(const std::vector<std::string> & path) const;
		bool sectionExists(const std::vector<std::string> & path) const;

		PropertyIterator begin();
		const_PropertyIterator begin() const;
		const_PropertyIterator cbegin() const;
		PropertyIterator end();
		const_PropertyIterator end() const;
		const_PropertyIterator cend() const;

	protected:
		Section(Section * parent, std::size_t level, std::string name, const INIFileSettings * settings);
		Property& getPropertyByName(std::string property_name);
		const Property& getPropertyByName(std::string property_name) const;

		std::string getProperName(std::string_view name) const;


		const INIFileSettings * settings; // settings must be propected, becouse INIFile have to modified it
	private:
		std::unordered_map<std::string, Section> sections;
		std::unordered_map<std::string, Property> propertes;

		std::string name;
		Section *parent;
		std::size_t level; // level of nesting
		

		friend SectionIterator;
		friend const_SectionIterator;
		friend PropertyIterator;
		friend const_PropertyIterator;
	};

	namespace helper_functions {
		template<typename T, typename U = void>
		struct can_by_converted_to_string : std::false_type {};

		template<typename T>
		struct can_by_converted_to_string<T, decltype(void(std::to_string(std::declval<T>())))> : std::true_type {};

		template<typename T>
		inline constexpr bool can_by_converted_to_string_v = can_by_converted_to_string<T>::value;
	}

	template<typename T>
	inline Property & Section::addProperty(std::string property_name, T && val)
	{
		property_name = getProperName(property_name);

		Property property{ property_name };

																			// Every possible type, but std::string
		if constexpr (helper_functions::can_by_converted_to_string_v<T>)	// Invalid types will trigger assertion in ini::Property::set
		{
			if (!settings->type_identification)
			{
				auto property_value = std::to_string(val);

				property.set(std::move(property_value));
				addProperty(property);
				return propertes.at(property_name);
			}
		}

		property.set<T>(std::forward<T>(val)); 

		addProperty(property);
		return propertes.at(property_name);
	}

	namespace exception
	{
		class SectionDoesntExist : public std::exception
		{
		public:
			SectionDoesntExist(std::string message) : message{ std::move(message) } {};
			const char* what() const override { return message.c_str(); };
		private:
			std::string message;
		};

		class SectionAlreadyExist : public std::exception
		{
		public:
			SectionAlreadyExist(std::string message) : message{ std::move(message) } {};
			const char* what() const override { return message.c_str(); };
		private:
			std::string message;
		};

		class PropertyDoesntExist : public std::exception
		{
		public:
			PropertyDoesntExist(std::string message) : message{ std::move(message) } {};
			const char* what() const override { return message.c_str(); };
		private:
			std::string message;
		};

		class PropertyAlreadyExist : public std::exception
		{
		public:
			PropertyAlreadyExist(std::string message) : message{ std::move(message) } {};
			const char* what() const override { return message.c_str(); };
		private:
			std::string message;
		};
	}
}

#include "PropertyIterator.h"

#endif // !SECTION_H