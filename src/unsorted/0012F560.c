/* UNIT 0x0012F560 0x0012F588 */
typedef struct Obj {
    char pad0[0x796C];
    int x796C;
} Obj;

extern void func_0012F588(int a, int b);

void func_0012F560(Obj *self, int arg)
{
    int value = self->x796C;
    func_0012F588(arg, value);
}
