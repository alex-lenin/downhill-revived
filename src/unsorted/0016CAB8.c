/* UNIT 0x0016CAB8 0x0016CB78 */
typedef struct { int x0[0x448c/4]; int x448C[11]; int x44B8; } Obj;
extern char D_0028AF60[];
extern char D_00383A10[];
extern void func_00254D08(char *, int, int);
extern int func_001591D8(unsigned int *);
extern void func_00158120(char *, int, float);
void func_0016CAB8(Obj *obj, unsigned int *arg) {
    if (obj->x44B8 >= 11) { func_00254D08(D_0028AF60, obj->x44B8 + 1, 10); return; }
    obj->x0[obj->x44B8] = func_001591D8(arg);
    if (arg) {
        obj->x448C[obj->x44B8] = (*arg >> 7) & 0x7ff;
        func_00158120(D_00383A10, obj->x0[obj->x44B8], 8.0f);
        obj->x44B8++;
    }
}
