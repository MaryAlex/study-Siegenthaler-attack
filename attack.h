#ifndef SIEGENTHALER_ATTACK_ATTACK_H
#define SIEGENTHALER_ATTACK_ATTACK_H

#include <stdint.h>
#include "generator.h"

/**
 * Run Siegenthaler algorithm
 * @param Number of LFSR in generator, where LFSR is linear-feedback shift register.
 * @param size of the output sequence
 * @param sequence on what algorithm will be written
 * @return calculated by algorithm initialized state of LFSR
 */
uint16_t *runSiegenthaler(uint8_t, uint32_t, uint8_t *);

#endif //SIEGENTHALER_ATTACK_ATTACK_H
