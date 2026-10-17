/* UNIT 0x001949A8 0x00194A10 */
struct Entry {
    char pad0[0x32];
    signed char x32;
    char pad33[0x50 - 0x33];
};

struct Game {
    char pad0[0x11A88];
    struct Entry entries[1];
};

extern int func_001822D8(int);

float func_001949A8(struct Game *game, int id)
{
    int n;
    struct Entry *e;

    e = (struct Entry *)((char *)game + (id - 4) * (int)sizeof(struct Entry) + 0x11A88);
    n = e->x32;
    return (float)(n + 1) / (float)func_001822D8(id);
}
