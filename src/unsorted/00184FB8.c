/* UNIT 0x00184FB8 0x001851F8 .sdata=0x29CD58:0x29CD60 .sbss=0x29DDF8:0x29DDFC */
struct Owner { int x0; char pad4[0xC84]; int xC88; };
struct Entry { int x0; char pad4[16]; };
struct Menu { int x0,x4,x8,xC,x10; char pad14[0xBC]; struct Entry rows[1]; };
extern char *strcpy(char *,const char *);
extern char D_003C6CC0[],D_003C85C0[],D_003C85E0[];
extern int func_00261F60(struct Menu *);
extern int func_00246960(int,int,char *,int,int,char *);
extern int func_002466A0(int,int *,int *);
extern void func_00184938(struct Menu *,int);
extern void func_00254544(void *,void *);

int func_00184FB8(struct Owner *o,struct Menu *m) {
 char name[16];
 int id;
 int i;
 if(m->x0) return 1;
 o->xC88=0;
 if(!m->rows[func_00261F60(m)].x0) return 1;
 strcpy(name,"*");
 func_00246960(m->x10,m->xC,name,0,100,D_003C6CC0);
 id=13;
 func_002466A0(0,&id,&o->xC88);
 if(o->xC88<0) {
  func_00184938(m,m->x4);
  return 1;
 }
 for(i=0;i<o->xC88;i++) func_00254544((char *)o+8+i*32,D_003C6CC0+0x20+i*64);
 return 1;
}

int D_0029CD5C=13;
int D_0029DDF8 __attribute__((section(".sbss")))=0;

int func_001850C0(struct Owner *o,struct Menu *m) {
 int i;
 if(m->x0) return 1;
 o->xC88=0;
 if(!m->rows[func_00261F60(m)].x0) return 1;
 switch(o->x0) {
 case 0:
  *(unsigned short *)&D_0029DDF8=*(unsigned short *)"*";
  func_00246960(m->x10,m->xC,(char *)&D_0029DDF8,0,100,D_003C85C0);
  o->x0 = o->x0 + 1;
 case 1:
  if(!func_002466A0(1,&D_0029CD5C,&o->xC88)) return 0;
  if(o->xC88<0) {
   func_00184938(m,m->x4);
   return 1;
  }
  for(i=0;i<o->xC88;i++) func_00254544((char *)o+8+i*32,D_003C85E0+i*64);
  return 1;
 default: return 0;
 }
}
