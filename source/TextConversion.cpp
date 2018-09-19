#include "TextConversion.h"

#include <cctype>
#include <algorithm>

std::string ini::DefaultTextConversion::tolower(std::string_view text)
{
	std::string converted_text{};
	std::back_insert_iterator<std::string> it{ converted_text };

	std::transform(text.begin(), text.end(), it, [](char x) { return std::tolower(static_cast<unsigned char>(x)); }); // Use static_cast<unsigned char> to prevent negative value of char, which may trigger assetion

	return converted_text;
}

// #define DO_NOT_USE_CODECVT_ // Uncomment if you want permanently disable use of the codecvt
							   // or add #define DO_NOT_USE_CODECVT_ before including INIFile

#ifdef DO_NOT_USE_CODECVT_
#define _USE_CODECVT_ false
#else
#define _USE_CODECVT_ true
#endif

#if _USE_CODECVT_

#include <locale>
#include <codecvt>

std::string ini::UTF8TextConversion::tolower(std::string_view text)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring utf16_text = converter.from_bytes(&text[0], &text[0] + text.size());

	std::locale locale{""}; // Get a system locale

	std::wstring converted_text{};
	std::back_insert_iterator<std::wstring> it{ converted_text };

	std::transform(utf16_text.begin(), utf16_text.end(), it, [&locale](wchar_t x) { return std::tolower(x, locale); });

	return converter.to_bytes(converted_text);
}

#endif // _USE_CODECVT_

#undef _USE_CODECVT_