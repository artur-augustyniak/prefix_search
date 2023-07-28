#include "bin_trie.h"

#define ERR_STATUS 1
#define ERR_NUM 0

void PHX_abort(const char *file, unsigned int line, const char *msg)
{
    error(ERR_STATUS, ERR_NUM, "file: %s:%d - %s", file, line, msg);
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

void PHX_destroy_bin_trie_node(PHX_BIN_TRIE_NODE n)
{
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

// supress # -fsanitize=undefined
uint32_t _PHX_get_ip_net(uint32_t base, uint8_t mask)
{
    if (32 == mask)
    {
        return base;
    }
    return base &= ~((1 << (32 - mask)) - 1);
}

int8_t PHX_add(PHX_BIN_TRIE_NODE trie_root_node, uint32_t base, uint8_t mask)
{
    if (32 < mask)
    {
        return -1;
    }
    uint32_t prefix = _PHX_get_ip_net(base, mask);
    int8_t prefix_bit = 31;
    while (prefix_bit >= 32 - mask)
    {

        uint8_t curr_b = (prefix >> prefix_bit) & 1;
        if (NULL == trie_root_node->children[curr_b])
        {
            trie_root_node->children[curr_b] = PHX_create_bin_trie_node();
        }
        trie_root_node = trie_root_node->children[curr_b];
        prefix_bit--;
    }
    trie_root_node->is_prefix = true;

    return 0;
}

PHX_BIN_TRIE_NODE _PHX_find_prefix(PHX_BIN_TRIE_NODE trie_root_node, uint32_t base, int8_t mask)
{

    int8_t prefix_bit = 31;
    while (prefix_bit >= 32 - mask)
    {

        uint8_t curr_b = (base >> prefix_bit) & 1;
        if (NULL == trie_root_node->children[curr_b])
        {
            return NULL;
        }
        trie_root_node = trie_root_node->children[curr_b];
        prefix_bit--;
    }

    if (trie_root_node->is_prefix)
    {
        return trie_root_node;
    }
    return NULL;
}

int8_t PHX_check(PHX_BIN_TRIE_NODE trie_root_node, uint32_t ip)
{
    for (int8_t pref_len = 32; pref_len >= 0; pref_len--)
    {
        uint32_t base = _PHX_get_ip_net(ip, pref_len);
        if (NULL != _PHX_find_prefix(trie_root_node, base, pref_len))
        {
            return pref_len;
        }
    }
    return -1;
}

bool _PHX_delete_prefix(PHX_BIN_TRIE_NODE trie_root_node, uint32_t base, int8_t mask)
{
    PHX_BIN_TRIE_NODE node = _PHX_find_prefix(trie_root_node, base, mask);
    if (NULL != node)
    {
        node->is_prefix = false;
        return true;
    }
    else
    {
        return false;
    }
}

int8_t PHX_del(PHX_BIN_TRIE_NODE trie_node, uint32_t base, uint8_t mask)
{
    uint32_t prefix = _PHX_get_ip_net(base, mask);
    if (_PHX_delete_prefix(trie_node, prefix, mask))
    {
        return 1;
    }
    return -1;
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
