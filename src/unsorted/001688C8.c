/* UNIT 0x001688C8 0x00168A78 */
typedef struct Screen {
    char pad0[0x500];
    int timer;                  /* frames shown, stops at 190 */
} Screen;

typedef struct Race {
    char pad0[0x288];
    int riderCount;
} Race;

typedef struct World {
    char pad0[0x50130];
    char *riders[40];
} World;

extern Race D_004B25A0;
extern char D_0043F870[];
extern char D_003B2270[];
extern World D_002AF2C0;

extern void func_0017A678(void *, int);
extern void func_001A3C18(void *, int);
extern void func_001A3E08(void *, int, int, int, int *);
extern void func_001A3F90(void *, int);
extern void func_001E5300(void *, int **, int **, int **, int **);
extern void func_001E5228(void *, int **, int **, int **, int **);
extern void func_0018DC20(void *);

void func_001688C8(Screen *screen)
{
    int i;
    char *rider;
    char *panel;
    int *first, *second, *extra, *unused;
    int *d0, *d1, *d2;
    int count, j;

    if (screen->timer < 190) {
        screen->timer = screen->timer + 1;
    }
    panel = D_003B2270;
    panel = panel + 0x150;
    func_0017A678(panel, -2);
    i = 0;
    func_001A3C18(D_0043F870, screen->timer);
    if (D_004B25A0.riderCount > 0) {
        for (; i < D_004B25A0.riderCount; i++) {
            rider = D_002AF2C0.riders[i];
            func_001E5300(rider + 0x6610, &first, &second, &extra, &unused);
            func_001E5228(rider + 0x6610, &d0, &d1, &d2, &unused);
            /* entries 1..42 count once if set in the first list, twice if also in the
               second; entries 1..7 of the extra list count once */
            count = 0;
            for (j = 1; j < 43; j++) {
                if (first[j] > 0) {
                    count++;
                    if (second[j] > 0) {
                        count++;
                    }
                }
            }
            for (j = 1; j < 8; j++) {
                if (extra[j] > 0) {
                    count++;
                }
            }
            func_001A3E08(D_0043F870, *(int *)(rider + 0x798C) - 1, screen->timer, count, d2);
        }
    }
    func_001A3F90(D_0043F870, screen->timer);
    func_0018DC20(screen);
}
