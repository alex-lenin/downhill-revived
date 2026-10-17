/* UNIT 0x001A8658 0x001A86C8 */
typedef struct Obj { char pad[0x20]; int count; char pad24[0x684-0x24]; char x684; } Obj;
void func_001A8658(Obj *arg0) {
    int i, j;
    signed char *p;
    for (i = 0; i < arg0->count; i++) {
        p = (signed char *)arg0 + i * 0x50 + 0x72;
        j = 5;
        do {
            if (*p != -1 && *(short *)((char *)arg0 + (*p << 5) + 0x6A6) == 0) {
                *p = -1;
            }
            j--;
            p++;
        } while (j >= 0);
    }
    arg0->x684 = 0;
}
