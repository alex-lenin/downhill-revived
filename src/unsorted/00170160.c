/* UNIT 0x00170160 0x00170228 */
typedef struct Flags {
    char pad0[0x5];
    unsigned char x5;
} Flags;

typedef struct Slot {
    char *x0;
    char pad4[0xC];
    float x10;
    char pad14[0xC];
} Slot;

typedef struct Obj {
    char pad0[0x68];
    float x68;
    char pad6C[0xE4];
    Slot slots[4];   /* 0x150 */
    char pad1D0[0x20];
    Flags *x1F0;
    char pad1F4[0x1C];
    float x210;
} Obj;

extern void func_002087C0(float *p);
extern void func_00222140(void *p, float *value);

void func_00170160(Obj *self)
{
    int i;
    Slot *slot;

    if (self->x1F0->x5 != 0) {
        self->x210 = self->x68 * 0.681818187f * 0.0244444441f;
        for (i = 0; i < 4; i++) {
            slot = &self->slots[i];
            if (slot->x0 != 0) {
                slot->x10 -= self->x210 * 0.713998318f;
                func_002087C0(&slot->x10);
                func_00222140(slot->x0 + 0x10, &slot->x10);
            }
        }
    }
}
