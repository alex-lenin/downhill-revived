/* UNIT 0x00194E10 0x00194E30 */
struct Obj {
    char pad0[0x526];
    short x526;
    int x528;
    char x52C[1];
};

void func_00194E10(struct Obj *obj, char **data, int *a, int *b)
{
    *data = obj->x52C;
    *a = obj->x526;
    *b = obj->x528;
}
