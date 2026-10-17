/* UNIT 0x001BF0B8 0x001BF218 .sdata=0x29D578:0x29D580 */
#include "event_queue.h"
int D_0029D578 = 15;
int D_0029D57C = 120;
class Object { public: virtual void a(); virtual void b(); virtual void c(); virtual char *get(); };
struct Pair { int x0, x4; };
struct State { char pad[0x5350]; char x5350[4]; char x5354[4]; };
extern State D_00398C40;
extern void func_001DB6B0(void *, int, int, int *, int *, int);
extern int func_001BC6A0(void *, int, int);
extern void func_001BC868(void *, int, int, int, int, int, int, void *, int, int, int);
extern int func_001BC480(void *, int);
extern int func_001C0958(void *);
extern void func_001BBF30(void *, int, void *);

int func_001BF0B8(Pair *pair, Object *object)
{
    int params[3];
    int found = 0;
    int other;
    int index;
    State *state;
    char *base;

    func_001DB6B0(object->get() + 16, D_0029D578, D_0029D57C, &found, &other, 0);
    if (!found)
        return -1;
    state = &D_00398C40;
    base = state->x5350;
    index = func_001BC6A0(base, pair->x4, pair->x0);
    if (index != 5) {
        func_001BC868(base, index, D_0029D578, D_0029D57C, pair->x4, 0, 1,
                      object->get() + 16, 0, 0, 2);
        pair->x0 = func_001BC480(base, index);
        params[0] = func_001C0958(base);
        params[1] = 0;
        params[2] = 0;
        func_00233BA8((EventQueue *)(state->x5354), index, 18, (int *)(params), 12, -1);
        func_001BBF30(state->x5354, index, base + (index * 132 + 0x614));
    }
    return index;
}
