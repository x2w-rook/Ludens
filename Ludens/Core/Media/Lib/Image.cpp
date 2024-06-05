#include <iostream>
#include <cstdio>
#include <stb/stb_image.h>
#include "Core/Header/Include/Error.h"
#include "Core/Media/Include/Image.h"

namespace LD
{

bool LoadImage(u8** pixels, const char* path, int* width, int* height, int* channels)
{
    // TODO: parameterize
    stbi_set_flip_vertically_on_load(false);

    stbi_uc* result = stbi_load(path, width, height, channels, STBI_rgb_alpha);
    LD_DEBUG_ASSERT(result);

    if (result == NULL)
        return false;

    *pixels = static_cast<u8*>(result);

    printf("===ImageLoad===\n");
    printf("image: %s\n", path);
    printf("%dx%d (%d ch)\n", *width, *height, *channels);
    return true;
}

void FreeImage(u8* pixels)
{
    stbi_image_free((void*)pixels);
}

} // namespace LD