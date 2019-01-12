#include <stdlib.h>
#include <string.h>
#include "utils.h"

uint16_t * copy_uint16_array(uint16_t *array, uint8_t size) {
    uint16_t *copy = malloc(size * sizeof(uint8_t));
    memcpy(copy, array, size * sizeof(uint8_t));
    return copy;
}