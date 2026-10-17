/* UNIT 0x001634C8 0x001634E0 */
typedef struct Obj {
    char pad0[4];
    float x4;
    float x8;
    char padC[0x2C - 0xC];
    unsigned char x2C;
    unsigned char x2D;
} Obj;

void func_001634C8(Obj *self)
{
    self->x2D = 0;
    self->x4 = self->x8;
    self->x2C = 0;
}
