/* UNIT 0x001715B8 0x00171738 */
typedef struct Handle {char bytes[16];} Handle;
typedef struct Owner {char pad[0x299C];float x299C;} Owner;
typedef struct Bits {unsigned int pad:7;unsigned int index:11;unsigned int id:14;} Bits;
typedef union Resource {unsigned long flags;Bits *ptr;} Resource;
typedef struct Obj {int x0;Resource *x4;Owner *x8;char padC[0x74];Handle x80,x90;char xA0[0x70];char x110[0xD0];int x1E0,x1E4,x1E8;float x1EC;int x1F0;float x1F4;int x1F8,x1FC,x200,x204;unsigned char x208,x209;} Obj;
extern void func_001DCDC8(void *);
extern void func_001DDB58(void *,void *);
extern char D_003AD8B0[];
extern void func_00171738(Obj *,int);
extern void func_00203590(Handle *,unsigned int,unsigned int,int);
extern void func_00203CB8(Handle *);
void func_001715B8(Obj *obj,Owner *owner,Resource *resource,int unused,int arg4) {
 Handle copy;unsigned int bits,index;
 obj->x8=owner;obj->x4=resource;
 __asm__ volatile("sqc2 $vf0,0x60(%0)"::"r"(obj):"memory");
 __asm__ volatile("sqc2 $vf0,0x70(%0)"::"r"(obj):"memory");
 obj->x1E0=0;obj->x1EC=1.0f;obj->x1E4=0;obj->x1E8=0;obj->x1F0=0;obj->x1F4=obj->x8->x299C;obj->x1F8=0;obj->x1FC=0;obj->x200=0;obj->x204=0;obj->x208=0;obj->x209=0;
 func_001DCDC8(obj->xA0);func_001DDB58(obj->x110,D_003AD8B0);func_00171738(obj,arg4);
 if(obj->x4) {
 obj->x4->flags &= 0xFDFFFFFFFFFFFFFFUL;
 func_00203590(&obj->x80,obj->x4->ptr->id,obj->x4->ptr->index,0);
 func_00203590(&obj->x90,obj->x4->ptr->id,obj->x4->ptr->index,1);
 if(*(int *)&obj->x80) {copy=obj->x80;func_00203CB8(&copy);}
 if(*(int *)&obj->x90) {copy=obj->x90;func_00203CB8(&copy);}
 }
}
