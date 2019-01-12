#include <stdio.h>
#include "Generator.h"

#define N 3
#define SEQUENCE_SIZE 16

void print_array(uint8_t *array, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d", array[i]);
    }
}

int main() {
    Generator generator = create_generator(N);
    uint8_t *sequence = generate_sequence(generator, SEQUENCE_SIZE);
    print_array(sequence, SEQUENCE_SIZE);
    return 0;
}