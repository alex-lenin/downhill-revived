/* UNIT 0x0017EB00 0x0017EB88 */
typedef struct Obj {
    char pad0[0x388];
    int counts[26];
    unsigned char flags[26];
} Obj;

extern void func_0017EB88(Obj *obj, int index, int value);
extern void func_0017EDE8(Obj *obj, int index, int value);

void func_0017EB00(Obj *obj, int index, int value)
{
    if (obj->counts[index] != 0) {
        obj->counts[index]--;
    } else {
        func_0017EB88(obj, index, value);
        if (obj->flags[index]) {
            func_0017EDE8(obj, index, value);
        }
    }
}
