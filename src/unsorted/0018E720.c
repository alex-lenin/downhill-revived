/* UNIT 0x0018E720 0x0018E770 */
struct Obj { char pad[0x46c]; int x46c, x470, x474, x478; char pad47c[0x500-0x47c]; int entries[12]; };
extern void func_0018E398(struct Obj *);
void func_0018E720(struct Obj *obj) {
    int i; int *entry;
    obj->x46c = 0;
    obj->x474 = 0;
    obj->x478 = 1;
    for (i=11, entry=&obj->entries[11]; i>=0; --i, --entry) *entry=0;
    func_0018E398(obj);
}
