/* UNIT 0x00163218 0x00163328 */
struct Vec { float x, y, z, w; };

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
    void func_0012FE38(State *next);
};

struct Obj {
    char *x0;
    int x4;
    int index;
    float xC;
};

extern void func_001AC578(void *part);
extern float func_00203C28(Vec *value);

void func_00163218(Obj *obj)
{
    char *base;
    char *part;
    char *entry;
    State *state;
    Vec value;
    struct { unsigned char v; } expired;

    base = obj->x0;
    part = base + 0x1E40;
    state = (State *)(base + 0x5E90);
    func_001AC578(part);
    if (state->IsDone()) {
        ((Machine *)base)->func_0012FE38(state);
        return;
    }
    entry = part;
    entry += obj->index * 16 + 0x14;
    value = *(Vec *)entry;
    expired.v = 1.0f - obj->xC < func_00203C28(&value);
    if (expired.v) {
        ((Machine *)base)->func_0012FE38((State *)(base + 0x4ED0));
    } else if (obj->index == 0) {
        ((Machine *)base)->func_0012FE38((State *)(base + 0x4ED0));
    }
}
