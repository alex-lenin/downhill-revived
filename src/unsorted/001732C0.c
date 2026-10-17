/* UNIT 0x001732C0 0x00173358 */
typedef struct { char pad[0xc]; char *x0c; char gap[0x10]; float x20[4]; char gap2[0x50]; float x80[4]; char gap3[0x24]; float xB4; char gap4[0x68]; char x120; } Obj;
void func_001732C0(Obj *obj) {
 float *v = obj->x80;
 float x, y, z, length, square, scale;
 char flag;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0x80(%0)" : : "r"(obj), "r"(obj->x20), "r"(obj->x0c + 0x10) : "memory");
 x = v[0]; y = v[1]; z = v[2];
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=f"(length) : "f"(x), "f"(y), "f"(z));
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(square) : "f"(x), "f"(y), "f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(scale) : "f"(1.0f), "f"(square));
 v[0] = x * scale; v[1] = y * scale; v[2] = z * scale;
 flag = 1;
 if (!(length < obj->xB4)) flag = 0;
 obj->x120 = flag;
 obj->xB4 = length;
}
