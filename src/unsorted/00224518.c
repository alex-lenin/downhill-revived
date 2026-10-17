/* UNIT 0x00224518 0x002245C0 */
typedef struct Obj { int x0; char pad4[0x90]; int x94; } Obj;
typedef struct Global { char pad0[0xB0]; int xB0; } Global;
extern char D_00663B40[], D_006754E8[];
extern Global D_00663B58;
extern int D_0029D9FC, D_0029E2CC;
extern void func_00230610(int), func_0023DFF8(void *), func_0025A530(int), func_0025A4F0(int), func_0025B000(int), func_0025A410(int,int), func_0023C240(void *), func_00242370(void *), func_002368E8(void *);
int func_00224518(Obj *obj) {
    if (obj->x94 == 0) {
        func_00230610(obj->x0);
        func_0023DFF8(D_00663B40);
        func_0025A530(D_0029E2CC);
        func_0025A4F0(D_0029E2CC);
        func_0025B000(2);
        func_0025A410(2,D_00663B58.xB0);
        D_0029D9FC = 0;
        func_0023C240(&D_00663B58);
        func_00242370(D_006754E8);
        func_002368E8((char *)obj+0x20);
        obj->x94 = 1;
        return 0;
    }
    return 1;
}
