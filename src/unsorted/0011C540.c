/* UNIT 0x0011C540 0x0011C5F8 */
typedef struct Rider { char pad0[0x40C0]; char x40C0[0x7BD0 - 0x40C0]; } Rider;
typedef struct Obj { char pad0[0x501A8]; int x501A8; } Obj;

extern float func_00220D98(void);
extern float func_001385A8(void *);
extern void func_0012D660(Rider *, int, float);

void func_0011C540(Obj *obj, int arg)
{
    int i;

    for (i = 0; i < obj->x501A8; i++) {
        Rider *rider = &((Rider *)obj)[i];
        if (rider) {
            float scale = func_00220D98();
            float speed = func_001385A8(rider->x40C0);
            func_0012D660(rider, arg, scale * (speed * speed));
        }
    }
}
