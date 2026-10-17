/* UNIT 0x001627C8 0x00162920 */
#include "particle.h"
/* Retains retail call scheduling; guarded PC permutations remain where needed. */
typedef struct Effect {
    char pad0[0x2724];
    unsigned short x2724;
    unsigned short x2726;
    char pad2728[0x2848 - 0x2728];
    struct Effect **x2848;
} Effect;

typedef struct Obj {
    char pad0[0x792C];
    char *x792C;
    char pad7930[0x7988 - 0x7930];
    int x7988;
    char pad798C[0x7A44 - 0x798C];
    Effect *x7A44;
    char pad7A48[4];
    Effect *x7A4C;
    Effect *x7A50;
    Effect *x7A54;
} Obj;

extern Effect *func_0022DD88(int handle);
extern void func_0022DDD8(Effect *effect);
extern void func_00239728(int x);

void func_001627C8(Obj *obj)
{
    int h0;
    int h1;
    int h2;
    Effect *e0;
    Effect *e1;
    Effect *e2;

    h0 = func_0022CFB8((Vector4 *)(obj->x792C + 0x10), obj->x792C + 0x40, 0x32, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
    h1 = SpawnParticleScaleBeforeDirection((Vector4 *)(obj->x792C + 0x10), obj->x792C + 0x40, 0x26, 1.0f, (Vector4 *)(0), (Matrix4 *)(0), (int)(0), (int)(0));
    h2 = func_0022CFB8((Vector4 *)(obj->x792C + 0x10), obj->x792C + 0x40, 0x27, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
    e0 = func_0022DD88(h0);
    e1 = func_0022DD88(h1);
    e2 = func_0022DD88(h2);
    if (e0 != 0) {
        func_0022DDD8(obj->x7A4C);
        obj->x7A4C = e0;
        e0->x2848 = &obj->x7A4C;
        obj->x7A44->x2724 = e0->x2726;
    }
    if (e1 != 0) {
        func_0022DDD8(obj->x7A50);
        obj->x7A50 = e1;
        e1->x2848 = &obj->x7A50;
    }
    if (e2 != 0) {
        func_0022DDD8(obj->x7A54);
        obj->x7A54 = e2;
        e2->x2848 = &obj->x7A54;
    }
    func_00239728(obj->x7988);
}
