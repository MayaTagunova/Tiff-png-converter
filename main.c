#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pngpart.h"
#include "tiffpart.h"

typedef enum fileType {TYPE_PNG, TYPE_TIFF} fileType;

int convert(const char *inputFileName, const char *outputFileName, fileType inputType, fileType outputType)
{
#ifdef DEBUG
    printf("convert...\n");
#endif
    assert(inputFileName != NULL);
    assert(outputFileName != NULL);

    size_t size[2];
    uint32_t *buffer = NULL;

    int (*getDataSize)(const char *, size_t *);
    int (*readData)(const char *, uint32_t *, size_t *);
    int (*writeData)(const char *, uint32_t *, size_t *);

    switch (inputType) {
    case TYPE_PNG:
        getDataSize = &pngGetDataSize;
        readData = &pngReadData;
        break;
    case TYPE_TIFF:
        getDataSize = &tiffGetDataSize;
        readData = &tiffReadData;
        break;
    default:
        return 1;
    }

    switch (outputType) {
    case TYPE_PNG:
        writeData = &pngWriteData;
        break;
    case TYPE_TIFF:
        writeData = &tiffWriteData;
        break;
    default:
        return 1;
    }

    if (getDataSize(inputFileName, size)) {
        printf("convert: getDataSize failed");
        return 1;
    }
    if ((buffer = (uint32_t *)malloc(size[0] * size[1] * sizeof(uint32_t))) == NULL) {
        printf("Memory allocation failed");
        return 1;
    }
    if (readData(inputFileName, buffer, size)) {
        printf("convert: readData failed");
        free(buffer);
        return 1;
    }
    if (writeData(outputFileName, buffer, size)) {
        printf("convert: writeData failed");
        free(buffer);
        return 1;
    }
    free(buffer);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        printf("Usage: %s -[options] [INPUT_FILE] [OUTPUT_FILE]\n", argv[0]);
        return 1;
    }

    if (argv [1][0] != '-') {
        printf("Invalid option.\n");
        return 1;
    }

    fileType inputType;
    fileType outputType;

    switch (argv [1][1]) {
    case 't':
        inputType = TYPE_TIFF;
        break;
    case 'p':
        inputType = TYPE_PNG;
        break;
    default:
        printf("Invalid option.\n");
        return 1;
    }

    switch (argv [1][2]) {
    case 't':
        outputType = TYPE_TIFF;
        break;
    case 'p':
        outputType = TYPE_PNG;
        break;
    default:
        printf("Invalid option.\n");
        return 1;
    }

    char inputFileName[128];
    char outputFileName[128];

    strcpy(inputFileName, argv[2]);
    strcpy(outputFileName, argv[3]);

    return convert(inputFileName, outputFileName, inputType, outputType);
}
