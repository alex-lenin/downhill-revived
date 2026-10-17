/* SPU2 delay network, independently implemented from the register equations
   documented by no$psx and cross-checked against PCSX2 Reverb.cpp. Registers
   are expressed in sample words. One channel executes per 48-kHz sample;
   the delay cursor advances at 24 kHz. No host clocks or floating point. */

/* The ten reverb presets (SetReverbType modes 0-9) are not in this source: they are read once from
   the player's own disc, from Sony's libsd module that the game loads on the IOP (MOD\LIBSD.IRX).
   libsd keeps them as a table of ten work-area sizes (uint32, in units of 4 sample words) followed
   by ten entries {uint32 flags; uint16 reg[32]} in SPU2 effect register order (finding 053).
   The table is found by its shape, not by an offset: mode 0 (OFF) has only mSAME and mDIFF
   (registers 10, 11, 18, 19) set, to 1; modes 1-9 have both input coefficients (registers 30,
   31) at 0x8000; every flags word is 0 and every work area fits SndReverb.ram. Without the table
   (no disc, another module) every mode plays as OFF and the mixer says so once. */
#define REVERB_MODULE "/MOD/LIBSD.IRX;1"
#define REVERB_MAX_SIZE (0x1804u * 8)  /* sample words: the largest preset, the delay RAM */
static struct {
    int state;               /* 0 not read yet, 1 read, -1 not found */
    uint32_t size[10];       /* work area in sample words */
    uint16_t reg[10][32];
} reverb_presets;

static uint32_t reverb_le32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }
static uint16_t reverb_le16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }

static int reverb_presets_match(const uint8_t *t)
{
    int mode, i;
    for (mode = 0; mode < 10; mode++) {
        const uint8_t *e = t + 40 + mode * 68;
        uint32_t size = reverb_le32(t + mode * 4);
        if (!size || size * 4 > REVERB_MAX_SIZE || reverb_le32(e)) return 0;
        for (i = 0; i < 32; i++) {
            uint16_t reg = reverb_le16(e + 4 + i * 2);
            if (mode == 0 ? reg != (i == 10 || i == 11 || i == 18 || i == 19)
                          : i >= 30 && reg != 0x8000) return 0;
        }
    }
    return 1;
}

/* Finds the table in a module image; 1 if found (and taken). */
static int reverb_presets_parse(const uint8_t *data, uint32_t size)
{
    uint32_t at;
    int mode, i;
    for (at = 0; size >= 40 + 680 && at <= size - (40 + 680); at += 2)
        if (reverb_presets_match(data + at)) {
            for (mode = 0; mode < 10; mode++) {
                const uint8_t *e = data + at + 40 + mode * 68;
                reverb_presets.size[mode] = reverb_le32(data + at + mode * 4) * 4;
                for (i = 0; i < 32; i++) reverb_presets.reg[mode][i] = reverb_le16(e + 4 + i * 2);
            }
            reverb_presets.state = 1;
            return 1;
        }
    return 0;
}

static void reverb_presets_load(void)
{
    uint32_t lsn, size, sectors;
    uint8_t *image = NULL;
    reverb_presets.state = -1;
    if (cdvd_find(REVERB_MODULE, &lsn, &size) && size && size <= (1u << 20)) {
        sectors = (size + 2047) / 2048;
        image = malloc((size_t)sectors * 2048);
        if (image && cdvd_read(lsn, sectors, image)) reverb_presets_parse(image, size);
        free(image);
    }
    if (reverb_presets.state < 0)
        hle_log("sound: no reverb presets found in %s on the disc; reverb is off\n", REVERB_MODULE);
}

typedef struct {
    uint16_t reg[32];
    uint32_t size, pos;
    uint32_t delay_cursor;
    uint32_t address[32], apf_delay[4];
    uint64_t cycles;
    int mode;
    int16_t ram[0x1804 * 8];
    int16_t down[2][128], up[2][128]; /* mirror the ring for contiguous FIR loads */
} SndReverb;
static SndReverb reverbs[2];

/* Documented 39-tap SPU half-band filter, in signed Q15. */
static const int16_t snd_reverb_fir[40] = {
    -1,0,2,0,-10,0,35,0,-103,0,266,0,-616,0,1332,0,-2960,0,10246,
    16384,10246,0,-2960,0,1332,0,-616,0,266,0,-103,0,35,0,-10,0,2,0,-1,0
};

static int16_t reverb_clip(int32_t sample)
{
    return (int16_t)(sample > 32767 ? 32767 : sample < -32768 ? -32768 : sample);
}

static int reverb_preset(int core, int mode, int delay, int feedback)
{
    SndReverb *r = &reverbs[core];
    int i;
    if (mode < 0 || mode > 9) return 0;
    if (!reverb_presets.state) reverb_presets_load();
    if (reverb_presets.state < 0) {  /* no presets: OFF */
        r->mode = 0; r->size = 0;
        return mode == 0;
    }
    if (r->mode != mode) {
        memset(r->ram, 0, sizeof r->ram);
        memset(r->down, 0, sizeof r->down); memset(r->up, 0, sizeof r->up);
    }
    r->mode = mode; r->size = reverb_presets.size[mode];
    for (i = 0; i < 32; i++) r->reg[i] = reverb_presets.reg[mode][i];
    if (mode == 7 || mode == 8) {
        uint16_t d = (uint16_t)((delay + 1) * 32);
        r->reg[7] = (uint16_t)(feedback * 258);
        r->reg[10] = (uint16_t)(d * 2 - r->reg[0]); r->reg[11] = (uint16_t)(d - r->reg[1]);
        r->reg[16] = (uint16_t)(r->reg[17] + d); r->reg[12] = (uint16_t)(r->reg[13] + d);
        r->reg[27] = (uint16_t)(r->reg[29] + d); r->reg[26] = (uint16_t)(r->reg[28] + d);
    }
    if (trace_on()) {  /* DHD_SND_TRACE: the registers a preset writes */
        hle_log("989snd reverb core %d mode %d size %u:", core, mode, (unsigned)r->size);
        for (i = 0; i < 32; i++) hle_log(" %04X", r->reg[i]);
        hle_log("\n");
    }
    if (r->size) {
        r->delay_cursor = (uint32_t)((r->cycles >> 1) % r->size);
        for (i = 0; i < 32; i++) r->address[i] = (r->reg[i] * 4u) % r->size;
        for (i = 0; i < 4; i++) {
            int32_t offset = ((int32_t)r->reg[26 + i] - r->reg[i / 2]) * 4;
            offset %= (int32_t)r->size;
            r->apf_delay[i] = (uint32_t)(offset < 0 ? offset + r->size : offset);
        }
    }
    return 1;
}

static uint32_t reverb_address(SndReverb *r, int32_t offset)
{
    int32_t at = (int32_t)r->delay_cursor + offset;
    /* Preset offsets are reduced once; the cursor and offset each fit size.
       Only the previous-sample tap may be -1. */
    if (at >= (int32_t)r->size) at -= r->size;
    if (at < 0) at += r->size;
    return (uint32_t)at;
}

static int32_t reverb_fir(const int16_t *samples, uint32_t pos, int up)
{
    int i;
    int32_t sum;
    int64_t wide;
    __m128i accum = _mm_setzero_si128();
    const int16_t *window = samples + ((pos - 39) & 63);
    /* The absolute down-filter sum is 47526: even full-scale signed input
       bounds every product pair, partial sum and final sum below INT32_MAX.
       Upsampling needs the wide final multiply and centre correction. */
    for (i = 0; i < 40; i += 8) {
        __m128i input = _mm_loadu_si128((const __m128i *)(window + i));
        __m128i coeff = _mm_loadu_si128((const __m128i *)(snd_reverb_fir + i));
        accum = _mm_add_epi32(accum, _mm_madd_epi16(input, coeff));
    }
    accum = _mm_add_epi32(accum, _mm_shuffle_epi32(accum, _MM_SHUFFLE(1,0,3,2)));
    accum = _mm_add_epi32(accum, _mm_shuffle_epi32(accum, _MM_SHUFFLE(2,3,0,1)));
    sum = _mm_cvtsi128_si32(accum);
    wide = up ? (int64_t)sum * 2 - window[19] : sum;
    return reverb_clip((int32_t)(wide >> 15));
}

static void reverb_sample(SndReverb *r, const int32_t input[2], int32_t output[2])
{
    int channel = (int)(r->cycles & 1), i;
    int32_t same, diff, value, stage[2], delayed, previous, filtered;
    uint32_t same_dst, diff_dst, apf_dst[2];
    output[0] = output[1] = 0;
    if (!r->mode || !r->size) { r->cycles++; return; }
    r->down[0][r->pos] = reverb_clip(input[0]); r->down[1][r->pos] = reverb_clip(input[1]);
    r->down[0][r->pos + 64] = r->down[0][r->pos];
    r->down[1][r->pos + 64] = r->down[1][r->pos];
    filtered = snd_q15(reverb_fir(r->down[channel], r->pos, 0), (int16_t)r->reg[30 + channel]);
    same_dst = reverb_address(r, r->address[10 + channel]);
    previous = r->ram[reverb_address(r, (int32_t)r->address[10 + channel] - 1)];
    value = r->ram[reverb_address(r, r->address[16 + channel])];
    same = previous + snd_q15(filtered + snd_q15(value, (int16_t)r->reg[7]) - previous, (int16_t)r->reg[2]);
    diff_dst = reverb_address(r, r->address[18 + channel]);
    previous = r->ram[reverb_address(r, (int32_t)r->address[18 + channel] - 1)];
    value = r->ram[reverb_address(r, r->address[24 + !channel])];
    diff = previous + snd_q15(filtered + snd_q15(value, (int16_t)r->reg[7]) - previous, (int16_t)r->reg[2]);
    value = 0;
    for (i = 0; i < 4; i++) {
        int reg = (i < 2 ? 12 : 16) + i * 2 + channel;
        value += snd_q15(r->ram[reverb_address(r, r->address[reg])], (int16_t)r->reg[3 + i]);
    }
    for (i = 0; i < 2; i++) {
        int reg = 26 + i * 2 + channel;
        apf_dst[i] = reverb_address(r, r->address[reg]);
        delayed = r->ram[reverb_address(r, r->apf_delay[i * 2 + channel])];
        stage[i] = value - snd_q15(delayed, (int16_t)r->reg[8 + i]);
        value = delayed + snd_q15(stage[i], (int16_t)r->reg[8 + i]);
    }
    r->ram[same_dst] = reverb_clip(same); r->ram[diff_dst] = reverb_clip(diff);
    for (i = 0; i < 2; i++) r->ram[apf_dst[i]] = reverb_clip(stage[i]);
    r->up[channel][r->pos] = reverb_clip(value); r->up[!channel][r->pos] = 0;
    r->up[0][r->pos + 64] = r->up[0][r->pos];
    r->up[1][r->pos + 64] = r->up[1][r->pos];
    r->pos = (r->pos + 1) & 63;
    output[0] = reverb_fir(r->up[0], r->pos, 1); output[1] = reverb_fir(r->up[1], r->pos, 1);
    if (channel && ++r->delay_cursor == r->size) r->delay_cursor = 0;
    r->cycles++;
}
