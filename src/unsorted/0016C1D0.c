/* UNIT 0x0016C1D0 0x0016C298 */
typedef struct Global {
    char pad0[0x288];
    int count;
} Global;

typedef struct Game {
    char pad0[0x50130];
    void *riders[1];
} Game;

typedef struct Racer {
    char pad0[0x380];
} Racer;

typedef struct Obj {
    int x0;
    int x4;
    char pad8[0x5BDC - 0x8];
    int x5BDC;
} Obj;

extern Global D_004B25A0;
extern Game D_002AF2C0;
extern Racer D_0037EA60[];
extern char D_003965E0[];
extern void func_001487D8(Racer *racer, void *rider);
extern void func_002302B8(float value);
extern void func_0016C2A0(Obj *obj, int arg);
extern void func_001675D8(void *arg);

void func_0016C1D0(Obj *obj)
{
    int i;

    for (i = 0; i < D_004B25A0.count; i++) {
        void *rider = D_002AF2C0.riders[i];
        func_001487D8(&D_0037EA60[i], rider);
    }
    obj->x4 = 0;
    func_002302B8(700.0f);
    func_0016C2A0(obj, obj->x5BDC);
    func_001675D8(D_003965E0);
}
