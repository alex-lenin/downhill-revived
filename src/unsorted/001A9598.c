/* UNIT 0x001A9598 0x001A95E8 */
typedef struct Entry { unsigned short flags; short x2; } Entry;
typedef struct Obj { char pad0[0x18]; Entry entries[7]; int x34; char pad38[15]; signed char x47; } Obj;
extern void func_001A95E8(Obj *, int);
void func_001A9598(Obj *p, int index) {
    if ((p->entries[index].flags & 0x2000) && p->x47 == 0) {
        func_001A95E8(p, index);
        return;
    }
    p->x34 = 1;
}
