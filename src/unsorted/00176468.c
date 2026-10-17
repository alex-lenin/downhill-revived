/* UNIT 0x00176468 0x001764F8 */
typedef struct { char pad0[8]; unsigned short x8; char padA[0x16]; char *x20; } Input;
typedef struct { char pad0[0xf6c]; Input *xF6C; char padF70[0x308]; unsigned char *x1278; } Obj;
typedef struct { char pad0[11]; unsigned char xB; } Result;
extern Result *func_00215B58(void *);
void func_00176468(Obj *obj, Input *input) {
    char *base;
    Result *result;
    unsigned char *p;
    int count;
    unsigned char alpha;
    if (input->x8 < 2) {
        obj->xF6C = input;
        base = input->x20;
        result = func_00215B58(base);
        obj->x1278 = (unsigned char *)base + 0x2b0;
        if (result->xB == 0x60) obj->x1278 = (unsigned char *)base + 0x2b8;
        p = obj->x1278;
        alpha = 0x80;
        count = 0x29;
        do { p[0] = 0; count--; p[1] = 0; p[2] = 0; p[3] = alpha; p += 4; } while (count >= 0);
    }
}
