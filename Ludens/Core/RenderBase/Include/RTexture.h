#pragma once

#include "Core/OS/Include/UID.h"

namespace LD {

	enum class RTextureFormat
	{
		R8 = 0,
		RGBA8,
		RGBA16F,
		D24S8,
		EnumCount
	};

	size_t GetTextureFormatPixelSize(RTextureFormat format);

	enum class RTextureType
	{
		Texture2D = 0,
	};

	struct RTextureInfo
	{
		RTextureType Type;
		RTextureFormat Format;
		u32 Width;
		u32 Height;
		const void* Data;   // pixel data
		size_t Size = 0;    // pixel data byte size
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