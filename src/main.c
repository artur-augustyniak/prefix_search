#include <stdio.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "bin_trie.h"
#include <stdlib.h>

int atohl(const char *ip)
{
    struct sockaddr_in saddr;
    inet_pton(AF_INET, ip, &saddr.sin_addr);
    return ntohl(saddr.sin_addr.s_addr);
}

void jitter_data(PHX_BIN_TRIE_NODE trie_root_node)
{

    // uint32_t no_tests = ~0;
    uint32_t no_tests = 400000;

    printf("[+] polluting trie with random %d prefixes\n", no_tests);
    for (uint32_t i = 0; i < no_tests; i++)
    {
        int t_ip = rand();
        int mask = rand() % 32;
        int res = add(trie_root_node, t_ip, mask);
        if (-1 == res)
        {
            PHX_abort(
                __FILE__,
                __LINE__,
                "trie insertion error, aborting");
        }
    }
    // PHX_dump_trie(trie_root_node);
}

void test_out_of_bounds_mask_should_return_err(PHX_BIN_TRIE_NODE trie_root_node)
{
    int ip = rand();
    int masks[5] = {-1, 0, 1, 32, 33};
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += add(trie_root_node, ip, masks[i]);
        sum += del(trie_root_node, ip, masks[i]);
    }
    printf("[!] test_out_of_bounds_mask_should_return_err %s\n", -4 == sum ? "SUCCESS" : "FAIL");
}

void test_edge_cases(PHX_BIN_TRIE_NODE trie_root_node)
{
    char *base_ip = "0.0.0.0";
    int base_n = atohl(base_ip);
    int res = -1;
    int found_mask = -1;
    int sum = 0;

    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    res = add(trie_root_node, base_n, 0);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie insertion error, aborting");
    }
    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    res = del(trie_root_node, base_n, 0);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie deletion error, aborting");
    }

    res = add(trie_root_node, base_n, 32);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie insertion error, aborting");
    }

    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    base_ip = "255.255.255.255";
    base_n = atohl(base_ip);

    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    res = add(trie_root_node, base_n, 0);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie insertion error, aborting");
    }

    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    res = add(trie_root_node, base_n, 32);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie insertion error, aborting");
    }

    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    char *base_ip1 = "0.0.0.0";
    char *base_ip2 = "255.255.255.255";

    for (int i = 0; i < 33; i++)
    {
        res = del(trie_root_node, atohl(base_ip1), i);
        if (-1 == res)
        {
            PHX_abort(
                __FILE__,
                __LINE__,
                "trie deletion error, aborting");
        }
        res = del(trie_root_node, atohl(base_ip2), i);
        if (-1 == res)
        {
            PHX_abort(
                __FILE__,
                __LINE__,
                "trie deletion error, aborting");
        }
    }

    // PHX_dump_trie(trie_root_node);
    printf("[!] test_edge_cases %s\n", 94 == sum ? "SUCCESS" : "FAIL");
}

void test_del_restore(PHX_BIN_TRIE_NODE trie_root_node)
{
    char *base_ip = "192.168.1.123";
    int mask = 13;
    int base_n = atohl(base_ip);
    int res = -1;
    int found_mask = -1;
    int sum = 0;

    res = add(trie_root_node, base_n, mask);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie insertion error, aborting");
    }
    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    res = del(trie_root_node, base_n, mask);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie deletion error, aborting");
    }
    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    res = add(trie_root_node, base_n, mask);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie insertion error, aborting");
    }
    found_mask = check(trie_root_node, base_n);
    sum += found_mask;

    res = del(trie_root_node, base_n, mask);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie deletion error, aborting");
    }

    printf("[!] test_del_restore %s\n", 25 == sum ? "SUCCESS" : "FAIL");
}

void test_trie_should_ret_neg_one_for_unknown_values(PHX_BIN_TRIE_NODE trie_root_node)
{
    // PHX_dump_trie(trie_root_node);
    int32_t no_tests = 40000;
    int64_t sum = 0;
    for (int32_t i = 0; i < no_tests; i++)
    {
        int t_ip = rand();
        int found_mask = check(trie_root_node, t_ip);
        sum += found_mask;
    }
    printf("[!] test_trie_should_ret_neg_one_for_unknown_values %s\n", no_tests * -1 == sum ? "SUCCESS" : "FAIL");
}

void test_random_ints_must_match_default_gateway_prefix(PHX_BIN_TRIE_NODE trie_root_node)
{
    char *base_ip = "0.0.0.0";
    // char *base_ip = "192.168.1.123"; // 0
    int mask = 0;
    int base_n = atohl(base_ip);
    // printf("[+] adding prefix 0x%x - %s/%d \n", base_n, base_ip, mask);

    int res = add(trie_root_node, base_n, mask);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie insertion error, aborting");
    }
    // PHX_dump_trie(trie_root_node);
    uint32_t no_tests = 40000;
    uint64_t sum = 0;
    for (uint32_t i = 0; i < no_tests; i++)
    {
        int t_ip = rand();
        int found_mask = check(trie_root_node, t_ip);
        sum += found_mask;
    }
    printf("[!] test_random_ints_must_match_default_gateway_prefix %s\n", 0 == sum ? "SUCCESS" : "FAIL");
}

void test_adding_specific_prefix_with_incremented_mask_must_find_biggest_mask(PHX_BIN_TRIE_NODE trie_root_node)
{
    char *base_ip = "54.20.1.123";
    int base_n = atohl(base_ip);
    uint32_t sum = 0;
    for (int mask = 0; mask <= 32; mask++)
    {
        int res = add(trie_root_node, base_n, mask);
        if (-1 == res)
        {
            PHX_abort(
                __FILE__,
                __LINE__,
                "trie insertion error, aborting");
        }

        int found_mask = check(trie_root_node, base_n);
        sum += found_mask;
    }
    uint32_t expected_sum = 32 * (32 + 1) / 2;
    printf("[!] test_adding_specific_prefix_with_incremented_mask_must_find_biggest_mask %s\n", expected_sum == sum ? "SUCCESS" : "FAIL");
}

void test_deleting_specific_prefix_with_decremented_mask_must_find_biggest_mask(PHX_BIN_TRIE_NODE trie_root_node)
{
    char *base_ip = "54.20.1.123";
    int base_n = atohl(base_ip);
    uint32_t sum = 0;
    for (int mask = 32; mask >= 0; mask--)
    {
        int res = del(trie_root_node, base_n, mask);
        if (-1 == res)
        {
            PHX_abort(
                __FILE__,
                __LINE__,
                "trie deletion error, aborting");
        }
        int found_mask = check(trie_root_node, base_n);
        sum += found_mask;
    }
    uint32_t expected_sum = 31 * (31 + 1) / 2 - 1;
    printf("[!] test_deleting_specific_prefix_with_decremented_mask_must_find_biggest_mask %s\n", expected_sum == sum ? "SUCCESS" : "FAIL");
}

void test_removing_specific_in_octet_prefix_should_return_proper_smaller_mask(PHX_BIN_TRIE_NODE trie_root_node)
{

    char *base_ip = "192.168.1.0";
    int base_n = atohl(base_ip);
    int masks[2] = {25, 23};
    int found_mask = -1;
    int sum = 0;

    for (int i = 0; i < 2; i++)
    {
        int res = add(trie_root_node, base_n, masks[i]);
        if (-1 == res)
        {
            PHX_abort(
                __FILE__,
                __LINE__,
                "trie insertion error, aborting");
        }
    }

    char *ip_a = "192.168.1.132";
    int ip_n_a = atohl(ip_a);

    char *ip_b = "192.168.1.127";
    int ip_n_b = atohl(ip_b);

    found_mask = check(trie_root_node, ip_n_a);
    sum += found_mask;

    found_mask = check(trie_root_node, ip_n_b);
    sum += found_mask;

    int res = del(trie_root_node, base_n, 25);
    if (-1 == res)
    {
        PHX_abort(
            __FILE__,
            __LINE__,
            "trie deletion error, aborting");
    }

    found_mask = check(trie_root_node, ip_n_a);
    sum += found_mask;

    found_mask = check(trie_root_node, ip_n_b);
    sum += found_mask;

    printf("[!] test_removing_specific_in_octet_prefix_should_return_proper_smaller_mask %s\n", 94 == sum ? "SUCCESS" : "FAIL");
}

int main(void)
{
    srand(42);

    PHX_BIN_TRIE_NODE trie_root_node = PHX_create_bin_trie_node();

    test_edge_cases(trie_root_node);

    test_del_restore(trie_root_node);

    test_out_of_bounds_mask_should_return_err(trie_root_node);

    test_trie_should_ret_neg_one_for_unknown_values(trie_root_node);

    test_random_ints_must_match_default_gateway_prefix(trie_root_node);

    test_adding_specific_prefix_with_incremented_mask_must_find_biggest_mask(trie_root_node);

    test_deleting_specific_prefix_with_decremented_mask_must_find_biggest_mask(trie_root_node);

    jitter_data(trie_root_node);

    test_removing_specific_in_octet_prefix_should_return_proper_smaller_mask(trie_root_node);

    PHX_destroy_bin_trie(trie_root_node);

    return 0;
}
