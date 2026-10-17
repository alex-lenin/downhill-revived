/* UNIT 0x00159690 0x001596E0 .rodata=0x28A2B0:0x28A2E0 */
typedef struct Obj {
    char pad0[0x4];
    unsigned int type;
    char pad8[0x1E4];
    short x1EC;
} Obj;

int func_00159690(Obj *obj)
{
    switch (obj->type) {
    case 1:
    case 2:
    case 3:
    case 5:
    case 7:
        return obj->x1EC >= 5;
    case 6:
        return 1;
    case 0:
    case 4:
    case 8:
    case 9:
    case 10:
    default:
        return 0;
    }
}
