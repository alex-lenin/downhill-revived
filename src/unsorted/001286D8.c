/* UNIT 0x001286D8 0x00128720 */
typedef struct Obj {
    char pad0[0x21420];
    int count;
    char pad21424[0x2724];
    int ids[1];
} Obj;

int func_001286D8(Obj *obj, int id)
{
    int i;

    for (i = 0; i < obj->count; i++) {
        if (obj->ids[i] == id) {
            return 1;
        }
    }
    return 0;
}
