/* UNIT 0x0012C398 0x0012C450 */
typedef struct Flags {
    unsigned long x0;
} Flags;

typedef struct Sub {
    char pad0[0x4];
    unsigned char x4;
} Sub;

typedef struct Obj {
    char pad0[0x3514];
    int x3514;
    char pad3518[0x78D4 - 0x3518];
    int x78D4;
    char pad78D8[0x792C - 0x78D8];
    Flags *x792C;
    Sub *x7930;
    char pad7934[0x7978 - 0x7934];
    int x7978;
    char pad797C[0x7984 - 0x797C];
    int x7984;
    int x7988;
    char pad798C[0x7A5C - 0x798C];
    int x7A5C;
} Obj;

extern void func_0010A730(int id, Obj *obj);
extern void func_002182A8(int handle, int value);

void func_0012C398(Obj *obj, int a1, int id)
{
    obj->x7978 = a1;
    obj->x7984 = 0;
    obj->x7A5C = 3;
    obj->x7988 = -1;
    obj->x78D4 = id;
    func_0010A730(id, obj);
    func_002182A8((int)obj->x792C, 1);
    func_002182A8(obj->x3514, 1);
    if (obj->x7930 != 0) {
        obj->x7930->x4 = 1;
    }
    obj->x792C->x0 &= ~((unsigned long)1 << 56);
    obj->x792C->x0 &= ~((unsigned long)1 << 63);
}
