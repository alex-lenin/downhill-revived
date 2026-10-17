/* UNIT 0x00148BE8 0x00148E70 .rodata=0x289C90:0x289CC0 */
typedef unsigned int Vec __attribute__((mode(TI)));
typedef struct Rider {
 char pad0[0x11BC]; int x11BC; char pad11C0[0xC4]; int x1284;
 char pad1288[0x15A]; unsigned char x13E2;
} Rider;
typedef struct Object {
 char pad0[0x80]; Vec x80,x90; char padA0[0x210]; char *x2B0; Rider *x2B4;
 int x2B8,x2BC,x2C0,x2C4; char pad2C8[0x90]; int x358,x35C,x360;
 char pad364[8]; unsigned char x36C,x36D; volatile unsigned char x36E,x36F; char pad370[4]; unsigned char x374,x375;
} Object;
extern unsigned char D_0043FA34[];
extern void func_00148E70(Object *),func_0014B5E0(Object *),func_0014BB70(Object *),func_0014B450(Object *),func_0014BF50(Object *),func_0014C800(Object *),func_0014D740(Object *);
extern void func_00148560(Object *,int),func_0014B1B8(Object *,int),func_0014B298(Object *,int);
extern void func_00221ED0(char *,Object *),func_0023D1F0(char *,int);
extern void func_0020ED30(int,int,int,int,float,float,float,float,float);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_00148BE8(Object *s,int arg1) {
 Vec previous; int old; int value; Vec *pos;
 COPY_VECTOR(previous,&s->x80);
 s->x375=0;
 switch(s->x2B8) {
 case 1: func_00148E70(s); break;
 case 2: func_0014B5E0(s); break;
 case 3: func_0014BB70(s); break;
 case 4: func_0014B450(s); break;
 case 5: case 6: func_0014BF50(s); break;
 case 9: func_0014C800(s); break;
 case 8:
  func_00148E70(s);
  if(s->x358==0) { s->x2C4=0; s->x358=1; func_0014D740(s); func_00148560(s,1); }
  break;
 default: break;
 }
 COPY_VECTOR(*(Vec *)(s->x2B0+0x10),&s->x80);
 COPY_VECTOR(*(Vec *)(s->x2B0+0xA0),&s->x90);
 pos=&s->x80;
 func_00221ED0(s->x2B0+0x40,s);
 old=s->x36C;
 if(D_0043FA34[0]==0 && s->x2B8!=4) {
  value=s->x2B4->x13E2 ? s->x2B4->x1284 : s->x2B4->x11BC;
  s->x36C=value!=0;
 } else s->x36C=0;
 if(old != s->x36C) func_0020ED30(arg1,0x80,0x80,0x80,(float)s->x36C,250.0f,500.0f,220.0f,1000.0f);
 if(s->x36C) func_0014B1B8(s,arg1);
 else if(s->x2B8!=4 && s->x360) func_0014B298(s,arg1);
 func_0023D1F0(s->x2B0+0x10,arg1);
 __asm__ volatile("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0xD0(%2)" : : "r"(&previous),"r"(pos),"r"(s) : "memory");
 s->x374=0;
 if(s->x375) s->x374=1;
 else if(s->x2BC==0) {
  s->x374=1;
  if(s->x2B8!=1 || s->x36C || s->x36E || s->x36F) s->x374=0;
 }
}
