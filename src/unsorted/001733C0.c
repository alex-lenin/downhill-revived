/* UNIT 0x001733C0 0x00173560 */
typedef struct { float v[4]; } Vec;
typedef struct State { char pad[0xa4]; float xA4, xA8; } State;
typedef struct Params { char pad[0x86c]; float x86C; char pad870[0x14]; float x884; } Params;
typedef struct Obj { char pad[0xc]; State *xC; Params *x10; char pad14[0x54]; float x68; char pad6c[0xc]; float x78; char pad7c[4]; Vec x80; char pad90[0x28]; float xB8, xBC; } Obj;
extern float func_00222640(Vec *);
extern void func_002087C0(float *);
extern float func_00235EA8(float, float, float, float);
extern float fabsf(float);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_001733C0(Obj *obj) {
 Vec v; float angle, limit, mag, value, result, turn, half; Params *params;
 COPY_VECTOR(v, &obj->x80);
 v.v[2] = 0.0f;
 result = func_00222640(&v);
 obj->x68 = result;
 obj->xBC = result - obj->xC->xA8;
 func_002087C0(&obj->xBC);
 half = 0.5f;
 angle = obj->xBC * half;
 /* Zero-byte labels (as in src/unsorted/00174098.c): keep the 0.1f load after
    |angle| and make the then-branch recompute |angle| * 0.1f, as retail does. */
 params = obj->x10;
 __asm__ volatile ("L001733C0_params:" : "+r"(params));
 limit = params->x884;
 mag = fabsf(angle);
 __asm__ volatile ("L001733C0_mag:" : "+f"(mag));
 if (mag * 0.1f < limit) limit = fabsf(angle) * 0.1f;
 if (angle > limit) angle = limit;
 else if (angle < -limit) angle = -limit;
 else angle *= half;
 turn = func_00235EA8(obj->x78, angle, 0.03f, 0.001f);
 obj->x78 = turn;
 obj->xC->xA8 += turn;
 func_002087C0(&obj->xC->xA8);
 value = obj->xBC;
 if (obj->xB8 < obj->x10->x86C) value = 0.0f;
 else if (value > 0.2617994f) value = 0.2617994f;
 else if (value < -0.2617994f) value = -0.2617994f;
 obj->xC->xA4 = func_00235EA8(obj->xC->xA4, value, 0.02f, 0.001f);
}
