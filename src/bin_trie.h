#ifndef PHX_BIN_TRIE_H
#define PHX_BIN_TRIE_H

#include <stdbool.h>
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

void PHX_destroy_bin_trie(PHX_BIN_TRIE_NODE trie_node);

void PHX_dump_trie(PHX_BIN_TRIE_NODE trie_node);

int add(PHX_BIN_TRIE_NODE trie_node, int base, int mask);

int check(PHX_BIN_TRIE_NODE trie_node, int ip);

int del(PHX_BIN_TRIE_NODE trie_node, int base, int mask);

#endif