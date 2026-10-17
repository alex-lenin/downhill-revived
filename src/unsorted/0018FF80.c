/* UNIT 0x0018FF80 0x0018FFE0 */
typedef struct { char pad[4]; char x4; } Child;
typedef struct { char pad[0x134]; Child *x134; } Obj;
extern Child *func_00209108(void);
extern void func_00218278(Child *, int *);
void func_0018FF80(Obj *obj, int *flags) {
    Child *child;
    if ((*flags & 0x3f) == 1) {
        child = func_00209108();
        obj->x134 = child;
        func_00218278(child, flags);
        obj->x134->x4 = 0;
    }
}
