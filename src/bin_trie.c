#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <error.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// gcc -std=c99 -Wextra -Wall -Wshadow -Wpointer-arith -Wcast-align  bin_trie.c && ./a.out
// gcc -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment -std=c99 -Wextra -Wall -Wshadow -Wpointer-arith -Wcast-align  bin_trie.c && ./a.out
// mips-linux-gnu-gcc bin_trie.c
// ┌──(artur㉿kokopyc)-[~/Desktop/prefix_search/src]
// └─$ qemu-mips -L /usr/mips-linux-gnu/ ./a.out

#define ERR_STATUS 1
#define ERR_NUM 0

typedef struct _BIN_TRIE_NODE
{
    bool is_prefix;
    struct _BIN_TRIE_NODE *children[2];
} PHX_BIN_TRIE_NODE_STRUCT, *PHX_BIN_TRIE_NODE;

void PHX_abort(const char *file, unsigned int line, const char *msg)
{
    error(ERR_STATUS, ERR_NUM, "file: %s:%d - %s", file, line, msg);
    abort();
}

// PHX_BIN_TRIE_NODE PHX_init_bin_trie_node();

PHX_BIN_TRIE_NODE PHX_create_bin_trie_node()
{
    // TODO - replace with arena allocation - for fast dealloc
    // or implment btrie using an array - cache friendly
    PHX_BIN_TRIE_NODE n = (PHX_BIN_TRIE_NODE)malloc(sizeof(PHX_BIN_TRIE_NODE_STRUCT));
    if (NULL == n)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "allocation error, aborting");
    }
    n->is_prefix = false;
    n->children[0] = NULL;
    n->children[1] = NULL;
    return n;
}

void PHX_destroy_bin_trie_node(PHX_BIN_TRIE_NODE n)
{
    // TODO - replace with arena allocation - for fast dealloc
    // or implment btrie using an array - cache friendly
    if (NULL != n->children[0])
    {
        PHX_destroy_bin_trie_node(n->children[0]);
    }
    if (NULL != n->children[1])
    {
        PHX_destroy_bin_trie_node(n->children[1]);
    }
    free(n);
}

uint32_t _PHX_get_ip_net(uint32_t base, uint8_t mask)
{
    return base &= ~((1 << (32 - mask)) - 1);
}

void dump(PHX_BIN_TRIE_NODE trie_node)
{
    printf("%p is prefix? %d - children [0]%p [1]%p\n",
           trie_node,
           trie_node->is_prefix,
           trie_node->children[0],
           trie_node->children[1]);
    if (NULL != trie_node->children[0])
    {
        dump(trie_node->children[0]);
    }

    if (NULL != trie_node->children[1])
    {
        dump(trie_node->children[1]);
    }
}

/*
 * assuming user used ntohl
 */
int8_t PHX_add(PHX_BIN_TRIE_NODE trie_root_node, uint32_t base, uint8_t mask)
{
    struct sockaddr_in antelope;
    char *some_addr;
    uint32_t prefix = _PHX_get_ip_net(base, mask);
    printf("prefix %u\n", prefix);
    printf("prefix 0x%x\n", prefix);

    antelope.sin_addr.s_addr = htonl(prefix);
    some_addr = inet_ntoa(antelope.sin_addr); // return the IP
    printf("%s\n", some_addr);                // prints "10.0.0.1"

    uint8_t prefix_bit = 31;

    while (prefix_bit > 31 - mask)
    {
        uint8_t curr_b = (prefix >> prefix_bit) & 1;
        if (NULL == trie_root_node->children[curr_b])
        {
            trie_root_node->children[curr_b] = PHX_create_bin_trie_node();
        }
        trie_root_node = trie_root_node->children[curr_b];
        // printf("msb %d %d\n", prefix_bit, (prefix >> prefix_bit) & 1);
        prefix_bit--;
    }
    trie_root_node->is_prefix = true;

    return 0;
}

// int main(int argc, char **argv)
int main(void)
{
    PHX_BIN_TRIE_NODE trie_root_node = PHX_create_bin_trie_node();

    struct sockaddr_in antelope;
    inet_pton(AF_INET, "192.168.1.123", &antelope.sin_addr);
    // uint32_t ip_int = htonl(antelope.sin_addr.s_addr);
    uint32_t ip_int = ntohl(antelope.sin_addr.s_addr);
    printf("ip uint %u\n", ip_int);
    printf("ip uint 0x%x\n", ip_int);

    int8_t res = PHX_add(trie_root_node, ip_int, 24);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie insertion error, aborting");
    }
    dump(trie_root_node);
    PHX_destroy_bin_trie_node(trie_root_node);
    return 0;
}