/* UNIT 0x00161698 0x00161760 */
typedef struct Screen {
    char pad0[0x500];
    int fade;       /* 0x500 */
    int stage;      /* 0x504 */
} Screen;

extern char D_0043F870[];
extern void func_001A42D8(void *p, int index, int alpha);
extern void func_001A46A0(void *p, int index, int alpha);
extern void func_0018DC20(Screen *screen);

void func_00161698(Screen *screen)
{
    if (screen->fade < 190) {
        screen->fade += 3;
    } else {
        screen->fade = 0;
        screen->stage++;
    }
    if (screen->stage == 0) {
        func_001A42D8(D_0043F870, 0, screen->fade);
    } else {
        func_001A46A0(D_0043F870, 0, screen->stage < 2 ? screen->fade : 170);
        if (screen->stage >= 2) {
            func_001A46A0(D_0043F870, 1, screen->stage < 3 ? screen->fade : 170);
        }
    }
    func_0018DC20(screen);
}
