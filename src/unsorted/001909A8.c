/* UNIT 0x001909A8 0x00190C88 */
typedef struct Entry {
    int x0;
    unsigned int x4;
    unsigned int x8;
} Entry;

typedef struct Obj {
    Entry entries[14];
    int xA8;
    unsigned char xAC;
    char padAD[3];
    int xB0;
    unsigned char xB4[4];
    unsigned char xB8[4][4];
} Obj;

typedef struct Global {
    char pad0[0x130];
    int x130;
} Global;

extern Global D_003F52D0;
extern int func_00238F48(void);
extern void func_0021F8E0(void);
extern int func_002205A0(int);
extern int func_00220AC0(int);
extern int func_002208E8(int, int);
extern int func_00220640(int, int, int);
extern int func_00220618(int, int);
extern int func_002201B0(int);
extern void func_0021F880(int);
extern void func_00190C88(Obj *, int, int, int);

void func_001909A8(Obj *obj)
{
    int i;
    int cur;

    if (obj->xAC != 0) {
        return;
    }
    if (obj->xB0 == func_00238F48()) {
        return;
    }
    obj->xB0 = func_00238F48();
    func_0021F8E0();
    for (i = 0; i < 4; i++) {
        int h = 0;
        h = func_002205A0(i);
        if (func_00220AC0(i) != 0) {
            obj->xB8[i][0] = func_002208E8(0, h);
            obj->xB8[i][1] = func_002208E8(1, h);
            obj->xB8[i][2] = func_002208E8(2, h);
            obj->xB8[i][3] = func_002208E8(3, h);
        } else {
            obj->xB8[i][0] = 0x80;
            obj->xB8[i][1] = 0x80;
            obj->xB8[i][2] = 0x80;
            obj->xB8[i][3] = 0x80;
        }
    }
    cur = D_003F52D0.x130;
    for (i = 0; i < 4; i++) {
        /* The redundant "= 0" initialisers here and on h are in the original:
           they change gcse's hash table size, which decides the stack slot order. */
        int j = 0;
        int k = 0;
        for (j = 0; j < 14; j++) {
            if (func_00220640(obj->entries[j].x0, func_002205A0(i), 1) != 0) {
                unsigned int v;
                v = obj->entries[j].x8 | (1 << i);
                obj->entries[j].x8 = (cur == i) ? (v | 0x10) : v;
            } else {
                unsigned int v;
                v = obj->entries[j].x8 & ~(1 << i);
                obj->entries[j].x8 = (cur == i) ? (v & ~0x10) : v;
            }
        }
        if (func_002201B0(func_002205A0(i)) == 0) {
            obj->xB4[i] = 1;
        }
        for (k = 0; k < 14; k++) {
            func_00190C88(obj, k, 0, i);
            if (i == cur) {
                func_00190C88(obj, k, 0, 4);
            }
        }
        if (obj->xB4[i] != 0) {
            for (j = 0; j < 14; j++) {
                int h = 0;
                h = func_002205A0(i);
                if (func_00220618(obj->entries[j].x0, h) != 0) {
                    unsigned int v;
                    v = obj->entries[j].x4 | (1 << i);
                    obj->entries[j].x4 = (cur == i) ? (v | 0x10) : v;
                    func_0021F880(h);
                    obj->xB4[i] = 0;
                }
            }
        }
    }
}
