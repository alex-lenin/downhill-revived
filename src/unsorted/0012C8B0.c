/* UNIT 0x0012C8B0 0x0012C990 */
typedef struct Obj { char pad0[0x4098]; unsigned long x4098; char pad1[0xe2c]; void *x4ecc; char pad2[0x2a00]; void *x78d0,*x78d4; char pad3[0xa8]; int x7980; char pad4[0xd8]; int x7a5c; } Obj;
extern int func_00238F60(void);
extern void func_0017DCC8(void *);
extern void func_0017DC78(void *);
extern void func_00199458(void *);
extern void func_0012C9E0(Obj *);
extern void func_0010BE38(void *);
extern int D_0043F348[],D_004B26AC[];
void func_0012C8B0(Obj *obj) {
    void *part=(char*)obj+0xfe0;
    int state;
    obj->x7980=func_00238F60();
    func_0017DCC8(part);
    func_0017DC78(part);
    if((unsigned int)(D_0043F348[0]-2)<3 && !((unsigned char)(obj->x4098>>2)&1) && obj->x4ecc!=(char*)obj+0x6320) {
        switch(obj->x7a5c) {
        case 1:
        case 2:
            if(D_004B26AC[0]!=10) func_00199458(obj->x78d0);
            func_0012C9E0(obj);
            break;
        case 3:
            func_0010BE38(obj->x78d4);
            break;
        }
    }
}
