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

/**
 * Initializes the bit-matrix with a given size (rows and columns), and sets all values to 0
 * @param r The number of rows in the matrix
 * @param c The number of columns in the matrix
 * @return A pointer on the heap to the bitmatrix_t struct
 */
bitmatrix_t* bitmatrix_init(size_t r, size_t c);

/**
 * Initialized a new bit-matrix by copying another bitmatrix_t object.
 * @param src The matrix to copy
 * @return A pointer on the heap to the bitmatrix_t struct
 */
bitmatrix_t* bitmatrix_init_cpy(bitmatrix_t* src);

/**
 * Get a given bit on the bit matrix
 * @param mat Pointer to the bit matrix struct
 * @param i Row of bit
 * @param j Column of bit
 * @return The value of said bit, either 1 or 0
 */
char bitmatrix_get(bitmatrix_t* mat, size_t i, size_t j);

/**
 * Sets the value of a given bit on the bit matrix
 * @param mat Pointer to the bit matrix struct
 * @param i Row of the bit
 * @param j Column of the bit
 * @param val value to set it to. Anything other than 0 is considered a 1
 */
void bitmatrix_set(bitmatrix_t* mat, size_t i, size_t j, uint8_t val);

/**
 * Fetches an entire row of the bit matrix and stores it on a memory space provided by the user.
 * @param mat Pointer to the bit matrix struct
 * @param data Pointer to memory where row should be copied. It must be at least mat->rowBytes bytes long
 * @param i The index of the row being copied
 */
void bitmatrix_getrow(bitmatrix_t* mat, uint8_t* data, size_t i);

/**
 * Sets the value of an entire row to a value stored in a location in memory provided by the user.
 * @param mat Pointer to the bit matrix struct
 * @param data Pointer to memory where row should be copied from. It must be at least mat->rowBytes bytes long
 * @param i The index of the row being copied
 */
void bitmatrix_setrow(bitmatrix_t* mat, uint8_t* data, size_t i);

/**
 * Performs an xor operation between all rows of the matrix.
 * @param mat Pointer to the bit matrix struct
 * @param data Where to copy the result to. It must be at least mat->rowBytes bytes long.
 */
void bitmatrix_xorrows(bitmatrix_t* mat, uint8_t* data);

/**
 * Prints out the entire bit matrix to the terminal. For debugging purposes
 * @param mat Pointer to the bit matrix struct
 */
void bitmatrix_print(bitmatrix_t* mat);

/**
 * Frees all the memory being used by the bit matrix struct, including the struct itself.
 * @param mat Pointer to the bit matrix struct.
 */
void bitmatrix_destroy(bitmatrix_t* mat);

#endif //HW4_BITS_H
