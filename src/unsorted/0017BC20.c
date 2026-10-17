/* UNIT 0x0017BC20 0x0017BCB0 */
typedef struct { char pad0[8]; unsigned short x8; char padA[0x16]; int x20[1]; } Input;
typedef struct { char pad0[0xef0]; int xEF0[22]; int xF48[15]; } Obj;
extern int func_00215B58(int);
extern int func_0017C830(Obj *, int);
void func_0017BC20(Obj *obj, Input *input) {
    int i;
    int value;
    for (i = 0; i < input->x8; i++) {
        value = input->x20[i];
        obj->xF48[i] = value;
        obj->xEF0[i] = func_0017C830(obj, func_00215B58(value));
    }
}
