/* UNIT 0x0020D260 0x0020D290 */
typedef struct Obj {
    char pad0[0x1694];
    int x1694;
} Obj;
extern int func_0020D660(Obj *, char *);
int func_0020D260(Obj *obj, char *name) {
    int result = -1;
    if (obj->x1694 != 0) {
        result = func_0020D660(obj, name);
    }
    if (result < 0) return 1;
    return 0;
}




