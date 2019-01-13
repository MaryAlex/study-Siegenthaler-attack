#include "generator.h"
#include "utils.h"

uint16_t POLYNOMIALS[] = {0x8016, 0x801C, 0x801F};

/**
 * Initialize generator's reverse_polynomials from POLYNOMIALS constant
 * and generates random values for init_states.
 * @param generator object which reverse_polynomials and init_states fields changed.
 */
void init_generators_arrays(Generator *generator);

Generator create_generator(uint8_t N) {
    Generator generator = {
            .N = N,
            .reverse_polynomials = malloc(N * sizeof(uint16_t)),
            .init_states = malloc(N * sizeof(uint16_t))
    };

    init_generators_arrays(&generator);

    return generator;
}

uint8_t *generate_sequence(Generator generator, uint32_t size) {
    uint8_t N = generator.N;
    uint16_t *reverse_polynomials = copy_uint16_array(generator.reverse_polynomials, N);
    uint16_t *init_states = copy_uint16_array(generator.init_states, N);
    uint8_t *tmp = malloc(N * sizeof(uint8_t));
    uint8_t *result = calloc(size, sizeof(uint8_t));

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

    free(reverse_polynomials);
    free(init_states);
    free(tmp);

    return result;
}

// Private functions

void init_generators_arrays(Generator *generator) {
    uint8_t N = generator->N;
    uint16_t *reverse_polynomials = generator->reverse_polynomials;
    uint16_t *init_states = generator->init_states;
    
	srand((unsigned int) time(NULL));  // NOLINT

    for (uint8_t i = 0; i < N; ++i) {
        reverse_polynomials[i] = POLYNOMIALS[i];
        init_states[i] = (uint16_t) (rand() %  UINT16_MAX);
    }	
}