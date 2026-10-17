/* UNIT 0x00190660 0x00190688 */
struct A {
    char pad0[0x12C];
    unsigned char x12C;
};

extern void func_00190528(struct A *a, int b, int c);

void func_00190660(struct A *a, int b)
{
    a->x12C = 0;
    func_00190528(a, b, 1);
}
