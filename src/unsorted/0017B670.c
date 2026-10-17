/* UNIT 0x0017B670 0x0017B7A8 */
typedef int s128 __attribute__((mode(TI)));
struct Display { unsigned short x0,x2,x4,x6; char pad[16]; int x18,x1C; };
extern struct Display *func_0023DF40(int);
extern void func_0017B7D0(void *,void *,int,int,int,int);
void func_0017B670(long *p) {
 struct Display *d; int x,y;
 d=func_0023DF40(0);
 x=d->x18-(d->x4>>1);
 y=d->x1C-(d->x6>>1);
 *(s128 *)p=0;
 *(int *)p=0x6000000C;
 p[2]=((long)0x8000<<45)|0x8003;
 p[3]=14;
 p[4]=0x20002;
 p[5]=0x47;
 p[6]=(x<<4)|((long)(y<<4)<<32);
 p[7]=24;
 p[8]=((long)(d->x4-1)<<16)|((long)(d->x6-1)<<48);
 p[9]=64;
 func_0017B7D0(p,(char *)p+0x50,(d->x4>>1)+x,y,2,d->x6);
 func_0017B7D0(p,(char *)p+0x90,x,(d->x6>>1)+y,d->x4-1,2);
}
