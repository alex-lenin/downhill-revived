/* The game's data from the player's own disc (finding 053). dhd.exe holds the translated code
   only; the data the code works on (.vutext's VU microcode and DMA packets, .data, .rodata,
   .gcc_except_table and .sdata: strings, tables, models of the menus) is read at start from
   SCUS_971.77, the game's program file on the disc image, into PS2 RAM at its addresses
   (ps2_image_load, port/runtime/memory.c). The file must be the retail one: its SHA-1 is checked
   before anything is used, so a changed or damaged disc image is named as such, never run.

   The disc check of the first start (host.c disc_problem) asks the same question, so a player
   whose disc image fails here is told and can choose another one. */
#include "hle.h"

#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define SECTOR 2048
#define GAME_ELF "SCUS_971.77"
#define GAME_ELF_SIZE 1691684u  /* bytes */
static const uint8_t game_elf_sha1[20] = {0x4e, 0x25, 0xa2, 0x22, 0xad, 0x8a, 0x47, 0x5b, 0x5d, 0x26,
                                          0xc2, 0xe9, 0x28, 0xe9, 0x0b, 0x5f, 0x2b, 0x70, 0x75, 0xeb};

/* ---- SHA-1 (FIPS 180-4), for the one check above ---- */
static uint32_t rol(uint32_t x, int n) { return x << n | x >> (32 - n); }

static void sha1_block(uint32_t h[5], const uint8_t *p)
{
    uint32_t w[80];
    for (int i = 0; i < 16; i++) w[i] = (uint32_t)p[4 * i] << 24 | p[4 * i + 1] << 16 | p[4 * i + 2] << 8 | p[4 * i + 3];
    for (int i = 16; i < 80; i++) w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
    for (int i = 0; i < 80; i++) {
        uint32_t f, k;
        if (i < 20) f = (b & c) | (~b & d), k = 0x5A827999;
        else if (i < 40) f = b ^ c ^ d, k = 0x6ED9EBA1;
        else if (i < 60) f = (b & c) | (b & d) | (c & d), k = 0x8F1BBCDC;
        else f = b ^ c ^ d, k = 0xCA62C1D6;
        uint32_t t = rol(a, 5) + f + e + k + w[i];
        e = d, d = c, c = rol(b, 30), b = a, a = t;
    }
    h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e;
}

static void sha1(const uint8_t *data, size_t size, uint8_t out[20])
{
    uint32_t h[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
    size_t i = 0;
    for (; i + 64 <= size; i += 64) sha1_block(h, data + i);
    uint8_t last[128] = {0};
    size_t rest = size - i, n = rest < 56 ? 64 : 128;
    memcpy(last, data + i, rest);
    last[rest] = 0x80;
    uint64_t bits = (uint64_t)size * 8;
    for (int k = 0; k < 8; k++) last[n - 1 - k] = (uint8_t)(bits >> 8 * k);
    for (size_t k = 0; k < n; k += 64) sha1_block(h, last + k);
    for (int k = 0; k < 20; k++) out[k] = (uint8_t)(h[k / 4] >> (24 - 8 * (k % 4)));
}

static uint32_t le32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }

/* ---- SCUS_971.77 from the disc image ---- */
const char *game_data_read(const char *path, uint8_t **elf)
{
    static char why[256];
    static uint8_t sector[SECTOR];
    if (elf) *elf = NULL;
    FILE *f = fopen(path, "rb");
    if (!f) return "The file can't be opened.";
    const char *problem = NULL;
    uint8_t *data = NULL;
#define READ(lsn, n, dst) (_fseeki64(f, (long long)(lsn) * SECTOR, SEEK_SET) == 0 && fread(dst, SECTOR, n, f) == (n))
    uint32_t lsn = 0, size = 0;
    if (READ(16, 1, sector) && sector[0] == 1 && !memcmp(sector + 1, "CD001", 5)) {
        /* the root directory (the primary volume descriptor's record at 156) */
        uint32_t dir = le32(sector + 156 + 2), dir_size = le32(sector + 156 + 10);
        for (uint32_t off = 0; off < dir_size && off < 16 * SECTOR && !lsn; off += SECTOR) {
            if (!READ(dir + off / SECTOR, 1, sector)) break;
            for (unsigned i = 0; i + 33 < SECTOR && sector[i]; i += sector[i]) {
                const uint8_t *r = sector + i;
                size_t n = sizeof GAME_ELF - 1;
                if (r[32] >= n && i + 33 + r[32] <= SECTOR && !_strnicmp((const char *)r + 33, GAME_ELF, n) &&
                    (r[32] == n || r[33 + n] == ';')) {
                    lsn = le32(r + 2);
                    size = le32(r + 10);
                    break;
                }
            }
        }
    }
    if (!lsn) {
        problem = "It has no " GAME_ELF ", the game's program and data.";
        goto done;
    }
    uint8_t digest[20];
    uint32_t sectors = (size + SECTOR - 1) / SECTOR;
    if (size != GAME_ELF_SIZE) {
        snprintf(why, sizeof why, "Its " GAME_ELF " (the game's program and data) is %u bytes, not the original "
                 "%u: the disc image is changed, or from another release of the game.", size, GAME_ELF_SIZE);
        problem = why;
        goto done;
    }
    if (!(data = malloc((size_t)sectors * SECTOR)) || !READ(lsn, sectors, data)) {
        problem = "Its " GAME_ELF " can't be read: the disc image is cut short or damaged.";
        goto done;
    }
    sha1(data, size, digest);
    if (memcmp(digest, game_elf_sha1, sizeof digest)) {
        problem = "Its " GAME_ELF " (the game's program and data) isn't the original one: the disc image "
                  "is changed or damaged, or from another release of the game.";
        goto done;
    }
    if (elf) *elf = data, data = NULL;
done:
#undef READ
    free(data);
    fclose(f);
    return problem;
}

int game_data_load(void)
{
    const char *path = getenv("DHD_ISO");
    uint8_t *elf = NULL;
    const char *problem = path ? game_data_read(path, &elf) : "No disc image is chosen (DHD_ISO).";
    if (!problem) {
        /* The one loadable segment (program header 0) holds .text through .sdata. */
        uint32_t phoff = le32(elf + 0x1C), offset = le32(elf + phoff + 4), vaddr = le32(elf + phoff + 8),
                 filesz = le32(elf + phoff + 16);
        if (vaddr <= PS2_IMAGE_START && PS2_IMAGE_END <= vaddr + filesz && offset + filesz <= GAME_ELF_SIZE) {
            ps2_image_load(elf + offset + (PS2_IMAGE_START - vaddr));
            hle_log("game data: %s from the disc image (%u bytes at 0x%06X)\n", GAME_ELF,
                    PS2_IMAGE_END - PS2_IMAGE_START, PS2_IMAGE_START);
        } else
            problem = "Its " GAME_ELF " has no data where the game expects it.";
        free(elf);
    }
    if (!problem) return 1;
    hle_log("game data: %s: %s\n", path ? path : "(none)", problem);
    if (!getenv("DHD_HEADLESS")) {
        char message[1024];
        snprintf(message, sizeof message, "The game's data can't be read from the disc image\n%s\n\n%s\n\n"
                 "Downhill Revived plays from your own copy of the disc: an unchanged .iso file of "
                 "Downhill Domination (USA), SCUS-97177.", path ? path : "(none chosen)", problem);
        MessageBoxA(NULL, message, "Downhill Revived", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
    }
    return 0;
}
