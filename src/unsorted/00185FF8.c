/* UNIT 0x00185FF8 0x00186320 .rodata=0x28C670:0x28C690 .sdata=0x29CD78:0x29CD80 */
struct Block64 { long x[8]; };
struct Block48 { long x[6]; };
struct Block16 { long x[2]; };
struct Icon { int x0; unsigned char x4; char x5; short x6; int x8,xC; struct Block64 x10; struct Block48 x50,x80; struct Block16 xB0; char xC0[68],x104[64],x144[64],x184[64]; };
struct Obj { int x0,x4,x8,xC,x10; char pad[109]; char x81[32],xA1[1]; };
extern struct Block64 D_0026F280;
extern struct Block48 D_0026F2C0,D_0026F2F0;
extern struct Block16 D_0026F320;
extern struct Signature { int x0; unsigned char x4; } D_0029CD70[2];
extern void *memcpy(void *,const void *,unsigned int);
int D_0029CD78=-1,D_0029CD7C=-1;
extern struct Icon D_003CD540;
extern char D_003CD908[],D_0028C658[];
extern void func_00254544(void *,const void *);
extern void strncat(char *,const char *,int);
extern void func_00232EE0(void *,void *);
extern void *func_00254204(void *,int,int);
extern int func_002460F8(int,int,void *,int);
extern int func_002466A0(int,int *,int *);
extern int func_002464B8(int,void *,int);
extern int func_00246258(int);
extern void func_00184938(struct Obj *,int);
int func_00185FF8(struct Obj *obj,unsigned int *state,void *name) {
 char buffer[64]; int a,b,c; char *p;
 switch(*state) {
 case 0:
 func_00254544(D_003CD908,name);
 strncat(D_003CD908,D_0028C658,74);
 p=obj->xA1;
 func_00232EE0(obj->x81,buffer);
 func_00254204(&D_003CD540,0,964);
 D_003CD540.x0=D_0029CD70[0].x0; D_003CD540.x4=D_0029CD70[0].x4;
 func_00254544(D_003CD540.xC0,buffer);
 D_003CD540.x6=18; D_003CD540.xC=128;
 memcpy(&D_003CD540.x10,&D_0026F280,64); memcpy(&D_003CD540.x50,&D_0026F2C0,48); memcpy(&D_003CD540.x80,&D_0026F2F0,48);
 memcpy(&D_003CD540.xB0,&D_0026F320,16);
 func_00254544(D_003CD540.x104,p); func_00254544(D_003CD540.x144,p); func_00254544(D_003CD540.x184,p);
 func_002460F8(obj->x10,obj->xC,D_003CD908,515); *state = *state + 1;
 case 1:
 a=2; if(func_002466A0(1,&a,&D_0029CD78)) { if(D_0029CD78<0) { func_00184938(obj,obj->x4); return 1; } *state = *state + 1; } break;
 case 2: func_002464B8(D_0029CD78,&D_003CD540,964); *state = *state + 1;
 case 3: b=6; if(func_002466A0(1,&b,&D_0029CD7C)) { if(D_0029CD7C<0) { func_00184938(obj,obj->x4); return 1; } *state = *state + 1; } break;
 case 4: func_00246258(D_0029CD78); *state = *state + 1;
 case 5: c=3; if(func_002466A0(1,&c,&D_0029CD7C)) { if(D_0029CD7C<0) {func_00184938(obj,obj->x4); } return 1; } break;
 }
 return 0;

}
