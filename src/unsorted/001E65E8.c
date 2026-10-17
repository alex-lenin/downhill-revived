/* UNIT 0x001E65E8 0x001E67A0 */
typedef struct Owner { char pad[0x633c]; int x633C; char pad2[0x1648]; unsigned x7988; char pad3[0x14]; int x79A0; } Owner;
typedef struct Entry { int kind, active, x8; } Entry;
typedef struct Obj { int x0; char p4[12]; float x10,x14,x18,x1C; char p20[12]; float x2C; int x30,x34,x38,x3C; Owner *x40; Entry entries[43]; int counts[43]; int x2F4; char p2f8[0xa8]; int counts2[8]; int entries2[213]; int x714,x718,x71C; char p720[0x9c2]; unsigned char x10E2; } Obj;
extern int func_001E5E58(Obj *,int);
extern float func_001E51C0(Obj *,int),func_001E51E8(Obj *,int);
extern void func_00179130(void *,int,int),func_001E67A8(Obj *),func_001E63C0(Obj *);
extern char D_003B2270[];
static __inline__ void play(unsigned id,int n) { char *base=D_003B2270; if(id<4) func_00179130((id*0x1380+0x150)+base,n,0); }
void func_001E65E8(Obj *o) {
float value; int i;
o->x10E2=1; o->x40->x633C=0;
value=o->x1C+(float)func_001E5E58(o,o->x0)+o->x10+o->x14+o->x18;
value*=func_001E51C0(o,o->x718); value*=func_001E51E8(o,o->x71C);
if(value!=0.0f) { unsigned id=o->x40->x7988; int n=(int)value; play(id,n); }
for(i=0;i<o->x714;i++) { o->counts[o->entries[i].kind]--; o->entries[i].kind=0; if(o->entries[i].active) {o->x2F4--;o->entries[i].active=0;} }
for(i=0;i<o->x718;i++) { o->counts2[o->entries2[i]]--; o->entries2[i]=0; }
o->x34=0;func_001E67A8(o);func_001E63C0(o);
if(!o->x40->x79A0)o->x2C-=(float)o->x3C;
}
