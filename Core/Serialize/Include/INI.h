#pragma once

#define MAX_SECTION_NAME_LEN       256
#define MAX_PROPERTY_NAME_LEN      128
#define MAX_PROPERTY_VALUE_LEN     128

namespace LD {

	struct INIParserConfig
	{
		void* UserData = nullptr;
	};

	struct INIParserCallback
	{
		void (*OnSection)(void* user, int lineno, const char* section) = nullptr;
		void (*OnProperty)(void* user, int lineno, const char* name, const char* value) = nullptr;
	};

	class INIParser
	{
	public:
		INIParser() = delete;
		INIParser(const INIParserConfig& config) : mConfig(config) {}
		~INIParser() {}

		void Config(const INIParserConfig& config);
		void ParseString(const char* src, const INIParserCallback* callbacks);

	private:
		void ParseLine(const char* lineBegin, const char* lineEnd);

		INIParserConfig mConfig;
		const INIParserCallback* mUserCallback = nullptr;
		int mLineNo;
		int mSectionNameLen;
		int mPropertyNameLen;
		int mPropertyValueLen;
		bool mIsInSectionName;
		bool mIsInPropertyName;
		bool mIsInPropertyValue;
		char mSectionNameBuf[MAX_SECTION_NAME_LEN];
		char mPropertyNameBuf[MAX_PROPERTY_NAME_LEN];
		char mPropertyValueBuf[MAX_PROPERTY_VALUE_LEN];
	};

} // namespace LD