/* UNIT 0x001FA3E8 0x001FA4B0 */
typedef struct { int x0,x4,x8; } Obj;
extern char D_004C84D0[],D_0029D960[],D_0029D968[],D_0029D970[],D_0029D978[];
extern int D_002776E8[],D_00277728[]; extern void func_00254D68(void*,void*,void*,void*,...);
void *func_001FA3E8(Obj *o) { if(o->x4==0) func_00254D68(D_004C84D0,D_0029D960,D_002776E8+o->x0,D_00277728,o->x8?D_0029D968:D_0029D970); else func_00254D68(D_004C84D0,D_0029D978,D_002776E8+o->x0,D_00277728+o->x4); return D_004C84D0; }
