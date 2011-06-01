#include <stdio.h>
#include <unistd.h>

void show_result(uint16_t m, uint16_t k, uint16_t x)
{
    printf("new:\n");
    printf("(m >> 8) & 0xFF = %x\n", (m >> 8));
    printf("(m >> 0) & 0xFF = %x\n", (m >> 0) & 0xFF);
    printf("(k >> 8) & 0xFF = %x\n", (k >> 8));
    printf("(k >> 0) & 0xFF = %x\n", (k >> 0) & 0xFF);
    printf("(x >> 8) & 0xFF = %x\n", (x >> 8));
    printf("(x >> 0) & 0xFF = %x\n", (x >> 0) & 0xFF);
    printf("\n");
}

int main(int argc, char *argv[])
{
    uint8_t body[32];
    uint16_t m = 0; // Staring address.
    uint16_t k = sizeof(body); // Packet size.
    uint16_t x = 10;

    show_result(m, k, x);
    m+= sizeof(body);
    show_result(m, k, x);
    m+= sizeof(body);
    show_result(m, k, x);

    return 0;
}