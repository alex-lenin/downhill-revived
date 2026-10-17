/* UNIT 0x0010BE38 0x0010BED8 */
typedef struct Obj { char *x0; char pad4[0x3B0]; int x3B4; char pad3B8[0x5C]; int x414; } Obj;
extern int func_00238F60(void);
extern void func_0010BED8(Obj *, char *);
extern void func_0010BF50(Obj *, char *);
extern void func_0010C4C0(Obj *, char *);
extern int D_0043F348[], D_004B26AC[];
void func_0010BE38(Obj *obj) {
    char *part = obj->x0 + 0xFE0;
    obj->x3B4 += func_00238F60();
    obj->x414 += func_00238F60();
    if (D_0043F348[0] == 0) {
        func_0010BED8(obj, part);
        return;
    }
    if (D_004B26AC[0] == 15) {
        func_0010BF50(obj, part);
        return;
    }
    func_0010C4C0(obj, part);
}
