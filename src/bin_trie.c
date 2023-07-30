#include "bin_trie.h"

#define IP_V4_MSB_IDX 31
#define IP_V4_BIT_LEN 32

void PHX_abort(const char *file, unsigned int line, const char *msg)
{
    char str[80];
    snprintf(str, 80, "file: %s:%d - %s", file, line, msg);
    perror(msg);
    abort();
}

PHX_BIN_TRIE_NODE PHX_create_bin_trie_node()
{

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

void PHX_destroy_bin_trie(PHX_BIN_TRIE_NODE trie_node)
{
    if (NULL != trie_node->children[0])
    {
        PHX_destroy_bin_trie(trie_node->children[0]);
    }
    if (NULL != trie_node->children[1])
    {
        PHX_destroy_bin_trie(trie_node->children[1]);
    }
    free(trie_node);
}

void PHX_dump_trie(PHX_BIN_TRIE_NODE trie_node)
{
    printf("%p is prefix? %d - children [0]%p [1]%p\n",
           trie_node,
           trie_node->is_prefix,
           trie_node->children[0],
           trie_node->children[1]);
    if (NULL != trie_node->children[0])
    {
        PHX_dump_trie(trie_node->children[0]);
    }

    if (NULL != trie_node->children[1])
    {
        PHX_dump_trie(trie_node->children[1]);
    }
}

PHX_BIN_TRIE_NODE _PHX_find_prefix(PHX_BIN_TRIE_NODE trie_root_node, uint32_t base, int8_t mask)
{
    int bit_idx = IP_V4_MSB_IDX;
    while (bit_idx >= IP_V4_BIT_LEN - mask)
    {
        uint8_t curr_b = (base >> bit_idx) & 1;
        if (NULL == trie_root_node->children[curr_b])
        {
            return NULL;
        }
        trie_root_node = trie_root_node->children[curr_b];
        bit_idx--;
    }
    if (trie_root_node->is_prefix)
    {
        return trie_root_node;
    }
    return NULL;
}

int add(PHX_BIN_TRIE_NODE trie_node, int base, int mask)
{

    if (IP_V4_BIT_LEN < mask || 0 > mask)
    {
        return -1;
    }

    int bit_idx = IP_V4_MSB_IDX;
    while (bit_idx >= IP_V4_BIT_LEN - mask)
    {
        int curr_b = (base >> bit_idx) & 1;
        if (NULL == trie_node->children[curr_b])
        {
            trie_node->children[curr_b] = PHX_create_bin_trie_node();
        }
        trie_node = trie_node->children[curr_b];
        bit_idx--;
    }
    trie_node->is_prefix = true;
    return 0;
}

int check(PHX_BIN_TRIE_NODE trie_node, int ip)
{
    int longest_prefix_found = -1;
    int bit_idx = IP_V4_MSB_IDX;
    for (; bit_idx >= 0; bit_idx--)
    {
        int curr_bit = (ip >> bit_idx) & 1;
        if (trie_node->is_prefix)
        {
            longest_prefix_found = IP_V4_MSB_IDX - bit_idx;
        }
        if (NULL == trie_node->children[curr_bit])
        {
            break;
        }
        trie_node = trie_node->children[curr_bit];
    }

    /**
     * Root node always exists and carry no information about symbol
     * If we've got 0 prefix, than it should only indicate that each ip match 0 mask
     * Becouse of this, in case of full 32 prefix, we need to check leaf additionally
     */
    if (-1 == bit_idx && trie_node->is_prefix)
    {
        longest_prefix_found = IP_V4_BIT_LEN;
    }

    return longest_prefix_found;
}

int del(PHX_BIN_TRIE_NODE trie_node, int base, int mask)
{
    if (IP_V4_BIT_LEN < mask || 0 > mask)
    {
        return -1;
    }
    PHX_BIN_TRIE_NODE node = _PHX_find_prefix(trie_node, base, mask);
    if (NULL != node)
    {
        node->is_prefix = false;
    }
    return 0;
}
