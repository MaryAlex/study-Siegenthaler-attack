#ifndef SIEGENTHALER_ATTACK_GENERATOR_H
#define SIEGENTHALER_ATTACK_GENERATOR_H

#include <stdint.h>

/**
 * @brief Combine generator - cryptographic object which generates some pseudo-random sequence.
 */
struct Generator {
    /**
     * Number of LFSR in generator, where LFSR is linear-feedback shift register.
     */
    uint8_t N;
    /**
     * @brief Reverse polynomials of feedback for LFSR.
     * @size N
     */
    uint16_t *reverse_polynomials;
    /**
     * @brief Initial states for LFSR
     * @size N
     */
    uint16_t *init_states;
};

typedef struct Generator Generator;

/**
 * @param Number of LFSR in generator(Max number is 3 for now)
 * @return initialized combine generator.
 */
Generator create_generator(uint8_t N);

/**
 * Generates pseudo-random sequence with <i>size</i> length with help of <i>generator</i>
 * @param generator object to help generate sequence
 * @param size length of output sequence
 * @return pseudo-random sequence with <i>size</i> length
 */
uint8_t * generate_sequence(Generator generator, uint32_t size);

#endif // SIEGENTHALER_ATTACK_GENERATOR_H

