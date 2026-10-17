/* UNIT 0x0015E630 0x0015E660 */
typedef struct Obj { int x0; unsigned char x4; } Obj;
extern Obj *D_003F5404[];
extern void func_0018E468(void *obj);
void func_0015E630(void *obj) {
    if (D_003F5404[0]) D_003F5404[0]->x4 = 0;
    func_0018E468(obj);
}
