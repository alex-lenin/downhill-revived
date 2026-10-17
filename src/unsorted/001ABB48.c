/* UNIT 0x001ABB48 0x001ABB68 */
typedef struct Sub {
    int x0;
    float x4;
    int x8;
} Sub;

typedef struct Obj {
    char pad0[0x1680];
    Sub x1680;
} Obj;

void func_001ABB48(Obj *obj)
{
    Sub *s = &obj->x1680;

    s->x0 = 0;
    s->x8 = 0;
    s->x4 = 10.0f;
}
