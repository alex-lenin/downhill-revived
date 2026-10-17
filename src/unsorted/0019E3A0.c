/* UNIT 0x0019E3A0 0x0019E518 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Obj { char pad0[0x1c]; short x1c; char pad1e[0x418-0x1e]; int x418; char pad41c[0x490-0x41c]; u128 x490; char pad4a0[0x61c-0x4a0]; unsigned char x61c[9]; unsigned char x625; } Obj;
typedef struct { char pad[0x20]; unsigned char x20; } State;
typedef struct { char pad[0x610]; unsigned char x610; } State2;
extern State D_003EB760;
extern State2 D_003E6830;
extern char D_003E3BD0[], D_003E5BF0[], D_00286070[];
extern int D_0029CD98;
extern Obj *D_003F5420[];
extern void func_00187048(void *, void *);
extern void func_001870E0(void *, void *);
extern int func_00187500(Obj *);
extern void func_0019E550(Obj *), func_0018A5A8(void *, int), func_00187438(Obj *);
extern int func_0019E518(Obj *);
static inline char *GetName(void) { return D_00286070; }
static inline int GetKind(Obj *obj) { return obj->x1c; }
void func_0019E3A0(Obj *obj) {
 struct { int x0; char *x4; } tmp;
 if (!D_003EB760.x20 && (GetKind(obj) ^ 45) == 0 && obj->x418) {
 tmp.x4=GetName(); tmp.x0=0;
 func_00187048(D_003E3BD0,&tmp);
 func_001870E0(D_003E3BD0,&D_003EB760);
 func_00187500(obj); obj->x625=1;
 if(D_003F5420[0]==obj) func_0019E550(obj);
 } else {
 unsigned char *p=obj->x61c;
 D_003E6830.x610=0; func_0018A5A8(D_003E5BF0,0);
 if(D_003EB760.x20 || p[func_0019E518(obj)]) {
 if(p[func_0019E518(obj)]) { D_003E6830.x610=1; func_0018A5A8(D_003E5BF0,D_0029CD98); }
 func_00187438(obj);
 } else obj->x490=(u128)0x4C45434E41435F53UL;
 }
}
