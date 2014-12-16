#include <stdio.h>
#include <stdint.h>

#ifndef PNGPART_H
#define PNGPART_H

#endif // PNGPART_H

int pngGetDataSize(const char *fileName, size_t *size);
int pngReadData(const char *fileName, uint32_t *buffer, size_t *size);
int pngWriteData(const char *fileName, uint32_t *buffer, size_t *size);
