#include <stdio.h>
#include "generator.h"
#include "attack.h"

#define GIVEN_N 3
#define SEQUENCE_SIZE 4

void print_array(uint16_t *array, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main() {
    Generator generator = create_generator(GIVEN_N);
    uint8_t *sequence = generate_sequence(generator, SEQUENCE_SIZE);
    print_array(generator.init_states, SEQUENCE_SIZE);
    uint16_t *test_sequence = runSiegenthaler(generator.N, SEQUENCE_SIZE, sequence);
    print_array(test_sequence, SEQUENCE_SIZE);
    return 0;
}