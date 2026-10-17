/* UNIT 0x00127738 0x001279D8 */
class Target;
typedef int (Target::*Check)();
class Target { public: virtual void dummy(); char p0[0x28]; int x28; char p2c[0x128]; Check x154; };
struct Handle { char p0[0x230]; Target *x230; };
struct Group { char p0[0xD0]; char xD0[0x14]; int xE4; char pE8[8]; };
struct Header { char p0[0x1420]; int count,processed,remaining,active,frame; char p1434[4]; int counts[100]; char p15c8[0x3B48-0x15C8]; int order[100]; };
struct Obj { char p0[0x10]; Group groups[32]; char pad[0x20000-0x10-32*0xF0]; Header header; };
struct Global { char p0[0x10C]; int x10C; char p110[0x178]; int x288; };
extern Global D_004B25A0;
extern int D_0029E4A0;
extern void func_00127BB8(Obj*),func_001279D8(Obj*),func_00192DB0(Handle*),func_001BEE28(void*,Handle*,int,Handle*,int);
extern Handle *func_00193498(int);
extern int func_00222B08(int,int);
static inline int check(Target *t) { return (t->*t->x154)(); }
void func_00127738(Obj *o) {
 int i,j,index,count,*p; Target *t; Handle *h; Group *g;
 if(D_004B25A0.x10C==14)return;
 if(D_004B25A0.x288>=3)return;
 if(!o->header.active)return;
 func_00127BB8(o);func_001279D8(o);
 o->header.processed=0;
 for(i=0;i<o->header.count;i++) {
 index=o->header.order[i];
 p=(int*)((char*)o+(index*0x60+0x215C8)); count=o->header.counts[index]; j=count;
 if(j)do {
 t=func_00193498(*p)->x230;
 if((unsigned char)check(t))func_00193498(*p)->x230->x28=1;
 else func_00193498(*p)->x230->x28=0;
 h=func_00193498(*p++);func_00192DB0(h);o->header.processed++;j--;
 }while(j);
 g=&o->groups[index];h=func_00193498(*(int*)((char*)o+(index*0x60+0x215C8)));
 func_001BEE28(g->xD0,h,o->header.counts[index],func_00193498(*(int*)((char*)o+index*0x60+func_00222B08(0,count)*4+0x215C8)),g->xE4);
 }
 o->header.remaining=D_0029E4A0-o->header.processed; o->header.frame++;
 if(o->header.processed<o->header.frame)o->header.frame=0;
}
