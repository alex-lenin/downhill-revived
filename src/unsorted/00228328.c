/* UNIT 0x00228328 0x002286F8 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Effect {
    Vec pos;
    float x10, x14, x18, x1C;
    float x20, x24, x28;
    unsigned short x2C, x2E;
} Effect;
typedef struct Sprite {
    char pad0[0x30];
    unsigned long x : 11;
    unsigned long pad30 : 5;
    unsigned long y : 11;
    char pad38[0x38];
} Sprite;
typedef struct Packet { char pad0[0x40]; } Packet;

extern int D_00720270[];
extern int D_00720280[];
extern Effect D_00720290[][74];
extern Effect *D_00723A10[];
extern Sprite D_00732600[];
extern Packet D_0072DC00[][74];
extern int func_0023D608(void);
extern void func_0023D618(int, int *, int *);
extern void func_0023D650(int, int *, int *);
extern void func_0023D6A0(int, float *, float *);
extern void func_00222D48(int, Effect *, Vec *);
extern void func_00238740(int, int, int);
extern int func_001FD238(float);
extern void func_0025A960(int);
extern void func_00244228(Sprite *, Packet *);

/* max.s (EE FPU extra). The repeated "f"(b) operand is needed to match: it raises the use
   count of the 1.5f constant so the register allocator gives it $f20 as in retail. */
static inline float Max(float a, float b)
{
    float r;
    __asm__ ("max.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b), "f"(b));
    return r;
}

int func_00228328(int index)
{
    Vec proj;
    int x, y;
    int w, h;
    float a, b;
    Effect *src, *out;
    int n, k, count;
    float left, right, top, bottom, t;
    Sprite *sprite;

    D_00720280[index] = 0;
    if (func_0023D608() >= 3) return 0;
    out = D_00723A10[index];
    src = D_00720290[index];
    count = 0;
    func_0023D618(index, &x, &y);
    func_0023D650(index, &w, &h);
    n = D_00720270[index];
    left = (float)(x - (w >> 1));
    top = (float)(y - (h >> 1));
    right = (float)(x + (w >> 1));
    bottom = (float)(y + (h >> 1));
    for (k = 0; k < n; k++) {
        func_00222D48(index, src, &proj);
        if (proj.z >= 1.0f) {
            *out = *src;
            out->pos.x = proj.x;
            out->pos.y = proj.y;
            out->pos.w = proj.z;
            out->x2C |= 1;
            if (out->x2C & 0x80) {
                out->pos.z = 1.0f;
                func_00238740((int)(proj.x * 16.0f), (int)(proj.y * 16.0f), index);
            } else {
                func_0023D6A0(index, &a, &b);
                t = proj.z - Max(src->x10, 1.5f);
                out->pos.z = (b + a * t) / t;
            }
            if (left < proj.x && proj.x < right && top < proj.y && proj.y < bottom) {
                sprite = &D_00732600[count];
                sprite->x = func_001FD238(proj.x) - 0x700;
                sprite->y = func_001FD238(proj.y) - 0x790;
                out->x2C |= 4;
            }
            D_00720280[index]++;
            out++;
            count++;
        }
        src++;
    }
    if (count != 0) {
        out = D_00723A10[index];
        func_0025A960(0);
        for (k = 0; k < count; k++, out++) {
            if (out->x2C & 4) func_00244228(&D_00732600[k], &D_0072DC00[index][k]);
        }
    }
    D_00720270[index] = 0;
    return D_00720280[index] != 0;
}
