/* UNIT 0x001B01D8 0x001B0238 */
typedef struct Obj {
    char pad0[0x60];
    char x60[0x80];
    char *xE0;
} Obj;

extern float func_00134E40(void *p);
extern void func_001AC6F8(void *p, float value);

void func_001B01D8(Obj *self)
{
    char *src = self->xE0;
    float value;

    if (src != 0) {
        value = func_00134E40(src + 0x43A0);
        func_001AC6F8(self, value);
        value = func_00134E40(src + 0x4420);
        func_001AC6F8(self->x60, value);
    }
}
