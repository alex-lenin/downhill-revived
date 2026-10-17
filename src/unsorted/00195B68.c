/* UNIT 0x00195B68 0x00195BB8 */
typedef struct { int x0; short x4[150]; short x130[150]; short x25c[150]; unsigned char x388[150]; } Obj;
void func_00195B68(Obj *obj, int index, float *pos, int value) {
    obj->x4[index] = (int)pos[0];
    obj->x130[index] = (int)pos[1];
    obj->x25c[index] = (int)pos[2];
    obj->x388[index] = value;
}
