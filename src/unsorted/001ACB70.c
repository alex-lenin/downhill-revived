/* UNIT 0x001ACB70 0x001ACBA0 */
typedef struct Target { char pad0[0x1C]; float x1C; } Target;
typedef struct Link { int x0; Target *x4; } Link;
typedef struct Obj { Link *x0; char pad4[0x38]; float x3C; } Obj;
void func_001ACB70(Obj *obj, float value) {
    Link *link = obj->x0;
    if (link && link->x0) {
        Target *target = link->x4;
        obj->x3C = value;
        target->x1C = value;
    }
}
