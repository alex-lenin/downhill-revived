/* State hashes for desync detection: XXH64, written here from its published specification
   (github.com/Cyan4973/xxHash, doc/xxhash_spec.md), checked against its reference values in
   test_lockstep.c. Ranges chain through the seed, so their order matters. */
#include "net.h"

#include <string.h>

#define P1 11400714785074694791ull
#define P2 14029467366897019727ull
#define P3 1609587929392839161ull
#define P4 9650029242287828579ull
#define P5 2870177450012600261ull

static uint64_t rotl(uint64_t x, int r) { return x << r | x >> (64 - r); }
static uint64_t read64(const uint8_t *p) { uint64_t v; memcpy(&v, p, 8); return v; }  /* little-endian host */
static uint32_t read32(const uint8_t *p) { uint32_t v; memcpy(&v, p, 4); return v; }

static uint64_t round64(uint64_t acc, uint64_t in)
{
    acc += in * P2;
    return rotl(acc, 31) * P1;
}

static uint64_t merge(uint64_t acc, uint64_t v)
{
    acc ^= round64(0, v);
    return acc * P1 + P4;
}

uint64_t net_hash64(const void *data, size_t size, uint64_t seed)
{
    const uint8_t *p = data, *end = p + size;
    uint64_t h;
    if (size >= 32) {
        uint64_t v1 = seed + P1 + P2, v2 = seed + P2, v3 = seed, v4 = seed - P1;
        const uint8_t *limit = end - 32;
        do {
            v1 = round64(v1, read64(p));
            v2 = round64(v2, read64(p + 8));
            v3 = round64(v3, read64(p + 16));
            v4 = round64(v4, read64(p + 24));
            p += 32;
        } while (p <= limit);
        h = rotl(v1, 1) + rotl(v2, 7) + rotl(v3, 12) + rotl(v4, 18);
        h = merge(h, v1), h = merge(h, v2), h = merge(h, v3), h = merge(h, v4);
    } else
        h = seed + P5;
    h += size;
    for (; p + 8 <= end; p += 8)
        h = rotl(h ^ round64(0, read64(p)), 27) * P1 + P4;
    if (p + 4 <= end)
        h = rotl(h ^ read32(p) * P1, 23) * P2 + P3, p += 4;
    for (; p < end; p++)
        h = rotl(h ^ *p * P5, 11) * P1;
    h ^= h >> 33, h *= P2;
    h ^= h >> 29, h *= P3;
    return h ^ h >> 32;
}

uint64_t net_hash_ranges(const NetRange *ranges, int n)
{
    uint64_t h = 0;
    int i;
    for (i = 0; i < n; i++)
        h = net_hash64(ranges[i].base, ranges[i].size, h);
    return h;
}
