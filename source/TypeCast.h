#ifndef TYPECAST_H
#define TYPECAST_H

#include <type_traits>
#include <string>
#include <locale>
#include <functional>

namespace ini
{
	template<typename F, typename ... Arg>
	auto invokeInCLocale(F f, Arg && ... arg)
	{
		auto locale = std::locale::global(std::locale("C"));
		auto val = std::invoke(f, std::forward<Arg>(arg)...);
		std::locale::global(locale);
		return val;
	}

	template<typename T>
	class TypeCast
	{
		static_assert(std::is_same<int, T>::value, "Type Cast error type!");
	};

	template<>
	class TypeCast<int>
	{
	public:
		int operator()(int x) { return x; };
		int operator()(std::string x) {
			return ini::invokeInCLocale([](auto && x) { return std::stoi(x); }, x); // Invoke in "C" locale to ensure consistent conversion results
		};
		int operator()(double x) { return static_cast<int>(x); };
		int operator()(long long x) { return static_cast<int>(x); };
		int operator()(bool x) { return static_cast<int>(x); };
	};

	template<>
	class TypeCast<std::string>
	{
	public:
		std::string operator()(int x) {
			return ini::invokeInCLocale([](auto && x) { return std::to_string(x); }, x); // Invoke in "C" locale to ensure consistent conversion results
		};
		std::string operator()(std::string x) { return x; };
		std::string operator()(double x) {
			return ini::invokeInCLocale([](auto && x) { return std::to_string(x); }, x); // Invoke in "C" locale to ensure consistent conversion results
		};
		std::string operator()(long long x) {
			return ini::invokeInCLocale([](auto && x) { return std::to_string(x); }, x); // Invoke in "C" locale to ensure consistent conversion results
		};
		std::string operator()(bool x) {
			using namespace std::string_literals;
			return x ? "true"s : "false"s;
		};
	};

	template<>
	class TypeCast<double>
	{
	public:
		double operator()(int x) { return static_cast<double>(x); };
		double operator()(std::string x) {
			return ini::invokeInCLocale([](auto && x) { return std::stod(x); }, x); // Invoke in "C" locale to ensure consistent conversion results
		};
		double operator()(double x) { return x; };
		double operator()(long long x) { return static_cast<double>(x); };
		double operator()(bool x) { return static_cast<double>(x); };
	};

	template<>
	class TypeCast<long long>
	{
	public:
		long long operator()(int x) { return static_cast<long long>(x); };
		long long operator()(std::string x) {
			return ini::invokeInCLocale([](auto && x) { return std::stoll(x); }, x); // Invoke in "C" locale to ensure consistent conversion results
		};
		long long operator()(double x) { return static_cast<long long>(x); };
		long long operator()(long long x) { return static_cast<long long>(x); };
		long long operator()(bool x) { return static_cast<long long>(x); };
	};

	template<>
	class TypeCast<bool>
	{
	public:
		bool operator()(int x) { return static_cast<bool>(x); };
		bool operator()(std::string x) {
			using namespace std::string_literals;
			if (x == "1"s || x == "true"s || x == "TRUE"s)
				return true;
			return false;
		}
		bool operator()(double x) { return static_cast<bool>(x); };
		bool operator()(long long x) { return static_cast<bool>(x); };
		bool operator()(bool x) { return x; };
	};
}

#endif // !TYPECAST_H

