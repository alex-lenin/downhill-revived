/* UNIT 0x00129FC8 0x0012A3E8 */
typedef struct Ref { int x0, x4, x8, xC; } __attribute__((packed)) Ref;
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Item { char pad0[0x10]; Vec pos; } Item;
typedef struct Anim { void *x0; int x4; short x8; short xA; } Anim;
typedef struct World { char pad0[0x5019C]; float x5019C; } World;
typedef struct Obj {
    int x0;
    Item *x4;
    Item *x8;
    char padC[0x24 - 0xC];
    void *x24;
    int state;              /* 0x28 */
    Ref x2C;
    char pad3C[0x4C - 0x3C];
    Ref x4C;
    char pad5C[0x6C - 0x5C];
    Anim x6C;
    char pad78[0x14C - 0x78];
    float x14C;
    char pad150[0x160 - 0x150];
    float x160;
    char pad164[0x16C - 0x164];
    float dist;             /* 0x16C */
    char pad170[0x194 - 0x170];
    int x194;
} Obj;

extern World D_002AF2C0;
extern float D_0029C250[];
extern int D_004B2840[];
extern int func_00163D70(void *, int);
extern void func_00129860(Obj *, int);
extern void func_002182A8(void *, int);
extern int func_0011C0D0(World *, Vec *, int, float);
extern void *func_0011B180(World *, Vec *, float *, Vec *, float);
extern int func_0011C168(World *, Vec *, float *, float);
extern int func_00222BD8(int, int);
extern void func_00204358(Ref *, int);
extern void func_00203DC8(Ref *, int);
extern void func_00203E10(Ref *, float);
extern void func_00202E38(Ref *, int, int, float);

void func_00129FC8(Obj *obj)
{
    Ref tmp;
    float range;
    Anim *anim;
    Ref *ref;
    int found;

    if (obj->x4 == 0) {
        return;
    }
    anim = &obj->x6C;
    found = func_00163D70(obj->x6C.x0, anim->x4);
    if (anim->xA == found) {
        func_00129860(obj, 9);
        func_002182A8(obj->x4, 0);
        func_002182A8(obj->x8, 0);
        return;
    }
    obj->dist = D_0029C250[0];
    if (func_0011C0D0(&D_002AF2C0, &obj->x4->pos, 0, 300.0f)) {
        obj->x24 = func_0011B180(&D_002AF2C0, &obj->x4->pos, &obj->dist, 0, 250.0f);
        if (obj->x24 != 0 && obj->dist < 122500.0f) {
            if (D_004B2840[0] != 3) {
                if (D_004B2840[0] != 4 && obj->dist < 100.0f && obj->x194 > 300) {
                    func_00129860(obj, 6);
                    goto sound;
                }
            }
            if (obj->state != 6) {
                if (obj->dist < 32400.0f) {
                    func_00129860(obj, 5);
                } else if (obj->state == 5) {
                    if (obj->dist > 32400.0f && func_00222BD8(0, 100) == 42) {
                        func_00129860(obj, 2);
                    }
                } else if ((unsigned int)(obj->state - 3) >= 2 && func_00222BD8(0, 13) == 1) {
                    func_00129860(obj, 4);
                }
            }
        } else if (obj->state != 4 && func_00222BD8(0, 13) == 1) {
            func_00129860(obj, 4);
        }
sound:
        if (func_0011C168(&D_002AF2C0, &obj->x4->pos, &range, 300.0f)) {
            ref = &obj->x4C;
            if (range < 40.0f) {
                func_00204358(ref, 0);
            } else if (range < 80.0f) {
                func_00204358(ref, 1);
            } else if (range < 160.0f) {
                func_00204358(ref, 2);
            } else {
                func_00204358(ref, 3);
            }
        }
    } else if (D_002AF2C0.x5019C + 200.0f < obj->x4->pos.z) {
        func_00129860(obj, 9);
    } else if (obj->state == 5 || (obj->state == 4 && obj->x14C > 0.0f)) {
        if (func_00222BD8(0, 100) == 42) {
            func_00129860(obj, 2);
        }
    } else if (obj->state == 4) {
        tmp = obj->x2C;
        func_00203DC8(&tmp, 0);
        tmp = obj->x2C;
        func_00203E10(&tmp, obj->x160);
        func_00202E38(&obj->x2C, 1, 1, 600.0f);
        func_00129860(obj, 1);
    }
}
