/* UNIT 0x00123760 0x00123788 */
typedef struct Obj {
    char pad0[0x3C];
    int timer; /* 0x3C */
    char pad40[0x144 - 0x40];
    int state; /* 0x144 */
} Obj;

void func_00123760(Obj *obj, int state)
{
    if (state != obj->state && obj->timer <= 0) {
        obj->state = state;
        obj->timer = 120;
    }
}
