/* UNIT 0x001FB160 0x001FB2D0 */
#include "player_slot_event.h"
class State {
public:
    int x0;
    virtual void v08();
    virtual void v10();
    virtual void v18();
    virtual int IsDone();             /* vtable +0x20 */
};

class Machine {
public:
    char pad0[0x1E40];
    char x1E40[0x3090];
    State x4ED0;
    char pad4ED8[0xFB8];
    State x5E90;
    char pad5E98[0x158];
    State x5FF0;
    char pad5FF8[0x2E8];
    State x62E0;
    char pad62E8[0x28];
    State x6310;
    char pad6318[0x1670];
    unsigned int x7988;

    void func_0012FE38(State *next);
};

class Mode {
public:
    Machine *machine;
    char pad4[0x1C];
    int time;
    int count;
    int x28;

    int func_001FAD78(int arg);
    void func_001FB2D0();
    void func_001FB160();
};

typedef struct Slot {
    char pad0[0x1380];
} Slot;

typedef struct Table {
    char pad0[0x150];
    Slot slots[4];
} Table;

extern Table D_003B2270;

extern "C" void func_001AC578(void *);
extern "C" int func_00238F60(void);


void Mode::func_001FB160()
{
    Machine *m;
    State *s;
    unsigned int slot;
    int sound;
    int n;
    Table *table;

    m = machine;
    func_001AC578(m->x1E40);
    time += func_00238F60();
    if ((s = &m->x5E90)->IsDone() || (s = &m->x5FF0)->IsDone()
        || (s = &m->x62E0)->IsDone() || (s = &m->x6310)->IsDone()) {
        m->func_0012FE38(s);
    } else if (time > 20 && func_001FAD78(x28)) {
        m->func_0012FE38(&m->x4ED0);
    }
    this->func_001FB2D0();
    slot = m->x7988;
    sound = x28 == 0 ? 0x2C : 0x2D;
    n = count;
    table = &D_003B2270;
    if (slot < 4) {
        SetPlayerSlotEventLegacy(&table->slots[slot], sound, 0, n, 0, 0, 0);
    }
    count++;
}
