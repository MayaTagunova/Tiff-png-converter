#include <stdio.h>
#include <stdint.h>

#ifndef TIFFPART_H
#define TIFFPART_H

#endif // TIFFPART_H

int tiffGetDataSize(const char *fileName, size_t *size);
int tiffReadData(const char *fileName, uint32_t *buffer, size_t *size);
int tiffWriteData(const char *fileName, uint32_t *buffer, size_t *size);
