#ifndef INIFILESETTINGS_H
#define INIFILESETTINGS_H

#include <string>
#include <string_view>

#include "TextConversion.h"

namespace ini 
{
	struct INIFileSettings
	{
		using ConversionFuncion = std::string(*)(std::string_view);

		INIFileSettings() {};
		INIFileSettings(bool case_sensitivity, bool inline_section, bool type_identification, bool advanced_save_layout, ConversionFuncion tolower) :
			case_sensitivity{ case_sensitivity },
			inline_section{ inline_section },
			type_identification{ type_identification },
			advanced_save_layout{ advanced_save_layout },
			tolower{ tolower } {};

		INIFileSettings(bool case_sensitivity, bool inline_section, bool type_identification, bool advanced_save_layout) :
			case_sensitivity{ case_sensitivity },
			inline_section{ inline_section },
			type_identification{ type_identification },
			advanced_save_layout{ advanced_save_layout } {};

		INIFileSettings(const INIFileSettings&) = default;
		INIFileSettings& operator=(const INIFileSettings&) = default;
		INIFileSettings(INIFileSettings&&) = default;
		INIFileSettings& operator=(INIFileSettings&&) = default;

		bool case_sensitivity{ true };
		bool inline_section{ true }; // Enable array like section definition
		bool type_identification{ true }; // Automatically cast property value
		bool advanced_save_layout{ true }; // Add tabulators to show level of nesting

		ConversionFuncion tolower = &DefaultTextConversion::tolower;

		~INIFileSettings() {};
	};
}

#endif // !INIFILESETTINGS_H

