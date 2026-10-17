/* UNIT 0x00118640 0x00118650 */
struct Elem { char pad0[0x20]; };

int func_00118640(struct Elem *begin, struct Elem *end)
{
    return end - begin;
}
