/* UNIT 0x0020D5B8 0x0020D660 */
struct Entry { int x0, x4, x8; char xc[0x2c]; int x38, x3c, x40; unsigned char x44; char pad45[3]; };
struct Obj { struct Entry entries[80]; int x1680; };
extern int func_0020DD38(void *, const char *);
extern int func_00254544(void *, const char *);
int func_0020D5B8(struct Obj *obj, int value, int other, int index, const char *text, unsigned char flag) {
    struct Entry *p = &obj->entries[index];
    p->x40 = index;
    p->x4 = value;
    p->x38 = 0;
    p->x44 = 0;
    p->x8 = other;
    p->x0 = flag;
    func_0020DD38((char *)p + 0xc, text);
    func_00254544((char *)p + 0x15, text);
    if (index >= obj->x1680) obj->x1680++;
    return value;
}
