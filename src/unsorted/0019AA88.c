/* UNIT 0x0019AA88 0x0019AB50 */
typedef unsigned int u128 __attribute__((mode(TI)));

/* 16-byte entries starting at +0x10, short type at +0xC */
typedef struct Slot {
    char pad0[0xC];
    short type;
    char padE[0x2];
} Slot;

typedef struct Obj {
    char pad0[0x490];
    u128 x490;
    char pad4A0[0x50C - 0x4A0];
    char names[4][13];
    signed char x540[4];
} Obj;

typedef struct Global43DCC0 {
    char pad0[0x11];
    char x11;
    char x12;
    char pad13[0x4C - 0x13];
    int x4C;
} Global43DCC0;

extern Global43DCC0 D_0043DCC0;
extern char D_003F52D0[];

extern void func_0019BB30(Global43DCC0 *dst, char *name);
extern void func_00190688(void *p);
extern void func_00190890(void *p);
extern void func_0019AEE0(Obj *obj);
extern void func_00187AB0(void);

void func_0019AA88(Obj *obj, int index)
{
    int found;

    if (index < 4) {
        if (obj->x540[index] > 0) {
            func_0019BB30(&D_0043DCC0, obj->names[index]);
            D_0043DCC0.x12 = index + 1;
            D_0043DCC0.x11 = 0;
            D_0043DCC0.x4C = 0;
            func_00190688(D_003F52D0);
        } else {
            obj->x490 = 0;
        }
    } else {
        found = ((Slot *)(index * 16 + (char *)obj + 0x10))->type == 0x23;
        if (found) {
            func_0019AEE0(obj);
            func_00190890(D_003F52D0);
            func_00187AB0();
        }
    }
}
