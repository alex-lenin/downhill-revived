/* UNIT 0x001457A8 0x001458E0 */
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));
typedef struct Mtx { Vec4 r[4]; } Mtx;

typedef struct Body {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x20];
    Mtx x40;
    char pad80[0x10];
    Vec4 x90;
    char padA0[0x10];
    float xB0;
    char padB4[0xC];
    Mtx xC0;
} Body;

typedef struct Track {
    int x0;
    int x4;
    int x8;
    int xC;
} Track;

typedef struct Obj {
    char pad0[0x8];
    Body *x8;
    char padC[0x34];
    Vec4 x40;
    char pad50[0x20];
    Vec4 x70;
    char pad80[0x30];
    Track xB0;
    char padC0[0xD8];
    float x198;
    float x19C;
} Obj;

/* quadword copies as the game's vector/matrix copy macros */
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
        : "=m"((dst).r[0]), "=m"((dst).r[1]), "=m"((dst).r[2]), "=m"((dst).r[3]) \
        : "r"(src) : "$8", "$9", "$10", "$11")

extern void func_001DD218(Vec4 *, Vec4 *, Track *, float, float);
extern void func_001DCE58(Vec4 *);
extern void func_00145998(Obj *, int);
extern void func_00211280(Body *);
extern int func_00238F30(void);
extern void func_001450A0(Obj *, int);

void func_001457A8(Obj *obj)
{
    Vec4 pos;
    Mtx m;
    Vec4 out;
    Vec4 *center;
    Track *track;

    COPY_VECTOR(pos, &obj->x8->x10);
    COPY_MATRIX(m, &obj->x8->x40);
    center = &obj->x40;
    track = &obj->xB0;
    func_001DD218(center, &out, track, obj->x198, obj->x19C);
    COPY_VECTOR(obj->x70, &out);
    func_001DCE58(center);
    COPY_VECTOR(obj->x8->x10, center);
    func_00145998(obj, 0);
    func_00211280(obj->x8);
    COPY_VECTOR(obj->x8->x90, &pos);
    COPY_MATRIX(obj->x8->xC0, &m);
    obj->x8->xB0 = 1.0f / (float)func_00238F30();
    if (track->x4 == track->xC) {
        func_001450A0(obj, 5);
    }
}
