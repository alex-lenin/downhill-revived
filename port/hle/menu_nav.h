/* Event-driven menu navigation, shared by the port (autopad.c, DHD_NAV) and the PCSX2 comparison
   adapter (port/tools/prepare_pcsx2_compare.py pastes this file into its pad hook, DHD_COMPARE_NAV),
   so both walk the menus by the same rule. Plain C, no includes: it must compile in both.

   A script is a list of steps "menu[/phase][@address=value][+settle]:chord[|chord]", a chord being
   button[+button], numbers decimal except the hex EE word address and value, e.g.
   "0:triangle|cross,15:cross,18@4C7F38=0:start,9/5:cross".
   Each step waits until the game shows that menu (EE 0x0029DED8), race phase (0x0043F348, any phase
   when omitted) and word value, lets the screen settle for `settle` fields (MENU_NAV_SETTLE), then
   holds the buttons for MENU_NAV_HOLD fields. The step is done when any of the three changes; if
   none has after MENU_NAV_RETRY fields, the press is repeated (screens that ignore input while they
   fade in), with the step's next chord: one menu value can show several screens (the title, 0,
   first shows the memory card dialog, where Cross retries and Triangle cancels). A step followed by
   a step for the same state is pressed once: the next starts MENU_NAV_RETRY fields after it.
   A screen that moves on by itself skips ahead to the first later step matching what it shows. */
#ifndef DHD_MENU_NAV_H
#define DHD_MENU_NAV_H

#define MENU_NAV_SETTLE 10
#define MENU_NAV_HOLD 4
#define MENU_NAV_RETRY 30
#define MENU_NAV_CHORDS 4

typedef unsigned (*MenuNavRead)(unsigned address);  /* an EE RAM word */

typedef struct MenuNavStep {
    int menu, phase;               /* phase -1: any */
    unsigned address, value;       /* address 0: no word condition */
    int settle, chords;
    unsigned mask[MENU_NAV_CHORDS];
} MenuNavStep;

typedef struct MenuNav {
    int step;                      /* index of the next step */
    int menu, phase, seen;         /* the state last seen, once seen != 0 */
    unsigned address, value;       /* and the word the step watches */
    unsigned long long since;      /* field it last changed */
    unsigned long long pressed;    /* field of this step's current press, 0 when not pressing */
    int tries;                     /* presses of this step so far */
} MenuNav;

/* Bits of the PS2 pad's button word (port/hle/hle.h PAD_*, PCSX2's compact press flags inverted). */
static unsigned menu_nav_button(const char *name, int length)
{
    static const char *const names[16] = {"select", "l3", "r3", "start", "up", "right", "down", "left",
                                          "l2", "r2", "l1", "r1", "triangle", "circle", "cross", "square"};
    for (int i = 0; i < 16; i++) {
        int k = 0;
        while (k < length && names[i][k] == name[k]) k++;
        if (k == length && !names[i][k]) return 1u << i;
    }
    return 0;
}

static int menu_nav_number(const char **text, unsigned base, unsigned *value)
{
    int digits = 0;
    for (*value = 0;; digits++) {
        char c = **text;
        unsigned d = c >= '0' && c <= '9' ? (unsigned)(c - '0') : base == 16 && c >= 'A' && c <= 'F' ? (unsigned)(c - 'A' + 10) : base;
        if (d >= base) return digits > 0;
        *value = *value * base + d;
        ++*text;
    }
}

/* Step `index` of the script: 1 if it exists, 0 past the end, -1 if the script is invalid. */
static int menu_nav_parse(const char *script, int index, MenuNavStep *step)
{
    for (int i = 0; *script; i++) {
        unsigned value;
        step->phase = -1;
        step->address = step->value = 0;
        step->settle = MENU_NAV_SETTLE;
        step->chords = 1;
        step->mask[0] = 0;
        if (!menu_nav_number(&script, 10, &value)) return -1;
        step->menu = (int)value;
        if (*script == '/') {
            script++;
            if (!menu_nav_number(&script, 10, &value)) return -1;
            step->phase = (int)value;
        }
        if (*script == '@') {
            script++;
            if (!menu_nav_number(&script, 16, &step->address) || !step->address || *script++ != '=' ||
                !menu_nav_number(&script, 16, &step->value))
                return -1;
        }
        if (*script == '+') {
            script++;
            if (!menu_nav_number(&script, 10, &value)) return -1;
            step->settle = (int)value;
        }
        if (*script++ != ':') return -1;
        for (;;) {
            const char *name = script;
            while ((*script >= 'a' && *script <= 'z') || (*script >= '0' && *script <= '9')) script++;
            unsigned bit = menu_nav_button(name, (int)(script - name));
            if (!bit) return -1;
            step->mask[step->chords - 1] |= bit;
            if (*script == '|' && step->chords < MENU_NAV_CHORDS)
                step->mask[step->chords++] = 0;
            else if (*script != '+')
                break;
            script++;
        }
        if (*script && (*script++ != ',' || !*script)) return -1;
        if (i == index) return 1;
    }
    return 0;
}

static int menu_nav_shows(const MenuNavStep *step, int menu, int phase, MenuNavRead read)
{
    return menu == step->menu && (step->phase < 0 || phase == step->phase) &&
           (!step->address || read(step->address) == step->value);
}

/* Once per field: the buttons to hold this field. Returns 0 for an invalid script. */
static int menu_nav(MenuNav *nav, const char *script, int menu, int phase, MenuNavRead read,
                    unsigned long long field, unsigned *buttons)
{
    MenuNavStep step, next;
    *buttons = 0;
    if (!nav->seen)  /* the whole script must parse */
        for (int i = 0, found; (found = menu_nav_parse(script, i, &step)) != 0; i++)
            if (found < 0) return 0;
    int found = menu_nav_parse(script, nav->step, &step);
    unsigned value = found > 0 && step.address ? read(step.address) : 0;
    if (!nav->seen || menu != nav->menu || phase != nav->phase ||
        (found > 0 && (step.address != nav->address || value != nav->value))) {
        int moved = nav->seen && step.address == nav->address;  /* not just a new word to watch */
        nav->seen = 1;
        nav->menu = menu;
        nav->phase = phase;
        nav->address = found > 0 ? step.address : 0;
        nav->value = value;
        nav->since = field;
        if (nav->pressed && moved) {  /* the screen moved on: this step is done */
            nav->pressed = 0;
            nav->tries = 0;
            nav->step++;
            return menu_nav(nav, script, menu, phase, read, field, buttons);
        }
    }
    if (found <= 0) return found == 0;
    if (!nav->pressed && !menu_nav_shows(&step, menu, phase, read))
        for (int later = nav->step + 1;; later++) {
            int more = menu_nav_parse(script, later, &next);
            if (!more) break;
            if (menu_nav_shows(&next, menu, phase, read)) {
                nav->step = later;
                nav->tries = 0;
                return menu_nav(nav, script, menu, phase, read, field, buttons);
            }
        }
    if (nav->pressed && field >= nav->pressed + MENU_NAV_RETRY) {
        nav->pressed = 0;
        if (menu_nav_parse(script, nav->step + 1, &next) > 0 && next.menu == step.menu && next.phase == step.phase &&
            next.address == step.address && next.value == step.value) {
            nav->step++;  /* the next step is for the same state: this one was a single press */
            nav->tries = 0;
            nav->since = field;
            return menu_nav(nav, script, menu, phase, read, field, buttons);
        }
    }
    if (!nav->pressed && menu_nav_shows(&step, menu, phase, read) && field >= nav->since + (unsigned)step.settle) {
        nav->pressed = field;
        nav->tries++;
    }
    if (nav->pressed && field < nav->pressed + MENU_NAV_HOLD) *buttons = step.mask[(nav->tries - 1) % step.chords];
    return 1;
}

#endif
