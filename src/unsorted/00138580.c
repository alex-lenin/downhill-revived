/* UNIT 0x00138580 0x001385A8 */
typedef struct Obj { char pad0[0xC0]; float xC0[3]; } Obj;
float func_00138580(Obj *obj) {
    float result;
    float *v = obj->xC0;
    __asm__ ("mula.s %3, %3\n\tmadda.s %1, %1\n\tmadd.s %0, %2, %2\n\tsqrt.s %0, %0"
        : "=&f" (result) : "f" (v[1]), "f" (v[2]), "f" (v[0]));
    return result;
}

