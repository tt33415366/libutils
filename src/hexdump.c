#include "hexdump.h"

// Function to print data in hexadecimal and ASCII format (hexdump)
void hexdump(const void *data, size_t size, size_t line_size) {
        const unsigned char *ptr = (const unsigned char *)data;
        size_t i, j;

        for (i = 0; i < size; i += line_size) {
                // Print the offset in hexadecimal
                printf("%08lx: ", (unsigned long)i);

                // Print the hexadecimal values of the bytes
                for (j = 0; j < line_size; j++) {
                        if (i + j < size) {
                                printf("%02x ", ptr[i + j]);
                        } else {
                                printf("   ");
                        }
                }

                // Print a separator
                printf(" ");

                // Print the ASCII values of the bytes
                for (j = 0; j < line_size; j++) {
                        if (i + j < size) {
                                unsigned char c = ptr[i + j];
                                if (c >= 32 && c <= 126) {
                                        printf("%c", c);
                                } else {
                                        printf(".");
                                }
                        } else {
                                printf(" ");
                        }
                }

                // Move to the next line
                printf("\n");
        }
}
