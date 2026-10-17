/* UNIT 0x00163B70 0x00163BE0 */
typedef struct { char pad[0x14]; unsigned short flags; } Child;
typedef struct { char pad[0x5C]; int count; } Obj;
extern Child *func_00163D40(Obj *, int);
int func_00163B70(Obj *obj) {
    int i;
    int count = 0;
    for (i = 0; i < obj->count; i++) {
        count += func_00163D40(obj, i)->flags & 1;
    }
    return count;
}
