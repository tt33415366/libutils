#include "checksum.h"

uint32_t checksum_partial (void * buffer, int bytes, uint32_t partial_sum)
{
    uint32_t   total;
    uint16_t * ptr;
    int        words;

    total = partial_sum;
    ptr   = (uint16_t *) buffer;
    words = (bytes + 1) / 2; // +1 & truncation on / handles any odd byte at end

    /*
     **   As we're using a 32 bit int to calculate 16 bit checksum
     **   we can accumulate carries in top half of DWORD and fold them in later
     **/
    while (words--) total += *ptr++;

    return total;
}

uint16_t checksum_fold(uint32_t csum)
{
    uint32_t total = csum;

    /*
     **   Fold in any carries
     **   - the addition may cause another carry so we loop
     **/
    while (total & 0xffff0000) total = (total >> 16) + (total & 0xffff);

    return (uint16_t) ~total;
}
