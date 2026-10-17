/* UNIT 0x00215B58 0x00215B68 */
struct Obj {
    char pad0[0x3D];
    unsigned char x3D;
};

char *func_00215B58(struct Obj *obj)
{
    return (char *)obj + obj->x3D * 16;
}
