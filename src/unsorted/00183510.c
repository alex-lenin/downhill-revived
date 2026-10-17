/* UNIT 0x00183510 0x001835E0 */
typedef struct Bits {
    unsigned int lo : 7;
    unsigned int val : 11;
    unsigned int hi : 14;
} Bits;

typedef struct Obj {
    char pad0[0x24];
    int x24;
    int x28;
    char pad2C[0x38 - 0x2C];
    Bits *x38[(0x100 - 0x38) / 4];
    int x100[(0x1C8 - 0x100) / 4];
    int x1C8[1];
} Obj;

extern int D_004B26AC[];
extern void func_00218498(int, int);

void func_00183510(Obj *obj)
{
    int i;
    int j;
    int mode;

    mode = D_004B26AC[0];
    if (mode == 0x11) {
        for (i = 1; i <= obj->x24; i++) {
            if (obj->x38[i] != 0) {
                obj->x38[i]->val = obj->x1C8[i];
            }
        }
        for (j = 0; j < obj->x28; j++) {
            func_00218498(obj->x100[j], 0);
        }
        obj->x24 = 0;
        obj->x28 = 0;
    }
}
