/* 989snd's R250 generator, independently implemented from the retail driver's
   arithmetic (finding 022, IOP D29C/D3B0). No stored game table is included:
   the initial 250 words are generated using its documented seed procedure. */
static uint16_t grain_rng_words[250];
static unsigned grain_rng_at;
static int grain_rng_initialized;

static void grain_random_seed(uint16_t seed)
{
    uint32_t lcg = seed;
    uint16_t mask = 65535, bit = 32768;
    unsigned i;
    for (i = 0; i < 250; i++) {
        lcg = lcg * 0x015A4E35u + 1;
        grain_rng_words[i] = (uint16_t)((lcg >> 16) & 32767);
    }
    for (i = 0; i < 250; i++) {
        lcg = lcg * 0x015A4E35u + 1;
        if (((lcg >> 16) & 32767) > 16384) grain_rng_words[i] |= 32768;
    }
    for (i = 0; i < 16; i++) {
        unsigned at = 3 + 11 * i;
        grain_rng_words[at] = (grain_rng_words[at] & mask) | bit;
        mask >>= 1; bit >>= 1;
    }
    grain_rng_at = 0; grain_rng_initialized = 1;
}

static uint32_t grain_random(uint32_t range)
{
    unsigned other;
    uint16_t result;
    if (!grain_rng_initialized) grain_random_seed(12345);
    other = grain_rng_at < 147 ? grain_rng_at + 103 : grain_rng_at - 147;
    result = grain_rng_words[grain_rng_at] ^ grain_rng_words[other];
    grain_rng_words[grain_rng_at] = result;
    if (++grain_rng_at == 250) grain_rng_at = 0;
    snd_debug_random(grain_samples, grain_rng_at, result);
    return range ? result % range : 0;
}
