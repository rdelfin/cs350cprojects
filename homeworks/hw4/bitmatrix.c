#include "bitmatrix.h"

static inline int get_byte_position(int i, int j, int len) {
    return (i*len + j) / (sizeof(uint8_t)*8);
}

static inline int get_bit_offset(int i, int j, int len) {
    return (i*len + j) % sizeof(uint8_t)*8;


void bitmatrix_init(bitmatrix_t* mat, size_t len) {
    mat.len = len;
    if(len != 0) {
        size_t elems = len*len;
        size_t bitsPerElem = sizeof(uint8_t)*8;
        mat.dataLen = 1 + ((len*len - 1) / bitsPerElem);
        mat = malloc(mat.dataLen * sizeof(uint8_t));

        for(int i = 0; i < mat.dataLen; i++) {
            data[i] = 0;
        }
}

char bitmatrix_get(bitmatrix_t* mat, size_t i, size_t j) {
    // Variables for positioning and data byte
    int b = get_byte_position(i, j, mat.len);
    int off = get_bit_offset(i, j, mat.len);
    uint8_t byte = mat.data[b];

    // Bitshift magic: 1 << off gets a 1 only at bit offset position, byte & .. gets only said bit, !! converts any number != 0 to 1.
    return !!(byte & (1 << off));
}

void bitmatrix_set(bitmatrix_t* mat, size_t i, size_t j, char val) {
    // set all but first bit to 0
    val = val & 1;

    // Variables for positioning and data byte
    int b = get_byte_position(i, j, mat.len);
    int off = get_bit_offset(i, j, mat.len);
    uint8_t byte = mat.data[b];
    
    // Mask: 1 only at bit being written
    uint8_t mask = 1 << off;
    // allval: every byte takes on the value of val's first byte
    uint8_t allval = val &  (val << 1) & (val << 2) & (val << 3) & (val << 4) & (val << 5) & (val << 6) & (val << 7);
    
    /* byte & !mask: set bit at offset to 0
     * mask & allval: set bit at offset to val, and set all other bits to 0
     * (byte & !mask) | (mask & allval): at offset bit, operation boils down to 0 | val. All other bits from original byte remain the same.
     */
    mat.data[b] = (byte & !mask) | (mask & allval);
}

void bitmatrix_destroy(bitmatrix_t* mat) {
    free(data);
}
