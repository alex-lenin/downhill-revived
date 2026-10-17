/* UNIT 0x001B12B0 0x001B13A0 */
struct Vec {
    float x;
    float y;
    float z;
    float w;
};

struct Child {
    char pad0[0x30];
    float pos[3];
};

struct Obj {
    char pad0[0xA0];
    struct Vec xA0[3];
    char padD0[4];
    struct Child *xD4;
    struct Child *xD8;
    char xDC;
    char padDD[3];
    int xE0;
    int xE4;
    int xE8;
    char padEC[0x18];
    float x104;
    float x108;
};

void func_001B12B0(struct Obj *obj, int arg1, int arg2, int arg3) {
    int v0;
    int t1;
    int i;
    float len1, len2;
    struct Vec *v;
    struct Child *c1, *c2;
    float *p;
    int a0, v1;

    arg1 ^= 1;
    v0 = arg1 * 6 + arg2 * 3 + arg3 * 26;
    a0 = v0 + 2;
    v1 = v0 + 1;
    obj->xE4 = v1;
    obj->xE8 = a0;
    obj->xE0 = v0;
    (void)&t1;
    t1 = arg1;
    if (arg2) {
        t1 = arg1 ^ 1;
    }
    obj->xDC = t1 != 0;
    v0 = arg1;

    for (i = 0, v = obj->xA0; i < 3; i++, v++) {
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = (float)i * -10.0f;
    }

    c1 = obj->xD4;
    c2 = obj->xD8;
    p = c1->pos;
    __asm__ ("sqrt.s %0, %1" : "=f"(len1) : "f"(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]));
    obj->x104 = len1;

    p = c2->pos;
    __asm__ ("sqrt.s %0, %1" : "=f"(len2) : "f"(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]));
    obj->x108 = len2;
}
