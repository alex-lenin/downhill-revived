/* UNIT 0x00173E58 0x00174098 */
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
typedef struct { float x[4]; } Vec;
typedef struct { char pad[0xA4]; float xA4,xA8; } Target;
typedef struct { char pad[0x86C]; float x86C; char pad870[0x14]; float x884; } Settings;
typedef struct { char pad[8]; int x8; Target *xC; Settings *x10; char pad14[0x54]; float x68; char pad6C[0xC]; float x78; char pad7C[4]; Vec x80,x90; char padA0[0x18]; float xB8,xBC; } Obj;
extern float fabsf(float);
extern float func_00222640(Vec *);
extern void func_002087C0(float *);
extern float func_00235EA8(float,float,float,float);
void func_00173E58(Obj *o,float delta) {
 Vec v; float angle, diff, speed, limit, result, a, b, goal, half, mag;
 Settings *params;
 if(o->xB8 > 0.24444444f) {
 COPY_VECTOR(v,&o->x80); v.x[2]=0;
 a=func_00222640(&v); o->x68=a; angle=a;
 } else {
 COPY_VECTOR(v,&o->x90); v.x[2]=0;
 b=func_00222640(&v); o->x68=b; angle=b-delta; func_002087C0(&angle);
 }
 o->xBC=o->x68-o->xC->xA8; func_002087C0(&o->xBC);
 diff=angle-o->xC->xA8; func_002087C0(&diff);
 half=0.5f;
 speed=diff*half;
 params=o->x10;
 __asm__ volatile ("L00173E58_params:" : "+r"(params));
 limit=params->x884;
 mag=fabsf(speed);
 __asm__ volatile ("L00173E58_mag:" : "+f"(mag));
 if(mag*0.1f < limit) limit=fabsf(speed)*0.1f;
 if(speed > limit) speed=limit; else if(speed < -limit) speed=-limit; else speed*=half;
 result=func_00235EA8(o->x78,speed,0.03f,0.001f);
 o->x78=result; o->xC->xA8+=result; func_002087C0(&o->xC->xA8);
 speed=diff;
 if(o->xB8 < o->x10->x86C) { speed=0.0f; if(o->x8!=6) goto done; goto negative; }
 else if(speed>0.5235988f) speed=0.5235988f;
 else if(speed < -0.5235988f) { negative: speed=-0.5235988f; }
 done:
 o->xC->xA4=func_00235EA8(o->xC->xA4,speed,0.03f,0.001f);
}
