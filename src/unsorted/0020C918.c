/* UNIT 0x0020C918 0x0020C9F0 */
struct Unit {
    char pad0[0x2A0];
    int x2A0;
    int x2A4;
    int x2A8;
    int x2AC;
    int x2B0;
    short x2B4;
    short x2B6;
    int x2B8;
    int x2BC;
};

extern struct Unit D_004D9720[];
extern int func_0023D608(void);
extern void func_0023D650(int index, int *a, int *b);
extern void func_0023D618(int index, int *a, int *b);
extern void func_0020C530(struct Unit *unit);

void func_0020C918(void) {
    int i;
    int count;
    struct Unit *unit;

    count = func_0023D608();
    for (i = 0; i < count; i++) {
        unit = &D_004D9720[i];
        unit->x2B6 = i;
        unit->x2B8 = 0;
        unit->x2BC = 0;
        unit->x2B4 = 0;
        unit->x2B0 = 0;
        func_0023D650(i, &unit->x2A0, &unit->x2A4);
        func_0023D618(i, &unit->x2A8, &unit->x2AC);
        func_0020C530(unit);
    }
}
