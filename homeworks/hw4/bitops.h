//
// Created by rdelfin on 2/17/17.
//

#include <stdint.h>

#ifndef HW4_BITS_H
#define HW4_BITS_H

/**
 * This function extends the lowest bit to cover the entire number. Useful for masks
 * The lower bit-extension is performed by or-ing the first digit with a shifted version, then shifting that by twice
 * as much and xoring back, and doing it one last time.
 */
static inline uint8_t lowbitextend_b(uint8_t n) {
    n = n & (uint8_t)1;
    uint8_t result = n | (n << 1);
    result |= (result << 2);
    result |= (result << 4);

    return result;
}

static inline uint8_t get_bit_b(uint8_t n, uint8_t i) {
    return !!(n & ((uint8_t)1<< i));
}

#endif //HW4_BITS_H
