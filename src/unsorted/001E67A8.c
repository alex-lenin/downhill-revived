/* UNIT 0x001E67A8 0x001E68A0 */
typedef struct {
    int a;
    int b;
    int c;
} Triple;

typedef struct Obj {
    int x0;
    int x4;
    char pad8[0x8];
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    char pad24[0x1C];
    char *x40;
    Triple x44[30];
    char pad1AC[0x3C0 - 0x1AC];
    int x3C0[30];
    char pad438[0x5A0 - 0x438];
    Triple x5A0[30];
    char pad708[0x4];
    int x70C;
    char pad710[0x4];
    int x714;
    int x718;
    int x71C;
    int x720;
    int x724;
    char pad728[0x10D8 - 0x728];
    int x10D8;
    int x10DC;
    unsigned char x10E0;
    unsigned char x10E1;
    unsigned char x10E2;
    char pad10E3;
    unsigned char x10E4;
    char pad10E5[3];
    unsigned char x10E8;
    unsigned char x10E9;
    char pad10EA[2];
    int x10EC;
    int x10F0;
    int x10F4;
    int x10F8;
} Obj;

extern void func_001E9A48(void *, int);

void func_001E67A8(Obj *obj)
{
    int i;

    if (obj->x10E2) {
        func_001E9A48(obj->x40 + 0x5FF0, 0);
    }
    obj->x0 = 0;
    obj->x4 = 0;
    obj->x1C = 0;
    obj->x10D8 = 0;
    obj->x10DC = 0;
    obj->x724 = 0;
    obj->x720 = 0;
    obj->x10E0 = 0;
    obj->x10E1 = 0;
    obj->x10 = 0;
    obj->x14 = 0;
    obj->x10E4 = 0;
    obj->x18 = 0;
    obj->x20 = 0;
    obj->x10F8 = 0;
    obj->x10EC = 0;
    obj->x10F4 = 0;
    obj->x10F0 = 0;
    obj->x714 = 0;
    obj->x718 = 0;
    obj->x71C = 0;
    obj->x70C = 0;
    obj->x10E8 = 0;
    obj->x10E9 = 0;
    for (i = 0; i < 30; i++) {
        obj->x44[i].a = 0;
        obj->x44[i].b = 0;
        obj->x44[i].c = 0;
        obj->x3C0[i] = 0;
        obj->x5A0[i].a = 0;
        obj->x5A0[i].b = 0;
        obj->x5A0[i].c = 0;
    }
}
