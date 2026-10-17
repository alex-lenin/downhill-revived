/* UNIT 0x00175100 0x00175148 */
extern void func_00176508(void *p);

void func_00175100(char *self)
{
    char *p;
    int i;

    p = self + 0x150;
    for (i = 3; i >= 0; i--) {
        func_00176508(p);
        p += 0x1380;
    }
}
