/* UNIT 0x00111080 0x00111110 */
typedef struct Obj { char *x0; char pad4[0x2B8]; float x2BC; char pad2C0[0x88]; float x348; char pad34C[0xF4]; unsigned long x440; } Obj;
extern int func_00111110(Obj *);
extern int func_001EA070(char *);
extern void func_00110870(Obj *, int);
extern void func_00111150(Obj *, int);
void func_00111080(Obj *obj, int arg) {
    if (func_00111110(obj)) {
        if (obj->x348 < obj->x2BC) {
            if (func_001EA070(obj->x0+0x5FF0)) func_00110870(obj, 0);
        } else {
            obj->x440 |= (unsigned long)0x8000 << 47;
            func_00111150(obj, arg);
        }
    }
}
