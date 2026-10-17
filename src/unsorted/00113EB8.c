/* UNIT 0x00113EB8 0x001140A0 */
typedef struct Switches {
    char pad0[0x1A4];
    int x1A4[50];
    int x26C[50];
    char pad334[0x3F4 - 0x334];
    unsigned char x3F4[50];
} Switches;

typedef struct Rider {
    char pad0[0x40];
    char x40[16];
} Rider;

typedef struct Game {
    char pad0[0x1190];
    int x1190;
    int x1194;
    char pad1198[0x11F4 - 0x1198];
    int x11F4;
    char pad11F8[0x1258 - 0x11F8];
    int x1258;
    int x125C;
    char pad1260[0x13D7 - 0x1260];
    unsigned char x13D7;
    unsigned char x13D8;
    char pad13D9[0x4308 - 0x13D9];
    float x4308;
    char pad430C[0x792C - 0x430C];
    Rider *x792C;
} Game;

typedef struct Obj {
    Game *x0;
    char pad4[0x90 - 0x4];
    char x90[0x40];
    char padD0[0x2B0 - 0xD0];
    float x2B0;
    char pad2B4[0x2D4 - 0x2B4];
    int x2D4;
    char pad2D8[0x440 - 0x2D8];
    unsigned long x440;
} Obj;

extern int func_00138560(void *);
extern void func_00221888(void *, void *, float *);
extern int func_00114430(void *);
extern void func_00116368(void *, int);

static __inline__ void set_switch(Switches *sw, int i, int on) {
    if (sw->x3F4[i]) {
        sw->x26C[i] = on;
    } else {
        sw->x1A4[i] = on;
        sw->x26C[i] = 0;
    }
}

void func_00113EB8(Obj *obj) {
    float v[4];
    int stop = 0;
    int active;
    Game *p;
    Switches *sw;

    if ((obj->x440 & (0x8000UL << 40)) && (func_00138560((char *)obj->x0 + 0x40C0) & 255)) {
        func_00221888(obj->x90, (char *)obj->x0->x792C + 0x40, v);
        p = obj->x0;
        if (p->x13D7)
            active = p->x1258;
        else
            active = p->x11F4;
        if (active) {
            if (v[1] > 0.4f && v[0] > 0.4f)
                stop = func_00114430(obj) == 0;
        } else {
            if (v[1] < -0.4f && v[0] > 0.4f)
                stop = func_00114430(obj) == 0;
        }
    } else {
        stop = 1;
    }
    if (stop) {
        obj->x2D4 = 4;
        obj->x440 &= ~(0x10UL << 48);
        func_00116368((char *)obj + 0x2D4, 4);
        return;
    }
    obj->x2B0 = 1.0f;
    p = obj->x0;
    sw = (Switches *)((char *)p + 0xFE0);
    if (p->x4308 > 0.0f) {
        set_switch((Switches *)((char *)p + 0xFE0), 3, 1);
        if (sw->x3F4[4])
            sw->x26C[4] = 0;
        else {
            sw->x1A4[4] = 0;
            sw->x26C[4] = 0;
        }
    } else {
        set_switch((Switches *)((char *)p + 0xFE0), 4, 1);
        if (sw->x3F4[3])
            sw->x26C[3] = 0;
        else {
            sw->x1A4[3] = 0;
            sw->x26C[3] = 0;
        }
    }
}
