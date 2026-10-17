/* UNIT 0x001E7460 0x001E74B8 */
struct Obj {
    char pad0[0xA1A];
    unsigned char xA1A;
    char padA1B;
    int xA1C;
};

int func_001E7460(struct Obj *obj, int arg1) {
    int yes = 1;
    int no = 0;
    int result;

    /* the 0/1 values must sit in variables to get retail's movn (not sltiu) */
    result = no;
    if (obj->xA1C == arg1) {
        result = yes;
    }
    if (result) {
        if (arg1 >= 2) {
            if (obj->xA1A == 0 && arg1 != 21) {
                result = 0;
            }
        } else if (arg1 == 1) {
            obj->xA1A ^= 1;
        }
    }
    return result;
}
