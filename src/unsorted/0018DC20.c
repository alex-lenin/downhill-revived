/* UNIT 0x0018DC20 0x0018DC90 */
typedef struct Global { char pad[0x10c]; int x10C; } Global;
extern Global D_003F52D0;
extern void func_00190358(Global *);
extern void func_0018E098(void *);
extern void func_0018E5A8(void *);
extern void func_0018DBA8(void *);
extern void func_0018DCE8(void *);
extern void func_0018E1C0(void *);
extern void func_0018E518(void *);
extern void func_002013F8(void);
void func_0018DC20(void *obj) {
 D_003F52D0.x10C = 15;
 func_00190358(&D_003F52D0);
 func_0018E098(obj);
 func_0018E5A8(obj);
 func_0018DBA8(obj);
 func_0018DCE8(obj);
 func_0018E1C0(obj);
 func_0018E518(obj);
 func_002013F8();
}
