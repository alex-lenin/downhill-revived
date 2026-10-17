/* UNIT 0x001E7BD8 0x001E7C00 */
typedef struct {
    char pad0[0x3C];
    int x3C;
    int x40;
    char pad44[0x10];
} Entry;

typedef struct {
    char pad0[0x2C];
    int x2C;
} Obj;

extern Entry D_00272888[];

int func_001E7BD8(Obj *obj)
{
    return D_00272888[obj->x2C].x3C != 0;
}
