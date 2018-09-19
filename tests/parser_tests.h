
#include "INIFile.h"
#include "INIParser.h"

TEST(INIParser, parseSection1)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[ \"nam e \"]";
	ini_file.load(a);
	ini_file("nam e "s);
}

TEST(INIParser, parseSection2)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "  [  n\\[ame  ]  ";
	ini_file.load(a);
	ini_file("n[ame"s);
}

TEST(INIParser, parseSection3)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[\"na[]me\"]";
	ini_file.load(a);
	ini_file("na[]me"s);
}

TEST(INIParser, parseSection4)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[\"na=me\"]";
	ini_file.load(a);
	ini_file("na=me"s);
}

TEST(INIParser, parseSection5)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[name]";
	ini_file.load(a);
	ini_file("name"s);
}

TEST(INIParser, parseSection6)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[name];fsfd";
	ini_file.load(a);
	ini_file("name"s);
}

TEST(INIParser, parseSection7)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[name] ;fsdf";
	ini_file.load(a);
	ini_file("name"s);
}

TEST(INIParser, parseSection8)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[nam\\\"e]";
	ini_file.load(a);
	ini_file("nam\"e"s);
}
	
TEST(INIParser, parseSection9)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[name] s";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection10)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[name]s";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection11)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[na me]";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection12)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[na\\eme]";
	EXPECT_EQ(ini_file.load(a), false);
}
	
TEST(INIParser, parseSection13)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[\"name]";
	EXPECT_EQ(ini_file.load(a), false);
}
	
TEST(INIParser, parseSection14)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[name";
	EXPECT_EQ(ini_file.load(a), false);
}
	
TEST(INIParser, parseSection15)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[\"name\"\"fsf\"]";
	EXPECT_EQ(ini_file.load(a), false);
}
	
TEST(INIParser, parseSection16)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[na[me]";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection17)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[nam=e]";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection18)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[]";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection19)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[x]\n[[name]]";
	ini_file.load(a);
	ini_file("x")("name");
}

TEST(INIParser, parseSection20)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[x]\n[[x]]\n[[[x]]]\n[[[[name]]]]";
	ini_file.load(a);
	ini_file("x")("x")("x")("name");
}

TEST(INIParser, parseSection21)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[x]\n[[x]]\n[[[x]]]\n[[[[name]]]]\n[[[name]]]";
	ini_file.load(a);
	ini_file("x")("x")("name");
}

TEST(INIParser, parseSection22)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[ [ds]";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection23)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[ds] ]";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection24)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[[name]";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection25)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[[name]]]";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseSection26)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[name] ;fsd[]f";
	ini_file.load(a);
	ini_file("name"s);
}

TEST(INIParser, parseSection27)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[nam\\]e]";
	ini_file.load(a);
	ini_file("nam]e"s);
}

TEST(INIParser, parseSection28)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[]";
	EXPECT_EQ(ini_file.load(a), false);
}


TEST(INIParser, parseProperty1)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"a = 1\n"
		"b=1\n"
		"c =1\n"
		"d= 1\n"
		" e=1\n"
		"f=1 \n"
		" g=1 \n"
	};
	ini_file.load(a);

	ini_file["a"];
	ini_file["b"];
	ini_file["c"];
	ini_file["d"];
	ini_file["e"];
	ini_file["f"];
	ini_file["g"];
}

TEST(INIParser, parseProperty2)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"a = 1\n"
		"b = 1.1\n"
		"c = .1\n"
		"d = ala\n"
		"e = true\n"
		"f = 1.5:i\n"
		"g = 35184372088832\n"
	};
	ini_file.load(a);
	bool good{ false };

	
	ini_file["a"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, int>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["b"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, double>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["c"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, double>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["d"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["e"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, bool>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["f"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, int>) { good = true; } });
	EXPECT_EQ(good, true);
	EXPECT_EQ(ini_file["f"].get<double>(), 1.0);

	good = false;
	ini_file["g"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, long long>) { good = true; } });
	EXPECT_EQ(good, true);
}

TEST(INIParser, parseProperty3)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data_name = data"
	};
	ini_file.load(a);

	EXPECT_EQ(ini_file["data_name"].get<std::string>(), "data"s);
}

TEST(INIParser, parseProperty4)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"  data_name  =  data  "
	};
	ini_file.load(a);

	EXPECT_EQ(ini_file["data_name"].get<std::string>(), "data"s);
}

TEST(INIParser, parseProperty5)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data name=data"
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty6)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data_name=dat a"
	};
	
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty7)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data=name=data"
	};
	
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty8)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"\"data_name=data\""
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty9)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data_n[ame=data"
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty10)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"dat\"a_name=data"
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty11)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data_name="
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty12)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"=data"
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty13)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"="
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty14)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data_name=\"da ta\""
	};
	ini_file.load(a);

	EXPECT_EQ(ini_file["data_name"].get<std::string>(), "da ta"s);
}

TEST(INIParser, parseProperty15)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"\"data name\"=data"
	};
	ini_file.load(a);

	EXPECT_EQ(ini_file["data name"].get<std::string>(), "data"s);
}

TEST(INIParser, parseProperty16)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"\"data name\"=\"da ta\""
	};
	ini_file.load(a);

	EXPECT_EQ(ini_file["data name"].get<std::string>(), "da ta"s);
}

TEST(INIParser, parseProperty17)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data_name=data#asdsad"
	};
	ini_file.load(a);

	EXPECT_EQ(ini_file["data_name"].get<std::string>(), "data"s);
}

TEST(INIParser, parseProperty18)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data_name=data ;asdsad"
	};
	ini_file.load(a);

	EXPECT_EQ(ini_file["data_name"].get<std::string>(), "data"s);
}

TEST(INIParser, parseProperty19)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"data\\\"name=data"
	};
	ini_file.load(a);

	EXPECT_EQ(ini_file["data\"name"].get<std::string>(), "data"s);
}

TEST(INIParser, parseProperty20)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"a=ha:b"
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty21)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"a=ha:i"
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty22)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"a=ha:"
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty23)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"a=:"
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty24)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"a=:s"
	};

	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseProperty25)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;
	ini::INIFile ini_file2;

	std::string a = {
		"a = \"1\"\n"
		"b = \"1\":i\n"
		"c = \"true\"\n"
		"d = \"true\":b\n"

		"e = 1,5\n"
		"f = 1.5\n"
	};
	ini_file.load(a);
	bool good{ false };


	ini_file["a"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["b"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, int>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["c"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["d"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, bool>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["e"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["f"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, double>) { good = true; } });
	EXPECT_EQ(good, true);

	auto locale = std::locale::global(std::locale{});
	ini_file2.load(a);

	good = false;
	ini_file["e"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["f"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, double>) { good = true; } });
	EXPECT_EQ(good, true);

	std::locale::global(locale);
}

TEST(INIParser, parseProperty26)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"a = 1\n"
		"b = 2\n"
		"c = 3\n"
	};
	std::string b = {
		"d = 3\n"
		"e = 2\n"
		"f = 1\n"
	};
	ini_file.load(a);

	ini_file["a"];
	ini_file["b"];
	ini_file["c"];

	ini_file.load(b);

	ini_file["d"];
	ini_file["e"];
	ini_file["f"];

	EXPECT_THROW(ini_file["a"], ini::exception::PropertyDoesntExist);
	EXPECT_THROW(ini_file["b"], ini::exception::PropertyDoesntExist);
	EXPECT_THROW(ini_file["c"], ini::exception::PropertyDoesntExist);

}

TEST(INIParser, parseProperty27)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = {
		"[x]\n"
		"a = 1\n"
		"b = 2\n"
		"[y]\n"
		"c = 3\n"
		"[[y]]\n"
		"d = 3\n"
		"e = 3\n"
		"[[x]]\n"
		"f = 3\n"
		"[[y]]\n"
		"d = 4\n"
		"[[x]]\n"
		"[[[x]]]\n"
		"g = 3\n"
		"[y]\n"
		"h = 3\n"
	};
	ini_file.load(a);

	ini_file("x")["a"];
	ini_file("x")["b"];
	ini_file("y")["c"];
	ini_file("y")["h"];
	ini_file("y")("y")["d"];
	ini_file("y")("y")["e"];
	ini_file("y")("x")["f"];
	ini_file("y")("x")("x")["g"];

	EXPECT_EQ(ini_file("y")("y")["d"].get<int>(), 4);
}

TEST(INIParser, parseProperty28)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{ ini::INIFileSettings{ false, true, true, true } };

	std::string a = {
		"[x]\n"
		"a = 1\n"
		"B = 2\n"
		"[y]\n"
		"c = 3\n"
		"[[Y]]\n"
		"d = 3\n"
		"E = 3\n"
		"[[x]]\n"
		"f = 3\n"
		"[[y]]\n"
		"D = 4\n"
		"[[x]]\n"
		"[[[X]]]\n"
		"g = 3\n"
		"[y]\n"
		"h = 3\n"
	};
	ini_file.load(a);
	ini_file.addSection("AAA");

	ini_file("aaa");
	ini_file("x")["A"];
	ini_file("X")["b"];
	ini_file("y")["C"];
	ini_file("y")["h"];
	ini_file("y")("y")["d"];
	ini_file("Y")("y")["e"];
	ini_file("y")("x")["f"];
	ini_file("y")("x")("x")["g"];

	EXPECT_EQ(ini_file("y")("y")["d"].get<int>(), 4);
}

TEST(INIParser, parseProperty29)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{ ini::INIFileSettings{} };

	std::string a = {
		"[x]\n"
		"a = 1\n"
		"B = 2\n"
		"[y]\n"
		"c = 3\n"
		"[[Y]]\n"
		"d = 3\n"
		"E = 3\n"
		"[[x]]\n"
		"f = 3\n"
		"[[Y]]\n"
		"d = 4\n"
		"[[x]]\n"
		"[[[X]]]\n"
		"g = 3\n"
		"[y]\n"
		"h = 3\n"
	};
	ini_file.load(a);
	ini_file.addSection("AAA");

	EXPECT_THROW(ini_file("aaa"), ini::exception::SectionDoesntExist);
	ini_file("x")["a"];
	ini_file("x")["B"];
	ini_file("y")["c"];
	ini_file("y")["h"];
	ini_file("y")("Y")["d"];
	ini_file("y")("Y")["E"];
	ini_file("y")("x")["f"];
	ini_file("y")("x")("X")["g"];

	EXPECT_EQ(ini_file("y")("Y")["d"].get<int>(), 4);
}

TEST(INIParser, parseInlineSection1)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[a] = {1 | 2 | 10|data| 1.5} ; comment";
	ini_file.load(a);
	ini_file("a")["0"];
	ini_file("a")["4"];
}

TEST(INIParser, parseInlineSection2)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[a] = {1}";
	ini_file.load(a);
	ini_file("a")["0"];
}

TEST(INIParser, parseInlineSection3)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[a] = {1|";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseInlineSection4)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "a] = {1}";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseInlineSection5)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[a = {1}";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseInlineSection6)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "a = {1}";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseInlineSection7)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[a] = |1|";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseInlineSection8)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[a] = 1}";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseInlineSection9)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[a] = {1 } 4}";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseInlineSection10)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[a] = {1 { 4}";
	EXPECT_EQ(ini_file.load(a), false);
}

TEST(INIParser, parseInlineSection11)
{
	using namespace std::string_literals;
	ini::INIFile ini_file;

	std::string a = "[\\|a] = {1:s | 2 | 10:l|	\"dat|a\"| 1.5} ; comment";
	ini_file.load(a);
	ini_file("|a")["0"];
	ini_file("|a")["4"];
}

TEST(INIParser, type_identification)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{ ini::INIFileSettings{} };

	std::string a = {
		"[x]\n"
		"a = 1\n"
		"B = 2\n"
		"[y]\n"
		"c = 3\n"
		"[[Y]]\n"
		"d = 3\n"
		"E = 3\n"
		"[[x]]\n"
		"f = 3\n"
		"[[Y]]\n"
		"[[[a]]] = {1 | 2 | 10|data| 1.5}\n"
		"d = 4\n"
		"[[x]]\n"
		"[[[X]]]\n"
		"g = 3\n"
		"[y]\n"
		"h = 3\n"
	};
	ini_file.load(a);
	ini_file.addSection("AAA");

	EXPECT_THROW(ini_file("aaa"), ini::exception::SectionDoesntExist);
	ini_file("x")["a"];
	ini_file("x")["B"];
	ini_file("y")["c"];
	ini_file("y")["h"];
	ini_file("y")("Y")["d"];
	ini_file("y")("Y")["E"];
	ini_file("y")("x")["f"];
	ini_file("y")("x")("X")["g"];

	ini_file("y")("Y")("a")["0"];
	ini_file("y")("Y")("a")["1"];
	ini_file("y")("Y")("a")["2"];
	ini_file("y")("Y")("a")["3"];
	ini_file("y")("Y")("a")["4"];

	EXPECT_EQ(ini_file("y")("Y")["d"].get<int>(), 4);
}

TEST(INIParser, type_identification1)
{
	using namespace std::string_literals;
	ini::INIFile ini_file{ ini::INIFileSettings{true, true, false, true} };

	std::string a = {
		"a = 1\n"
		"b = 1.1\n"
		"c = .1\n"
		"d = ala\n"
		"e = true\n"
		"f = 1.5:i\n"
		"g = 35184372088832\n"
	};
	ini_file.load(a);
	bool good{ false };


	ini_file["a"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["b"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["c"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["d"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["e"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["f"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);

	good = false;
	ini_file["g"].visit([&good](auto && x) { using T = std::decay_t<decltype(x)>; if constexpr (std::is_same_v<T, std::string>) { good = true; } });
	EXPECT_EQ(good, true);
}