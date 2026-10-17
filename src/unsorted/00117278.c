/* UNIT 0x00117278 0x00117308 */
typedef struct Other {
    char pad0[0x414];
    int x414;
} Other;

typedef struct Obj {
    char pad0[0x58];
    char *x58;
    Other *x5C;
    char pad60[0x4C];
    int xAC;
} Obj;

extern int func_00138560(void *p);
extern int func_00238F60(void);

void func_00117278(Obj *obj)
{
    if (obj->x5C != 0 && obj->x5C->x414 < 180) {
        int value = 7;

        if (value < obj->xAC) {
            value = obj->xAC;
        }
        obj->xAC = value;
    } else if (!(unsigned char)func_00138560(obj->x58 + 0x40C0) && obj->xAC != 0) {
        int value = obj->xAC - func_00238F60();

        if (value < 0) {
            value = 0;
        }
        obj->xAC = value;
    }
}
