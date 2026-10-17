/* UNIT 0x0018E700 0x0018E720 */
typedef struct Sub {
    short x0;
    short x2;
    char pad4[3];
    char x7;
} Sub;

typedef struct Obj {
    char pad0[0x448];
    Sub x448;
    char pad450[0x14];
    int x464;
} Obj;

void func_0018E700(Obj *obj)
{
    Sub *sub = &obj->x448;
    int value = obj->x464;

    sub->x0 = value;
    sub->x2 = 0xCC;
    sub->x7 = 0;
}
