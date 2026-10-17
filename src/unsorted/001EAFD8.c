/* UNIT 0x001EAFD8 0x001EB058 */
typedef struct { char pad[0x34]; unsigned int *x34; } Obj;
extern void func_00210868(Obj *);
extern void func_00203590(void *, unsigned int, int, int);
extern float func_00203C28(void *);
extern unsigned char D_004C5C50[];
void func_001EAFD8(void *obj) {
    int local[4];
    Obj *p = (Obj *)((char *)obj + 0x10);
    func_00210868(p);
    local[2] = 0;
    func_00203590(local, *p->x34 >> 18, 0, 0);
    if (func_00203C28(local) >= 1.0f) {
        D_004C5C50[0xf1] = 0;
        D_004C5C50[0xf2] = 1;
    }
}
