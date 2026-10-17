/* UNIT 0x0013D750 0x0013D888 */
typedef struct Entry { int x0, x4, x8; } Entry;

typedef struct Obj {
    int x0;
    char pad4[0x574 - 0x4];
    Entry *x574;
    unsigned char x578[0x64];
    int x5DC;
} Obj;

extern char D_004C8508[];
extern int func_001F9F20(void *p, Entry *entry);
extern int func_001F9E50(void *p, Entry *entry);

/* Sort the entries into x578 by class: 0 first, then 1, then 2. */
void func_0013D750(Obj *obj, Entry *entries, int count)
{
    signed char cls[100];
    int i;
    int n;
    int c;

    i = 0;
    n = 0;
    obj->x574 = entries;
    obj->x5DC = count;
    for (; i < obj->x5DC; i++) {
        if (func_001F9F20(D_004C8508, &obj->x574[i])) {
            cls[i] = 0;
        } else if (func_001F9E50(D_004C8508, &obj->x574[i])) {
            cls[i] = 2;
        } else {
            cls[i] = 1;
        }
    }
    for (c = 0; c < 3; c++) {
        for (i = 0; i < obj->x5DC; i++) {
            if (cls[i] == c) {
                obj->x578[n++] = i;
            }
        }
    }
}
