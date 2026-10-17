/* UNIT 0x0018EA60 0x0018EAB0 */
struct Obj { char pad[0x500]; int entries[12]; };
extern void func_0018EAB0(struct Obj *, int, int);
void func_0018EA60(struct Obj *obj, int value) {
    int i; int *entry;
    i=0; entry=obj->entries;
    do {
        if (*entry == 0) { func_0018EAB0(obj,i,value); break; }
        ++i; ++entry;
    } while(i<12);
}
