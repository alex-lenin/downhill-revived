/* UNIT 0x00110BF8 0x00110C98 */
struct Obj { char pad0[0x2BC]; float x2BC; char pad2C0[0x344-0x2C0]; float x344; };
extern int func_00222BD8(int, int);
extern int func_00110D38(struct Obj *, int);
int func_00110BF8(struct Obj *obj) {
    int result = 0;
    int index, count;
    if (obj->x344 < obj->x2BC) {
        index = func_00222BD8(0, 28);
        for (count = 0; count < 28; count++) {
            if (func_00110D38(obj, index)) { result = index; break; }
            index++;
            if (index >= 28) index = 0;
        }
    }
    return result;
}
