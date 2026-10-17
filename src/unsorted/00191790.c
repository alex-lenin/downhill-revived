/* UNIT 0x00191790 0x00191828 */
typedef struct Obj {
    char pad0[0x204];
    float x204;
    char pad208[0x220 - 0x208];
    int x220;
} Obj;

extern struct {
    char pad0[0x50198];
    float x50198;
} D_002AF2C0;
extern char D_003041B0[];
extern int func_00163C58(void *, int, int, float);
extern int func_00163D40(void *, int);

int func_00191790(Obj *obj, int flags) {
    int result = 0;
    float minimum;
    float limit = D_002AF2C0.x50198 - 200.0f;

    __asm__ volatile("min.s %0,%1,%2" : "=&f"(minimum) : "f"(obj->x204), "f"(limit));
    obj->x204 = minimum;
    obj->x220 = func_00163C58(D_003041B0, obj->x220, flags, minimum);
    if (obj->x220 >= 0) {
        result = func_00163D40(D_003041B0, obj->x220);
    }
    return result;
}
