/* UNIT 0x001E2F18 0x001E2FB0 */
typedef struct Obj {
    char pad0[0x500];
    int x500;
    int x504[7];
    unsigned char x520[4][13];
    unsigned char x554[4];
} Obj;

void func_001E2F18(Obj *obj, int index, int value) {
    int i;
    unsigned char carry;
    unsigned char tmp;

    obj->x504[obj->x500] = 1;
    carry = obj->x520[obj->x500][index];
    obj->x520[obj->x500][index] = value;
    for (i = index + 1; i < 12; i++) {
        tmp = obj->x520[obj->x500][i];
        obj->x520[obj->x500][i] = carry;
        carry = tmp;
    }
    obj->x554[obj->x500]++;
}
