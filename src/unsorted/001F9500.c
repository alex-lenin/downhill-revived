/* UNIT 0x001F9500 0x001F95D0 */
extern char D_004C7530[];
extern char D_003F52D0[];

extern int *func_0018EAC8(void *, int);
extern int func_001F7A98(int);
extern void func_001F95D0(void *, int);
extern void func_001B5AF0(void *, int, int);
extern void func_00190660(void *, void *);

void func_001F9500(void *obj, int id)
{
    int *entry;

    entry = func_0018EAC8(obj, id);
    if (entry != 0) {
        if (*entry == 13) {
            func_001F95D0(D_004C7530, 0x1A);
        } else {
            func_001F95D0(D_004C7530, id + 6 + func_001F7A98((*entry + 1) << 5));
        }
    } else {
        func_001F95D0(D_004C7530, id + 6);
    }
    func_001B5AF0(D_003F52D0, 3, 1);
    func_00190660(D_003F52D0, D_004C7530);
}
