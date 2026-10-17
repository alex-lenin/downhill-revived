/* UNIT 0x0017BCB0 0x0017BD60 */
struct Obj {
    unsigned int type : 6;
    unsigned int rest : 26;
    int x4;
    unsigned short count;
    char padA[0x16];
    int ids[1];
};

struct Mgr {
    char pad0[0xC84];
    char slots[1][0x10];
};

extern int func_00215B58(int id);
extern int func_0017C830(struct Mgr *mgr, int res);
extern void func_0017C8A8(struct Mgr *mgr, int item, void *slot);

void func_0017BCB0(struct Mgr *mgr, struct Obj *obj)
{
    int i;

    for (i = 0; i < obj->count; i++) {
        func_0017C8A8(mgr, func_0017C830(mgr, func_00215B58(obj->ids[i])), mgr->slots[i]);
    }
    obj->type = 9;
}
