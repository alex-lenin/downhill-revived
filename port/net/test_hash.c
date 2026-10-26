/* Unit test of the state hash (hash.c) against XXH64's reference values. */
#include "net.h"

#include <stdio.h>
#include <string.h>

static int failures;
#define CHECK(c) do { if (!(c)) { printf("%s:%d: CHECK(%s) failed\n", __FILE__, __LINE__, #c); failures++; } } while (0)

static void test_hash(void)
{
    static const char text[] = "Nobody inspects the spammish repetition";
    uint8_t big[1000];
    NetRange r[2];
    int i;
    CHECK(net_hash64("", 0, 0) == 0xEF46DB3751D8E999ull);
    CHECK(net_hash64("abc", 3, 0) == 0x44BC2CF5AD770999ull);
    CHECK(net_hash64(text, sizeof text - 1, 0) == 0xFBCEA83C8A378BF1ull);
    for (i = 0; i < (int)sizeof big; i++)
        big[i] = (uint8_t)(i * 7);
    r[0].base = big, r[0].size = 300, r[1].base = big + 300, r[1].size = 700;
    CHECK(net_hash_ranges(r, 2) == net_hash64(big + 300, 700, net_hash64(big, 300, 0)));
    CHECK(net_hash_ranges(r, 2) != net_hash64(big, sizeof big, 0));
    {
        uint64_t before = net_hash_ranges(r, 2);
        big[500] ^= 0x80;
        CHECK(net_hash_ranges(r, 2) != before);
    }
}

int main(void)
{
    test_hash();
    if (failures) {
        printf("test_hash: %d failures\n", failures);
        return 1;
    }
    printf("test_hash: ok\n");
    return 0;
}
