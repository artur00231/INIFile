#ifndef TEXTCONVERSION_H
#define TEXTCONVERSION_H

#include <string>
#include <string_view>

namespace ini
{
	class DefaultTextConversion
	{
	public:
		static std::string tolower(std::string_view text);
	};

	class UTF8TextConversion
	{
	public:
		static std::string tolower(std::string_view text);
	};
}

#endif // !TEXTCONVERSION_H

