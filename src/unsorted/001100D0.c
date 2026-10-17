/* UNIT 0x001100D0 0x001101B8 */
struct Obj {
    char *x0;
    char pad4[0x240 - 0x4];
    float x240;
    char pad244[0xC];
    float x250;
    float x254;
    char pad258[0x3BC - 0x258];
    int x3BC;
};

extern int D_004B26AC[];
extern void func_0010B2B8(struct Obj *, int);
extern float func_001385A8(void *);
extern void func_00155E78(void *, int);

void func_001100D0(struct Obj *obj, float limit)
{
    obj->x3BC++;
    if (obj->x240 < limit) {
        func_0010B2B8(obj, 0);
    } else if (obj->x3BC > 180) {
        /* The empty asm operands reproduce retail's register allocation: the
           barriers give m, c and b their own quantities so the abs.s results
           are not tied to their inputs, and their live lengths order the
           allocation as $f1 (abs of x254), $f2 (0.2617994f), $f3 (x250). */
        float v = obj->x254;
        float m;
        float c;
        float b;
        __asm__ volatile("" : "=f"(c));
        __asm__ volatile("" : "=f"(m));
        m = __builtin_fabsf(v);
        __asm__ volatile("" : : "f"(m));
        __asm__ volatile("" : "=f"(b));
        c = 0.2617994f;
        b = obj->x250;
        __asm__ volatile("" : : "f"(v));
        if (m + c < __builtin_fabsf(b)) {
            func_0010B2B8(obj, 0);
        } else if (D_004B26AC[0] != 15) {
            if (func_001385A8(obj->x0 + 0x40C0) < 0.1f || obj->x3BC > 480) {
                func_00155E78(obj->x0 + 0x3FD0, 1);
                obj->x3BC = 0;
            }
        }
    }
}
