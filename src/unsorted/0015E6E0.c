/* UNIT 0x0015E6E0 0x0015E750 */
struct Values { int x0,x4,x8; char padC[0x1B]; unsigned char x27; };
struct Obj { char pad[0x510]; int x510,x514,x518,x51C,x520,x524; };
struct Global { char pad[0x11A60]; struct Values x11A60; };
extern struct Global D_004225A0;
struct Index { int x0; char pad[8]; };
extern struct Index D_003F5400;
extern int func_002205A0(int);
extern int func_00220180(int);
static inline struct Values *get_values(struct Global *g) { return &g->x11A60; }
void func_0015E6E0(struct Obj *p) {
 struct Values *v=get_values(&D_004225A0);
 p->x510=v->x8; p->x518=v->x0; p->x514=v->x4; p->x51C=v->x27;
 p->x520=func_00220180(func_002205A0(D_003F5400.x0)); p->x524=0;
}
