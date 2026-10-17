/* UNIT 0x001BE468 0x001BE5B0 */
struct Entry { char pad[0x24]; int x24,x28; char pad2C[0x174]; };
struct Small { int x0,x4,x8; };
struct Obj { int x0,x4; int x8[30]; int x80,x84,x88; struct Small x8C[30]; int x1F4,x1F8,x1FC,x200; };
extern char D_0032CA00[];
extern int func_001BE328(struct Obj*,void*);
extern int func_001BE5B0(struct Obj*);
extern void func_001BE8B0(void*,int);
extern void func_001BE638(void*,void*,int,int);
void func_001BE468(struct Obj *o) {
 char *g=D_0032CA00;
 void **table=(void**)(g+0x30C0);
 void *p=table[o->x80];
 struct Entry *entry;
 int index, index2;
 char *rec; void *dest;
 if(p) {
 o->x8[o->x80]=func_001BE328(o,(char*)p+16);
 func_001BE5B0(o);
 if(o->x1FC) { func_001BE8B0(&o->x1F4,o->x200); o->x1FC=0; }
 index=o->x84;
 if(o->x8[index]<160) { entry=(struct Entry*)((char*)g+index*0x1A0); func_001BE638(&o->x8C[index],table[index],entry->x24,entry->x28); }
 index2=o->x88;
 if(o->x8[index2]<160) { dest=(char*)o+(index2*12+0x8C); rec=(char*)g+o->x84*0x1A0; func_001BE638(dest,table[index2],*(int*)(rec+0x24),*(int*)(rec+0x28)); }
 o->x80++;
 if(o->x80==*(int*)(g+0x3244)) o->x80=0;
 }
}
