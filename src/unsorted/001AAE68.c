/* UNIT 0x001AAE68 0x001AAF40 */
struct Obj { char pad[0x18]; int *x18; int x1C; int x20; char *x24; };
extern char D_002AF2C0[];
extern void func_0011C208(void *,void *,float *,float *,float *);
extern void func_00209060(void *);
int func_001AAE68(struct Obj *obj) {
 float x,y,z;
 if (obj->x20) return 0;
 func_0011C208(D_002AF2C0,obj->x24+0x10,&x,&y,&z);
 __asm__ volatile("sqrt.s %0, %1" : "=f"(z) : "f"(z));
 if (z>150.0f) {
 func_00209060(obj->x24);
 obj->x24=0;
 *obj->x18=(*obj->x18 & ~0x3F)|0x2C;
 return 0;
 }
 if (z>6.0f) *(unsigned long *)obj->x24 |= (unsigned long)0x8000 << 42;
 return 1;
}
