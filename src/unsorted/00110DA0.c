/* UNIT 0x00110DA0 0x00110EF8 */
struct Sub { char pad[0xc4]; int xC4; char p[4]; int xCC; };
struct Obj { char *base; char p4[0x2b8]; float x2BC,x2C0; int x2C4; char p2c8[8]; float x2D0; struct Sub sub; int x3A4; char p3a8[0x84]; int x42C; char p430[4]; int x434; char p438[8]; unsigned long flags; };
extern float func_001E7C00(void *,int);
extern void func_001E6E50(void *,int);
extern void func_001146F0(struct Sub *);
extern int func_001E7B78(void *,int);
extern int func_00222BD8(int,int);
void func_00110DA0(struct Obj *obj,int arg,int enabled) {
 struct Sub *sub; int chance;
 obj->x3A4=arg;
 obj->flags &= 0x7fffffffffffffffUL;
 obj->x42C=(int)(func_001E7C00(obj->base+0x5ff0,arg)*60.0f)-11;
 if(enabled) *(int *)(obj->base+0x61dc)=1;
 sub=&obj->sub;
 func_001E6E50(obj->base+0x6610,arg);
 func_001146F0(sub);
 obj->flags=(obj->flags | 0x100000000000UL) & 0xbfffffffffffffffUL;
 if(((sub->xCC >> 5)&1) && func_001E7B78(obj->base+0x5ff0,arg)) {
 chance=sub->xC4;
 if(func_00222BD8(0,100)<chance) obj->flags |= 0x4000000000000000UL;
 }
 obj->x2C4=0; obj->x2C0=obj->x2BC;
 obj->x2D0=func_001E7C00(obj->base+0x5ff0,arg);
 obj->x434=0;
}
