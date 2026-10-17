/* UNIT 0x00192DD8 0x00192ED8 */
typedef struct Entry {
    char pad0[0x250];
} Entry;

extern Entry D_003F5690[];
extern int D_0029E4A0;
extern void *D_0029E4C8;
extern int D_0029E4CC;
extern int D_00768050[];
extern int D_0076F9E8[];
extern int D_0076FA30[];
extern int D_0076FA78[][100];
extern int D_00771508[][100];

extern void func_00192948(Entry *e);

void func_00192DD8(void)
{
    int i;
    int j;

    for (i = 0; i < 200; i++) {
        func_00192948(&D_003F5690[i]);
    }
    D_0029E4A0 = 0;
    for (i = 0; i < 17; i++) {
        D_00768050[i] = 0;
        D_0076F9E8[i] = 0;
        D_0076FA30[i] = 0;
        for (j = 0; j < 100; j++) {
            D_0076FA78[i][j] = 0;
            D_00771508[i][j] = 0;
        }
    }
    D_0029E4C8 = 0;
    D_0029E4CC = 0;
}
