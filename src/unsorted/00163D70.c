/* UNIT 0x00163D70 0x00163D80 */
struct Elem {
    char pad0[0x20];
};

struct Array {
    struct Elem *start;
};

int func_00163D70(struct Array *arr, struct Elem *elem)
{
    return elem - arr->start;
}
