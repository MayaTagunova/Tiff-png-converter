#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <png.h>
#include "pngpart.h"

int pngGetDataSize(const char *fileName, size_t *size)
{
    assert(fileName != NULL);
    assert(size != NULL);

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    if (!(png_image_begin_read_from_file(&image, fileName))) {
        printf("pngGetDataSize: png_image_begin_read_from_file failed");
        return 1;
    }

    size[0] = image.width;
    size[1] = image.height;
    png_image_free(&image);
    return 0;
}

int pngReadData(const char *fileName, uint32_t *buffer, size_t *size)
{
    assert(fileName != NULL);
    assert(buffer != NULL);
    assert(size != NULL);

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.format = PNG_FORMAT_RGBA;

    if (!(png_image_begin_read_from_file(&image, fileName))) {
        printf("pngReadData: png_image_begin_read_from_file failed");
        return 1;
    }

    if ((image.width != size[0]) || (image.height != size[1])) {
        printf("pngReadData: size does not match");
        return 1;
    }

    if (!(png_image_finish_read(&image, NULL, buffer, 0, NULL))) {
        printf("pngReadData: png_image_finish_read failed");
        return 1;
    }

    png_image_free(&image);
    return 0;
}

int pngWriteData(const char *fileName, uint32_t *buffer, size_t *size)
{
#ifdef DEBUG
    printf("pngWriteData...\n");
#endif
    assert(fileName != NULL);
    assert(buffer != NULL);
    assert(size != NULL);

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.format = PNG_FORMAT_RGBA;
    image.width = size[0];
    image.height = size[1];

    if (!(png_image_write_to_file(&image, fileName, 0, buffer, 0, NULL))) {
        printf("pngWriteData: png_image_write_to_file failed");
        return 1;
    }
    return 0;
}

