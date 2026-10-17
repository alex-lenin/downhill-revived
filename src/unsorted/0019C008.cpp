/* UNIT 0x0019C008 0x0019C148 */
class State {
public:
    int x0;
    virtual void v08();
    virtual void v10();
    virtual void v18();
    virtual int v20();
};

class Machine {
public:
    char pad0[0x1E40];
    char x1E40[0x4ED0 - 0x1E40];
    State x4ED0;
    char pad4ED8[0x5E40 - 0x4ED8];
    State x5E40;
    char pad5E48[0x5E70 - 0x5E48];
    State x5E70;
    char pad5E78[0x5E90 - 0x5E78];
    State x5E90;
    char pad5E98[0x5FF0 - 0x5E98];
    State x5FF0;
    char pad5FF8[0x62E0 - 0x5FF8];
    State x62E0;
    char pad62E8[0x6310 - 0x62E8];
    State x6310;

    void func_0012FE38(State *next);
    float func_0012E070();
};

class Owner {
public:
    Machine *x0;

    void func_0019C008();
    void func_0019C148();
};

extern "C" void func_001AC578(void *);

void Owner::func_0019C008()
{
    Machine *machine = x0;
    State *state1, *state2, *state3, *state4, *state5, *state6;

    func_001AC578(machine->x1E40);
    if ((state1 = &machine->x5E40)->v20()) {
        machine->func_0012FE38(state1);
    } else if ((state2 = &machine->x5E70)->v20()) {
        machine->func_0012FE38(state2);
    } else if ((state3 = &machine->x5E90)->v20()) {
        machine->func_0012FE38(state3);
    } else if ((state4 = &machine->x5FF0)->v20()) {
        machine->func_0012FE38(state4);
    } else if ((state5 = &machine->x62E0)->v20()) {
        machine->func_0012FE38(state5);
    } else if ((state6 = &machine->x6310)->v20()) {
        machine->func_0012FE38(state6);
    } else if (machine->func_0012E070() == 0.0f) {
        machine->func_0012FE38(&machine->x4ED0);
    }
    this->func_0019C148();
}
