#include "attack.h"
#include "utils.h"
#include "generator.h"

#define MINUS_ONE (-1)
#define SIGNIFICANCE_LEVEL 0.05

const float MAGIC_NUMBERS[] = {-0.1, 0.5, 0.5};
const float PROBABILITIES[] = {0.5, 0.75, 0.75};

uint8_t *createAndWorkTestR(uint16_t, uint32_t, uint8_t);

uint16_t *runSiegenthaler(uint8_t N, uint32_t size, uint8_t *sequence) {
    uint8_t *test_sequence = NULL;
    double *prior_probabilities = malloc(N * sizeof(double));
    uint16_t *result = malloc((N) * sizeof(uint16_t));
    uint32_t n = size / 2;

    for (uint8_t j = 0; j < N; ++j) {			
        prior_probabilities[j] = PROBABILITIES[j];

        for (uint16_t d = 1; d < UINT16_MAX; ++d) {	            
            if (test_sequence) {
                free(test_sequence);
            }
            double cross_correlation_fun = 0;
            uint32_t tmp = 0;

            test_sequence = createAndWorkTestR(d, size, j);

			for (uint32_t i = 0; i < n; ++i) {
                cross_correlation_fun += (pow(MINUS_ONE,sequence[i])) * (pow(MINUS_ONE, test_sequence[i]));				
            }
            cross_correlation_fun /= n;

            if (cross_correlation_fun < MAGIC_NUMBERS[j]) {
                continue;
            }

			for (uint32_t i = n; i <= size; ++i) {
				if (sequence[i] == test_sequence[i]) {
					tmp++;
				}
			}
            double posteriori_probabilities = (tmp * 1.0) / n;

            if (posteriori_probabilities > prior_probabilities[j] - SIGNIFICANCE_LEVEL &&
                posteriori_probabilities < prior_probabilities[j] + SIGNIFICANCE_LEVEL) {				
                result[j] = d;               
                break;
            }            
        }
    }

    free(prior_probabilities);

    return result;
}

uint8_t *createAndWorkTestR(uint16_t iteration, uint32_t size, uint8_t init_sequence_index) {	
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