/* UNIT 0x0020F420 0x0020F518 */
typedef struct Color {
    char pad0[0x18];
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    char pad1C[0x20 - 0x1C];
} Color;

typedef struct Packet {
    long x0;
    union {
        long x8;
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
            float q;
        } rgbaq;
    } u;
} Packet;

extern Color D_004F3570[];
extern char D_004F3AD0[][0x8000];
extern Packet *D_00513AC0[];
extern unsigned short *D_00513AD0[];
extern int D_00513AE0[];
extern int D_00513AF0[];

void func_0020F420(int index, int ctx)
{
    Packet *p;
    Color *c;

    p = D_00513AC0[ctx];
    if (D_00513AF0[ctx] != 1) {
        *D_00513AD0[ctx] = D_00513AE0[ctx];
        D_00513AF0[ctx] = 1;
        D_00513AD0[ctx] = (unsigned short *)D_004F3AD0[ctx];
        D_00513AE0[ctx] = 0;
        p->x0 = 0x2400000000000001;
        p->u.x8 = 0x10;
        p++;
    } else {
        p--;
    }
    p->x0 = 0x156;
    c = &D_004F3570[index];
    p->u.rgbaq.r = c->r;
    p->u.rgbaq.g = c->g;
    p->u.rgbaq.b = c->b;
    p->u.rgbaq.a = c->a;
    p->u.rgbaq.q = 1.0f;
    p++;
    D_00513AC0[ctx] = p;
}
