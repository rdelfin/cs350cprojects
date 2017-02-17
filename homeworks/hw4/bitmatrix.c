#include "bitmatrix.h"

#include <stdio.h>
#include <string.h>
#include "bitops.h"

/**
 * Facilitates matrix offset calculations. Gets the index of the byte in the data
 * array where where a given bit index can be found.
 */
static inline int get_byte_position(size_t i, size_t j, size_t len) {
    return (int) ((i * len + j) / (sizeof(uint8_t) * 8));
}

/**
 * Facilitates matrix offset calculations. Gets the index within a byte where a given
 * bit can be found.
 */
static inline int get_bit_offset(size_t i, size_t j, size_t len) {
    return (int) ((i * len + j) % (sizeof(uint8_t) * 8));
}


bitmatrix_t* bitmatrix_init(size_t r, size_t c) {
    bitmatrix_t* mat = malloc(sizeof(bitmatrix_t));         // All memory is allocated on the heap

    // Initialize all default values
    mat->rows = r;
    mat->cols = c;
    mat->dataLen = 0;
    if(r != 0 && c != 0) {
        size_t bitsPerElem = sizeof(uint8_t)*8;
        mat->rowBytes = 1 + ((mat->cols - 1) / bitsPerElem);
        mat->dataLen = mat->rowBytes * mat->rows;
        mat->data = malloc(mat->dataLen * sizeof(uint8_t));

        // Initialize all data to 0
        for(int i = 0; i < mat->dataLen; i++) {
            mat->data[i] = 0;
        }
    }

    return mat;
}

bitmatrix_t* bitmatrix_init_cpy(bitmatrix_t* src) {
    bitmatrix_t* mat = bitmatrix_init(src->rows, src->cols);
    // Just copy the data over using memcpy
    memcpy(mat->data, src->data, mat->dataLen);

    return mat;
}

char bitmatrix_get(bitmatrix_t* mat, size_t i, size_t j) {
    // Variables for positioning and data byte
    int b = get_byte_position(i, j, mat->rowBytes*8);
    int off = get_bit_offset(i, j, mat->rowBytes*8);
    uint8_t byte = mat->data[b];

    // Bitshift magic: 1 << off gets a 1 only at bit offset position, byte & .. gets only said bit, !! converts any number != 0 to 1.
    return !!(byte & (1 << off));
}

void bitmatrix_set(bitmatrix_t* mat, size_t i, size_t j, uint8_t val) {
    // set all but first bit to 0
    val = val & (uint8_t)1;

    // Variables for positioning and data byte
    int b = get_byte_position(i, j, mat->rowBytes*8);
    int off = get_bit_offset(i, j, mat->rowBytes*8);
    uint8_t byte = mat->data[b];
    
    // Mask: 1 only at bit being written
    uint8_t mask = (uint8_t)1 << off;
    // allval: every byte takes on the value of val's first byte
    uint8_t allval = lowbitextend_b(val);
    
    /* byte & ~mask: set bit at offset to 0
     * mask & allval: set bit at offset to val, and set all other bits to 0
     * (byte & ~mask) | (mask & allval): at offset bit, operation boils down to 0 | val. All other bits from original byte remain the same.
     */
    mat->data[b] = (byte & ~mask) | (mask & allval);
}

void bitmatrix_getrow(bitmatrix_t* mat, uint8_t* data, size_t i) {
    // This result will probably contained unused bits (since each row must start at it's own byte)
    uint8_t* rowStart = &mat->data[i * mat->rowBytes];
    memcpy(data, rowStart, mat->rowBytes);
}

void bitmatrix_setrow(bitmatrix_t* mat, uint8_t* data, size_t i) {
    // This result will probably set unused bits (since each row must start at it's own byte)
    uint8_t* rowStart = &mat->data[i * mat->rowBytes];
    memcpy(rowStart, data, mat->rowBytes);
}

void bitmatrix_xorrows(bitmatrix_t* mat, uint8_t* data) {
    uint8_t rowBuf[mat->rowBytes];

    // Since xor has no idempotent value, write first row to data
    bitmatrix_getrow(mat, data, 0);

    // Actual calculation. Get each row and xor with
    for(size_t i = 0; i < mat->rows; i++) {
        // Get the row to fetch
        bitmatrix_getrow(mat, rowBuf, i);

        // Calculate xor
        for(size_t j = 0; j < mat->rowBytes; j++) {
            data[j] = data[j] ^ rowBuf[j];
        }
    }
}

void bitmatrix_print(bitmatrix_t* mat) {
   for(int i = 0; i < mat->rows; i++) {
       for(int j = 0; j < mat->cols; j++)
           printf("%i ", bitmatrix_get(mat, i, j));

       printf("\n");
   }
}

void bitmatrix_destroy(bitmatrix_t* mat) {
    free(mat->data);
    free(mat);
}
