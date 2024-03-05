#pragma once

#include "Core/OS/Include/UID.h"

namespace LD {

	enum class RTextureFormat
	{
		RGBA8 = 0,
		RGBA16F,
		D24S8,
		EnumCount
	};

	enum class RTextureType
	{
		Texture2D = 0,
	};

	struct RTextureInfo
	{
		RTextureType Type;
		RTextureFormat Format;
		u16 Width;
		u16 Height;
		const u8* Data;
	};

	class RTexture
	{
		friend class RTextureBase;
		friend class RTextureGL;
	public:
		using TBase = RTextureBase;

		inline bool operator==(const RTexture& other) const { return mID == other.mID; }
		inline bool operator!=(const RTexture& other) const { return mID != other.mID; }

		inline operator bool() const { return mID != 0 && mTexture != nullptr; }
		inline operator RTextureBase*() const { return mTexture; }

	private:
		UID mID = 0;
		RTextureBase* mTexture = nullptr;
	};

} // namespace LD