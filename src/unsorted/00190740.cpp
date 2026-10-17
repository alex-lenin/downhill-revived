/* UNIT 0x00190740 0x001907B0 */
struct Entry { char pad[14]; unsigned char xE; };
struct Obj {
    char pad[0x154]; Entry *x154[13]; int x188;
    Entry **entries() { return x154; }
};
extern void func_00190410(Obj *, int);
extern void func_001904B8(Obj *, Entry *);
void func_00190740(Obj *obj, int index) {
    int offset = index * 4;
    Entry **entry = (Entry **)((int)obj->entries() + offset);
    if (*entry == 0) func_00190410(obj, 0);
    else { (*entry)->xE = 1; func_001904B8(obj, *entry); }
    obj->x188 = index + 1;
}
