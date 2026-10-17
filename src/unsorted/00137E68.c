/* UNIT 0x00137E68 0x00138558 .sdata=0x29C364:0x29C370 */
/* Uses VU0/FPU inline assembly and the SDK matrix copy. */
typedef float Vec[4] __attribute__((aligned(16)));
typedef struct Mat { Vec v[4]; } Mat;
typedef struct Obj {
    char pad0[0x38]; Mat *x38;
    char pad3c[0x240-0x3c]; float x240,x244,x248;
    char pad24c[0x2a0-0x24c]; Mat x2a0;
    char pad2e0[0x410-0x2e0]; float x410,x414;
    char pad418[0x460-0x418]; float x460,x464;
    char pad468[0x4a4-0x468]; short x4a4;
} Obj;
float D_0029C364=0.400000006f;
float D_0029C368=1.700000048f;
float D_0029C36C=0.1800000072f;
extern void func_00221DB0(Mat *,Vec *,float,float);
extern void func_00231A78(Obj *,Mat *);
extern float func_00238F38(void);
extern float func_002487A0(float);
extern float func_00248990(float);
extern float fabsf(float);
static inline void Cross(Vec *d,Vec *a,Vec *b) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*d) : "r"(a),"r"(b));
}
static inline float Dot(Vec *a,Vec *b) {
    float d;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(d) : "f"((*a)[0]),"f"((*b)[0]),"f"((*a)[1]),"f"((*b)[1]),"f"((*a)[2]),"f"((*b)[2]));
    return d;
}
static inline float Normalize(Vec *v) {
    float x=(*v)[0],y=(*v)[1],z=(*v)[2],len,inv;
    __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x),"f"(y),"f"(z));
    __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(len));
    (*v)[0]=x*inv; (*v)[1]=y*inv; (*v)[2]=z*inv;
    return len;
}
static inline void Rotate(Vec *v,Mat *m) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%1)" : "=m"(*v) : "r"(v),"r"(m));
}
/* SDK matrix copy. */
#define COPY_MATRIX(dst,src) __asm__ volatile("lq $8,0(%4)
	lq $9,16(%4)
	lq $10,32(%4)
	lq $11,48(%4)
	sq $8,%0
	sq $9,%1
	sq $10,%2
	sq $11,%3" : "=m"((dst).v[0]),"=m"((dst).v[1]),"=m"((dst).v[2]),"=m"((dst).v[3]) : "r"(src) : "$8","$9","$10","$11")
static inline float Abs(float x) { float r; __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x)); return r; }
static inline float Min(float a,float b) { float r; __asm__ volatile("min.s %0,%1,%2" : "=&f"(r):"f"(a),"f"(b)); return r; }
static inline float Max(float a,float b) { float r; __asm__ volatile("max.s %0,%1,%2" : "=&f"(r):"f"(a),"f"(b)); return r; }
static inline float InvLength(float a,float b) {
    float r;
    __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(r) : "f"(1.0f),"f"(a*a+b*b));
    return r;
}
static inline void RotateAngles(float *a,float *b,float old,float sine,float cosine) { *a=*a*cosine+*b*sine; *b=*b*cosine-old*sine; }
void func_00137E68(Obj *obj) {
    Vec cross;
    Mat m, rotation;
    float sinX,cosX,sinY,cosY,sinZ,cosZ,scale,sine,old,oldc,olda;
    float weight,t,lo,hi,n,y,inv,limit;
    int i;
    Vec *up=&m.v[1], *forward=&m.v[2], *right=&m.v[0];
    Vec *oldforward=&obj->x2a0.v[2], *oldright=&obj->x2a0.v[0], *oldup=&obj->x2a0.v[1];
    COPY_MATRIX(m,obj->x38);
    Cross(&cross,oldforward,right);
    if (Normalize(&cross)>0.0f) {
        sinX=Dot(&cross,forward); cosX=Dot(&cross,up);
    } else {
        sinX=Dot(oldright,up); cosX=Dot(oldup,up);
    }
    func_00221DB0(&rotation,right,sinX,cosX);
    Rotate(up,&rotation); Rotate(forward,&rotation);
    Cross(&cross,oldforward,up);
    Normalize(&cross);
    sinY=Dot(forward,&cross); cosY=Dot(forward,oldforward);
    func_00221DB0(&rotation,up,sinY,cosY);
    Rotate(right,&rotation); Rotate(forward,&rotation);
    sinZ=Dot(up,oldright); cosZ=Dot(up,oldup);
    func_00221DB0(&rotation,oldforward,sinZ,cosZ);
    Rotate(up,&rotation); Rotate(right,&rotation);
    scale=60.0f/func_00238F38();
    sinZ=-sinZ; sinY=-sinY; sinX=-sinX;
    old=sinZ; oldc=sinY; olda=sinX;
    COPY_MATRIX(m,&obj->x2a0);
    if (obj->x248!=0.0f) {
        float s=func_00248990(-obj->x248*scale);
        float c=func_002487A0(-obj->x248*scale);
        RotateAngles(&sinZ,&cosZ,old,s,c);
    }
    func_00221DB0(&rotation,forward,sinZ,cosZ);
    Rotate(right,&rotation); Rotate(up,&rotation);
    old=-sinZ; y=Abs(cosZ);
    n=(float)obj->x4a4*(1.0f/60.0f);
    weight=obj->x464; lo=D_0029C364; hi=D_0029C368;
    n=Max(lo,Min(hi,n));
    t=(n-hi)/(lo-hi);
    weight=D_0029C36C+((t*-2.0f+3.0f)*t*t*(weight-D_0029C36C));
    sinZ=y*weight+(1.0f-weight); old=old*weight;
    inv=InvLength(old,sinZ);
    sinZ*=inv; old*=inv;
    limit=func_00248990(obj->x460*scale);
    sine=cosY*old-sinY*sinZ;
    if (fabsf(sine)<limit) { sinY=old; cosY=sinZ; }
    else {
        float c;
        sine=(sine<=0.0f) ? -limit : limit;
        c=func_002487A0(obj->x460*scale);
        sinY=sinY*c+cosY*sine; cosY=cosY*c-oldc*sine;
    }
    func_00221DB0(&rotation,up,sinY,cosY);
    Rotate(right,&rotation); Rotate(forward,&rotation);
    if (obj->x240!=0.0f) {
        float s=func_00248990(-obj->x240*scale);
        float c=func_002487A0(-obj->x240*scale);
        sinX=0.0f; sinX=olda*c+cosX*s; cosX=cosX*c-olda*s;
    }
    func_00221DB0(&rotation,right,sinX,cosX);
    Rotate(up,&rotation); Rotate(forward,&rotation);
    sinZ=0.0f;
    {
        float *row=m.v[3], *col=&m.v[0][3];
        for (i=0;i<3;i++) { *col=sinZ; *row=sinZ; col+=4; row++; }
    }
    m.v[3][3]=0.0f;
    func_00231A78(obj,&m);
    obj->x410=sinX; obj->x414=cosX;
}
