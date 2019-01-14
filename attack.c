#include <stdlib.h>
#include <math.h>
#include "attack.h"
#include "utils.h"

#define MINUS_ONE (-1)

// TODO: Check if 0.5 and round to decimal
const float MAGIC_NUMBERS[] = {0.95, 0.4, 0.4}; // see Readme for more information.

/**
 * Generates sequence of length <i>size</i> with polynomial with index <i>init_sequence_index</i>
 * and with initial state as <i>iteration</i>.
 * @return sequence from zero and ones with length <i>size</i>
 */
uint8_t *createTestSequenceLFSR(uint16_t, uint32_t, uint8_t);


/**
 * TODO
 * @return sequence from zero and ones with length <i>size</i>
 */
 // TODO: remove generator from there
uint8_t *createTestSequenceGenerator(uint16_t, uint32_t, Generator, __uint16_t *);

uint16_t *runSiegenthaler(Generator generator, uint32_t size, uint8_t *sequence) {
    uint8_t N = generator.N;
    uint8_t *test_sequence = NULL;
    uint16_t *result = malloc((N) * sizeof(uint16_t));
    uint32_t n = size / 2;

    for (uint8_t j = 1; j < N; ++j) {
        for (uint16_t d = 1; d < UINT16_MAX; ++d) {
            if (test_sequence) {
                free(test_sequence);
            }
            double cross_correlation_fun = 0;

            test_sequence = createTestSequenceLFSR(d, size, j);

            for (uint32_t i = 0; i < n; ++i) {
                // TODO: Optimize pow
                cross_correlation_fun += (pow(MINUS_ONE, sequence[i])) * (pow(MINUS_ONE, test_sequence[i]));
            }
            cross_correlation_fun /= n;

            if (cross_correlation_fun > MAGIC_NUMBERS[j]) {
                result[j] = d;
                break;
            }
        }
    }

    // TODO: duplication. Must be in some function.
    for (uint16_t d = 1; d < UINT16_MAX; ++d) {
        if (test_sequence) {
            free(test_sequence);
        }
        double cross_correlation_fun = 0;

        test_sequence = createTestSequenceGenerator(d, size, generator, result);

        for (uint32_t i = 0; i < size; ++i) {
            cross_correlation_fun += (pow(MINUS_ONE, sequence[i])) * (pow(MINUS_ONE, test_sequence[i]));
        }
        cross_correlation_fun /= size;

        if (cross_correlation_fun > MAGIC_NUMBERS[0]) {
            result[0] = d;
            break;
        }
    }
    return result;
}

uint8_t *createTestSequenceLFSR(uint16_t iteration, uint32_t size, uint8_t init_sequence_index) {
    uint16_t reverse_polynomial = POLYNOMIALS[init_sequence_index];
    uint16_t initial_state = iteration;
    uint8_t *test_sequence = malloc((size) * sizeof(uint8_t));

    for (uint32_t i = 0; i < size; ++i) {
        if (initial_state & ONE) {
            initial_state = ((initial_state ^ reverse_polynomial) >> ONE) | (INT16_MAX + ONE);
            test_sequence[i] = ONE;
        } else {
            initial_state = initial_state >> ONE;
            test_sequence[i] = ZERO;
        }
    }
    return test_sequence;
}

uint8_t *createTestSequenceGenerator(uint16_t iteration, uint32_t size, Generator generator, u_int16_t *founded) {
    // TODO: refact this copy-past as inline function maybe
    uint8_t N = generator.N;
    uint16_t *reverse_polynomials = copy_uint16_array(generator.reverse_polynomials, N);
    uint16_t *init_states = copy_uint16_array(founded, N);
    uint8_t *tmp = malloc(N * sizeof(uint8_t));
    uint8_t *result = malloc(size * sizeof(uint8_t));
    init_states[0] = iteration;
    for (uint32_t i = 0; i < size; ++i) {
        for (uint8_t j = 0; j < N; ++j) {
            if (init_states[j] & ONE) {
                init_states[j] = ((init_states[j] ^ reverse_polynomials[j]) >> ONE) | (INT16_MAX + ONE);
                tmp[j] = ONE;
            } else {
                init_states[j] = init_states[j] >> ONE;
                tmp[j] = ZERO;
            }
        }
        result[i] = (tmp[0] * tmp[1]) ^ ((tmp[0] ^ 1) * tmp[2]);
    }
    return result;
}
