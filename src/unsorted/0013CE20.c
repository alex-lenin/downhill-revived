/* UNIT 0x0013CE20 0x0013CE48 */
typedef struct Obj {
    char pad0[0x56C];
    unsigned char x56C;
} Obj;

extern void func_0013D908(Obj *self, int arg);

void func_0013CE20(Obj *self)
{
    if (self->x56C == 0) {
        func_0013D908(self, 1);
    }
}
