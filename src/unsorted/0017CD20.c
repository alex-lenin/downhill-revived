/* UNIT 0x0017CD20 0x0017CD70 */
typedef struct Flags { unsigned int value : 6; unsigned int rest : 26; } Flags;
typedef struct Obj { char pad[0x14]; void *x14; int x18; Flags *x1C; } Obj;
extern void func_00209060(void *);
void func_0017CD20(Obj *obj)
{
    Flags *flags;
    if (obj->x14) {
        func_00209060(obj->x14);
        obj->x14 = 0;
        obj->x1C->value = 3;
    }
}
