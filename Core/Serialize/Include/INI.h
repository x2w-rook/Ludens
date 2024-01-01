#pragma once


namespace LD {

	struct INIParserConfig
	{
		// ROOM: for configuring parser behaviour
	};

	struct INIParserCallback
	{
		void (*OnSectionEnter)(void* user, int lineno, const char* section);
		void (*OnSectionLeave)(void* user, int lineno, const char* section);
		void (*OnProperty)(void* user, int lineno, const char* name, const char* value);
	};

	class INIParser
	{
	public:
		INIParser() {}
		~INIParser() {}

		void Config(const INIParserConfig& config);
		void ParseString(const char* src, const INIParserCallback* callbacks);

	private:
		INIParserConfig mConfig;
		const INIParserCallback* mUserCallback = nullptr;
	};

} // namespace LD