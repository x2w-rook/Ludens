#pragma once

#define MAX_SECTION_NAME_LEN  256

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
		INIParser() {}
		~INIParser() {}

		void Config(const INIParserConfig& config);
		void ParseString(const char* src, const INIParserCallback* callbacks);

	private:
		void ParseLine(const char* lineBegin, const char* lineEnd);

		INIParserConfig mConfig;
		const INIParserCallback* mUserCallback = nullptr;
		int mLineNo;
		char mSectionNameBuf[MAX_SECTION_NAME_LEN];
		int mSectionNameLen;
		bool mIsInSectionName;
	};

} // namespace LD