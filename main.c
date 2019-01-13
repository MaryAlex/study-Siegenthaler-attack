#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include "attack.h"

#define GIVEN_N 3
#define SEQUENCE_SIZE 100000

void print_array(uint16_t *array, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main() {
    Generator generator = create_generator(GIVEN_N);
    uint8_t *sequence = generate_sequence(generator, SEQUENCE_SIZE);
    print_array(generator.init_states, GIVEN_N);
    uint16_t *test_sequence = runSiegenthaler(generator.N, SEQUENCE_SIZE, sequence);
    print_array(test_sequence, GIVEN_N);
    free(generator.init_states);
    free(generator.reverse_polynomials);
    free(sequence);
    free(test_sequence);
    return 0;
}