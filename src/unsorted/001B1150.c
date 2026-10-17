/* UNIT 0x001B1150 0x001B1188 */
typedef struct Row { char pad0[0xD0]; int xD0[3]; char padDC[0x34]; } Row;
void func_001B1150(Row *obj, int index, int value) {
    obj[index / 3].xD0[index % 3] = value;
}
