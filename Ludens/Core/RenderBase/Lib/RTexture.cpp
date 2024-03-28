#include "Core/Header/Include/Error.h"
#include "Core/RenderBase/Include/RTexture.h"

namespace LD {

	struct TextureFormatData
	{
		size_t PixelSize; // byte size for one pixel
	};

	static TextureFormatData sTextureFormatData[] {
		{ 1 }, // R8
		{ 4 }, // RGBA8
		{ 8 }, // RGBA16F
		{ 4 }, // D24S8
	};

	LD_STATIC_ASSERT(sizeof(sTextureFormatData) / sizeof(TextureFormatData) == (size_t)RTextureFormat::EnumCount);

	size_t GetTextureFormatPixelSize(RTextureFormat format)
	{
		return sTextureFormatData[(size_t)format].PixelSize;
	}

} // namespace LD