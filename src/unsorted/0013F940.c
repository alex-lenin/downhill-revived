/* UNIT 0x0013F940 0x0013FAE0 .sbss=0x29DD28:0x29DD2C */
#include "menu_items.h"
#include "menu.h"
struct IntArray { int item[64]; };
struct ByteArray { signed char item[64]; };

typedef struct Obj {
    char pad0[0x418];
    int count;
    char pad41C[0x500 - 0x41C];
    struct IntArray values;
    struct IntArray types;
    struct ByteArray flags;
} Obj;
typedef struct Menu { char pad0[0x4F8]; void *x4F8; } Menu;
typedef struct Sub { char pad0[0x1E40]; char x1E40[4]; } Sub;
typedef struct Globals { char pad[0x12208]; int x12208; } Globals;

int D_0029DD28 __attribute__((section(".sbss"))) = 0;
extern char D_00286F30[];
extern char D_003F52D0[];
extern void *D_0029DD30;
extern Sub D_002AF2C0;
extern Menu D_00361E80;
extern Globals D_004225A0;

extern void func_0018E398(Menu *);
extern void func_0018DB20(Menu *);
extern void func_0018E638(Menu *, int, void *);
extern void func_00190660(void *, Menu *);
extern void func_0013F730(Obj *, int);
extern void func_001AC300(void *, int);

static inline int *IntAt(int index, struct IntArray *array)
{
    return index + array->item;
}

static inline signed char *ByteAt(int index, struct ByteArray *array)
{
    return &array->item[index];
}

void func_0013F940(Obj *obj, int index)
{
    int *value;
    signed char *flag;
    int count;
    int i;
    Sub *sub;
    int offset;
    char *base;

    offset = index * 4;
    base = (char *)obj + 0x500;
    value = (int *)(base + offset);
    if (*value == 0) {
        flag = ByteAt(index, &obj->flags);
        if (*flag >= 0) {
            *flag = (*flag == 0);
        }
    } else {
        if (!D_0029DD28) {
            func_0018D9D0((SharedMenu *)(&D_00361E80));
            D_0029DD28 = 1;
            D_00361E80.x4F8 = D_00286F30;
        }
        func_0018E398(&D_00361E80);
        func_0018DB20(&D_00361E80);
        if (D_004225A0.x12208 / 20 >= *value && *value > 0) {
            func_0013F730(obj, index);
        } else {
            func_0018E638(&D_00361E80, 27, &D_0029DD30);
            func_0018E3A8((struct SharedMenu *)(void *)(&D_00361E80), 16, &D_0029DD30, 1);
            func_00190660(D_003F52D0, &D_00361E80);
        }
    }
    count = obj->count;
    for (i = 0; i < count; i++) {
        sub = &D_002AF2C0;
        if (obj->types.item[i] == 59) {
            func_001AC300(sub->x1E40, obj->flags.item[i] != 0);
        }
    }
}
