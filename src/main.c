#include <stdio.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "bin_trie.h"

/*
gcc -O0 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment -std=c99 -Wextra -Wall -Wshadow -Wpointer-arith -Wcast-align  bin_trie.c main.c && ./a.out
mips-linux-gnu-gcc -std=c99 -Wall -Wextra -O0 main.c bin_trie.c &&  qemu-mips -L /usr/mips-linux-gnu/ ./a.out
*/

#define EX_NUM 3
#define TEST_NUM 3
#define DEL_NUM 1
int main(void)
{

    char *ex_prefixes[EX_NUM] = {
        "192.168.1.123",
        "192.168.1.123",
        "192.168.1.123",
        // "0.0.0.0",
        // "255.255.255.255",
        // "192.168.1.0",
        // "192.168.1.0",
        // "192.168.1.0",
        // "192.168.2.0",
        // "10.20.0.0",
        // "10.20.0.0",

    };

    uint8_t ex_masks[EX_NUM] = {
        17,
        16,
        18,
        // 0,
        // 0,
        // 25,
        // 24,
        // 16,
        // 24,
        // 16,
        // 24

    };

    char *ex_ips[TEST_NUM] = {
        "192.168.127.255",
        "192.168.128.255",
        "192.168.63.255",

        // "192.168.1.132",
        // "192.168.1.127",
        // "10.20.255.1",
        // "10.20.0.15",
        // "192.167.1.255",
        // "255.255.255.255",

    };

    struct sockaddr_in saddr;

    PHX_BIN_TRIE_NODE trie_root_node = PHX_create_bin_trie_node();
    printf("[!] inserting test data...\n");
    for (u_int32_t i = 0; i < EX_NUM; i++)
    {

        inet_pton(AF_INET, ex_prefixes[i], &saddr.sin_addr);
        uint32_t ip = ntohl(saddr.sin_addr.s_addr);
        printf("[+]adding prefix 0x%x-%s/%d \n", ip, ex_prefixes[i], ex_masks[i]);
        int8_t res = PHX_add(trie_root_node, ip, ex_masks[i]);
        if (-1 == res)
        {
            PHX_abort(
                __FILE__,
                __LINE__,
                "trie insertion error, aborting");
        }
    }
    PHX_dump_trie(trie_root_node);
    printf("\n[!] testing...\n");
    for (int j = 0; j < TEST_NUM; j++)
    {
        inet_pton(AF_INET, ex_ips[j], &saddr.sin_addr);
        uint32_t ip = ntohl(saddr.sin_addr.s_addr);
        int8_t res = PHX_check(trie_root_node, ip);
        printf("[*]ip addr:%s - mask %d\n", ex_ips[j], res);
    }

    uint8_t idx_for_deletion[DEL_NUM] = {2};
    printf("\n[!] deleting...\n");
    for (int k = 0; k < DEL_NUM; k++)
    {

        inet_pton(AF_INET, ex_prefixes[idx_for_deletion[k]], &saddr.sin_addr);
        uint32_t ip = ntohl(saddr.sin_addr.s_addr);

        printf("[+]deleting prefix 0x%x-%s/%d \n",
               ip,
               ex_prefixes[idx_for_deletion[k]],
               ex_masks[idx_for_deletion[k]]);

        int8_t res = PHX_del(trie_root_node, ip, ex_masks[idx_for_deletion[k]]);
        if (-1 == res)
        {
            PHX_abort(
                __FILE__,
                __LINE__,
                "trie deletion error, aborting");
        }
    }
    PHX_dump_trie(trie_root_node);

    printf("\n[!] testing again...\n");
    for (int j = 0; j < TEST_NUM; j++)
    {
        inet_pton(AF_INET, ex_ips[j], &saddr.sin_addr);
        uint32_t ip = ntohl(saddr.sin_addr.s_addr);
        int8_t res = PHX_check(trie_root_node, ip);
        printf("[*]ip addr:%s - mask %d\n", ex_ips[j], res);
    }

    PHX_destroy_bin_trie_node(trie_root_node);
    return 0;
}