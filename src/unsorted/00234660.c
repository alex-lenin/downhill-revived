/* UNIT 0x00234660 0x00234670 */
struct Item {
    int x0;
    int x4;
};

int func_00234660(struct Item *a, struct Item *b)
{
    return a->x4 - b->x4;
}
