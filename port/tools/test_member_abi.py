"""Synthetic SN member-function pointer layout and dispatch regression."""
from pathlib import Path
import sys
import transform


def prepare(directory):
    root = Path(__file__).resolve().parents[2]
    sys.path.insert(0, str(root / 'port'))
    import patches
    caller = (root / 'src/unsorted/001933A0.cpp').read_text()
    for kind, old, new in patches.PATCHES.get('unsorted/001933A0.cpp', []):
        assert kind == 'text' and caller.count(old) == 1
        caller = caller.replace(old, new)
    caller = caller.replace('#include "model.h"', (root / "include/model.h").read_text())
    source = r'''
extern "C" int puts(const char *);
static void *expected;
static int called;
static int target(void *object) { return object == expected ? 41 : -1; }
static void notify(void *object) { called += object == expected; }
class Object;
typedef int (Object::*Check)();
class Object { public: int value; Check check; int action() { return value; } };
static int run(Object *obj) { return (obj->*obj->check)(); }
extern "C" int main(void) {
    if (check_state_call()) return 6;
    Object object;
    object.value = 37;
    object.check = &Object::action;
    if (sizeof(Check) != 8 || run(&object) != 37) return 1;
    unsigned char storage[64] __attribute__((aligned(16))) = {0};
    ps2_member<int> member;
    member.delta = -4; member.index = -1; member.target = (unsigned int)&target;
    expected = storage;
    if (member.invoke(storage + 4) != 41) return 2;
    /* A virtual member finds the vptr through its signed offset, then adds
       both this adjustments. Its index is one greater than the slot. */
    ps2_vslot table[3] = {{0,0,0}, {8,0,(void *)&target}, {0,0,0}};
    *(ps2_vslot **)(storage + 16) = table;
    member.delta = -4; member.index = 2; member.target = 16;
    expected = storage + 4;
    if (member.invoke(storage) != 41) return 3;
    member.delta = 4; member.target = (unsigned int)(short)-16;
    expected = storage + 44;
    if (member.invoke(storage + 32) != 41) return 4;
    /* The retail direct-table tag is 0xffff0000, not a host code address. */
    ps2_member<void> direct = {0,-1,(unsigned int)&notify};
    expected = storage;
    direct.invoke(storage);
    if (called != 1) return 5;
    puts("SN member pointers: layout, assignment, direct/virtual calls and signed adjustments passed");
    return 0;
}
'''
    # Exercise the real state-registration body with a synthetic SN vtable.
    fixture = r'''
static State *state_expected;
static Flags state_flags;
static int state_failures;
int D_00768050[2];
int D_0076FA78[2][100];
int func_002623C0(void *p) { state_failures += p != state_expected; return 17; }
int func_00192BA8(int id) { state_failures += id != 17; return 1; }
int func_00193478(State *p) { state_failures += p != state_expected; return 29; }
static Flags *state_get(void *p) { state_failures += p != state_expected; return &state_flags; }
static int check_state_call() {
    State state;
    ps2_vslot table[5] = {};
    table[4].function = (void *)&state_get;
    state.ps2_vptr = table;
    state_expected = &state;
    state_flags.x0 = ~0UL;
    state.func_001933A0();
    return state_failures || state.x24C != 1 || D_00768050[1] != 1 ||
           D_0076FA78[1][0] != 29 || state_flags.x0 != 0xFDFFFF00FFFFFFFFUL;
}
'''
    path = Path(directory) / 'test_member_abi.cpp'
    # The fixture sets the explicit vptr introduced by virtual lowering.
    path.write_text('#include "port.h"\n' + transform.transform(caller, cpp=True) +
                    transform.rewrite_long(fixture) + transform.transform(source, cpp=True))
    return str(path)
