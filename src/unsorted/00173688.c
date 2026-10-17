/* UNIT 0x00173688 0x001737F0 */
typedef unsigned int Vec __attribute__((mode(TI)));
typedef struct Body { char p0[0x10]; Vec x10; char p20[0x80]; float xA0; } Body;
typedef struct Config { char p0[0x884]; float x884; } Config;
typedef struct Obj { char p0[0xC]; Body *xC; Config *x10; char p14[0xC]; Vec x20; char p30[0x10]; int x40,x44; char p48[0x18]; float x60; char p64[0xC]; float x70; char p74[0x48]; float xBC; } Obj;
extern void func_002087C0(float *);
extern float func_00222690(Vec *);
extern float func_002487A0(float);
extern float func_00235EA8(float,float,float,float);
extern float fabsf(float);
void func_00173688(Obj *obj) {
 Vec delta; float target,error,speed,limit,mag,cfg,half; Config *params;
 if (obj->x44 == obj->x40) target=0.0f;
 else {
  __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(delta) : "r"(&obj->x20), "r"(&obj->xC->x10));
  speed=func_00222690(&delta);
  target=speed*func_002487A0(obj->xBC);
  func_002087C0(&target);
 }
 obj->x60=func_00235EA8(obj->x60,target,0.1f,0.001f);
 error=obj->x60-obj->xC->xA0;
 func_002087C0(&error);
 params=obj->x10;
 __asm__ volatile("L00173688_params:" : "+r"(params));
 half=0.5f;
 cfg=params->x884;
 __asm__ volatile("L00173688_cfg:" : "+f"(cfg));
 limit=cfg*half;
 mag=fabsf(error);
 __asm__ volatile("L00173688_mag:" : "+f"(mag));
 if (mag*0.1f < limit) limit=fabsf(error)*0.1f;
 if (limit<error) error=limit; else { float value=-limit; if (!(error < value)) value=error*0.5f; error=value; }
 obj->x70=func_00235EA8(obj->x70,error,0.02f,0.001f);
 obj->xC->xA0+=obj->x70;
 func_002087C0(&obj->xC->xA0);
}
