/* UNIT 0x00178100 0x00178170 */
struct Owner { char pad[0x1340]; float x1340; };
struct Item { char pad[0x4C]; float x4C; };
void func_00178100(struct Owner *owner, unsigned char *a, unsigned char *b, struct Item **items, int count) {
    int i;
    for (i = 0; i < count; i++, items++) {
        if (a[i] != b[i]) {
            struct Item *item = *items;
            float value = item->x4C + owner->x1340;
            item->x4C = value;
            if (value > 1.0f) item->x4C = 1.0f;
        }
    }
}
