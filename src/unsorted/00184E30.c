/* UNIT 0x00184E30 0x00184F98 .rodata=0x28C630:0x28C670 .sdata=0x29CD50:0x29CD58 .sbss=0x29DDF0:0x29DDF8 */
typedef struct { unsigned int x0; int x4,x8,xC,x10; } Obj;
int D_0029CD50=2; int D_0029CD54=0;
int D_0029DDF0 __attribute__((section(".sbss")))=0;
int D_0029DDF4 __attribute__((section(".sbss")))=0;
extern int func_00184938(Obj *,int),func_002460F8(int,int,int,int),func_00246258(int),func_002464B8(int,int,int);
extern int func_002466A0(int,int *,int *);
int func_00184E30(Obj *obj, Obj *other) {
 if(other->x0) return 1;
 switch(obj->x0) {
 case 0: func_002460F8(other->x10,other->xC,obj->x8,0x203); obj->x0++;
 case 1: if(func_002466A0(1,&D_0029CD50,&D_0029DDF0)) {
 if(D_0029DDF0 == -1) goto error;
 goto advance;
 } break;
 case 2: func_002464B8(D_0029DDF0,obj->xC,obj->x10); obj->x0++;
 case 3: D_0029CD50=6;
 if(func_002466A0(1,&D_0029CD50,&D_0029DDF4)) {
 if(D_0029DDF4 < 0) { error: func_00184938(other,other->x4); return 1; }
 advance: obj->x0++;
 } break;
 case 4: func_00246258(D_0029DDF0); obj->x0++;
 case 5: D_0029CD50=3;
 if(func_002466A0(1,&D_0029CD50,&D_0029DDF4)) {
 if(D_0029DDF4<0) func_00184938(other,other->x4);
 return 1;
 } break;
 }
 return 0;
}
const char D_0028C648[16]="icon.sys";
const char D_0028C658[24]="/icon.sys";
