/* UNIT 0x00175148 0x001751C8 */
typedef struct { char pad0[0x140]; int x140; char pad144[12]; char x150[4][0x1380]; } Obj;
extern void func_00176568(void *, int);
extern void func_0017B670(Obj *);
extern char D_0026EC78[];
extern void *D_0029DD78;
extern float D_0029DD88;
void func_00175148(Obj *obj) {
    char *part = obj->x150[0];
    int i = 0;
    do { func_00176568(part, i); i++; part += 0x1380; } while (i < 4);
    func_0017B670(obj);
    obj->x140 = 0;
    D_0029DD88 = 0.625f;
    D_0029DD78 = D_0026EC78;
}
