#ifndef PHX_BIN_TRIE_H
#define PHX_BIN_TRIE_H

#include <stdbool.h>
#include <error.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

typedef struct _BIN_TRIE_NODE
{
    bool is_prefix;
    struct _BIN_TRIE_NODE *children[2];
} PHX_BIN_TRIE_NODE_STRUCT, *PHX_BIN_TRIE_NODE;

void PHX_abort(const char *file, unsigned int line, const char *msg);

PHX_BIN_TRIE_NODE PHX_create_bin_trie_node();

void PHX_destroy_bin_trie_node(PHX_BIN_TRIE_NODE n);

int8_t PHX_add(PHX_BIN_TRIE_NODE trie_root_node, uint32_t base, uint8_t mask);

int8_t PHX_check(PHX_BIN_TRIE_NODE trie_root_node, uint32_t ip);

int8_t PHX_del(PHX_BIN_TRIE_NODE trie_node, uint32_t base, uint8_t mask);

void PHX_dump_trie(PHX_BIN_TRIE_NODE trie_node);

#endif