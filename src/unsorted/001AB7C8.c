/* UNIT 0x001AB7C8 0x001AB808 */
struct Obj { char pad[0x20]; char *x20; };
extern void func_00232A80(void *, void *, void *);
void func_001AB7C8(struct Obj *obj, void *input, void *output) {
    if (obj->x20) {
        func_00232A80(obj->x20 + 0x40, output, input);
    } else {
        __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(output) : "memory");
    }
}
