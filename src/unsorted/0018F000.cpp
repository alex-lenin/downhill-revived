/* UNIT 0x0018F000 0x0018F090 */
class Entry { char pad[0x4f8]; public: virtual void unused(); virtual void pop(); virtual void resume(); };
struct Obj { char pad[0x500]; Entry *entries[12]; int x530; int x534; };
extern void func_0018E468(Obj *);
void func_0018F000(Obj *obj) {
    Entry **entries; Entry **current;
    if (obj->x534 == 0) { func_0018E468(obj); return; }
    entries = obj->entries;
    current = entries;
    current += obj->x534;
    (*current)->pop();
    --obj->x534;
    entries += obj->x534;
    (*entries)->resume();
}
