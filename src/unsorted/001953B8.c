/* UNIT 0x001953B8 0x00195420 */
struct Vec3 {
    int x0;
    int x4;
    int x8;
};

struct VecList {
    struct Vec3 items[63];
    char pad[0x4A4 - 63 * 12];
    int count;
};

void func_001953B8(struct VecList *list, struct Vec3 *v)
{
    if (list->count < 63) {
        list->items[list->count].x4 = v->x4;
        list->items[list->count].x0 = v->x0;
        list->items[list->count].x8 = v->x8;
        list->count++;
    }
}
