/* UNIT 0x0023DD58 0x0023DDE0 */
typedef struct Vec { float x,y,z,w; } Vec;
void func_0023DD58(Vec *out, Vec *a, Vec *b, Vec *c) {
 Vec ab __attribute__((aligned(16))), ac __attribute__((aligned(16))), normal __attribute__((aligned(16)));
 int bits;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(ab) : "r"(b),"r"(a));
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(ac) : "r"(c),"r"(a));
 __asm__ volatile("lqc2 $vf11,%1\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(normal) : "m"(ab),"r"(&ac));
 __asm__ volatile("lwc1 $f2,%1\n\tlwc1 $f3,%2\n\tlwc1 $f0,%3\n\tmula.s $f2,$f2\n\tmadda.s $f3,$f3\n\tmadd.s $f4,$f0,$f0\n\tsqrt.s $f4,$f4" : : "m"(normal), "m"(normal.x), "m"(normal.y), "m"(normal.z) : "$f0", "$f2", "$f3", "$f4");
 __asm__ volatile("li %3,0x3f800000\n\tmtc1 %3,$f1\n\tnop\n\tdiv.s $f1,$f1,$f4\n\tmul.s $f0,$f0,$f1\n\tmul.s $f2,$f2,$f1\n\tmul.s $f3,$f3,$f1\n\tswc1 $f0,%2\n\tswc1 $f2,%0\n\tswc1 $f3,%1" : "=m"(out->x), "=m"(out->y), "=m"(out->z), "=r"(bits) : : "$f0", "$f1", "$f2", "$f3", "$f4");
}
