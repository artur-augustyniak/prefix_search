#ifndef PHX_BIN_TRIE_H
#define PHX_BIN_TRIE_H

typedef struct _BIN_TRIE_NODE
{
    bool is_prefix;
    PHX_BIN_TRIE_NODE children[2];
    // pointer for 0
    // pointer for 1
    // children = dict()
} PHX_BIN_TRIE_NODE_STRUCT, *PHX_BIN_TRIE_NODE;

#endif