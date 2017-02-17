#include <stdlib.h>
#include <stdint.h>

#ifndef HW4_BITSMATRIX_H
#define HW4_BITSMATRIX_H

typedef struct {
    size_t rows, cols;
    size_t dataLen;
    size_t rowBytes;
    uint8_t* data;
} bitmatrix_t;

bitmatrix_t* bitmatrix_init(size_t r, size_t c);
bitmatrix_t* bitmatrix_init_cpy(bitmatrix_t* src);

char bitmatrix_get(bitmatrix_t*, size_t i, size_t j);
void bitmatrix_set(bitmatrix_t*, size_t i, size_t j, uint8_t val);

void bitmatrix_getrow(bitmatrix_t*, uint8_t* data, size_t i);
void bitmatrix_setrow(bitmatrix_t*, uint8_t* data, size_t i);

void bitmatrix_print(bitmatrix_t*);

void bitmatrix_destroy(bitmatrix_t*);

#endif //HW4_BITS_H
