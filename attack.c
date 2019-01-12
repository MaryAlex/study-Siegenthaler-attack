#include <stdlib.h>
#include <stdbool.h>
#include "attack.h"
#include "utils.h"

#define MINUS_ONE (-1)
#define MAGIC_NUMBER 0.4 // Look at readme.
#define SIGNIFICANCE_LEVEL 0.05

const float PROBABILITIES[] = {0.5, 0.75, 0.75};

uint8_t *createAndWorkTestR(uint16_t, uint32_t, uint8_t);

uint16_t *runSiegenthaler(uint8_t N, uint32_t size, uint8_t *sequence) {
    uint8_t *test_sequence;
    double cross_correlation_fun = 0;
    double posteriori_probabilities = 0;
    float *prior_probabilities = malloc(N * sizeof(float));
    uint16_t *result = malloc((N) * sizeof(uint16_t));
    uint32_t n = size / 2;
    for (uint8_t i = 0; i < N; ++i) {
        bool isToOuterFor = false;
        prior_probabilities[i] = PROBABILITIES[i];
        for (uint16_t j = 0; j <= UINT16_MAX; ++j) {
            if (isToOuterFor) {
                break;
            }
            cross_correlation_fun = 0;
            uint32_t tmp = 0;
            test_sequence = createAndWorkTestR(j, size, i);
            for (int k = 0; k < n; ++k) {
                cross_correlation_fun += (MINUS_ONE ^ sequence[k]) * (MINUS_ONE ^ test_sequence[k + j]);
            }
            cross_correlation_fun /= n;
            if (cross_correlation_fun < MAGIC_NUMBER) {
                continue;
            }
            for (int l = n; l <= size; ++l) {
                if (sequence[l] == test_sequence[l]) {
                    tmp++;
                }
                posteriori_probabilities = (tmp * 1.0) / n;
                if (posteriori_probabilities > prior_probabilities[i] - SIGNIFICANCE_LEVEL &&
                    posteriori_probabilities < prior_probabilities[i] + SIGNIFICANCE_LEVEL) {
                    result[i] = j;
                    isToOuterFor = true;
                    break;
                }
            }
        }
    }
    return result;
}

uint8_t *createAndWorkTestR(uint16_t iteration, uint32_t size, uint8_t init_sequence_index) {
    uint16_t reverse_polynomial = POLYNOMIALS[init_sequence_index];
    uint16_t initial_state = iteration;
    uint8_t *test_sequence = malloc((iteration + size) * sizeof(uint8_t));
    for (int i = 0; i < iteration + size; ++i) {
        if (initial_state & ONE) {
            initial_state = (initial_state ^ reverse_polynomial >> ONE) | (INT16_MAX + ONE);
            test_sequence[i] = ONE;
        } else {
            initial_state = initial_state >> ONE;
            test_sequence[i] = ZERO;
        }
    }
    return test_sequence;
}