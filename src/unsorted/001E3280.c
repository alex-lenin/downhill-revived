/* UNIT 0x001E3280 0x001E3538 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct {
    int x0;
    char pad4[0x414];
    int x418;
    char pad41c[0x40];
    int x45c;
    char pad460[0xa4];
    int x504;
    int x508;
} Object;

typedef struct {
    char pad[0x500];
    int x500;
    char pad504[0x50];
    unsigned char x554[4];
} MenuState;

typedef struct {
    char pad[0x64];
    char keys[0x100];
} Input;

extern MenuState D_004C37A0;
extern Input D_003F52D0;
extern char D_004B2A90[];
extern char D_004C3D00[];
extern char D_00293AC0[];
extern char D_00293AD0[];
extern unsigned char D_004C3CC0[];
extern void *D_0026EFE8[];

extern void func_00190358(void *);
extern int func_00190D60(void *, int, int);
extern void func_001E2958(void *, int);
extern void func_00190660(void *, void *);
extern void func_001C57A8(void *, u128);
extern void func_001E2FB0(void *, int);
extern void func_0018E408(void *, void *, int);
extern void func_001E2968(void *);
extern void func_0018DC20(void *);

#define SOUND_OK ((u128)((0xA8868A988AA6UL << 15) | 0x5f53))
#define SOUND_BAD ((u128)((0x988A869C8286UL << 15) | 0x5f53))

void func_001E3280(Object *obj)
{
    Input *base = &D_003F52D0;
    char *input;
    int i;
    int count;
    int size;
    unsigned char *sizes;
    MenuState *menu;
    unsigned char value;

    func_00190358(base);
    input = base->keys;
    if (func_00190D60(input, 6, obj->x45c)) {
        short firstSize;

        firstSize = D_004C37A0.x554[D_004C37A0.x500];
        if (firstSize < 12) {
            obj->x504 = obj->x0;
            obj->x508 = 0;
            func_00190660(base, D_004C3D00);
            func_001C57A8(D_004B2A90, SOUND_OK);
        } else {
            func_001E2958(&D_004C37A0, 2);
            func_001C57A8(D_004B2A90, SOUND_BAD);
        }
    } else if (func_00190D60(input, 7, obj->x45c) && (size = D_004C37A0.x554[D_004C37A0.x500]) > 0) {
        if (obj->x0 < size) {
            obj->x504 = obj->x0;
            obj->x508 = 1;
            func_00190660(base, D_004C3D00);
            func_001C57A8(D_004B2A90, SOUND_OK);
        }
    } else if (func_00190D60(input, 10, obj->x45c)) {
        int sz;

        menu = &D_004C37A0;
        sizes = menu->x554;
        sz = sizes[menu->x500];
        if (sz > 0 && obj->x0 < sz) {
            i = 0;
            func_001C57A8(D_004B2A90, SOUND_OK);
            func_001E2FB0(menu, obj->x0);
            obj->x418 = 0;
            count = sizes[menu->x500] + 1;
            for (; i < count;) {
                if (count == 1)
                    func_0018E408(obj, D_00293AD0, 1);
                else {
                    value = D_004C3CC0[menu->x500 * 13 + i];
                    if (value == 33)
                        func_0018E408(obj, D_00293AC0, 1);
                    else
                        func_0018E408(obj, D_0026EFE8[value], 1);
                }
                i++;
            }
        }
    }
    func_001E2968(&D_004C37A0);
    func_0018DC20(obj);
}
