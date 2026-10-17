/* UNIT 0x00144A40 0x00144AC0 */
typedef struct Obj {
    char pad0[0x1510];
    int x1510[12];
    char pad1540[0x14];
    int count; /* 0x1554 */
} Obj;

extern char D_00383A10[];
extern int func_001591D8(int);
extern void func_00158120(void *, int, float);

void func_00144A40(Obj *obj, int arg) {
    if (obj->count < 12) {
        obj->x1510[obj->count] = func_001591D8(arg);
        func_00158120(D_00383A10, obj->x1510[obj->count], 5.0f);
        obj->count++;
    }
}
