/* UNIT 0x001341F8 0x00134338 */
typedef struct { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct { char pad0[8]; float x8; char padC[0x10]; float x1C; Vec x20; } Part;
typedef struct { char pad[0x20]; Part *x20; int x24; } Group;
typedef struct { char pad0[0x10]; Vec x10; char pad20[0x10C]; int x12C; } Obj;
extern void func_00231A48(Vec *,Vec *);
extern float fabsf(float);
void func_001341F8(Obj *obj,Group *groups) {
    Vec maximum,scaled;
    float length;
    int i,count;
    Part *part;
    __asm__ volatile("sqc2 $vf0,%0" : "=m"(maximum));
    for(i=0;i<obj->x12C;i++) {
        count=groups[i].x24;
        part=groups[i].x20;
        while(count) {
            if(part->x1C<0.0f) {
                __asm__ volatile("lqc2 $vf11,0(%1)
	mfc1 $2,%2
	qmtc2.ni $2,$vf12
	vmulx.xyz $vf11,$vf11,$vf12x
	sqc2 $vf11,%0" : "=m"(scaled) : "r"(&part->x20),"f"(part->x8) : "$2");
                { float s,a,m,ma;
                  s=scaled.x; a=fabsf(s);
                  __asm__ volatile("Lxx_a:");
                  m=maximum.x; ma=fabsf(m); __asm__ volatile("Lxx_m:" : : "f"(m));
                  if(ma<a) maximum.x=s;  }
                { float s,a,m,ma;
                  s=scaled.y; a=fabsf(s);
                  __asm__ volatile("Lyy_a:");
                  m=maximum.y; ma=fabsf(m); __asm__ volatile("Lyy_m:" : : "f"(m));
                  if(ma<a) maximum.y=s;  }
                { float s,a,m,ma;
                  s=scaled.z; a=fabsf(s);
                  __asm__ volatile("Lzz_a:");
                  m=maximum.z; ma=fabsf(m); __asm__ volatile("Lzz_m:" : : "f"(m));
                  if(ma<a) maximum.z=s;  }
            }
            count--; part++;
        }
    }
    __asm__ volatile("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3
	sqrt.s %0,%0" : "=&f"(length) : "f"(maximum.x),"f"(maximum.y),"f"(maximum.z));
    if(!(length>1.0f)) func_00231A48(&obj->x10,&maximum);
}
