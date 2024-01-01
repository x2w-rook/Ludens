#include "INI.h"

namespace LD {

	void INIParser::Config(const INIParserConfig& config)
	{
		mConfig = config;
	}

	void INIParser::ParseString(const char* src, const INIParserCallback* callbacks)
	{
		if (!callbacks)
			return;

		mUserCallback = callbacks;
	}

} // namespace LD