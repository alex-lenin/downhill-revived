/* UNIT 0x0017BB90 0x0017BC20 */
typedef struct { char pad0[8]; unsigned short x8; char padA[0x16]; int x20[1]; } Input;
typedef struct { char pad0[0xee4]; int xEE4[15]; int xF20[15]; } Obj;
extern int func_00215B58(int);
extern int func_0017C830(Obj *, int);
void func_0017BB90(Obj *obj, Input *input) {
    int i;
    int value;
    for (i = 0; i < input->x8; i++) {
        value = input->x20[i];
        obj->xF20[i] = value;
        obj->xEE4[i] = func_0017C830(obj, func_00215B58(value));
    }
}
