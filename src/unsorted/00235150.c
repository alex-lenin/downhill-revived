/* UNIT 0x00235150 0x002351C0 */
typedef struct { unsigned int *x0; unsigned int x4; } Obj;
extern void *func_001FD430(unsigned int);
Obj *func_00235150(Obj *obj, unsigned int count) {
    unsigned int size = 2;
    unsigned int *p;
    if (count > size - 1) size = count;
    p = func_001FD430(size * 4);
    obj->x4 = 0;
    obj->x0 = p;
    if (p) obj->x4 = size;
    obj->x0[0] = 0xffffffff;
    obj->x0[1] = size - 2;
    return obj;
}
