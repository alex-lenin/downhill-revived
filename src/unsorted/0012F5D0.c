/* UNIT 0x0012F5D0 0x0012F6A0 */
typedef struct Part {
    char pad0[0xc0];
    float xc0[4];
    char pad1[0x84];
    float x154;
} Part;

typedef struct Obj {
    char pad0[0x40c0];
    Part x40c0;
    char pad1[0x377c];
    int x7994;
    char pad2[0xa4];
    float x7a3c;
    char pad3[0x1c];
    int x7a5c;
    char pad4[8];
    unsigned char x7a68;
} Obj;

extern int func_00238F60(void);
extern float func_0017C008(void);
extern float func_00235EA8(float, float, float, float);

void func_0012F5D0(Obj *obj)
{
    float speed;
    float *v;

    if (!obj->x7a68) {
        obj->x7994 += func_00238F60();
        v = obj->x40c0.xc0;
        __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                          : "=&f"(speed) : "f"(v[0]), "f"(v[1]), "f"(v[2]));
        speed *= 0.6818182f;
        speed *= func_0017C008();
        obj->x7a3c += speed;
    } else if (obj->x7a5c == 1) {
        Part *part = &obj->x40c0;
        part->x154 = func_00235EA8(part->x154, 0.2f, 0.02f, 0.001f);
    }
}
