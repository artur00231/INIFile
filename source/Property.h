#ifndef PROPERTY_H
#define PROPERTY_H

#include <variant>
#include <string>
#include <typeinfo>

#include "TypeCast.h"

namespace ini
{
	// Possible types: int, long long, bool, double, std::string
	class Property
	{
	public:
		using value_type = std::variant<std::string, int, long long, double, bool>;

		Property(std::string property_name);
		Property(const Property&) = default;
		Property& operator=(const Property&) = default;
		Property(Property&&) = default;
		Property& operator=(Property&&) = default;
		~Property();

		std::string getName() const;

		template<typename F>
		void visit(F && f) const;

		template<typename T>
		void set(T value);
		template<typename T>
		T get() const;

	protected:

	private:
		value_type value;
		std::string name;
	};

	namespace exception
	{
		class PropertyConversionError : public std::exception
		{
		public:
			PropertyConversionError(std::string message) : message{ std::move(message) } {};
			const char* what() const override { return message.c_str(); };
		private:
			std::string message;
		};
	}

}

template<typename F>
inline void ini::Property::visit(F && f) const
{
	std::visit(std::forward<F>(f), value);
}


template<typename T>
inline void ini::Property::set(T value)
{
	static_assert(std::is_same<T, int>::value, "Invalid type");
}

template<>
inline void ini::Property::set<int>(int value)
{
	this->value = std::move(value);
}

template<>
inline void ini::Property::set<std::string>(std::string value)
{
	this->value = std::move(value);
}

template<>
inline void ini::Property::set<double>(double value)
{
	this->value = std::move(value);
}

template<>
inline void ini::Property::set<long long>(long long value)
{
	this->value = std::move(value);
}

template<>
inline void ini::Property::set<bool>(bool value)
{
	this->value = std::move(value);
}

template<typename T>
inline T ini::Property::get() const
{
	using namespace std::string_literals;

	try {
		TypeCast<T> tc;
		return std::visit(tc, value);
	}
	catch ([[maybe_unused]] const std::invalid_argument & exception)
	{
		std::string message = "Conversion to: \""s + typeid(T).name();
		throw typename exception::PropertyConversionError{ message };
	}

}

#endif // !PROPERTY_H