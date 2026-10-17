/* UNIT 0x00121530 0x001215C8 */
typedef struct Vec { float f[4]; } Vec;
typedef struct Obj { char pad0[0x1C0]; Vec x1C0; Vec x1D0; } Obj;
extern void func_00222F60(Vec *, Vec *, Vec *);
extern void func_00223168(Vec *, Vec *);
extern void func_002235B0(Vec *, Vec *, Vec *);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_00121530(Obj *obj) {
 Vec v;
 Vec *dst = &obj->x1C0;
 Vec *src = &obj->x1D0;
 func_00222F60(&v, src, dst);
 if (v.f[3] < 0.9999619126319885f) {
  v.f[3] = 0.9999619126319885f;
  func_00223168(&v, &v);
  func_002235B0(dst, &v, dst);
 } else {
  COPY_VECTOR(*dst, src);
 }
}
