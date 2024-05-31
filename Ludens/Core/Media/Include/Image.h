#pragma once

#include "Core/Header/Include/Types.h"

namespace LD
{

bool LoadImage(u8** pixels, const char* path, int* width, int* height, int* channels);
void FreeImage(u8* pixels);

} // namespace LD