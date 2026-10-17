/* UNIT 0x001EA218 0x001EA7C8 */
#include "rider.h"
typedef Matrix4 Matrix;
typedef struct Stats {
    char pad0[0x718];
    int x718;
    char pad71C[0x10E8-0x71C];
    char x10E8, x10E9;
} Stats;
typedef struct Input {
    char pad0[0x498];
    float x498, x49C;
} Input;
typedef struct Game {
    char pad0[0x40C0];
    Input input;
    char pad4560[0x6610-0x4560];
    Stats stats;
    char pad76FA[0x792C-0x76FC];
    SceneNode *bodyNode;
} Game;
typedef struct Obj {
    Game *x0;
    char pad4[0x14-4];
    float x14,x18;
    int x1C,x20;
    char pad24[8];
    int x2C;
    char pad30[0x1D0-0x30];
    float x1D0,x1D4;
    char pad1D8[0x1F2-0x1D8];
    char x1F2;
} Obj;
typedef struct Config {
    char pad0[0x44];
    float x44;
    char pad48[0xC];
} Config;
extern Config D_00272888[];

extern void func_001E5BD8(Stats *, int);
extern void func_001E5B20(Stats *, int);

/* Sony sceVu0CopyMatrix, as used by the promoted matrix-copy routines. */
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\tsq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
        : "=m"((dst).row[0]), "=m"((dst).row[1]), "=m"((dst).row[2]), "=m"((dst).row[3]) \
        : "r"(src) : "$8", "$9", "$10", "$11")

void func_001EA218(Obj *obj)
{
    Matrix matrix;
    float angles[4];
    Game *game = obj->x0;
    Stats *stats = &game->stats;
    Config *config = &D_00272888[obj->x2C];
    float z, x, az, ax;
    float xscale, zscale;
    Input *input;
    COPY_MATRIX(matrix, &game->bodyNode->matrix);
    func_002224E0(&matrix, (Vector3 *)angles);
    z = angles[2] * 57.29578f + 180.0f;
    x = angles[0] * 57.29578f + 180.0f;
    az = z - obj->x1D0;
    ax = x - obj->x1D4;
    if (az > 180.0f) az -= 360.0f;
    else if (az < -180.0f) az += 360.0f;
    if (ax > 180.0f) ax -= 360.0f;
    else if (ax < -180.0f) ax += 360.0f;
    obj->x14 += ax;
    obj->x18 += az;
    obj->x1D4 = x;
    obj->x1D0 = z;
    ax = obj->x14;
    if (ax < 0.0f) ax = -ax;
    az = obj->x18;
    if (az < 0.0f) az = -az;
    input = &game->input;
    xscale = 1.0f;
    zscale = 1.0f;
    if (input->x498 == 1.0f) xscale = 2.0f;
    if (input->x49C == 1.0f) zscale = 2.0f;
    if (ax >= 85.0f && obj->x1C == 0) {
        if (stats->x718 == 0) func_001E5BD8(&game->stats, (int)(xscale * 37.5f));
        obj->x1C++;
    } else if (ax >= 160.0f && obj->x1C == 1) {
        obj->x1C = 2;
    } else if (ax >= 230.0f && obj->x1C == 2) {
        obj->x1C = 3;
    } else if (ax >= 270.0f && obj->x1C == 3) {
        if (stats->x718 > 0) func_001E5BD8(&game->stats, (int)(xscale * 37.5f));
        if (obj->x14 < 0.0f) func_001E5B20(&game->stats, 1);
        else func_001E5B20(&game->stats, 2);
        if (xscale == 1.0f) stats->x10E8 = 1;
        obj->x1C++;
    } else if (ax >= 360.0f && obj->x1C == 4) {
        obj->x1C = 0;
        obj->x14 = 0.0f;
    }
    if (az > 90.0f && obj->x20 == 0) {
        if (stats->x718 == 0) func_001E5BD8(&game->stats, (int)(zscale * 37.5f));
        obj->x20++;
    } else if (az > 170.0f && obj->x20 == 1) {
        obj->x20 = 2;
    } else if (az > 260.0f && obj->x20 == 2) {
        obj->x20 = 3;
    } else if (az > 340.0f && obj->x20 == 3) {
        if (stats->x718 > 0) func_001E5BD8(&game->stats, (int)(zscale * 37.5f));
        if (obj->x18 < 0.0f) func_001E5B20(&game->stats, 3);
        else func_001E5B20(&game->stats, 4);
        if (zscale == 1.0f) stats->x10E9 = 1;
        obj->x20++;
    } else if (az >= 360.0f && obj->x20 == 4) {
        obj->x20 = 0;
        obj->x18 = 0.0f;
    }
    if (config->x44 != -1.0f && (config->x44 < ax || config->x44 < az)) obj->x1F2 = 1;
}
