/* UNIT 0x0013EF28 0x0013EFF8 */
struct Sub { char pad[0x740]; unsigned char x740; char pad741[0x3f]; unsigned char x780; };
struct Global { char pad[0x10c]; int x10c; };
extern struct Global D_003F52D0;
extern char D_004B25A0[];
extern void func_00190358(void *);
extern void func_0018E098(void *);
extern void func_0013F4D8(void *);
extern void func_0013F098(void *);
extern void func_0018DBA8(void *);
extern void func_0018DCE8(void *);
extern void func_0018E1C0(void *);
extern void func_0018E518(void *);
extern void *func_0013AF60(int);
extern void func_00218498(void *, int);
extern void func_001BB040(void *, int);
extern void func_001BAF10(void *, int);
void func_0013EF28(char *obj) {
 *(int *)(obj+0x4c8)=0;
 D_003F52D0.x10c=15;
 func_00190358(&D_003F52D0);
 func_0018E098(obj);
 func_0013F4D8(obj);
 func_0013F098(obj);
 func_0018DBA8(obj);
 func_0018DCE8(obj);
 func_0018E1C0(obj);
 func_0018E518(obj);
 func_00218498(func_0013AF60(0x25b2),((struct Sub *)(obj+*(int *)obj))->x740);
 func_00218498(func_0013AF60(0x25b3),((struct Sub *)(obj+*(int *)obj))->x780);
 func_001BB040(D_004B25A0,3);
 func_001BAF10(D_004B25A0,3);
}
