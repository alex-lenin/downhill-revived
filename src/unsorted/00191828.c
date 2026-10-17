/* UNIT 0x00191828 0x001918C0 */
typedef struct Obj {
    char pad0[0x1F0];
    int x1F0[6];
    int x208[3];
    int x214;
} Obj;

extern int func_001591D8(unsigned int *);
extern void func_00158120(void *, int, float);
extern char D_00383A10[];

void func_00191828(Obj *obj, unsigned int *value)
{
    obj->x1F0[obj->x214] = func_001591D8(value);
    if (value != 0) {
        obj->x208[obj->x214] = (*value >> 7) & 0x7FF;
    }
    func_00158120(D_00383A10, obj->x1F0[obj->x214], 7.0f);
    obj->x214++;
}
