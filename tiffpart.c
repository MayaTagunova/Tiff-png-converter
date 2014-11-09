#include "tiffpart.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <tiffio.h>
#include <tiff.h>

int tiffGetDataSize(const char *fileName, size_t *size)
{
#ifdef DEBUG
    printf("tiffGetDataSize...\n");
#endif
    assert(fileName != NULL);
    assert(size != NULL);

    TIFF *image = TIFFOpen(fileName, "r");

    if (!image) {
        printf("tiffGetDataSize: TIFFOpen failed.\n");
        return 1;
    }

    uint32_t width;
    uint32_t height;
    TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height);

    size[0] = width;
    size[1] = height;

    TIFFClose(image);
    return 0;
}

int tiffReadData(const char *fileName, uint32_t *buffer, size_t *size)
{
#ifdef DEBUG
    printf("tiffReadData...\n");
#endif
    assert(fileName != NULL);
    assert(buffer != NULL);
    assert(size != NULL);

    TIFF *image;

    if (!(image = TIFFOpen(fileName, "r"))) {
        printf("tiffReadData: TIFFOpen failed.\n");
        return 1;
    }

    size_t size2[2];

    TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &size2[0]);
    TIFFGetField(image, TIFFTAG_IMAGELENGTH, &size2[1]);

    if ((size2[0] != size[0]) || (size2[1] != size[1])) {
        printf("tiffReadData: size does not match.\n");
        TIFFClose(image);
        return 1;
    }

    if (!(TIFFReadRGBAImageOriented(image, size[0], size[1],
                                    buffer, ORIENTATION_TOPLEFT, 0))) {
        printf("tiffReadData: TIFFReadRGBAImage failed.\n");
        TIFFClose(image);
        return 1;
    }

    TIFFClose(image);
    return 0;
}

int tiffWriteData(const char *fileName, uint32_t *buffer, size_t *size)
{
#ifdef DEBUG
    printf("tiffWriteData...\n");
#endif
    assert(fileName != NULL);
    assert(buffer != NULL);
    assert(size != NULL);

    TIFF *image;

    if (!(image = TIFFOpen(fileName, "w"))) {
        printf("tiffWriteData: TIFFOpen failed.\n");
        return 1;
    }

    uint16_t alphaTypes[] = { EXTRASAMPLE_UNASSALPHA };

    TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 4);
    TIFFSetField(image, TIFFTAG_EXTRASAMPLES, 1, alphaTypes);
    TIFFSetField(image, TIFFTAG_IMAGEWIDTH, size[0]);
    TIFFSetField(image, TIFFTAG_IMAGELENGTH, size[1]);
    TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
    TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_LZW);

    if ((TIFFWriteEncodedStrip(image, 0, buffer, size[0] * size[1] * sizeof(uint32_t))) == -1) {
        printf("tiffWriteData: TIFFWriteRawTile failed.\n");
        TIFFClose(image);
        return 1;
    }

    TIFFClose(image);
    return 0;
}

