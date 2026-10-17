/* UNIT 0x0022F870 0x0022F8B8 */
typedef struct { char pad[0x140]; unsigned int x140, x144; } Obj;
extern char D_1050[], D_12F0[], D_1638[], D_6F0[];
void func_0022F870(int flag, Obj *obj) {
    char *first;
    char *second;
    if (flag) {
        first = D_1050;
        second = D_12F0;
    } else {
        first = D_1638;
        second = D_6F0;
    }
    obj->x140 = ((unsigned int)first >> 3) | 0x15000000;
    obj->x144 = ((unsigned int)second >> 3) | 0x15000000;
}
