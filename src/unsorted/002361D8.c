/* UNIT 0x002361D8 0x00236260 */
typedef struct Vec { float x,y,z,w; } Vec;
void func_002361D8(Vec *a, Vec *b, Vec *c, Vec *out) {
 Vec tmp __attribute__((aligned(16)));
 __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vsub.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0" : "=m"(tmp) : "r"(a),"r"(b));
 /* SDK scalar vector normalization. */
 __asm__ volatile("lwc1 $f3,%1
	lwc1 $f4,%2
	lwc1 $f1,%3
	mula.s $f3,$f3
	madda.s $f4,$f4
	madd.s $f0,$f1,$f1
	sqrt.s $f0,$f0
	li $2,0x3f800000
	mtc1 $2,$f2
	nop
	div.s $f2,$f2,$f0
	mul.s $f1,$f1,$f2
	lwc1 $f5,0xC(%4)
	mul.s $f3,$f3,$f2
	mul.s $f4,$f4,$f2
	swc1 $f1,%3
	swc1 $f3,%1
	swc1 $f4,%2" : "+m"(tmp), "+m"(tmp.x), "+m"(tmp.y), "+m"(tmp.z) : "r"(a) : "$2", "$f0", "$f1", "$f2", "$f3", "$f4", "$f5");
 __asm__ volatile("lqc2 $vf11,%0
	mfc1 $2,$f5
	qmtc2 $2,$vf12
	vmulx.xyz $vf11,$vf11,$vf12x
	sqc2 $vf11,%0" : "+m"(tmp) : : "$2");
 __asm__ volatile("lqc2 $vf11,%1
	lqc2 $vf12,0(%2)
	vsub.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0" : "=m"(*out) : "m"(tmp),"r"(c));
}
