/* UNIT 0x00207D90 0x00207E68 */
/* x0 is cleared by storing VU0 vf0 (0,0,0,1); uses inline asm for that sqc2. */
typedef struct Obj {
    float x0[4];
    int x10[7];
    char x2C;
} Obj;

extern int func_00202DB0(int, int);

void func_00207D90(Obj *obj, int arg) {
    int i;

    __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(obj) : "memory");
    obj->x10[0] = func_00202DB0(0, arg);
    obj->x10[1] = func_00202DB0(1, arg);
    obj->x10[2] = func_00202DB0(2, arg);
    obj->x10[3] = func_00202DB0(3, arg);
    obj->x10[4] = func_00202DB0(4, arg);
    obj->x10[5] = func_00202DB0(5, arg);
    obj->x10[6] = func_00202DB0(6, arg);
    for (i = 0; i < 7; i++) {
        if (obj->x10[i] != 0) {
            break;
        }
    }
    obj->x2C = i < 7;
}
