
#include "INIFile.h"
#include "SectionIterator.h"

TEST(Section, Adding_Section)
{
	ini::INIFile ini_file{};

	ini_file.addSection("x").addSection("y");

	EXPECT_EQ(ini_file.sectionExists({ "x" }), true);
	EXPECT_EQ(ini_file.sectionExists({ "x", "y" }), true);

	EXPECT_THROW(ini_file.addSection("x"), ini::exception::SectionAlreadyExist);
}

TEST(Section, Accessing_to_Section)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{};

	ini_file.addSection("x").addSection("y");
	ini_file.addSection("z");

	EXPECT_EQ(ini_file("x").getName(), "x"s);
	EXPECT_EQ(ini_file("z").getName(), "z"s);
	EXPECT_EQ(ini_file("x")("y").getName(), "y"s);
	EXPECT_EQ(ini_file.getSection({"x", "y"}).getName(), "y"s);

	EXPECT_THROW(ini_file("f"), ini::exception::SectionDoesntExist);
}

TEST(Section, Adding_Property)
{
	ini::INIFile ini_file{};

	ini_file.addProperty("x", 1);

	EXPECT_EQ(ini_file.propertyExists({ "x" }), true);
	EXPECT_THROW(ini_file.addProperty("x", 1.5), ini::exception::PropertyAlreadyExist);
}

TEST(Section, Accessing_to_Property)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{};

	ini_file.addSection("x").addSection("y").addProperty("a", 1);
	ini_file.addSection("z").addProperty("b", 1);

	EXPECT_EQ(ini_file("x")("y")["a"].getName(), "a"s);
	EXPECT_EQ(ini_file("z")["b"].getName(), "b"s);
	EXPECT_EQ(ini_file.getProperty({ "x", "y", "a" }).getName(), "a"s);

	EXPECT_THROW(ini_file["f"], ini::exception::PropertyDoesntExist);
}

TEST(Section, SectionIterator)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{};

	ini_file.addSection("x");
	ini_file.addSection("y");
	ini_file.addSection("z");
	ini_file.addSection("a");

	std::vector<std::string> names;

	ini::SectionIterator it{ ini_file };
	for (; it != ini::SectionIterator{}; ++it)
	{
		names.push_back(it->getName());
	}

	std::sort(names.begin(), names.end());
	EXPECT_EQ(names.front(), "a"s);
	EXPECT_EQ(names.back(), "z"s);

	EXPECT_THROW(it++, std::out_of_range);
}

TEST(Section, const_SectionIterator)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{};

	ini_file.addSection("x");
	ini_file.addSection("y");
	ini_file.addSection("z");
	ini_file.addSection("a");

	std::vector<std::string> names;

	ini::const_SectionIterator it{ ini_file };
	for (; it != ini::const_SectionIterator{}; ++it)
	{
		names.push_back(it->getName());
	}

	std::sort(names.begin(), names.end());
	EXPECT_EQ(names.front(), "a"s);
	EXPECT_EQ(names.back(), "z"s);

	EXPECT_THROW(it++, std::out_of_range);
}

TEST(Section, PropertyIterator)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{};

	ini_file.addProperty("x", 1);
	ini_file.addProperty("y", 1);
	ini_file.addProperty("z", 1);
	ini_file.addProperty("a", 1);

	std::vector<std::string> names;

	auto it = ini_file.begin();
	for (; it != ini_file.end(); ++it)
	{
		names.push_back(it->getName());
	}

	std::sort(names.begin(), names.end());
	EXPECT_EQ(names.front(), "a"s);
	EXPECT_EQ(names.back(), "z"s);

	EXPECT_THROW(it++, std::out_of_range);
}

TEST(Section, const_PropertyIterator)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{};

	ini_file.addProperty("x", 1);
	ini_file.addProperty("y", 1);
	ini_file.addProperty("z", 1);
	ini_file.addProperty("a", 1);

	std::vector<std::string> names;

	auto it = ini_file.cbegin();
	for (; it != ini_file.cend(); ++it)
	{
		names.push_back(it->getName());
	}

	std::sort(names.begin(), names.end());
	EXPECT_EQ(names.front(), "a"s);
	EXPECT_EQ(names.back(), "z"s);

	EXPECT_THROW(it++, std::out_of_range);
}

TEST(Property, set)
{
	ini::Property property{ "x" };
	property.set(10);
	auto foo1 = [](auto x) { if constexpr (std::is_same_v<int, decltype(x)>) { throw 1; }};

	EXPECT_THROW(property.visit(foo1), int);

	property.set(1.5);
	auto foo2 = [](auto x) { if constexpr (std::is_same_v<double, decltype(x)>) { throw 1; }};

	EXPECT_THROW(property.visit(foo2), int);

	property.set(true);
	auto foo3 = [](auto x) { if constexpr (std::is_same_v<bool, decltype(x)>) { throw 1; }};

	EXPECT_THROW(property.visit(foo3), int);

	property.set(std::string{"a"});
	auto foo4 = [](auto x) { if constexpr (std::is_same_v<std::string, decltype(x)>) { throw 1; }};

	EXPECT_THROW(property.visit(foo4), int);
}

TEST(Property, get)
{
	using namespace std::string_literals;
	ini::Property property{ "x" };
	property.set(10);
	
	EXPECT_EQ(property.get<int>(), 10);
	EXPECT_EQ(property.get<double>(), 10.0);
	EXPECT_EQ(property.get<std::string>(), "10"s);

	property.set(1.8);

	EXPECT_EQ(property.get<int>(), 1);
	EXPECT_EQ(property.get<double>(), 1.8);
	EXPECT_EQ(property.get<std::string>().substr(0, 3), "1.8"s);

	property.set(true);

	EXPECT_EQ(property.get<int>(), 1);
	EXPECT_EQ(property.get<double>(), 1.0);
	EXPECT_EQ(property.get<std::string>(), "true"s);
}

TEST(INIFIle, load_no_ASCII_characters)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;
	std::string a{
		"[main]\n"
	"info = data\n"
	"value = 25\n"
	"text_value = 364:s\n"
	"utf-8_text = \"\xc5\xBC\xC3\xB3\xC5\x82\xC4\x87\"\n"
	"\"\xC5\xBB\xC3\x93\xC5\x81\xC4\x86\" = 24"
	};

	ini_file.load(a);

	EXPECT_GT(ini_file("main")["utf-8_text"].get<std::string>().size(), 4u);
}

TEST(INIFIle, case_insensitivity)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{ ini::INIFileSettings{false, true, true, true, ini::UTF8TextConversion::tolower} };
	std::string a{
		"[main]\n"
	"info = data\n"
	"value = 25\n"
	"text_value = 364:s\n"
	"utf-8_text = \"\xc5\xBC\xC3\xB3\xC5\x82\xC4\x87\"\n"
	"\"\xC5\xBB\xC3\x93\xC5\x81\xC4\x86\" = 24"
	};

	ini_file.load(a);

	ini_file("main")[ini_file("main")["utf-8_text"].get<std::string>()];
}

TEST(INIFIle, save1)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{};

	auto & x = ini_file.addSection("section");
	x.addProperty("0", 1);
	x.addProperty("1", std::string{ "Ala" });
	x.addProperty("2", 4);
	x.addProperty("3", 556);
	x.addProperty("4", true);

	ini::Property property{ "5" };
	property.set<std::string>("1");

	x.addProperty(property);

	std::string buffer;

	ini_file.save(buffer);


	EXPECT_EQ(buffer, "[section] = { 1 | Ala | 4 | 556 | true | 1:s }\n"s);
	EXPECT_EQ(x["1"].get<std::string>(), "Ala"s);
}

TEST(INIFIle, save2)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{};

	ini_file.addSection("x");
	ini_file.addSection("y");
	ini_file.addSection("z");

	ini_file("x").addSection("x");
	ini_file("x").addProperty("0", 1);
	ini_file("x").addProperty("1", 2);
	ini_file("x").addProperty("2", 3);

	ini_file("y").addProperty("0", 1);
	ini_file("y").addProperty("3", 2);
	ini_file("y").addProperty("2", 3);
	ini_file("y").addProperty("1", 4);

	ini_file("z").addSection("x");
	ini_file("z").addSection("y");
	
	ini_file("z")("x").addProperty("a", "4"s);
	ini_file("z")("x").addProperty("b", true);
	ini_file("z")("x").addProperty("v", "78"s);

	ini_file("z")("y").addProperty("0", 5);
	ini_file("z")("y").addProperty("2", 5);
	ini_file("z")("y").addProperty("1", 5);
	ini_file("z")("y").addProperty("3", 5);

	std::string buffor{};
	ini_file.save(buffor);
}

TEST(INIFIle, inline_section)
{
	using namespace std::string_literals;
	ini::INIFileSettings settings{ true, false, true, true };
	ini::INIFile ini_file1{ settings };
	ini::INIFile ini_file2{};

	ini_file1.addSection("x");

	ini_file1("x").addProperty("0", 1);
	ini_file1("x").addProperty("1", 2);
	ini_file1("x").addProperty("2", 3);

	ini_file2.addSection("x");

	ini_file2("x").addProperty("0", 1);
	ini_file2("x").addProperty("1", 2);
	ini_file2("x").addProperty("2", 3);

	std::string buffor1{};
	std::string buffor2{};
	ini_file1.save(buffor1);
	ini_file2.save(buffor2);

	EXPECT_NE(buffor1, buffor2);
}

TEST(INIFIle, advanced_save_layout)
{
	using namespace std::string_literals;
	ini::INIFileSettings settings{ true, true, true, false };
	ini::INIFile ini_file1{ settings };
	ini::INIFile ini_file2{};

	ini_file1.addSection("x");

	ini_file1("x").addProperty("0", 1);
	ini_file1("x").addSection("1");
	ini_file1("x")("1").addProperty("2", 3);

	ini_file2.addSection("x");

	ini_file2("x").addProperty("0", 1);
	ini_file2("x").addSection("1");
	ini_file2("x")("1").addProperty("2", 3);

	std::string buffor1{};
	std::string buffor2{};
	ini_file1.save(buffor1);
	ini_file2.save(buffor2);

	EXPECT_NE(buffor1, buffor2);
}

TEST(INIFIle, type_identification2)
{
	using namespace std::string_literals;
	ini::INIFileSettings settings{ true, true, false, true };
	ini::INIFile ini_file1{ settings };
	ini::INIFile ini_file2{};

	ini_file1.addSection("x");

	ini_file1("x").addProperty("0", 1);
	ini_file1("x").addSection("1");
	ini_file1("x")("1").addProperty("2", 3);

	ini_file2.addSection("x");

	ini_file2("x").addProperty("0", 1);
	ini_file2("x").addSection("1");
	ini_file2("x")("1").addProperty("2", 3);

	std::string buffor1{};
	std::string buffor2{};
	ini_file1.save(buffor1);
	ini_file2.save(buffor2);

	bool good{ false };

	ini_file1("x")["0"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file2("x")["0"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, int>) { good = true; } });
	EXPECT_EQ(good, true);

	EXPECT_EQ(buffor1, buffor2);
}

TEST(INIFIle, load_save_special)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{};
	ini::INIFile ini_file1{};
	std::string a{
		"a = \"[name]\\na = 4\\n\\tb = 5\""
	};

	ini_file.load(a);

	ini_file1.load(ini_file["a"].get<std::string>());

	ini_file1("name")["a"];
	ini_file1("name")["b"];
}