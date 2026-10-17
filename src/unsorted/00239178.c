/* UNIT 0x00239178 0x00239540 .rodata=0x299360:0x299380 */
typedef int Quad __attribute__((mode(TI)));
typedef struct Vec { float x, y, z, w; } Vec;
typedef union { Quad q; unsigned int words[4]; Vec v; } Entry;
typedef struct Obj {
    char pad0[0x2610];
    Entry x2610;
    Entry x2620;
    Entry x2630;
    char pad2640[0x50];
    Entry x2690;
    char pad26A0[0x17C];
    int x281C;
    char pad2820[0x10];
    Entry *x2830;
    Entry *x2834;
    int *x2838;
    char pad283C[0x14];
    int x2850;
    int x2854;
    char pad2858[2];
    short x285A;
} Obj;

extern void func_00223628(void *, void *);
extern void func_002390D0(Obj *, Entry *, int, int);
extern void func_00239148(Obj *);
extern void func_00227518(Obj *);

/* vf30/vf31 hold the running bounding box minimum/maximum */
static inline void LoadBox(void *min, void *max) {
    __asm__ volatile ("lqc2 $vf30,0(%0)\n\tlqc2 $vf31,0(%1)" : : "r"(min), "r"(max));
}
static inline void AddPoint(void *p) {
    __asm__ volatile ("lqc2 $vf11,0(%0)\n\tvmini.xyzw $vf30,$vf30,$vf11\n\tvmax.xyzw $vf31,$vf31,$vf11" : : "r"(p));
}
static inline void Sub(Vec *out, void *a, void *b) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a), "r"(b));
}
static inline float Length(Vec *v) {
    float len;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len) : "f"(v->x), "f"(v->y), "f"(v->z));
    return len;
}
static inline float Dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}
/* the new point continues the last segment in a straight line (within about 8 degrees) */
static inline unsigned char IsStraight(Obj *obj, Entry *pos, Vec *delta) {
    if (obj->x2850 >= 2) {
        Sub(delta, pos, &obj->x2830[obj->x2850 - 2]);
        if (Length(delta) < 3.0f && Dot(&obj->x2834[0].v, &obj->x2834[1].v) > 0.990268052f) {
            return 1;
        }
    }
    return 0;
}

void func_00239178(Obj *obj) {
    int old = obj->x2850;
    int start = old + obj->x285A - obj->x2854;
    int count, flags;
    Entry *src, *dst;
    int *srcv, *dstv;
    int i, j;
    Entry tmp;

    if (start < 0) start = 0;
    src = &obj->x2830[start];
    srcv = &obj->x2838[start];
    LoadBox(src, src);
    if (start != 0) {
        dst = obj->x2830;
        dstv = obj->x2838;
        for (i = start; i < obj->x2850; i++) {
            AddPoint(src);
            *dst++ = *src++;
            *dstv++ = *srcv++;
        }
    } else {
        for (j = 0; j < obj->x2850; j++) {
            AddPoint(src);
            src++;
        }
    }
    func_00223628(&obj->x2610, &obj->x2620);
    count = obj->x2850 - start;
    obj->x2850 = count;
    if (count != 0) flags = obj->x2830[count - 1].words[3] & 3;
    else flags = 4;
    if (obj->x285A != 0) {
        if (flags != 4) {
            LoadBox(&obj->x2610, &obj->x2620);
            AddPoint(&obj->x2630);
            func_00223628(&obj->x2610, &obj->x2620);
        }
        switch (flags) {
        case 3:
            obj->x2850--;
        case 0:
        case 4:
            tmp = obj->x2690;
            func_002390D0(obj, &tmp, obj->x281C, 2);
            obj->x2834[0] = obj->x2834[1];
            obj->x2850++;
            func_00239148(obj);
            break;
        case 2:
            tmp = obj->x2690;
            func_002390D0(obj, &tmp, obj->x281C, 1);
            obj->x2834[0] = obj->x2834[1];
            obj->x2850++;
            break;
        case 1:
            if (IsStraight(obj, &obj->x2630, &tmp.v)) obj->x2850--;
            else obj->x2834[0] = obj->x2834[1];
            tmp = obj->x2690;
            func_002390D0(obj, &tmp, obj->x281C, 1);
            obj->x2850++;
            break;
        }
    } else {
        switch (flags) {
        case 2: {
            Entry *last = &obj->x2830[obj->x2850 - 1];
            last->words[3] |= 3;
            break;
        }
        case 1: {
            Entry *last;
            func_00239148(obj);
            last = &obj->x2830[obj->x2850 - 1];
            last->words[3] &= ~3;
            break;
        }
        }
    }
    if (obj->x2850 != 0 && obj->x2850 != old) func_00227518(obj);
}
