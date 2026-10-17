/* UNIT 0x0013F6F8 0x0013F730 */
typedef struct Pair {
    int key;
    int value;
} Pair;

extern Pair D_0026E370[];

int func_0013F6F8(int unused, int key)
{
    int result = -1;
    Pair *p = D_0026E370;
    int i;

    for (i = 0x57; i >= 0; i--, p++) {
        if (p->key == key) {
            result = p->value;
        }
    }
    return result;
}
