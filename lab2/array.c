//
// Created by rdelfin on 4/2/17.
//

#include "array.h"

#include <stdlib.h>

array_t* array_init(y86addr_t init_size) {
    array_t* result = malloc(sizeof(array));

    result->length = init_size;
    result->mem = (init_size == 0 ?  NULL : malloc(init_size * sizeof(uint8_t)));

    return result;
}

int array_get(array_t* arr, y86addr_t address, uint8_t* value) {
    if(arr->length <= address)
        return -1;

    *value = arr->mem[address];
    return 0;
}

int array_set(array_t* arr, y86addr_t address, uint8_t value) {
    // If address is outside the bounds of the array, expand the array accordingly
    if(arr->length <= address) {
        y86addr_t old_len = arr->length;
        if(old_len == 0)
            arr->mem = malloc(address+1);
        else
            arr->mem = realloc(address+1);

        if(!arr->mem)
            return -1;

        // Only write up to address, and write said byte outside this if
        for(int i = old_len; i < address; i++)
            arr->mem[i] = 0;
    }

    arr->mem[address] = value;
    return 0;
}

void array_destroy(array_t* arr) {
    if(arr->mem)
        free(arr->mem);

    free(arr);
}

