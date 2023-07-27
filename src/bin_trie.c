#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <error.h>
// gcc -std=c99 -Wextra -Wall -Wshadow -Wpointer-arith -Wcast-align  bin_trie.c && ./a.out
// gcc -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment -std=c99 -Wextra -Wall -Wshadow -Wpointer-arith -Wcast-align  bin_trie.c && ./a.out

#define ERR_STATUS 1
#define ERR_NUM 0

typedef struct _BIN_TRIE_NODE
{
    bool is_prefix;
    // PHX_BIN_TRIE_NODE children[2];
    struct _BIN_TRIE_NODE *zero_child;
    struct _BIN_TRIE_NODE *one_child;
    // pointer for 0
    // pointer for 1
    // children = dict()
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
    PHX_BIN_TRIE_NODE n = (PHX_BIN_TRIE_NODE)malloc(sizeof(PHX_BIN_TRIE_NODE));
    if (NULL == n)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "allocation error, aborting");
    }
    return n;
}

void PHX_destroy_bin_trie_node(PHX_BIN_TRIE_NODE n)
{
    // TODO - replace with arena allocation - for fast dealloc
    // or implment btrie using an array - cache friendly
    free(n);
}

// int add(unsigned int base, char mask)
// Dodanie prefiksu do zbioru. Zwraca 0 lub -1 dla błędnych argumentów wywołania.
// int del(unsigned int base, char mask)
// Usunięcie prefiksu ze zbioru. Zwraca 0 lub -1 dla błędnych argumentów wywołania.
// char check(unsigned int ip)

uint32_t _PHX_get_ip_net(uint32_t base, uint8_t mask){
    return }

int8_t PHX_add(PHX_BIN_TRIE_NODE trie_root_node, uint32_t base, uint8_t mask)
{

    //     for i, char in enumerate(word):
    //     if char not in current.children:
    //         # prefix = word[0:i+1]
    //         prefix = "dupa"
    //         current.children[char] = TrieNode(prefix)
    //     current = current.children[char]
    // current.is_word = True
    return 0;
}

// int main(int argc, char **argv)
int main(void)
{
    PHX_BIN_TRIE_NODE trie_root_node = PHX_create_bin_trie_node();
    int8_t res = PHX_add(trie_root_node, 10, 1);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie insertion error, aborting");
    }

    PHX_destroy_bin_trie_node(trie_root_node);
    return 0;
}