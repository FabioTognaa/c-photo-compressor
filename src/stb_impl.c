/*
 * Single compilation unit for stb_image and stb_image_write.
 * Include this once in the build; other files must NOT define
 * STB_IMAGE_IMPLEMENTATION or STB_IMAGE_WRITE_IMPLEMENTATION.
 */

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../third-party/stb_image.h"
#include "../third-party/stb_image_write.h"
