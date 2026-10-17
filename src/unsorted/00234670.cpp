/* UNIT 0x00234670 0x002347B0 */
struct Item {
    int index;
    int score;
};

struct Rec {
    char pad0[0x5];
    unsigned char flags;
    char pad6[0xC - 0x6];
};

extern "C" void func_002347B0(void *, char *, int, int, int *, Item *, int *, int *, int);
extern "C" void func_00254FE8(void *, int, int, int (*)(Item *, Item *));
extern "C" int func_00234660(Item *, Item *);

class Obj {
    char pad0[0x1D4];
    Rec *recs;
    char pad1D8[0x5FC - 0x1D8];
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual unsigned int Random();
    int func_00234670(char *src, int a2, int a3);
};

int Obj::func_00234670(char *src, int a2, int a3)
{
    Item list[10];
    int count;
    int a;
    int b;
    int i;
    unsigned int pick;

    a = -1;
    count = 0;
    b = -1;
    func_002347B0(this, src + 0x14, a2, a3, &count, list, &a, &b, 0);
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < count && i < 10; i++) {
        recs[list[i].index].flags &= ~2;
    }
    if (count > 20) {
        count = 20;
    }
    func_00254FE8(list, count > 10 ? 10 : count, 8, func_00234660);
    count >>= 1;
    if (count != 0) {
        pick = Random() % count;
    } else {
        pick = 0;
    }
    return list[pick].index;
}
