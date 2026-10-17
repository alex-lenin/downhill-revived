/* UNIT 0x00184848 0x00184888 */
typedef struct Racer {
    char pad0[0x104];
    unsigned char x104;
    unsigned char x105;
} Racer;

typedef struct World {
    char pad0[0x78D0];
    Racer *x78D0;
} World;

typedef struct Obj {
    World *x0;
    int x4;
    int x8;
} Obj;

void func_00184848(Obj *obj)
{
    Racer *r = obj->x0->x78D0;

    if (r != 0) {
        if (r->x104) {
            obj->x8 = 0x31;
        } else if (r->x105) {
            obj->x8 = 0x30;
        }
    }
}
