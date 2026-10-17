/* UNIT 0x0011A5E8 0x0011A938 */
typedef struct Pos { char pad[0x18]; float x18; } Pos;
typedef struct Racer {
    char pad0[0x4098];
    unsigned long x4098;
    char pad40A0[0x388C];
    Pos *x792C;
    char pad7930[0x138];
    unsigned char x7A68;
    char end[0x167];
} Racer;
typedef struct Obj {
    Racer *x0, *x4, *x8;
    char padC[0x394];
    int x3A0;
    char pad3A4[0x5C];
    int x400;
    char pad404[4];
    int x408, x40C;
    char pad410[0x30];
    unsigned long x440; /* bit 40: x4 holds the current owner */
} Obj;
typedef struct Slot { char pad[0x78D4]; Obj *x78D4; char tail[0x2F8]; } Slot;

extern float D_0029C148[]; /* FLT_MAX */
extern int D_0043EEC0[];
extern int func_00111A88(Obj *);
extern int func_0010E918(Obj *, void *, void *);

#define OWNED 0x10000000000UL

static inline Racer *owner(Obj *o) { return (o->x440 & OWNED) ? o->x4 : 0; }
static inline float maxf(float b, float a) { float v; __asm__ volatile ("max.s %0,%1,%2" : "=&f"(v) : "f"(a), "f"(b)); return v; }
static inline float minf(float b, float a) { float v; __asm__ volatile ("min.s %0,%1,%2" : "=&f"(v) : "f"(a), "f"(b)); return v; }

void func_0011A5E8(Slot *slots, Racer *r, int count) {
    float backDist = D_0029C148[0], frontDist = backDist, d;
    Obj *front = 0, *back = 0;
    int keep, n;

    /* The do { } while (0) wrappers only add loop depth: gcc weights register
       references by it, which is what puts backDist, 30.0f and frontDist in
       $f3, $f4, $f5 like retail. */
    n = 0;
    if (n < count) do {
        Obj *o;
        for (; n < count; n++) {
            o = ((volatile Slot *)slots)[n].x78D4;
            if (o && ((o->x3A0 >> 2) & 1) == 0 && o->x8 == r
                && ((unsigned char)(r->x4098 >> 2) & 1) == 0 && !r->x7A68) {
                d = r->x792C->x18 - o->x0->x792C->x18;
                if (d > 0.0f) {
                    if (owner(o) == r) do { d = maxf(d - 30.0f, 0.0f); } while (0);
                    if (d < frontDist) { front = o; frontDist = d; }
                } else {
                    if (owner(o) == r) d = minf(d + 30.0f, 0.0f);
                    d = -d;
                    if (d < backDist) { back = o; backDist = d; }
                }
            }
        }
    } while (0);

    {
        Obj *o;
        for (n = 0; n < count; n++) {
            o = ((volatile Slot *)slots)[n].x78D4;
            if (o && owner(o)) {
                keep = 1;
                if (owner(o)->x7A68) keep = 0;
                else if (func_00111A88(o) < 4 && o->x408 < 120) keep = 0;
                else if (o->x40C < 1200) keep = 0;
                else if (o != back && o != front) keep = 0;
                if (!keep) { o->x440 &= ~OWNED; o->x400 = 0; o->x4 = o->x8; }
            }
        }
    }

    if (D_0043EEC0[2] >= 421) {
        if (front && (func_00111A88(front) >= 4
                      || func_0010E918(front, (char *)front->x8->x792C + 16, (char *)front->x0->x792C + 16))) {
            front->x400 = 0; front->x4 = front->x8; front->x440 = (front->x440 & ~OWNED) | OWNED;
        }
        if (back && !owner(back)) {
            back->x400 = 0; back->x4 = back->x8; back->x440 = (back->x440 & ~OWNED) | OWNED;
        }
    }
}
