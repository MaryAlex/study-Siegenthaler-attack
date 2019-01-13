#include "utils.h"

uint16_t * copy_uint16_array(uint16_t *array, uint8_t size) {
    uint16_t *copy = malloc(size * sizeof(uint16_t));
    memcpy(copy, array, size * sizeof(uint16_t));
    return copy;
}