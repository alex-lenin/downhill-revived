/* UNIT 0x0018EB90 0x0018EBC0 */
typedef struct Obj {
    char pad0[0x14];
    void *x14;
    unsigned char x18;
} Obj;

extern void func_0018EE80(void *p, int arg);

void func_0018EB90(Obj *obj)
{
    if (obj->x14 != 0) {
        func_0018EE80(obj->x14, obj->x18);
    }
}
