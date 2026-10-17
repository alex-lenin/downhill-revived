/* UNIT 0x00127170 0x001272E0 */
struct Point { float x0,x4,x8,xC; };
struct Table { int x0,count; char pad8[0x98]; unsigned short ids[1]; };
struct Result { char pad0[4]; char x4; char pad5[11]; float pos[4]; char pad20[0x20]; char x40[1]; };
struct Vtable { char pad0[0x20]; short delta; short pad22; struct Result *(*call)(); };
struct Obj { struct Vtable *vt; int x4; void *x8; };
struct State { int x0,x4; char pad8[0x128]; struct Obj *x130; char pad134[0x28]; struct Table *x15C; struct Point *x160,*x164; int x168; char pad16C[0x74]; float x1E0[4]; };
extern float func_00222B68(float,float);
extern struct Point *func_00163D40(void *,unsigned short);
extern char D_00412510[];
extern struct { char pad0[0x50198]; float x50198; } D_002AF2C0;
extern void func_002182A8(void *,int);
extern void func_00126B20(struct State *,int);
extern void func_00123F60(struct State *,int);
extern void func_00123DB0(struct State *,void *);
extern void func_00211280(struct Result *);
#define GET(o) ((o)->vt->call((char *)(o)+(o)->vt->delta))
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_00127170(struct State *s,int flag) {
 struct Point *p; struct Result *r,*r2; float w; int index; float *pos;
 p=s->x160;
 if (!p) { index=(int)func_00222B68(0.0f,(float)s->x15C->count); p=func_00163D40(D_00412510,s->x15C->ids[index]); }
 if (flag && D_002AF2C0.x50198-200.0f < p->x8) return;
 s->x164=p; s->x168=0; s->x0=0; s->x4=0;
 if (s->x130->x8) func_002182A8(s->x130->x8,1);
 r=GET(s->x130); r->x4=1;
 func_00126B20(s,0); func_00123F60(s,1);
 r2=GET(s->x130); pos=s->x1E0; w=pos[3]; COPY_VECTOR(s->x1E0,r2->pos); pos[3]=w;
 r2=GET(s->x130); func_00123DB0(s,r2->x40); func_00211280(r);
}
