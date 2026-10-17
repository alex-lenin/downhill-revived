/* UNIT 0x0017EEF8 0x0017EF70 */
struct Obj { int x0; int x4[25][4]; char pad194[0x388-0x194]; int x388[25]; };
void func_0017EEF8(struct Obj *obj, int skip, int value, int replacement) {
    int i, j, entry;
    for (i = 0; i < 25; i++) {
        if (i != skip) {
            for (j = 0; j < 4; j++) {
                entry = obj->x4[i][j];
                if (entry == value) obj->x388[i] = replacement;
                else if (!entry) break;
            }
        }
    }
}
