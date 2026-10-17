/* UNIT 0x001140A0 0x00114390 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Switches {
    char pad0[0x1A4];
    int x1A4[50];
    int x26C[50];
    char pad334[0x3F4 - 0x334];
    unsigned char x3F4[50];
} Switches;

typedef struct Rider {
    char pad0[0x18];
    float x18;
    char pad1C[0x40 - 0x1C];
    u128 x40[4];
} Rider;

typedef struct Game {
    char pad0[0xFE0];
    Switches xFE0;
    char pad1408[0x4308 - 0x1408];
    float x4308;
    char pad430C[0x4558 - 0x430C];
    float x4558;
    float x455C;
    char pad4560[0x4564 - 0x4560];
    short x4564;
    char pad4566[0x792C - 0x4566];
    Rider *x792C;
} Game;

typedef struct Obj {
    Game *x0;
    char pad4[4];
    Game *x8;
    char padC[0x90 - 0xC];
    u128 x90[4];
    char padD0[0x2B0 - 0xD0];
    float x2B0;
    char pad2B4[0x2CC - 0x2B4];
    float x2CC;
    char pad2D0[0x2D4 - 0x2D0];
    int x2D4;
    char pad2D8[0x388 - 0x2D8];
    int x388;
    char pad38C[0x390 - 0x38C];
    int x390;
    int x394;
    char pad398[0x3A0 - 0x398];
    int x3A0;
    char pad3A4[0x41C - 0x3A4];
    int x41C;
    char pad420[0x440 - 0x420];
    unsigned long x440;
} Obj;

typedef struct Sub {
    int x0;
    char pad4[0xBC - 4];
    int xBC;
    int xC0;
    char padC4[0xCC - 0xC4];
    int xCC;
} Sub;

static __inline__ void set_switch(Switches *sw, int i, int on) {
    if (sw->x3F4[i]) {
        sw->x26C[i] = on;
    } else {
        sw->x1A4[i] = on;
        sw->x26C[i] = 0;
    }
}

extern int func_00113B08(Obj *);
extern int func_00114390(Obj *);
extern int func_00222BD8(int, int);
extern float func_00238F38(void);
extern float D_0029C374;
extern float D_0029C378;

void func_001140A0(Obj *obj) {
    Sub *s;
    Switches *sw;
    unsigned long m;
    int r;
    int v;

    if ((obj->x440 & 0x4000000000000UL) && !obj->x388 && obj->x8
        && (obj->x8->x792C->x18 - obj->x0->x792C->x18) > -10.0f
        && func_00113B08(obj)) {
        obj->x2D4 = 0;
        obj->x41C = 0;
    }
    s = (Sub *)&obj->x2D4;
    if (obj->x2D4 == 4 && ((s->xCC >> 9) & 1) && func_00114390(obj)
        && obj->x0->x4564 < 20) {
        m = 0x8000UL << 40;
        obj->x440 |= m;
        r = func_00222BD8(0, 100) < s->xC0;
        obj->x440 = (obj->x440 & ~(0x8000UL << 42)) | ((long)r << 57);
        /* 64-byte matrix copy (as sceVu0CopyMatrix) */
        __asm__ volatile (
            "lq $8, 0x0(%4)\n\t"
            "lq $9, 0x10(%4)\n\t"
            "lq $10, 0x20(%4)\n\t"
            "lq $11, 0x30(%4)\n\t"
            "sq $8, %0\n\t"
            "sq $9, %1\n\t"
            "sq $10, %2\n\t"
            "sq $11, %3"
            : "=m"(obj->x90[0]), "=m"(obj->x90[1]), "=m"(obj->x90[2]), "=m"(obj->x90[3])
            : "r"(&obj->x0->x792C->x40[0])
            : "$8", "$9", "$10", "$11");
        if (func_00222BD8(0, 100) < s->xBC) {
            obj->x440 |= 0x8000UL << 36;
            if (obj->x440 & m) {
                obj->x0->x4558 = D_0029C374;
            }
            if (obj->x440 & (0x8000UL << 42)) {
                set_switch(&obj->x0->xFE0, 6, 1);
            } else {
                set_switch(&obj->x0->xFE0, 5, 1);
            }
            v = 2;
        } else {
            obj->x440 |= 0x8000UL << 37;
            if (obj->x440 & m) {
                obj->x0->x455C = D_0029C378;
            }
            obj->x2B0 = 1.0f;
            sw = &obj->x0->xFE0;
            if (obj->x0->x4308 > 0.0f) {
                set_switch(&obj->x0->xFE0, 3, 1);
                sw->x26C[4] = 0;
                if (sw->x3F4[4]) {
                    asm volatile ("");
                    goto done;
                } else {
                    sw->x1A4[4] = 0;
                }
            } else {
                set_switch(&obj->x0->xFE0, 4, 1);
                sw->x26C[3] = 0;
                if (sw->x3F4[3]) {
                    asm volatile ("");
                    goto done;
                } else {
                    sw->x1A4[3] = 0;
                }
            }
done:
            v = 3;
        }
        obj->x2D4 = v;
        obj->x2CC += 1.0f / func_00238F38();
    }
}
