#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include "Core/Serialize/Include/INI.h"


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
		mIsInSectionName = false;
		mLineNo = 0;

		const char* lineBegin = src;
		const char* lineEnd;
		while (lineBegin != nullptr)
		{
			lineEnd = strchr(lineBegin, '\n');
			ParseLine(lineBegin, lineEnd);
			if (lineEnd != nullptr)
				lineEnd++;
			lineBegin = lineEnd;
		}
	}

	void INIParser::ParseLine(const char* lineBegin, const char* lineEnd)
	{
		if (lineEnd == nullptr)
			lineEnd = strchr(lineBegin, '\n');
		if (lineEnd == nullptr)
			lineEnd = lineBegin + strlen(lineBegin);

		int lineLen = lineEnd - lineBegin;
		mLineNo++;

		for (int i = 0; i < lineLen; i++)
		{
			char c = lineBegin[i];
			if (c == '[')
			{
				mSectionNameLen = 0;
				mIsInSectionName = true;
			}
			else if (c == ']')
			{
				mSectionNameBuf[mSectionNameLen] = '\0';
				mIsInSectionName = false;
				if (mUserCallback && mUserCallback->OnSection)
					mUserCallback->OnSection(mConfig.UserData, mLineNo, mSectionNameBuf);
			}
			else if (mIsInSectionName)
			{
				assert(mSectionNameLen < MAX_SECTION_NAME_LEN);
				mSectionNameBuf[mSectionNameLen++] = c;
			}
		}
	}

} // namespace LD