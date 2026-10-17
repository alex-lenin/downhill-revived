/* UNIT 0x00194AB8 0x00194B50 */
struct Part {
    char pad[0x694];
    Part &func_001950F8(const Part &o);
    Part &operator=(const Part &o) { return func_001950F8(o); }
};
struct Tail {
    char pad[0xD8];
    Tail &func_00195A98(const Tail &o);
    Tail &operator=(const Tail &o) { return func_00195A98(o); }
};
struct State {
    Part parts[18];
    Tail tails[33];
    State &func_00194AB8(const State &o);
};

State &State::func_00194AB8(const State &o) {
    int i;
    for (i = 0; i < 18; i++) {
        parts[i] = o.parts[i];
    }
    Tail *dst = tails;
    const Tail *src = o.tails;
    for (i = 32; i >= 0; i--) {
        *dst++ = *src++;
    }
    return *this;
}
