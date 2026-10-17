/* UNIT 0x001AE258 0x001AE318 */
#include "animation.h"
typedef AnimationHandle Result;

typedef struct Callback {
    unsigned long type : 6;
    unsigned long rest : 26;
    void (*func)();
    void *arg;
} Callback;

typedef struct Level {
    char pad0[0x16D4];
    unsigned int **x16D4;
    char pad16D8[0xA8C];
    int x2164;
} Level;

typedef struct Obj {
    char pad0[0x58];
    Callback x58;
    char pad68[0x8];
    Result x70;
    char pad80[0x20];
    int xA0[3];
} Obj;

extern void func_001AE660();



void func_001AE258(Obj *obj, Level *level)
{
    Result res;
    int *timer;

    timer = obj->xA0;
    obj->xA0[0] = 0;
    timer[1] = 0;
    timer[2] = 0;
    func_002037D0(&obj->x70, **level->x16D4 >> 18, 0, level->x2164);
    obj->x58.type = 0x2B;
    obj->x58.func = func_001AE660;
    obj->x70.x8 = (int)&obj->x58;
    obj->x58.arg = obj;
    res = obj->x70;
    func_002033E0(&res, obj, 2, 10, 1.0f);
}
