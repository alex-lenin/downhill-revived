/* UNIT 0x001BC810 0x001BC868 */
typedef struct {
    int x0;
    int x4;
    char pad8[0x84 - 0x8];
} Entry;

typedef struct {
    char pad0[0x638];
    Entry entries[1];
} Object;

/* returns D_0029E3D8; takes no arguments */
extern int func_00238F48(void);

void func_001BC810(Object *object, int index) {
    if (object->entries[index].x0 == 0) {
        object->entries[index].x0 = 1;
        object->entries[index].x4 = func_00238F48();
    }
}
