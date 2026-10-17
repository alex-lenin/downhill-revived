/* UNIT 0x0011CD58 0x0011CF28 */
typedef struct Point { float x0,x4,x8; } Point;
typedef struct Obj { char pad0[0x4A9C]; int x4A9C[13]; float x4AD0[13]; } Obj;
extern char D_003041B0[];
extern struct { char pad0[0x50198]; float x50198; } D_002AF2C0;
extern int func_00163C58(void *,int,int,float);
extern Point *func_00163D40(void *,int);
Point *func_0011CD58(Obj *obj,int flags) {
 Point *result=0;
 int index;
 float *position,*positions;
 int *handle=(int *)obj;
 float minimum,limit;
 switch(flags) {
 case 4:index=1;break; case 8:index=2;break; case 16:index=3;break;
 case 32:index=4;break; case 64:index=5;break; case 128:index=6;break;
 case 256:index=7;break; case 512:index=8;break; case 1024:index=9;break;
 case 2048:index=10;break; case 4096:index=11;break; case 8192:index=12;break;
 default:index=0;break;
 }
 limit=D_002AF2C0.x50198-200.0f;
 positions=(float *)((char *)handle+0x4AD0); position=positions+index;
 __asm__ volatile("min.s %0,%1,%2" : "=&f"(minimum) : "f"(*position), "f"(limit));
 *position=minimum;
 handle=(int *)((char *)handle+0x4A9C);
 handle+=index;
 *handle=func_00163C58(D_003041B0,*handle,flags,minimum);
 if(*handle>=0) { result=func_00163D40(D_003041B0,*handle); *position=result->x8; }
 return result;
}
