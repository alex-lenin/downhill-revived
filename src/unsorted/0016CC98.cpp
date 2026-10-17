/* UNIT 0x0016CC98 0x0016CD78 */
extern const float D_0029C8AC[];

class Sub {
public:
    void func_0016D0E0();
};

struct Slot {
    char pad0[0x50];
    Sub x50;
    char pad51[0x294 - 0x51];
    unsigned char active;
    char pad295[0x6D0 - 0x295];
};

class Mgr {
public:
    Slot slots[10];
    char pad[0x4484 - 10 * 0x6D0];
    float x4484;
    float x4488;
    char pad448C[0x44B4 - 0x448C];
    int count;
    char pad44B8[0x44C8 - 0x44B8];
    int current;
    unsigned char x44CC;
    Slot *GetSlot(int i) { return &slots[i]; }
    void func_0016C8A0();
    void func_0016CC98();
};

void Mgr::func_0016CC98()
{
    Slot *slot;
    int i;

    if (count == 0) {
        return;
    }
    if (x44CC) {
        this->func_0016C8A0();
        x44CC = 0;
        return;
    }
    slot = GetSlot(current);
    current++;
    slot->active = 0;
    if (current >= count) {
        current = 0;
    }
    GetSlot(current)->active = 1;
    x4484 = x4488;
    x4488 = D_0029C8AC[0];
    for (i = 0; i < count; i++) {
        slots[i].x50.func_0016D0E0();
    }
}
