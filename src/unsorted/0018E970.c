/* UNIT 0x0018E970 0x0018E9D8 */
typedef struct Table {
    char pad0[0x30];
    short x30;
    char pad32[2];
    void (*x34)(void *);
} Table;
typedef struct Obj {
    char pad0[0x4F8];
    Table *x4F8;
} Obj;
extern void func_0018E770(void *);
extern Obj *func_0018EAC8(void *, int);

void func_0018E970(void *owner) {
    Obj *obj;
    Table *table;
    int i;

    func_0018E770(owner);
    for (i = 0; i < 12; i++) {
        obj = func_0018EAC8(owner, i);
        if (obj) {
            table = obj->x4F8;
            table->x34((char *)obj + table->x30);
        }
    }
}
