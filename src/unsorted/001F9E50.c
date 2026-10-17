/* UNIT 0x001F9E50 0x001F9F20 */
typedef struct Key {
    int x0;
    int x4;
    int x8;
} Key;

typedef struct Group {
    Key keys[7];
    int count;
    char pad58[0xA0 - 0x58];
} Group;

typedef struct Table {
    char pad0[0x30];
    Group groups[6];
} Table;

extern Key D_00277FE0[8];

int func_001F9E50(Table *table, Key *key)
{
    int i;
    int j;

    for (i = 0; i < 6; i++) {
        Group *group = &table->groups[i];
        int count = group->count;

        for (j = 0; j < count; j++) {
            if (group->keys[j].x4 == key->x4 && group->keys[j].x0 == key->x0 &&
                group->keys[j].x8 == key->x8) {
                return 1;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (D_00277FE0[i].x4 == key->x4 && D_00277FE0[i].x0 == key->x0 && D_00277FE0[i].x8 == key->x8) {
            return 1;
        }
    }
    return 0;
}
