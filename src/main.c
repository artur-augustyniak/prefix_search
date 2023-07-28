#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

uint32_t _PHX_get_ip_net(uint32_t base, uint8_t mask)
{   

    
    // printf("addr 0x%x\n", base);
    // printf("mask 0x%x\n", ~((1 << (32 - mask)) - 1));
    // u_int32_t r = base & ~((1 << (32 - mask)) - 1);
    // printf("base 0x%x\n", r);
    return  base &= ~((1 << (32 - mask)) - 1);
    // return r;
}


void printBits(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
    {
        for (j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}



int main(void)
{
    struct sockaddr_in antelope;
    char *some_addr;

    inet_aton("192.168.1.123", &antelope.sin_addr); // store IP in antelope
    printf("%u\n", ntohl(antelope.sin_addr.s_addr));
    u_int8_t mask = 9;
    uint32_t base = antelope.sin_addr.s_addr;
    base = base >> (32 - mask);
    printBits(sizeof(uint32_t), &base);
    uint32_t prefix;
    prefix = base;
    int what_bit_i_am_testing = 0;
    while (what_bit_i_am_testing < mask)
    {
        if (prefix & 0x01)
        {
            printf("bit %d is 1\n", what_bit_i_am_testing);
        }
        else
        {
            printf("bit %d is 0\n", what_bit_i_am_testing);
        }

        what_bit_i_am_testing++;
        prefix = prefix >> 1;
    }


    // u_int32_t masked = _PHX_get_ip_net(ntohl(antelope.sin_addr.s_addr), 9);
    // printf("masked %x\n", masked);

    // antelope.sin_addr.s_addr = htonl(masked);
    // some_addr = inet_ntoa(antelope.sin_addr); // return the IP

    // // printf("%s\n",inet_ntoa(*(struct in_addr *)ip));


    // some_addr = inet_ntoa(antelope.sin_addr); // return the IP
    // printf("%s\n", some_addr);                // prints "10.0.0.1"

    // // and this call is the same as the inet_aton() call, above:
    // antelope.sin_addr.s_addr = inet_addr("10.0.0.1");

    return 0;
}