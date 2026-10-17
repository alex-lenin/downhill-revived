/* UNIT 0x001E95B8 0x001E9628 */
typedef struct Pair { long a,b; } __attribute__((packed)) Pair;
typedef struct Base { char pad[0x1E54]; Pair entries[1]; } Base;
typedef struct Obj { Base *x0; char pad[0x30]; int x34; } Obj;
extern int func_00204230(Pair *);
extern void func_0012FE38(void *,void *);
void func_001E95B8(Obj *p) {
 Base *base=p->x0;
 Pair value=base->entries[p->x34];
 if(((func_00204230(&value)^1)&0xFF)) func_0012FE38(base,(char *)base+0x4ED0);
}
