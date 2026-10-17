/* UNIT 0x001E72F0 0x001E73B0 */
typedef struct Obj {
    char pad0[0x96C];
    int codes[(0xA10 - 0x96C) / 4];
    int count;
} Obj;

extern int D_0027244C[][9];

int func_001E72F0(Obj *obj)
{
    int id = 1;

    int result;
    int i;

    if (obj->count == 0) {
        return 0;
    }
    do {
        result = id;
        for (i = 0; i < obj->count; i++) {
            if (obj->codes[i] != D_0027244C[id][i]) {
                result = 0;
            }
        }
        if (i < 8 && D_0027244C[id][i] != 0) {
            result = 0;
        }
        id++;
    } while (id < 27 && result == 0);
    return result;
}
