/* UNIT 0x0023C2A0 0x0023C2E0 */
typedef struct Obj { char pad0[0x48]; int x48; } Obj;
typedef struct Params { long x0, x8; int x10, x14; } Params;
extern char D_00663BA0[];
extern int func_0023BE10(void *, Params *);
int func_0023C2A0(Obj *obj, long a, long b, int c, int d) {
    Params params;
    params.x0 = a;
    params.x8 = b;
    params.x10 = c - obj->x48;
    params.x14 = d;
    return func_0023BE10(D_00663BA0, &params);
}
