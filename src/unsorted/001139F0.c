/* UNIT 0x001139F0 0x00113B08 */
typedef struct Switches {
    char pad0[0x1A4];
    int x1A4[50];
    int x26C[50];
    char pad334[0x3F4 - 0x334];
    unsigned char x3F4[50];
} Switches;

typedef struct Game {
    char pad0[0xFE0];
    Switches xFE0;
} Game;

typedef struct Obj {
    Game *x0;
    char pad4[0x2D4 - 0x4];
    int x2D4;
    char pad2D8[0x410 - 0x2D8];
    int x410;
    char pad414[0x41C - 0x414];
    int x41C;
    char pad420[0x440 - 0x420];
    long x440;
} Obj;

extern int func_00238F60(void);
extern int func_00113B08(Obj *);
extern void func_001162D0(int *);

static inline void set_switch(Switches *sw, int i, int on) {
    if (sw->x3F4[i]) {
        sw->x26C[i] = on;
    } else {
        sw->x1A4[i] = on;
        sw->x26C[i] = 0;
    }
}

void func_001139F0(Obj *obj) {
    int done = 0;
    Switches *sw;

    obj->x41C += func_00238F60();
    if (obj->x440 & 0x4000000000000) {
        if (func_00113B08(obj)) {
            if (obj->x2D4 == 0) {
                sw = &obj->x0->xFE0;
                set_switch(sw, 5, 1);
                set_switch(sw, 17, 1);
            } else {
                sw = &obj->x0->xFE0;
                set_switch(sw, 6, 1);
                set_switch(sw, 19, 1);
                obj->x410 = 0;
            }
        } else {
            done = obj->x41C > 300;
        }
    } else {
        done = 1;
    }
    if (done) {
        obj->x2D4 = 4;
        func_001162D0(&obj->x2D4);
        obj->x41C = 0;
    }
}
