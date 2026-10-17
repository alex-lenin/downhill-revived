/* UNIT 0x001389B8 0x001389D0 */
typedef struct Obj {
    char pad0[0x31714];
    int x31714;
    int x31718;
} Obj;

void func_001389B8(Obj *obj)
{
    obj->x31714 = 0;
    obj->x31718 = 0;
}
