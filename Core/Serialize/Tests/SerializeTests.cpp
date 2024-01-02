#include <string>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "Core/Serialize/Include/INI.h"

using namespace LD;

struct TestContext
{
	int LineNo;
	std::string SectionName;
};


static void OnSection(void* user, int lineno, const char* section)
{
	TestContext* test = (TestContext*)user;
	CHECK(test->LineNo == lineno);
	CHECK(test->SectionName == std::string(section));
}


TEST_CASE("ini sections")
{
	TestContext test;
	INIParser parser;

	INIParserConfig testConfig;
	testConfig.UserData = &test;
	parser.Config(testConfig);

	INIParserCallback testCallbacks;
	testCallbacks.OnSection = OnSection;
	
	const char* src = "[Section 1]";
	test.SectionName = "Section 1";
	test.LineNo = 1;
	parser.ParseString(src, &testCallbacks);

	src = "[ white space surround\t]";
	test.SectionName = " white space surround\t";
	test.LineNo = 1;
	parser.ParseString(src, &testCallbacks);

	src = "\n\n[On Third Line]";
	test.SectionName = "On Third Line";
	test.LineNo = 3;
	parser.ParseString(src, &testCallbacks);
}