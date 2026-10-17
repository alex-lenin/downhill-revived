/* UNIT 0x00193630 0x001936A0 */
typedef struct Entry { void *x0; int x4; int x8; int xC; } Entry;
extern char D_00287378[];
extern char D_003F5690[];
void func_00193630(int arg0, int arg1) {
    char *p;
    int i, j;
    if (arg1 == 0xFFFF && arg0 != 0) {
        p = D_003F5690;
        i = 199;
        do {
            char *next;
            int remaining;
            ((Entry *)p)->x0 = D_00287378;
            next = p + 0x250;
            remaining = i - 1;
            p += 16;
            j = 33;
            do {
                ((Entry *)p)->x8 = 0;
                j--;
                p += 16;
            } while (j != -1);
            i = remaining;
            p = next;
        } while (i != -1);
    }
}
