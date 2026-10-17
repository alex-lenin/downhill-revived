/* UNIT 0x002286F8 0x00228BE0 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[0x2838];
    short *x2838;
    char pad283C[0x24];
} Obj;

typedef struct Blob {
    float x0;
    float x4;
    float x8;
    char padC[0xC];
    float x18;
    float x1C;
    char pad20[0xC];
    unsigned short x2C;
    short x2E;
} Blob;

typedef struct Pair { float x0, x4; } Pair;
typedef struct Texture { int x0, x4, x8, xC; } Texture;

extern int D_00720280[];
extern Blob *D_00723A10[];
extern Obj D_00723A80[];
extern Pair D_00720110[][4];
extern unsigned int D_0072DC00[][74][16];
extern Texture D_0067DF90[];
extern float D_0029E310;

extern void func_00226588(Obj *, int);
extern unsigned int func_001FD238(float);
extern int func_00219EE8(int, Texture *, u128 *);

u128 *func_002286F8(u128 *packet, int idx, u128 **end)
{
    Obj *obj;
    Blob *blob;
    int i;
    int visible;
    float total;
    int *p;
    int j;
    int k;

    /* sqrt.s is an EE FPU instruction gcc cannot emit: inline asm */
    if (D_00720280[idx] != 0) {
        obj = &D_00723A80[idx];
        total = 0.0f;
        visible = 0;
        func_00226588(obj, idx);
        blob = D_00723A10[idx];
        for (i = 0; i < D_00720280[idx]; i++, blob++) {
            float count = 0.0f;

            if (blob->x2C & 4) {
                unsigned int *cell;
                unsigned int x;
                unsigned int y;
                float size;

                if (blob->x2C & 0x80) {
                    float sumx;
                    float sumy;
                    float root;
                    unsigned int limit;

                    cell = D_0072DC00[idx][i];
                    limit = func_001FD238(blob->x8);
                    sumx = 0.0f;
                    sumy = 0.0f;
                    for (y = 0; y < 4; y++) {
                        for (x = 0; x < 4; x++) {
                            if ((*cell & 0xFFFFFF) <= limit) {
                                sumx += x;
                                sumy += y;
                                count += 1.0f;
                                blob->x2C |= 2;
                            }
                            cell++;
                        }
                    }
                    if (blob->x2C & 2) {
                        blob->x0 += sumx / count;
                        blob->x4 += sumy / count;
                    }
                    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(root) : "f"(count * 0.0625f));
                    size = blob->x18 * root;
                    visible = (blob->x2C >> 1) & 1;
                    blob->x18 = size;
                    total += size;
                } else {
                    float root;
                    unsigned int limit;

                    cell = &D_0072DC00[idx][i][5];
                    limit = func_001FD238(blob->x8);
                    for (y = 1; y < 3; y++) {
                        for (x = 1; x < 3; x++) {
                            if ((*cell & 0xFFFFFF) <= limit) {
                                count += 1.0f;
                                blob->x2C |= 2;
                            }
                            cell++;
                        }
                        cell += 2;
                    }
                    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(root) : "f"(count * 0.25f));
                    blob->x18 *= root;
                }
            }
            if (blob->x2E != 0 && blob->x2E < 0x7FF) {
                Pair *pair = &D_00720110[blob->x2E][idx];
                float size = blob->x18;

                blob->x18 = (pair->x4 + size) * 0.5f;
                pair->x4 = size;
            }
            blob->x1C = blob->x18 * blob->x18;
        }
        if (visible) {
            D_0029E310 += (total - D_0029E310) * 0.125f;
        } else {
            D_0029E310 -= D_0029E310 * 0.125f;
        }
        for (j = 0; j < 4; j++) {
            short slot = obj->x2838[j];

            packet += func_00219EE8(D_0067DF90[slot].x8, &D_0067DF90[slot], packet);
        }
        *end = packet;
        p = (int *)packet;
        p[0] = 0x50000002;
        p[1] = ((int)obj + 12) & 0xFFFFFFF0;
        p[2] = 0x11000000;
        p[3] = 0x64040008;
        packet++;
        for (k = 0; k < 4; k += 2) {
            short slot = obj->x2838[k];

            p = (int *)packet;
            p[0] = D_0067DF90[slot].x0;
            p[1] = D_0067DF90[slot].x4;
            slot = obj->x2838[k + 1];
            if (k + 1 < 4) {
                p[2] = D_0067DF90[slot].x0;
                p[3] = D_0067DF90[slot].x4;
            } else {
                p[2] = 0;
                p[3] = 0;
            }
            packet++;
        }
    }
    return packet;
}
