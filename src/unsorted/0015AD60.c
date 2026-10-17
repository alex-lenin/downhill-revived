/* UNIT 0x0015AD60 0x0015AE10 */
#include "rider_geometry.h"
#include "npc_motion.h"

extern float func_00238F40(void);
extern void func_00236558(void *, void *, float);
extern void func_0015BEA0(void *, float);

extern void func_0015B798(void *);
extern int func_00222BD8(int, int);
extern void func_00159CE0(void *, int);
void func_0015AD60(char *obj) {
    float vector[4];
    void *p = obj + 0x140;
    func_00260E08(obj + 0xF0, p, obj + 0x150, vector);
    func_00236558(p, vector, func_00238F40());
    func_0015BEA0(obj, 0.017111111432313919f);
    func_0015A968((NpcMotionActor *)(obj));
    func_0015B798(obj);
    if (*(short *)(obj + 0x1E0) >= 181 && func_00222BD8(0, 100) == 42)
        func_00159CE0(obj, 1);
}
