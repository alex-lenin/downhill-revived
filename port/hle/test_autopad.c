#include "hle.h"
#include "menu_nav.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

uint64_t kernel_vsync_count;  /* read by the comparison heartbeat (kernel.c in the game) */
void gs_capture_request(const char *name) { (void)name; }
void graphics_settle(void) {}
static unsigned sim_word;  /* the simulated race setup's confirmation state, EE word 0x100 */
static unsigned sim_read(unsigned address) { return address == 0x100 ? sim_word : 0; }

int main(void)
{
    uint16_t buttons;
    const char *script = "1:0.25:circle,2:0.5:cross+start,2.25:1:down";
    assert(autopad_read(script, 0.9, &buttons) && !buttons);
    assert(autopad_read(script, 1, &buttons) && buttons == PAD_CIRCLE);
    assert(autopad_read(script, 1.25, &buttons) && !buttons);
    assert(autopad_read(script, 2.25, &buttons) && buttons == (PAD_CROSS | PAD_START | PAD_DOWN));
    assert(autopad_read(script, 2.5, &buttons) && buttons == PAD_DOWN);
    assert(autopad_read(script, 3.25, &buttons) && !buttons);
    assert(!autopad_read("1:0.5:typo", 1, &buttons));
    assert(!autopad_read("1:-1:cross", 1, &buttons));
    assert(!autopad_read("1:1:cross,", 1, &buttons));
    FILE *file = fopen("test_autopad_input.txt", "wb");
    assert(file);fputs(script,file);fputs("\r\n",file);fclose(file);
    assert(autopad_read("@test_autopad_input.txt", 2.25, &buttons) &&
           buttons == (PAD_CROSS | PAD_START | PAD_DOWN));
    file=fopen("test_autopad_input.txt","wb");assert(file);fclose(file);
    assert(autopad_read("@test_autopad_input.txt", 2.25, &buttons) && !buttons);
    file=fopen("test_autopad_input.txt","wb");assert(file);
    for(int i=0;i<4096;i++)fputc('x',file);fclose(file);
    assert(!autopad_read("@test_autopad_input.txt", 2.25, &buttons));
    file=fopen("test_autopad_input.txt","wb");assert(file);
    fwrite("1:1:cross\0hidden",1,16,file);fclose(file);
    assert(!autopad_read("@test_autopad_input.txt", 1, &buttons));
    remove("test_autopad_input.txt");
    assert(!autopad_read("@test_autopad_input.txt", 1, &buttons));

    /* menu_nav.h against a simulated front end: screens that ignore input while they fade in
       (`deaf` fields), two screens sharing menu 0 (the memory card dialog, then the title), the
       race setup (18) where Start opens a confirmation (word 0x100 = 1) that Cross accepts and an
       earlier Cross would select something else, and a load (9/5) that ends on a press. */
    static const struct { int menu, phase, deaf, button, next; } screens[] = {
        {0, 0, 100, PAD_TRIANGLE, 5}, {15, 0, 50, PAD_CROSS, 2}, {18, 0, 60, 0, 3}, {9, 5, 200, PAD_CROSS, 4},
        {9, 1, 0, 0, 4}, {0, 0, 20, PAD_CROSS, 1},
    };
    MenuNav nav = {0};
    const char *steps = "0:triangle|cross,15:cross,18@100=0:start,18@100=1:cross,9/5:cross";
    int at = 0, entered = 0, held = 0, wrong = 0;
    unsigned long long field;
    for (field = 1; field < 2000 && at != 4; field++) {
        unsigned pad;
        assert(menu_nav(&nav, steps, screens[at].menu, screens[at].phase, sim_read, field, &pad));
        int want = screens[at].menu == 18 ? (sim_word ? PAD_CROSS : PAD_START) : screens[at].button;
        if (screens[at].menu == 18 && !sim_word && pad == PAD_CROSS) wrong = 1;
        held = want && pad == (unsigned)want ? held + 1 : 0;
        if (held == 2 && field >= (unsigned long long)entered + screens[at].deaf) {  /* accepted */
            held = 0;
            if (screens[at].menu == 18 && !sim_word++) continue;
            at = screens[at].next, entered = (int)field;
        }
    }
    assert(at == 4 && !wrong && field < 700);  /* each wait is the screen's own, plus a few fields */
    unsigned pad;
    assert(!menu_nav(&nav, "0:typo", 0, 0, sim_read, 1, &pad));
    MenuNav fresh = {0};
    assert(!menu_nav(&fresh, "0cross", 0, 0, sim_read, 1, &pad) && !menu_nav(&fresh, "0:cross,", 0, 0, sim_read, 1, &pad));
    assert(!menu_nav(&fresh, "0:cross|", 0, 0, sim_read, 1, &pad) && !menu_nav(&fresh, "0:cross@100:cross", 0, 0, sim_read, 1, &pad));
    puts("Scripted controller tests passed");
    return 0;
}
