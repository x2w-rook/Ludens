#pragma once

#include <cstddef>
#include "Core/OS/Include/UID.h"


namespace LD {

	enum class RTextureFormat
	{
		Undefined = 0,
		R8,
		BGRA8,
		RGBA8,
		RGBA16F,
		D24S8,
		D32F,
		EnumCount
	};

	size_t GetTextureFormatPixelSize(RTextureFormat format);

	enum RTextureUsageFlags : u8
	{
		// this texture can be used as a frame buffer attachment
		TEXTURE_USAGE_FRAME_BUFFER_ATTACHMENT_BIT = (1 << 0),
	};

	enum class RTextureType
	{
		Texture2D = 0,
	};

	struct RTextureInfo
	{
		RTextureType Type;
		RTextureFormat Format;
		RTextureUsageFlags TextureUsage;
		u32 Width;
		u32 Height;
		const void* Data;   // pixel data
		size_t Size = 0;    // pixel data byte size
	};

	struct RTextureBase;
	struct RTextureGL;

	class RTexture
	{
		friend struct RTextureBase;
		friend struct RTextureGL;
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
