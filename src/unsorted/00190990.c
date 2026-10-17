/* UNIT 0x00190990 0x001909A8 */
typedef struct Obj {
    char pad0[0xB8];
    unsigned char xB8[1][4];
} Obj;

unsigned char func_00190990(Obj *obj, int col, int row)
{
    return obj->xB8[row][col];
}
