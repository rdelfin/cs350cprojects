//
// Created by rdelfin on 4/2/17.
//

#ifndef LAB2_MAP_H
#define LAB2_MAP_H

#include "defines.h"

typedef struct {
    uint32_t length;
    uint8_t* mem;
} array_t;

/**
 * Create a new, empty, array object from the heap
 * @param init_size The initial size of the array. Can be zero
 * @return Said array object, or NULL if allocation was unsuccessful
 */
array_t* array_init(y86addr_t init_size);

/**
 * Get the value of the array at a given address
 * @param arr The array to read from
 * @param address The address to read at
 * @param value The uint8_t to write the result to
 * @return 0 if operation succeeded, -1 if index out of bounds
 */
int array_get(array_t* arr, y86addr_t address, uint8_t* value);

/**
 * Set the value of the array at a given address. Expands the array if necessary and sets all new values to zero.
 * @param arr Array to write to
 * @param address Aadress to write at
 * @param value Value to write
 * @return 0 if operation succeeded, -1 if there was an allocation error
 */
int array_set(array_t* arr, y86addr_t address, uint8_t value);

/**
 * Free all the memory used by a given array
 * @param arr The array to free
 */
void array_destroy(array_t* arr);


#endif //LAB2_MAP_H
