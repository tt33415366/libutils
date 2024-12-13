#include <stdint.h>

uint32_t checksum_partial (void * buffer, int bytes, uint32_t partial_sum);
uint16_t checksum_fold(uint32_t csum);
