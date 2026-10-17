/* UNIT 0x00234EC8 0x00234F38 */
typedef struct Entry {
    int x0;
    int x4;
    int x8;
} Entry;

typedef struct Obj {
    char pad0[0x18C];
    unsigned int x18C[18];
    Entry *x1D4;
} Obj;

void func_00234EC8(Obj *obj, unsigned int pos, int *out, unsigned int *index)
{
    *index = 0;
    do {
        if (pos < obj->x18C[*index + 1]) {
            if (*index != 0) {
                *out -= obj->x1D4[obj->x18C[*index]].x0;
            }
            return;
        }
        (*index)++;
    } while (*index < 16);
}
