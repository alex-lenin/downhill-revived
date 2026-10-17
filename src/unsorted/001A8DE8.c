/* UNIT 0x001A8DE8 0x001A8EF0 */
typedef struct Sub {
    int x0;
    int x4;
} Sub;

typedef struct Obj {
    char pad0[0x10];
    Sub *x10;
    char pad14[0x24 - 0x14];
    int x24;
    char pad28[0x44 - 0x28];
    unsigned char x44;
    char pad45[3];
    signed char x48;
} Obj;

extern int D_0043F348[];
extern int D_004B26AC[];
extern char D_004B1F40[];

extern void func_001A9EA8(Obj *obj);
extern int func_001A9058(Obj *obj);
extern void func_001AA0F0(Obj *obj);
extern void func_001B45F8(void *p, int a, int b);
extern void func_001A8EF0(Obj *obj, int a);
extern void func_001A9230(Obj *obj);

int func_001A8DE8(Obj *obj)
{
    int result;

    if (D_0043F348[0] < 2 || D_0043F348[0] > 4) {
        return 1;
    }
    result = 1;
    if (D_004B26AC[0] == 10) {
        func_001A9EA8(obj);
    } else if (obj->x44) {
        if (obj->x24 < obj->x10->x4) {
            result = func_001A9058(obj);
        } else {
            result = 0;
            func_001AA0F0(obj);
            func_001B45F8(D_004B1F40, 6, 0);
        }
    } else {
        obj->x10->x4 = obj->x24;
        if (obj->x10->x4 < 0x626F) {
            func_001A8EF0(obj, 0);
            obj->x24++;
        }
        if (obj->x48) {
            func_001A9230(obj);
        }
    }
    return result;
}
