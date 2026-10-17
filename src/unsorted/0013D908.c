/* UNIT 0x0013D908 0x0013DA40 */
typedef struct Entry {
    int x0;
    int x4;
    int x8;
} Entry;

typedef struct State {
    int index;
    char pad4[0x570];
    Entry *x574;
    unsigned char x578[1];
} State;

extern int func_0013D6F8(State *, Entry *);
extern void func_0013D608(State *, int, int, int);
extern int func_0013D5A0(State *, int, int);

void func_0013D908(State *state, int arg)
{
    int found;
    Entry *entry;

    found = func_0013D6F8(state, &state->x574[state->x578[state->index]]);
    if (found >= 0) {
        func_0013D608(state, 0, 0, arg);
        func_0013D608(state, 1, found, arg);
    } else {
        entry = &state->x574[state->x578[state->index]];
        func_0013D608(state, 0, 1, arg);
        func_0013D608(state, 2, func_0013D5A0(state, 1, entry->x0), arg);
        func_0013D608(state, 3, func_0013D5A0(state, 2, entry->x4), arg);
        func_0013D608(state, 4, func_0013D5A0(state, 3, entry->x8), arg);
    }
}
