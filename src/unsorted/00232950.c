/* UNIT 0x00232950 0x00232A38 */
typedef struct Vec4 {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4;

typedef float Mtx[4][4];

typedef struct Obj {
    float x0;
    char pad4[0x1C];
    Vec4 x20;
    char pad30[0x8];
    float (*x38)[4];
    char pad3C[0x24];
    Vec4 x60;
    Vec4 x70;
    Mtx x80 __attribute__((aligned(16)));
    Vec4 xC0;
    Vec4 xD0;
} Obj;

extern void func_00221F20(float (*)[4]);
extern void func_00221A60(float (*)[4], float (*)[4], float (*)[4]);
extern void func_00221888(float (*)[4], Vec4 *, Vec4 *);

void func_00232950(Obj *obj)
{
    Mtx tmp __attribute__((aligned(16)));
    float (*rot)[4];
    int i;
    int j;

    rot = obj->x38;
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(obj->xC0) : "r"(&obj->x60), "f"(obj->x0) : "$2");
    func_00221F20(tmp);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            tmp[i][j] = (&obj->x20.x)[i] * rot[j][i];
        }
    }
    func_00221A60(obj->x80, tmp, rot);
    func_00221888(obj->x80, &obj->x70, &obj->xD0);
}
