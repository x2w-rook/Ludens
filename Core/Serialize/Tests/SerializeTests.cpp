#include <string>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "Core/Serialize/Include/INI.h"

using namespace LD;

struct TestContext
{
	int LineNo;
	std::string SectionName;
	std::string PropertyName;
	std::string PropertyValue;
};


static void OnSection(void* user, int lineno, const char* section)
{
	TestContext* test = (TestContext*)user;
	CHECK(test->LineNo == lineno);
	CHECK(test->SectionName == std::string(section));
}

static void OnProperty(void* user, int lineno, const char* name, const char* value)
{
	TestContext* test = (TestContext*)user;
	CHECK(test->LineNo == lineno);
	CHECK(test->PropertyName == std::string(name));
	CHECK(test->PropertyValue == std::string(value));
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


TEST_CASE("ini properties")
{
	TestContext test;
	INIParser parser;

	INIParserConfig testConfig;
	testConfig.UserData = &test;
	parser.Config(testConfig);

	INIParserCallback testCallbacks;
	testCallbacks.OnProperty = OnProperty;

	const char* src = "prop1=value1";
	test.PropertyName = "prop1";
	test.PropertyValue = "value1";
	test.LineNo = 1;
	parser.ParseString(src, &testCallbacks);

	src = "\n prop_2 = value_2 ";
	test.PropertyName = "prop_2";
	test.PropertyValue = "value_2";
	test.LineNo = 2;
	parser.ParseString(src, &testCallbacks);
}