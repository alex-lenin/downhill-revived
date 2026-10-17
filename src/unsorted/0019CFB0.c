/* UNIT 0x0019CFB0 0x0019CFF0 */
typedef struct Obj { char pad0[0x508]; unsigned char x508; } Obj;
extern char D_0043DD38[];
extern void func_0019C358(void *);
void func_0019CFB0(Obj *obj) {
    if (obj->x508 != 0) {
        func_0019C358(D_0043DD38);
        obj->x508 = 0;
    }
}
