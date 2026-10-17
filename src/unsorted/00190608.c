/* UNIT 0x00190608 0x00190660 */
struct Entry { short delta; short pad; void (*call)(void *); };
struct VTable { struct Entry slots[15]; };
struct Target { char pad[0x480]; int x480; char pad484[0x74]; struct VTable *x4F8; };
struct Obj { char pad[0x14c]; int x14C; };
extern int func_0018DB10(struct Target *, int);
extern int func_00238F48(void);
void func_00190608(struct Obj *obj, struct Target *target) {
    struct VTable *table;
    if (target && func_0018DB10(target, obj->x14C)) {
        table = target->x4F8;
        table->slots[14].call((char *)target + table->slots[14].delta);
        target->x480 = func_00238F48();
    }
}
