/* UNIT 0x00192DB0 0x00192DD8 */
struct A {
    char pad0[0x230];
    void *x230;
};

extern void func_00122AA0(void *p);

void func_00192DB0(struct A *a)
{
    if (a->x230 != 0) {
        func_00122AA0(a->x230);
    }
}
