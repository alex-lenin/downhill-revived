/* UNIT 0x0017CD70 0x0017CE98 */
typedef struct Vec3 { float x, y, z; } Vec3;

typedef struct Inst {
    union {
        unsigned long flags;
        struct { int x0; unsigned char x4; } b;
    } u;
    float x8;
    char padC[0x4];
    Vec3 pos;
    char pad1C[0x24];
    char x40[1];
} Inst;

typedef struct Obj {
    char pad0[0x4];
    Vec3 pos;
    char pad10[0x4];
    Inst *inst;
    int x18;
    int *x1C;
    char pad20[0x4];
    float x24;
} Obj;

extern Inst *func_00208FB8(void);
extern void func_00211280(Inst *);
extern float func_00238F40(void);
extern void func_00209248(Inst *, Vec3 *, void *, float);

int func_0017CD70(Obj *obj)
{
    Inst *inst;
    Inst *target;
    Vec3 *pos;
    float x, y, z;

    if (obj->inst == 0) {
        inst = func_00208FB8();
        obj->inst = inst;
        if (inst != 0) {
            x = obj->pos.x;
            y = obj->pos.y;
            z = obj->pos.z;
            pos = &inst->pos;
            pos->x = x;
            pos->y = y;
            pos->z = z;
            obj->inst->u.flags |= (unsigned long)0x8000 << 42;
            obj->inst->u.b.x0 = obj->x18;
            obj->inst->u.flags |= (unsigned long)0x8000 << 43;
            obj->inst->x8 = obj->x24;
            obj->inst->u.b.x4 = 1;
            obj->inst->u.flags &= (unsigned long)-1 >> 1;
            func_00211280(obj->inst);
            target = obj->inst;
            func_00209248(obj->inst, &target->pos, target->x40, func_00238F40());
            *obj->x1C = (*obj->x1C & ~0x3F) | 9;
        }
    }
    return obj->inst != 0;
}
