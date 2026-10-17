/* UNIT 0x001950F8 0x001951B0 */
struct Elem {
    char pad0[0x50];
    Elem &func_001952D0(const Elem &o);
    Elem &operator=(const Elem &o) { return func_001952D0(o); }
};

struct Slot {
    Slot &operator=(const Slot &o) { return *this; }
};

struct Part {
    Elem elems[10];
    Slot slotsA[33];
    Slot slotsB[43];
    Part &func_001950F8(const Part &o);
};

Part &Part::func_001950F8(const Part &o)
{
    {
        Slot *dst = slotsA;
        const Slot *src = o.slotsA;
        int n;
        for (n = 32; n >= 0; n--) {
            *dst++ = *src++;
        }
    }
    {
        int i;
        for (i = 0; i < 10; i++) {
            elems[i] = o.elems[i];
        }
    }
    {
        Slot *dst = slotsB;
        const Slot *src = o.slotsB;
        int n;
        for (n = 42; n >= 0; n--) {
            *dst++ = *src++;
        }
    }
    return *this;
}
