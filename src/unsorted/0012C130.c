/* UNIT 0x0012C130 0x0012C2C0 */
typedef struct Flag {char pad[4];unsigned char x4;} Flag;
typedef struct Part {char pad[0x16D4];long *x16D4;char p16D8[0xA88];unsigned char x2160;} Part;
typedef struct Obj {char pad[0x1E40];Part part;char p3FA4[0x392C];void *x78D0;char p78D4[0x58];long *x792C;Flag *x7930;char p7934[8];int x793C;char p7940[0x38];unsigned int x7978;char p797C[8];int x7984;unsigned int x7988;char p798C[0xD0];int x7A5C;} Obj;
typedef struct Slot {char pad[0x12B4];Obj *x12B4;char tail[0xC8];} Slot;
extern Slot D_003B2270[];
extern char D_0037EA60[][0x380];
extern int D_0039E834[],D_004B2828[];
extern void func_00199338(void*,Obj*);
extern void func_001487D8(void*,Obj*);
extern void func_002182A8(long*,int);
extern void func_001AC300(Part*,int);
extern int func_0021D638(long*,float);
extern void func_0021D738(int,long*);
void func_0012C130(Obj *obj,unsigned int id,void *arg){
 Part *part; Slot *slots;int handle;
 obj->x7A5C=1;obj->x7984=1;obj->x7978=id;obj->x7988=id;obj->x78D0=arg;
 func_00199338(arg,obj);
 slots=D_003B2270;
 obj->x7A5C=1;
 if(obj->x7978<4)slots[obj->x7978].x12B4=obj;
 func_001487D8(D_0037EA60[obj->x7978],obj);
 if(obj->x7978<4)slots[obj->x7978].x12B4=obj;
 part=&obj->part;
 func_002182A8(obj->x792C,1);
 func_002182A8(part->x16D4,1);
 func_001AC300(part,D_0039E834[0]!=0);
 if(D_004B2828[0]==1){
 if(part->x2160){handle=func_0021D638(obj->x792C,9.0f);obj->x793C=handle;if(handle)func_0021D738(handle,part->x16D4);}
 else obj->x793C=func_0021D638(obj->x792C,3.0f);
 if(obj->x7930)obj->x7930->x4=0;
 *obj->x792C&=0xFEFFFFFFFFFFFFFFUL;
 }else if(obj->x7930)obj->x7930->x4=1;
}
