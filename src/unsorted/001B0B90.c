/* UNIT 0x001B0B90 0x001B0C10 .sdata=0x29D3F0:0x29D3F8 */
struct Vec { float x,y,z; };
struct Obj { char p0[0x5b0]; char x5b0; char p5b1[15]; char x5c0[0x454]; float xA14,xA18,xA1c; char pA20[0x30]; struct Vec xA50; int pA5c; struct Vec xA60; };
char D_0029D3F0=0;
int D_0029D3F4=0;
extern void func_00254204(void *,int,int);
extern void func_001B0F28(void *);
extern void func_001B3068(void *);
void func_001B0B90(struct Obj *o) {
 struct Vec *a,*b;
 func_00254204(o,0,0xa70);
 o->x5b0=0;
 func_001B0F28(o->x5c0);
 a=&o->xA50; a->x=0; a->y=0; a->z=0;
 b=&o->xA60; b->x=0; b->y=0; b->z=0;
 func_001B3068(o);
 o->xA14=1.0f; o->xA18=1.0f; o->xA1c=1.0f;
 D_0029D3F0=0; D_0029D3F4=0;
}
