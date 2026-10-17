/* UNIT 0x00145BD0 0x00145E08 */
typedef struct Body {
    char pad0[0x10]; float pos[4]; char pad20[0x20];
    float matrix[16]; char pad80[0x20]; float angles[3];
} Body;
typedef struct Obj {
    char pad0[8]; Body *x8; char padC[0x34]; float matrix[16];
    char pad80[0x12c]; int x1AC;
} Obj;
extern char D_002AF2C0[];
extern int func_0011C0D0(void *, void *, int, float);
extern void func_001DD030(void *, void *, int, int);
extern void func_00222460(void *, float *);
extern void func_00221F50(void *, float *);
extern void func_0020B880(float *, void *, float);
extern float fabsf(float);
extern float func_00235EA8(float, float, float, float);
static inline void WrapAngle(Body *body, int index, float target)
{
    float current = body->angles[index];
    float diff = target - current;
    float wrapped;

    /* Neither line emits code. The dead test keeps abs.s ahead of the pi load and
       in its own register. The zero-byte label (as in the promoted 0012DEB0.c) is
       volatile, so the delay-slot pass does not treat the reload of obj->x8 after
       the wrap as redundant and fills bc1fl with it, as retail does. */
    if (diff < 0.0f) wrapped = diff;
    __asm__ volatile ("1:");
    if (fabsf(diff) > 3.1415927f) {
        if (current < target) wrapped = current + 6.2831855f;
        else wrapped = current - 6.2831855f;
        body->angles[index] = wrapped;
    }
}
void func_00145BD0(Obj *obj, int arg1, int flag)
{
    float angles[4];
    Body *body;
    if ((obj->x1AC || func_0011C0D0(D_002AF2C0, obj->x8->pos, 1, 150.0f)) && !flag) {
        func_001DD030(obj->matrix, obj->x8->matrix, arg1, 0);
        func_00222460(obj->x8->matrix, angles);
        obj->x8->angles[2] = angles[2];
        WrapAngle(obj->x8, 0, angles[0]);
        obj->x8->angles[0] = func_00235EA8(obj->x8->angles[0], angles[0], 0.04f, 0.001f);
        WrapAngle(obj->x8, 1, angles[1]);
        obj->x8->angles[1] = func_00235EA8(obj->x8->angles[1], angles[1], 0.04f, 0.001f);
        body = obj->x8;
        func_00221F50(body->matrix, body->angles);
    } else {
        func_001DD030(obj->matrix, obj->x8->matrix, arg1, 0);
        body = obj->x8;
        func_0020B880(body->pos, body->matrix, 30.0f);
        body = obj->x8;
        func_00222460(body->matrix, body->angles);
    }
}
