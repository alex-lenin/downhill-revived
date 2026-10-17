/* UNIT 0x0012FE38 0x0012FE98 */
class State {
public:
    int x0;
    virtual void v08();
    virtual void v10();
    virtual void v18();
    virtual void v20();
    virtual void Enter();             /* vtable +0x28 */
    virtual void Exit(State *next);   /* vtable +0x30 */
};

class Machine {
public:
    char pad0[0x4ECC];
    State *cur;

    void func_0012FE38(State *next);
};

void Machine::func_0012FE38(State *next)
{
    cur->Exit(next);
    next->Enter();
    cur = next;
}
