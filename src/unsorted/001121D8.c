/* UNIT 0x001121D8 0x001122D8 */
typedef struct Target {
    char pad0[0x14];
    unsigned short flags; /* bit 6, bit 7 */
} Target;

typedef struct Obj {
    char pad0[0x74];
    Target *x74;
    Target *x78;
    Target *x7C;
    char pad80[0x240 - 0x80];
    float x240;
    char pad244[0x2B4 - 0x244];
    float x2B4;
    char pad2B8[0x448 - 0x2B8];
    long x448;
} Obj;

extern int D_004B2840[];
extern int func_00182290(int);

void func_001121D8(Obj *obj)
{
    int ok;
    Target *t;
    unsigned long low;

    if (obj->x7C != 0) {
        return;
    }
    ok = 1;
    if (func_00182290(D_004B2840[0]) == 0) {
        ok = obj->x448 & 1;
    }
    if (!ok) {
        return;
    }
    t = obj->x74;
    if ((t->flags >> 6) & 1) {
        obj->x7C = t;
        obj->x2B4 = 50.0f;
        return;
    }
    if (!((t->flags >> 7) & 1)) {
        if (!(obj->x240 < 50.0f)) {
            return;
        }
        if ((obj->x78->flags >> 6) & 1) {
            obj->x7C = obj->x78;
            obj->x2B4 = 50.0f;
        }
        t = obj->x78;
        low = (unsigned char)t->flags;
        if (!(low >> 7)) {
            return;
        }
    }
    obj->x7C = t;
    obj->x2B4 = 15.0f;
}
