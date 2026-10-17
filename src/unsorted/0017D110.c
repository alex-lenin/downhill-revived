/* UNIT 0x0017D110 0x0017D120 */
struct Obj { char pad[0x28]; float x28; };
void func_0017D110(struct Obj *obj, void *unused, float *out) {
    __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(out) : "memory");
    out[2] = obj->x28;
}
