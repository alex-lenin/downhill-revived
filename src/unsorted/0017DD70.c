/* UNIT 0x0017DD70 0x0017DEA0 */
typedef struct Obj {
    char pad0[0x440];
    char x440[1];
} Obj;

typedef struct Entry {
    char pad0[0x2B8];
    int x2B8;
    char pad2BC[0x380 - 0x2BC];
} Entry;

extern Entry D_0037EA60[];
extern int func_002205A0(int index);
extern void func_0017DF00(Obj *obj, int value);
extern void func_0017DFB0(Obj *obj, int value);
extern void func_0017E070(Obj *obj, int value);
extern void func_0017E118(Obj *obj, int value);
extern void func_0017E1C0(Obj *obj, int value);
extern void func_0017E300(Obj *obj, int value);
extern void func_0017E440(Obj *obj, int value);
extern void func_0017E7F0(Obj *obj, int value);
extern void func_0017E900(Obj *obj, int value);
extern void func_0017E970(Obj *obj, int value);
extern void func_0017E848(Obj *obj, int value);
extern void func_0017E898(Obj *obj, int value);
extern void func_0017E8E0(Obj *obj, int value);
extern void func_0017E990(Obj *obj, int value);
extern void func_0017E950(Obj *obj, int value);
extern void func_0017EAE0(Obj *obj, int value);
extern void func_0017DEA0(Obj *obj);
extern void func_001E6F58(void *p);

void func_0017DD70(Obj *obj, int index)
{
    int value;
    Entry *entry;

    value = func_002205A0(index);
    entry = &D_0037EA60[index];
    if (entry->x2B8 < 5) {
        func_0017DF00(obj, value);
        func_0017DFB0(obj, value);
        func_0017E070(obj, value);
        func_0017E118(obj, value);
        func_0017E1C0(obj, value);
        func_0017E300(obj, value);
        func_0017E440(obj, value);
        func_0017E7F0(obj, value);
        func_0017E900(obj, value);
        func_0017E970(obj, value);
        func_0017E848(obj, value);
        func_0017E898(obj, value);
        func_0017E8E0(obj, value);
        func_0017E990(obj, value);
        func_0017E950(obj, value);
        func_0017EAE0(obj, value);
        func_0017DEA0(obj);
    }
    func_001E6F58(obj->x440);
}
