/* UNIT 0x00164CE8 0x00164EE8 */
typedef struct { int x0,x4,x8,xC; } Value;
typedef struct { Value *x0; int *x4,*x8,*xC,*x10,*x14,*x18; Value *x1C,*x20,*x24; } Input;
typedef struct { char pad0[0x19C]; unsigned char x19C; char pad19D[0x273]; Value x410,x420,x430,x440; } Obj;
extern void func_00202E38(Value *,int,int,float),func_001AC7B0(void *,int *);
void func_00164CE8(Obj *obj,Input *in) {
 obj->x410.x0=0; obj->x420.x0=0; obj->x430.x0=0; obj->x440.x0=0;
 if(in->x0 && in->x0->x0) { func_00202E38(in->x0,1,1,100.0f); obj->x440=*in->x0; }
 if(in->x1C && in->x1C->x0) obj->x410=*in->x1C;
 if(in->x20 && in->x20->x0) obj->x420=*in->x20;
 if(in->x24 && in->x24->x0) obj->x430=*in->x24;
 if(!in->x4 || !*in->x4) { obj->x19C=0; return; }
 func_001AC7B0((char *)obj+0x1B8,in->x4);
 if(in->x8 && *in->x8) func_001AC7B0((char *)obj+0x21C,in->x8);
 if(in->xC && *in->xC) func_001AC7B0((char *)obj+0x280,in->xC);
 if(in->x10 && *in->x10) func_001AC7B0((char *)obj+0x2E4,in->x10);
 if(in->x14 && *in->x14) func_001AC7B0((char *)obj+0x348,in->x14);
 if(in->x18 && *in->x18) func_001AC7B0((char *)obj+0x3AC,in->x18);
 obj->x19C=1;
}
