/* UNIT 0x00161210 0x00161280 */
struct Owner { char pad[0x78D0]; void *x78D0; };
struct Global { char pad[0x50130]; struct Owner *x50130; };
struct Obj { char pad[0x4A8]; int x4A8; };
extern struct Global D_002AF2C0;
extern char D_003F52D0[];
extern void func_0018DB08(struct Obj *);
extern void func_001B5AF0(void *,int,int);
extern void func_0021F8E0(void);
extern void func_0019A0D8(void *);
void func_00161210(struct Obj *p) {
 p->x4A8=5; func_0018DB08(p);
 func_001B5AF0(D_003F52D0,1,1); func_0021F8E0();
 if(D_002AF2C0.x50130) { if(D_002AF2C0.x50130->x78D0) func_0019A0D8(D_002AF2C0.x50130->x78D0); }
}
