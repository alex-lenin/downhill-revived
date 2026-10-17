/* UNIT 0x00145368 0x00145480 .rodata=0x289C70:0x289C90 */
typedef struct Thing {
    char pad0[4];
    unsigned char active;
} Thing;

typedef struct Anim {
    int x0;
    int x4;
    int x8;
} Anim;

typedef struct Obj {
    char pad0[4];
    unsigned int state;
    Thing *thing;
    char padC[0xB0 - 0xC];
    Anim anim;
    char padBC[0x1A8 - 0xBC];
    int time;
} Obj;

extern int func_00238F60(void);
extern void func_001450A0(Obj *, int);
extern void func_00145480(Obj *);
extern void func_00145768(Obj *);
extern void func_001458E0(Obj *);
extern void func_001454D8(Obj *);
extern void func_00145920(Obj *);
extern int func_00163DA0(int, int, int);
extern void func_002182A8(Thing *, int);

void func_00145368(Obj *obj)
{
    Anim *anim;

    if (obj->thing == 0 || obj->thing->active == 0) {
        return;
    }
    obj->time += func_00238F60();
    switch (obj->state) {
    case 0:
        func_001450A0(obj, 1);
        break;
    case 1:
        func_00145480(obj);
        break;
    case 2:
        func_00145768(obj);
        break;
    case 3:
        func_001458E0(obj);
        func_001454D8(obj);
        break;
    case 4:
        func_00145920(obj);
        break;
    }
    anim = &obj->anim;
    if (func_00163DA0(obj->anim.x0, anim->x8, 0) == anim->x4) {
        if (obj->state != 6) {
            func_002182A8(obj->thing, 0);
            func_001450A0(obj, 6);
        }
    }
}
