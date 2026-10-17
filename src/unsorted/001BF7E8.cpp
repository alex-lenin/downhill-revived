/* UNIT 0x001BF7E8 0x001BFD50 .rodata=0x28F7D0:0x28F860 */
#include "skat_queue.h"
#include "event_queue.h"
typedef struct Rider {
    char pad0[0x3514];
    void *x3514;
    char pad3518[0x4414];
    void *x792C;
    char pad7930[0x3C];
    int x796C;
    char pad7970[0x3C];
    int x79AC;
    char pad79B0[0xAC];
    int x7A5C;
    char pad7A60[8];
    unsigned char x7A68;
} Rider;

typedef struct Obj {
    int x0;
    int x4;
    Rider *x8;
    int blocked() { return x8->x7A68; }
} Obj;

typedef struct Mode {
    char pad0[0x10C];
    int x10C;
    char pad110[0x190];
    int x2A0;
    char pad2A4[0xC0];
    int x364;
    int kind() { return x2A0; }
    int mode() { return x10C; }
} Mode;

typedef struct Game {
    char pad0[0x5350];
    int mgr;
    int queue;
    void *info() { return &mgr; }
    void *getqueue() { return &queue; }
    void *entry(int i) { return (char *)this + i * 0x84 + 0x5964; }
} Game;

extern Mode D_004B25A0;
extern Game D_00398C40;

extern int func_001BC6A0(void *, int, int);
extern int func_001BC618(void *, int);

extern void func_001DB6B0(void *, int, int, int *, int *, int);
extern void func_001BC868(void *, int, int, int, int, int, int, void *, int, int, int);
extern int func_001BC480(void *, int);
extern void func_001C7BA8(void *, unsigned int);
extern int func_001BFFF0(void *, int);
extern void func_001BBF30(void *, int, void *);

static inline void set_int(void *p, int index, int code, int value) { func_00233BA8((EventQueue *)(p), index, code, (int *)(&value), 4, -1); }

static inline int equal(int a, int b) { return a == b; }
static inline int greater(int a, int b) { return a > b; }

int func_001BF7E8(Obj *s, int id, int a, int b) {
    int special[4];
    int found, other;
    int flag, index, res;

    if (id == 1 || id == 0x17) {
        if (D_004B25A0.kind() == 0x1B || D_004B25A0.kind() == 0x1C ||
            D_004B25A0.mode() == 0xD || D_004B25A0.mode() == 0x12) return -1;
    }
    flag = 0;
    if (D_004B25A0.mode() == 0xF)
        flag = (D_004B25A0.x364 == 1);
    if (flag && greater(s->x8->x79AC, 1)) return -1;
    if (s->blocked() != 0) return -1;
    found = 0;
    if (s->x8->x7A5C == 1)
        func_001DB6B0((char *)s->x8->x792C + 16, a, b, &found, &other, 0);
    else
        func_001DB6B0((char *)s->x8->x3514 + 16, a, b, &found, &other, 0);
    if (!found) return -1;

    if (id == 0xF)
        index = func_001BC6A0(D_00398C40.info(), 0xB, s->x0);
    else
        index = func_001BC6A0(D_00398C40.info(), 9, s->x0);
    if (index != 5) {
    if (id == 0xF) {
        int temp = func_001BC618(D_00398C40.info(), s->x0);
        if (temp != 5) func_00235798((SkatQueueOwner *)(D_00398C40.getqueue()), temp);
        func_001BC868(D_00398C40.info(), index, a, b, 0xB, 0, 1,
                      (char *)s->x8->x3514 + 16, 0, 0, 2);
    } else {
        func_001BC868(D_00398C40.info(), index, a, b, s->x4, 0, 1,
                      (char *)s->x8->x3514 + 16, 0, 0, 2);
    }
    s->x0 = func_001BC480(D_00398C40.info(), index);
    func_001C7BA8((char *)s->x8 + 0x7720, id);
    res = func_001BFFF0(D_00398C40.info(), s->x8->x796C);
    switch (id) {
    case 0:    set_int(D_00398C40.getqueue(), index, 0, res); break;
    case 1:    set_int(D_00398C40.getqueue(), index, 1, res); break;
    case 2:    set_int(D_00398C40.getqueue(), index, 2, res); break;
    case 0xA:  set_int(D_00398C40.getqueue(), index, 0xA, res); break;
    case 0x18: set_int(D_00398C40.getqueue(), index, 0x18, res); break;
    case 0x19: set_int(D_00398C40.getqueue(), index, 0x19, res); break;
    case 3:    set_int(D_00398C40.getqueue(), index, 3, res); break;
    case 4:    set_int(D_00398C40.getqueue(), index, 4, res); break;
    case 5:    set_int(D_00398C40.getqueue(), index, 5, res); break;
    case 6:    set_int(D_00398C40.getqueue(), index, 6, res); break;
    case 7:    set_int(D_00398C40.getqueue(), index, 7, res); break;
    case 8:    set_int(D_00398C40.getqueue(), index, 8, res); break;
    case 9:    set_int(D_00398C40.getqueue(), index, 9, res); break;
    case 0xB:  set_int(D_00398C40.getqueue(), index, 0xB, res); break;
    case 0xC:  set_int(D_00398C40.getqueue(), index, 0xC, res); break;
    case 0xD:  set_int(D_00398C40.getqueue(), index, 0xD, res); break;
    case 0xE:  set_int(D_00398C40.getqueue(), index, 0xE, res); break;
    case 0xF:  set_int(D_00398C40.getqueue(), index, 0xF, res); break;
    case 0x10: set_int(D_00398C40.getqueue(), index, 0x10, res); break;
    case 0x17: set_int(D_00398C40.getqueue(), index, 0x17, res); break;
    case 0x1D: set_int(D_00398C40.getqueue(), index, 0x1D, res); break;
    case 0x22: special[0] = res;  func_00233BA8((EventQueue *)(D_00398C40.getqueue()), index, 0x22, (int *)(&special[0]), 8, -1); break;
    default: break;
    }
    func_001BBF30(D_00398C40.getqueue(), index, D_00398C40.entry(index));
    }
    return index;
}
