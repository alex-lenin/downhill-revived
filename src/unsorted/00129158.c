/* UNIT 0x00129158 0x00129230 */
struct Obj {
    char pad0[0x30C0];
    int handles[67];
    int ids[30];
    int count;
};

extern int D_004B2828[];
extern char D_00383A10[];
extern int func_001591D8(unsigned int *);
extern void func_002182A8(int, int);
extern void func_00158120(void *, int, float);

void func_00129158(struct Obj *obj, unsigned int *arg) {
    if (obj->count < 30 && D_004B2828[0] < 3) {
        obj->handles[obj->count] = func_001591D8(arg);
        func_002182A8(obj->handles[obj->count], 0);
        if (arg != 0) {
            obj->ids[obj->count] = (*arg >> 7) & 0x7FF;
        }
        func_00158120(D_00383A10, obj->handles[obj->count], 7.0f);
        obj->count++;
    }
}
