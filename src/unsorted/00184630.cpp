/* UNIT 0x00184630 0x00184848 .sbss=0x29DDDC:0x29DDE8 */
class Item { public: int x0; virtual void v1(); virtual void v2(); virtual void v3(); virtual int active(); };
struct State { unsigned char *base; int x4,x8,xC,x10,x14,x18; };
extern void func_001AC578(void *);
extern void func_0012FE38(void *,void *);
extern void func_00184848(State *);
extern int func_00238F48();
int D_0029DDDC __attribute__((section(".sbss"))) = 0;
int D_0029DDE0 __attribute__((section(".sbss"))) = 0;
#define FIELD(p,o) (*(int *)((p)+(o)))
#define ITEM(o) ((Item *)(base+(o)))
void func_00184630(State *state) {
 unsigned char *base=state->base;
 Item *item=ITEM(0x5E40);
 int elapsed, active;
 func_001AC578(base+0x1E40);
 if(item->active() || (item=ITEM(0x5E70))->active() || (item=ITEM(0x5E90))->active() || (item=ITEM(0x5FF0))->active() || (item=ITEM(0x6210))->active() || (item=ITEM(0x621C))->active() || (item=ITEM(0x62B4))->active()) func_0012FE38(base,item);
 else { item=ITEM(0x6310); if(item->active()) func_0012FE38(base,item); }
 func_00184848(state);
 elapsed=func_00238F48()-state->x10;
 if((unsigned char)base[0x13E2]) active=FIELD(base,0x1284); else active=FIELD(base,0x11BC);
 if(active) state->xC=1;
 if(state->xC==1) { if(!((unsigned char)base[0x13E2] ? FIELD(base,0x1284) : FIELD(base,0x11BC))) func_0012FE38(base,base+0x4ED0); }
 if(!D_0029DDE0) { D_0029DDE0=1; D_0029DDDC=state->x18/16+30; }
 if(!state->xC && elapsed>D_0029DDDC) func_0012FE38(base,base+0x4ED0);
}
