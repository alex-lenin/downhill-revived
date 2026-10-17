/* UNIT 0x001A86C8 0x001A87C0 */
typedef struct Obj {
    char pad20[0x20];
    int count;
} Obj;

void func_001A86C8(Obj *obj) {
    int i;
    int j;
    int k;
    int m;
    int value;
    int value2;
    signed char *p;
    signed char *q;
    signed char *r;
    signed char *s;

    for (i = 0; i < obj->count; i++) {
        p = (signed char *)obj + (i * 0x50 + 0x30);
        q = p + 0x41;
        for (k = 3; k >= 0; k--) {
            *q-- = -1;
        }
        r = p + 0x38;
        s = p + 0x49;
        q = p + 0x48;
        value2 = -1;
        for (j = 0; j >= 0; j--) {
            *q-- = value2;
        }
        q = r + 5;
        for (j = 5; j >= 0; j--) {
            *q-- = -1;
        }
        q = s;
        value = -1;
        m = 0;
        do {
            *q-- = value;
            m--;
        } while (m >= 0);
    }

    ((signed char *)obj)[0x685] = 0;
    ((signed char *)obj)[0x687] = 0;
    ((signed char *)obj)[0x688] = 0;
    ((signed char *)obj)[0x689] = 0;
}
