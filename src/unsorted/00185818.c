/* UNIT 0x00185818 0x00185938 */
typedef struct Obj {
    int state;
    int x4;
    char names[0x64][0x20];
    int xC88;
    int xC8C;
} Obj;

typedef struct Slot {
    int x0;
    char pad4[0x10];
} Slot;

typedef struct Input {
    char pad0[0xC];
    int xC;
    int x10;
    char pad14[0xD0 - 0x14];
    Slot slots[1];
} Input;

extern char D_0029CD60[];
extern char D_003CBC00[];
extern char D_003CBC20[][0x40];

extern int func_00261F60(Input *);
extern int func_00254D68(char *, const char *, ...);
extern int func_00246960(int, int, char *, int, int, char *);
extern int func_002466A0(int, int *, int *);
extern char *func_00254544(char *, char *);

int func_00185818(Obj *o, Input *in, int *step)
{
    char path[0x30];
    int mode;
    int i;

    switch (*step) {
    case 0:
        o->xC88 = 0;
        if (in->slots[func_00261F60(in)].x0 == 0) {
            return 1;
        }
        func_00254D68(path, D_0029CD60, o->xC8C);
        func_00246960(in->x10, in->xC, path, 0, 100, D_003CBC00);
        (*step)++;
    case 1:
        mode = 13;
        if (!func_002466A0(1, &mode, &o->xC88)) {
            return 0;
        }
        for (i = 0; i < o->xC88; i++) {
            func_00254544(o->names[i], D_003CBC20[i]);
        }
        return 1;
    }
    return 0;
}
