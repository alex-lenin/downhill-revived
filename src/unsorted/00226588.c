/* UNIT 0x00226588 0x00226658 */
extern int D_00720280[];
extern char D_26D8[];
typedef struct Obj { char pad[0x27c0]; int flags; char pad2[0x6c]; int *x2830; int *x2834; } Obj;
void func_00226588(Obj *obj, int index) {
 int count; int *base; int *p;
 p=obj->x2834; *p=0;
 count=D_00720280[index];
 base=(int *)((unsigned int)((char *)obj+12)&~15);
 p=obj->x2830;
 if(count) {
 p[-1]=((count*3)<<16)|0x6c000031;
 p=(int *)((char *)p+count*48);
 p[0]=((unsigned int)D_26D8>>3)|0x14000000; p[1]=0x11000000; p[2]=0; p[3]=0;
 base[0]=((int)((char *)p-(char *)base)>>4)|0x60000000; base[2]=0x6c25000c; base[6]=count;
 } else { base[0]=0x60000000; base[2]=0; base[3]=0; }
 obj->flags|=0x800;
}
