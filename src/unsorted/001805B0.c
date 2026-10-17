/* UNIT 0x001805B0 0x001808D0 */
typedef struct Body { int *x0; char pad4[12]; float pos[4]; char pad20[48]; float dir[4]; } Body;
typedef struct Status { int x0; struct Flags *x4; } Status;
typedef struct State { char pad0[8]; Body *x8; char padC[204]; Status xD8; char padE0[192]; float x1A0,x1A4; } State;
struct Flags { char pad0[20]; unsigned short x14; };
extern int D_004B2840[];
extern int func_00211390(float *,int,int,Body **,int,float);
extern float func_00235EA8(float,float,float,float);
extern void func_00181618(State *,float);
static __inline__ int disabled(Status *s) { return (s->x4->x14>>1)&1; }
static __inline__ float dot(float *a,float *b) { float r; __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a[0]),"f"(b[0]),"f"(a[1]),"f"(b[1]),"f"(a[2]),"f"(b[2])); return r; }
/* Sony libvu0 length/reciprocal wrapper, as in matched 00123DB0. */
static __inline__ void normalize(float *v) {
 float x=v[0],y=v[1],z=v[2],len,inv; int bits;
 __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(len),"=r"(bits) : "f"(x),"f"(y),"f"(z));
 v[0]=x*inv; v[1]=y*inv; v[2]=z*inv;
}
static __inline__ void sub(float (*out)[4],float *a,float *b) {
 __asm__ volatile("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*out):"r"(b),"r"(a));
}
/* Uses VU0 vector subtraction and the Sony libvu0 normalization wrapper. */
void func_001805B0(State *s) {
    Body *list[3];
    float diff[4] __attribute__((aligned(16)));
    int count, remaining;
    Body **p;

    if (D_004B2840[0] == 5 && ((*s->x8->x0 & 0x3FF80) == 0x27600))
        count = func_00211390(s->x8->pos, 1260, 1460, list, 3, 200.0f);
    else
        count = func_00211390(s->x8->pos, 1260, 1460, list, 3, 40.0f);

    if (count < 2)
        goto low_count;
    if (count >= 1) {
        unsigned int mask = 0x3FF80;
        Status *status = &s->xD8;
        remaining = count;
        p = list;
        do {
            Body *other = *p;
            Body *self = s->x8;
            if ((*other->x0 & mask) != (*self->x0 & mask)) {
                sub(&diff, self->pos, other->pos);
                normalize(diff);
                if (dot(s->x8->dir, diff) > 0.0f &&
                    dot(s->x8->dir, (*p)->dir) > 0.0f) {
                    s->x1A0 = func_00235EA8(s->x1A0, s->x1A4 + 0.24444444f, 0.08f, 0.001f);
                    func_00181618(s, s->x1A0);
                } else if (dot(s->x8->dir, (*p)->dir) > 0.0f) {
                    s->x1A0 = func_00235EA8(s->x1A0, 0.04888889f, 0.01f, 0.001f);
                    func_00181618(s, s->x1A0);
                } else if (!disabled(status)) {
                    s->x1A0 = func_00235EA8(s->x1A0, s->x1A4, 0.01f, 0.001f);
                    func_00181618(s, s->x1A0);
                }
            }
            p++;
            remaining--;
        } while (remaining);
    }
    return;
low_count:
    {
        if (!disabled(&s->xD8)) {
            s->x1A0 = func_00235EA8(s->x1A0, s->x1A4, 0.01f, 0.001f);
            func_00181618(s, s->x1A0);
        }
    }
}
