/* UNIT 0x00231968 0x00231998 */
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Vec;
typedef struct { char pad[0x70]; Vec x70; char x80[0x50]; char xD0[16]; } Obj;

extern void func_00221888(void *, void *, void *);

void func_00231968(Obj *obj, Vec *src) {
    Vec *v = &obj->x70;
    COPY_VECTOR(obj->x70, src);
    func_00221888(obj->x80, v, obj->xD0);
}
