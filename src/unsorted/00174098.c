/* UNIT 0x00174098 0x00174230 */
extern float fabsf(float);

typedef struct Sub { char pad0[0xA0]; float xA0; } Sub;
typedef struct Params { char pad0[0x884]; float x884; } Params;
typedef struct Obj {
    char pad0[0xC];
    Sub *xC;
    Params *x10;
    char pad14[0xC];
    float x20[4];
    char pad30[0x10];
    float *x40;
    float *x44;
    char pad48[0x18];
    float x60;
    char pad64[0xC];
    float x70;
    char pad74[0x1C];
    float x90[4];
    char padA0[0x18];
    float xB8;
    float xBC;
} Obj;

extern void func_002087C0(float *);
extern float func_00222690(float *);
extern float func_00235EA8(float, float, float, float);
extern float func_002487A0(float);

void func_00174098(Obj *obj) {
    float delta[4];
    Params *params;
    float speed, angle, limit, mag, clipped, result, turn, t;

    if (obj->xB8 > 0.24444444f && obj->x44 != obj->x40) {
        __asm__ volatile ("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%2)"
                          : : "r"(obj->x20), "r"(obj->x40), "r"(delta) : "memory");
        speed = func_00222690(delta);
    } else {
        t = func_00222690(obj->x90);
        t = t * func_002487A0(obj->xBC);
        speed = t;
        func_002087C0(&speed);
    }
    result = func_00235EA8(obj->x60, speed, 0.1f, 0.001f);
    obj->x60 = result;

    angle = (result * 0.5f - obj->xC->xA0) * 0.5f;
    func_002087C0(&angle);
    /* Zero-byte labels (same device as src/unsorted/0014A580.c and 00155A20.c):
       the first keeps the obj->x10 load ahead of the angle load, the second
       makes the compare's |angle| a value cse can't reuse, so the then-branch
       recomputes |angle| * 0.1 as retail does. */
    params = obj->x10;
    __asm__ volatile ("L00174098_params:" : "+r"(params));
    limit = params->x884;
    mag = fabsf(angle);
    __asm__ volatile ("L00174098_mag:" : "+f"(mag));
    if (mag * 0.1f < limit)
        limit = fabsf(angle) * 0.1f;
    if (limit < angle)
        angle = limit;
    else {
        clipped = -limit;
        if (!(angle < clipped))
            clipped = angle * 0.5f;
        angle = clipped;
    }

    turn = func_00235EA8(obj->x70, angle, 0.02f, 0.001f);
    obj->x70 = turn;
    obj->xC->xA0 += turn;
    func_002087C0(&obj->xC->xA0);
}
