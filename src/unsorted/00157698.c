/* UNIT 0x00157698 0x001576B8 */
extern void func_001577B8(void *a);

void func_00157698(int *p, int value)
{
    p[0] = value;
    func_001577B8((char *)p + 0xE20);
}
