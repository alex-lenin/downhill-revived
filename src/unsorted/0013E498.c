/* UNIT 0x0013E498 0x0013E610 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[0x490];
    u128 x490;
    char pad4A0[0x70];
    int x510;
    int x514[64];
    int x614[64];
    char x714[12];
    char x720[0x5C0];
    int xCE0;
} Obj;

typedef struct Global {
    char pad[0x12208];
    int x12208;
} Global;

extern Global D_004225A0;
extern char D_003F52D0[];
extern int func_00194008(Global *, int);
extern int func_00194050(Global *, int);
extern int func_00194098(Global *, int);
extern void func_0013E640(void *, int, int, int, int);
extern void func_00190660(void *, void *);

void func_0013E498(Obj *obj, int index)
{
    int half = 0;
    int count = obj->x514[index];

    switch (obj->x510) {
    case 1: half = func_00194008(&D_004225A0, obj->x614[index]) > 0; break;
    case 2: half = func_00194050(&D_004225A0, obj->x614[index]) > 0; break;
    case 3: half = func_00194098(&D_004225A0, obj->x614[index]) > 0; break;
    }
    if (half) count /= 2;
    if (D_004225A0.x12208 / 20 >= count) {
        func_0013E640(obj->x720, obj->x510, obj->x614[index], obj->x514[index], half);
        func_00190660(D_003F52D0, obj->x720);
    } else {
        obj->xCE0 = 60;
        obj->x490 = (u128)0x4C45434E41435F53L;
    }
}
