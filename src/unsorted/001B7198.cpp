/* UNIT 0x001B7198 0x001B97E0 .sdata=0x29D430:0x29D440 .rodata=0x28EAE0:0x28EEE0 */
#include "race_limit.h"
#include "load_queue.h"
/* C++ unit: func_001B8B10 walks D_004D0318 with an inline list iterator; the
   iterator's by-reference operator!= is what leaves the sentinel in its own
   register (copied from the address before the first test). All functions
   are extern "C" so they keep their splat names. */
#include "include_asm.h"
#include "animation.h"

struct Game {
    char pad0[0x10C];
    int x10C;
    char pad110[0x2A0 - 0x110];
    int x2A0;
    char pad2A4[0x35C - 0x2A4];
    unsigned char x35C;
    char pad35D[0x36C - 0x35D];
    int x36C;
    char pad370[0x374 - 0x370];
    char x374[0xAF3C - 0x374];
    char xAF3C[1];
};

struct SoundSlots { char pad[0x254]; int x254[4],x264[4]; char pad274[0x14]; int x288,x28C; };
struct Entry { char pad[0x7AF4]; int x7AF4; };
struct SoundGlobal { char pad[0x50130]; struct Entry *x50130[4], *x50140[4]; char pad50150[0x58]; int count; };

struct Mode {
    char pad0[0x2A0];
    int x2A0;
};

struct Slot {
    int x0;
    int x4;
    int x8;
};

struct Race {
    char pad0[0x10C];
    int x10C;
    char pad110[0x14C - 0x110];
    Slot x14C[1];
    char pad158[0x1E8 - 0x158];
    Slot x1E8[4];
    char pad218[0x264 - 0x218];
    int x264[1];
    char pad268[0x288 - 0x268];
    int x288;
    int x28C;
    union { int x290; int lanes[1]; };
    char pad294[0x2A0 - 0x294];
    int x2A0;
    char pad2A4[0x2A8 - 0x2A4];
    int x2A8[16];
    char pad2E8[0x2F8 - 0x2E8];
    int x2F8[4];
    char pad308[0x32C - 0x308];
    int x32C[1];
    char pad330[0x350 - 0x330];
    unsigned char x350;
    char pad351[0x358 - 0x351];
    int x358;
    char pad35C[0x368 - 0x35C];
    int x368;
    int x36C;
};

struct Global {
    char pad0[0x2A8];
    int x2A8[4];
    char pad2B8[0x2DC - 0x2B8];
    int x2DC;
    int x2E0;
    char pad2E4[0xB174 - 0x2E4];
    int xB174;
};

struct State {
    char pad0[0x4C];
    int x4C;
};

struct Node {
    void *x0;
    Node *next;
    Node *prev;
};

struct ListIter {
    Node *node;
    ListIter(Node *n) : node(n) {}
    void *operator*() const { return node->x0; }
    ListIter &operator++() { node = node->next; return *this; }
    int operator!=(const ListIter &other) const { return node != other.node; }
};

struct List {
    int x0;
    Node *next;
    Node *prev;
    int xC;
    ListIter begin() { return ListIter(next); }
    ListIter end() { return ListIter((Node *)this); }
};

struct Course { char pad[0x694]; };

struct LoadArena {
    int x0;
    int *x4;
    int *x8;
};

struct LoadContext {
    char pad0[0x1684];
    LoadArena *x1684;
};

struct LoadRec {
    int x0;
    int x4;
    int x8;
};

struct LoadObj {
    char pad0[0x11C];
    LoadRec x11C[4];
    LoadRec x14C[4];
    char pad17C[0x288 - 0x17C];
    int x288;
    int x28C;
};

struct SetupSlot { int x0,x4,x8; };
struct Setup { char pad[0x11c]; SetupSlot a[4], b[4]; char pad17c[0x10c]; int countA,countB; char pad290[0xc8]; int active; };

struct CourseTable { char pad0[0x12218]; short x12218[20]; };
struct Mode19Header { char x0; unsigned char count; };
struct Mode19Data { char pad0[0x11C]; int x11C[1]; };
inline int indexOffset(const int &i) { return i * 4; }
extern "C" {
extern int func_001B89C8(Race *);
extern void func_001B7A40(Race *, int, int);
extern char D_00381CF0[];
extern char D_0043FA00[];
extern void func_001B98F0(Race *, int, int, int, int);

extern char D_004225A0[];
extern int D_004B2840[];
extern int func_00194C80(void *,int);

char D_0029D430[8] = "LVL";
static char *D_0029D438 = (char *)0x0028EAB0;
unsigned char D_0029D43C = 0;

struct LoadValues { int x[4]; };
extern const LoadValues D_0028EED0;

extern struct SoundGlobal D_002AF2C0;



extern int D_0029D9D4;
extern char D_0029D458[];
extern char D_0029D460[];
extern int D_0029DD68;
extern char D_00398C40[];
extern char D_003C6370[];
extern State D_0043EEC0;
extern int D_004B1F40[];
extern Global D_004B25A0;
extern char D_004BD4DC[];
extern List D_004D0318;
extern char D_004D0350[];
extern LoadArena D_004D0368;
extern char D_004D0380[];
extern LoadContext D_004D1A18;
extern LoadContext D_004D30B0;
extern char D_004D4748[];
extern char D_00663B08[];

extern void func_001B8CD8(void *, short);
extern void func_001B70F0(void *);
extern int func_001BA6B8(void *);
extern void func_0020F058(int, int);
extern void func_0020F240(void);
extern void func_001B74E8(void *);
extern int func_00182330(int, int);
extern float func_00182B18(void *, int);
extern void func_00182838(void *, int, int, int, float);
extern void func_00223B00(void *);
extern void func_0020D0A0(void *);
extern void func_0016B8C8(void *);
extern void func_002079B0(void *);
extern void func_002079C8(void *);
extern void func_0013AF80(void);
extern void func_001B9220(void *, void *, int, int);
extern void func_001B8D10(LoadObj *, void *);
extern void func_001B94F8(void *);
extern void func_00158680(void *, int, int);
extern void func_001B87F8(void *);
extern void func_0016BC98(void *);
extern void func_001B7360(struct SoundSlots *);
extern void func_001C6F00(void *);
extern void func_001C66B8(void *);
extern void func_001DDE70(void *);
extern void func_00182920(void *);
extern void func_001B7C08(Race *, int, int);
extern void func_001B99B8(Race *, int);
extern void func_001B7CF0(Race *, int, int);
extern void func_001B7F38(Race *, int, int);
extern void func_001B8228(Race *, int, int);
extern void func_001B84F8(Race *, int, int);
extern void func_001B9978(Race *, int, int, int, int);
extern int func_001FA7C0(int);
extern int func_00182290(int);
extern void func_002112D0(void *);
extern void func_00238A68(int);
extern void func_00218038(void);
extern void func_0020E860(void);
extern void func_00220CC0(void);
extern void func_0022F8C0(void);
extern void func_0023D6C8(void);
extern void func_0016B748(void *);
extern void func_00220550(int);
extern void func_0011AD18(SoundGlobal *,int,int);
extern void func_0011AD80(SoundGlobal *,int,int);
extern void func_001FABE8(void *,SetupSlot *);

void func_001B7198(Game *game)
{
    int handle;
    int on;
    void *sub;
    char *snd;

    on = 1;
    sub = game->x374;
    func_001B8CD8(game, 1);
    func_001B70F0(game);
    func_0020F058(func_001BA6B8(game), 0);
    func_0020F240();
    func_001B74E8(game);
    snd = D_003C6370;
    handle = func_00182330(game->x2A0, game->x10C);
    func_00182838(snd, handle, game->x10C, 1, func_00182B18(snd, game->x2A0));
    func_00223B00(D_004D0350);
    func_00223B00(&D_004D0368);
    func_0020D0A0(D_004D0380);
    func_0020D0A0(&D_004D1A18);
    func_0020D0A0(&D_004D30B0);
    func_0020D0A0(D_004D4748);
    func_0016B8C8(D_00398C40);
    game->x35C = on;
    func_002079B0(D_004D0350);
    func_002079C8(D_0029D458);
    func_002079C8(D_0029D460);
    func_0013AF80();
    func_001B9220(game, game->xAF3C, D_0029D43C, 1);
    func_001B8D10((LoadObj *)game, game->xAF3C);
    func_001B94F8(game);
    func_00158680(&D_0029DD68, D_0029D9D4, 0);
    func_001B87F8(game);
    game->x36C = on;
    func_0016BC98(D_00398C40);
    func_001B7360((SoundSlots *)game);
    func_001C6F00(sub);
    func_001C66B8(sub);
    func_001DDE70(D_00398C40 + 8);
    func_00182920(snd);
}

void func_001B7360(struct SoundSlots *p) {
 AnimationHandle local; int i; int *values; struct Entry **entries;
 local.x8=0;
 for(i=0;i<p->x288;i++) {
 func_002036C8(&local,38,0,1,D_002AF2C0.x50130[i]->x7AF4-3);
 func_00203A80(&local,1,(float)(p->x254[i] *2));
 }
 for(i=0;i<p->x28C;i++) {
 func_002036C8(&local,38,0,1,D_002AF2C0.x50140[i]->x7AF4-3);
 func_00203A80(&local,1,(float)(p->x264[i] *2));
 }
}

int func_001B74A0(Mode *object)
{
    switch (object->x2A0) {
    case 0: case 1: case 2: case 3: case 5: case 11: return 0;
    case 6: case 7: case 8: case 9: return 1;
    case 4: case 10: return 2;
    default: return 0;
    }
}

void func_001B74E8(void *arg)
{
    Race *obj = (Race *)arg;
    int i;
    int count;
    int width;
    int flag;
    int level;
    int value;
    Mode19Header *data;

    count = 0;
    if (obj->x10C == 20) {
        obj->x288 = 0;
        obj->x28C = 0;
        obj->x358 = 0;
        return;
    }

    i = 0;
    obj->x358 = obj->x28C = func_001B89C8(obj);

    if (obj->x288 > 0) {
        int more;
        int index = 0;
        do {
            int *lanes = obj->lanes + index;
            Slot *slots = (Slot *)((char *)obj + 0x1BC) + index;
            obj->x2A8[*lanes / 32 - 1] = i;
            lanes[0x68/4] = *lanes;
            if (obj->x10C == 10) slots->x0 = 0;
            do { do {
            int limit = obj->x288;
            i++;
            index++;
            more = i < limit;
            } while (0); } while (0);
        } while (more);
    }

    if (obj->x10C == 19) {
        data = *(Mode19Header **)D_0043FA00;
        obj->x358 = data->count;
        i = 0;
        {
            int index = 0;
            for (; i < data->count; i++, index++) {
                int *record = (int *)((char *)data + 0x4C) + index * 3;
                unsigned short *widthp = (unsigned short *)((char *)data + 0x12) + index;
                int *out = obj->x32C + index;
                int offset;
                int width;
                width = *widthp;
                *out = width;
                int laneIndex = width / 32 - 1;
                obj->x2A8[laneIndex] = i + 4;
                offset = indexOffset(i);
                func_001B9978(obj, *out, i, record[0x68 / 4], record[0x6C / 4]);
                func_001B98F0(obj, i, record[-2], record[-1], record[0]);
                *(int *)((int)obj + offset + 0x264) =
                    *(int *)((int)data + offset + 0x11C);
            }
        }
        return;
    }

    if (obj->x10C < 3 && RaceLimitReachedLegacy(D_00381CF0 + obj->x10C * 24)) {
        width = ((CourseTable *)D_004225A0)->x12218[obj->x10C];
        if (obj->x2A8[width / 32 - 1] == -1) {
            func_001B7A40(obj, 0, width);
            count = 1;
        }
    }

    else if (obj->x10C == 3) {
        level = func_00194C80(D_004225A0 + ((obj->x290 >> 5) - 1) * 0x694, 3);
        if (level == 21) {
            if (obj->x2A8[(0x1A0 >> 5) - 1] == -1) {
                func_001B7A40(obj, 0, 0x1A0);
                count = 1;
            }
            if (obj->x2A8[(0x140 >> 5) - 1] == -1) {
                func_001B7A40(obj, count, 0x140);
                count++;
            }
        } else if (level == 22) {
            if (obj->x2A8[(0xE0 >> 5) - 1] == -1) {
                func_001B7A40(obj, 0, 0xE0);
                count = 1;
            }
            if (obj->x2A8[(0x140 >> 5) - 1] == -1) {
                func_001B7A40(obj, count, 0x140);
                count++;
            }
        } else if (level == 23) {
            if (obj->x2A8[(0x100 >> 5) - 1] == -1) {
                func_001B7A40(obj, 0, 0x100);
                count = 1;
            }
            if (obj->x2A8[(0xE0 >> 5) - 1] == -1) {
                func_001B7A40(obj, count, 0xE0);
                count++;
            }
            if (obj->x2A8[(0x180 >> 5) - 1] == -1) {
                func_001B7A40(obj, count, 0x180);
                count++;
            }
        }
    }

    flag = 0;
    if (obj->x10C == 2 || obj->x10C == 4) {
        if (RaceLimitReachedLegacy(D_00381CF0 + obj->x10C * 24)) {
            flag = 1;
            D_004B25A0.x2DC = 4;
            D_004B25A0.x2E0 = 4;
        }
    }
    if (obj->x10C >= 6 && obj->x10C <= 9) {
        if (RaceLimitReachedLegacy(D_00381CF0 + obj->x10C * 24)) {
            flag = 1;
            D_004B25A0.x2DC = 4;
            D_004B25A0.x2E0 = 4;
        }
    }

    width = obj->x368;
    for (int k = 0; k < 16; k++) {
        int originalSpecial;
        int candidateSpecial;

        if (count < obj->x28C) {
            if (obj->x2A8[width / 32 - 1] == -1) {
                value = obj->x2F8[0];
                originalSpecial = (value == 0xE0 || value == 0x100 || value == 0x140 ||
                                   value == 0x180 || value == 0x1A0);
                if (originalSpecial) {
                    if (!flag) goto add_special_lane;
                }
                candidateSpecial = (width == 0xE0 || width == 0x100 || width == 0x140 ||
                                    width == 0x180 || width == 0x1A0);
                if (candidateSpecial && !flag) goto advance_lane;
                func_001B7A40(obj, count, width);
                count++;
                goto advance_lane;
add_special_lane:
                func_001B7A40(obj, count, width);
                count++;
            }
advance_lane:
            width += 0x20;
            if (width >= 0x1E1) {
                width = 0x20;
            }
        }
    }
}



void func_001B7A40(Race *obj, int idx, int width)
{
    int ok;

    D_004B25A0.x2A8[width / 32 - 1] = 4;
    obj->x32C[idx] = width;
    ok = 0;
    if (width == 0xE0 || width == 0x100 || width == 0x140 || width == 0x180 || width == 0x1A0) {
        ok = 1;
    }
    if (ok) {
        func_001B7C08(obj, width, idx);
    } else {
        func_001B99B8(obj, idx);
        if (obj->x10C == 0 && obj->x288 == 1) {
            func_001B7CF0(obj, width, idx);
        } else if (obj->x10C == 1 && obj->x288 == 1) {
            func_001B7F38(obj, width, idx);
        } else if (obj->x10C == 2 && obj->x288 == 1) {
            func_001B8228(obj, width, idx);
        } else if (obj->x10C == 3 && obj->x288 == 1) {
            func_001B84F8(obj, width, idx);
        } else {
            func_001B9978(obj, width, idx, 0, 0);
        }
    }
    obj->x264[idx] = func_001FA7C0(obj->x14C[idx].x0);
}

void func_001B7C08(Race *obj, int width, int idx)
{
    int type;
    Slot *slot = (Slot *)(type = (int)&obj->x1E8[idx]);
    Slot *dst = &obj->x14C[idx];
    int value, group;

    value = (width >> 5) - 1;
    slot->x0 = value;
    slot->x4 = 0;
    slot->x8 = 0;

    if (width == 0xE0) {
        if (func_00182290(D_004B2840[0]) == 0) {
            value = 0x1C;
            type = 4;
            group = 5;
            goto finish;
        } else {
            value = 0x1D;
            type = 6;
            group = 5;
            goto finish;
        }
    } else if (width == 0x100) {
        value = 0x22;
        type = 0x15;
        goto group6;
    } else if (width == 0x140) {
        value = 0x1A;
        type = 2;
        group = 4;
        goto finish;
    } else if (width == 0x180) {
        value = 0x1E;
        type = 0x13;
        goto group6;
    } else if (width == 0x1A0) {
        value = 0x1F;
        type = 0xB;
        goto group6;
    } else {
        return;
    }
group6:
    group = 6;
finish:
    dst->x8 = group;
    dst->x4 = type;
    dst->x0 = value;
}
void func_001B7CF0(Race *obj,int width,int idx)
{
    Slot *slot=&obj->x1E8[idx];
    int a[3][5]={{12,16,1,0,0},{12,12,16,16,1},{12,12,16,16,1}};
    int b[3][5]={{3,12,3,-1,-1},{3,3,12,12,3},{-1,3,3,12,3}};
    int level=func_00194C80(D_004225A0+((obj->x290>>5)-1)*0x694,obj->x10C);
    int special;
    slot->x8=0;
    slot->x0=(width>>5)-1;
    special=0;
    if(width==0xE0 || width==0x100 || width==0x140 || width==0x180 || width==0x1A0) special=1;
    if(special) goto zero;
    if(level<4) { slot->x4=0; goto finish; }
    if(width==0x1E0 || width==0x1C0) goto zero;
    goto lookup;
zero:
    slot->x4=0;
    goto finish;
lookup:
    slot->x4=a[level-4][idx];
finish:
    if(level>=4) {
        Slot *dst=&obj->x14C[idx];
        int value=b[level-4][idx];
        if(value>=0) dst->x0=value;
    }
}
void func_001B7F38(Race *obj,int width,int idx)
{
    Slot *slot=&obj->x1E8[idx];
    int a[3][7]={{11,5,17,17,0,0,0},{11,11,11,5,5,17,17},{11,11,5,5,17,17,17}};
    int b[3][7]={{19,19,15,15,-1,-1,-1},{19,19,19,19,19,15,15},{-1,19,19,19,19,15,15}};
    int level=func_00194C80(D_004225A0+((obj->x290>>5)-1)*0x694,obj->x10C);
    int special;
    slot->x8=0;
    slot->x0=(width>>5)-1;
    special=0;
    if(width==0xE0 || width==0x100 || width==0x140 || width==0x180 || width==0x1A0) special=1;
    if(special) goto zero;
    if(level<4) { slot->x4=0; goto finish; }
    if(width==0x1E0 || width==0x1C0) goto zero;
    goto lookup;
zero:
    slot->x4=0;
    goto finish;
lookup:
    slot->x4=a[level-4][idx];
finish:
    if(level>=4) {
        Slot *dst=&obj->x14C[idx];
        int value=b[level-4][idx];
        if(value>=0) dst->x0=value;
    }
}
void func_001B8228(Race *obj,int width,int idx)
{
    Slot *slot=&obj->x1E8[idx];
    int a[3][9]={{6,6,10,10,8,0,0,0,0},{6,6,6,10,10,10,8,8,8},{6,6,6,10,10,8,8,8,8}};
    int b[3][9]={{1,1,4,4,1,-1,-1,-1,-1},{1,1,1,4,4,4,1,1,1},{-1,1,1,1,4,4,1,1,1}};
    int level=func_00194C80(D_004225A0+((obj->x290>>5)-1)*0x694,obj->x10C);
    int special;
    slot->x8=0;
    slot->x0=(width>>5)-1;
    special=0;
    if(width==0xE0 || width==0x100 || width==0x140 || width==0x180 || width==0x1A0) special=1;
    if(special) goto zero;
    if(level<4) { slot->x4=0; goto finish; }
    if(width==0x1E0 || width==0x1C0) goto zero;
    goto lookup;
zero:
    slot->x4=0;
    goto finish;
lookup:
    slot->x4=a[level-4][idx];
finish:
    if(level>=4) {
        Slot *dst=&obj->x14C[idx];
        int value=b[level-4][idx];
        if(value>=0) dst->x0=value;
    }
}
void func_001B84F8(Race *obj,int width,int idx)
{
    Slot *slot=&obj->x1E8[idx];
    int a[6][9]={{4,2,3,14,0,0,0,0,0},{4,2,14,0,0,0,0,0,0},{4,2,14,3,15,0,0,0,0},
                 {4,2,14,3,15,7,0,0,0},{4,14,15,0,0,0,0,0,0},{4,2,3,15,0,0,0,0,0}};
    int b[6][9]={{22,18,10,10,-1,-1,-1,-1,-1},{22,18,0,-1,-1,-1,-1,-1,-1},
                 {22,18,0,10,11,-1,-1,-1,-1},{-1,-1,22,22,18,0,10,11,2},
                 {-1,-1,22,0,11,-1,-1,-1,-1},{-1,-1,-1,22,18,10,11,-1,-1}};
    int level=func_00194C80(D_004225A0+((obj->x290>>5)-1)*0x694,obj->x10C);
    int special;
    slot->x8=0;
    slot->x0=(width>>5)-1;
    special=0;
    if(width==0xE0 || width==0x100 || width==0x140 || width==0x180 || width==0x1A0) special=1;
    if(special) goto zero;
    if(level<18) { slot->x4=0; goto finish; }
    if(width==0x1E0 || width==0x1C0) goto zero;
    goto lookup;
zero:
    slot->x4=0;
    goto finish;
lookup:
    slot->x4=a[level-18][idx];
finish:
    if(level>=18) {
        Slot *dst=&obj->x14C[idx];
        int value=b[level-18][idx];
        if(value>=0) dst->x0=value;
    }
}
void func_001B87F8(void *arg)
{
    Setup *obj = (Setup *)arg;
    int i;

    if (D_004B25A0.xB174) {
        for (i = 0; i < obj->countA; i++) {
            func_0011AD18(&D_002AF2C0, i, i);
            func_001FABE8((char *)D_002AF2C0.x50130[i] + 0x78D8, &obj->a[i]);
        }
        int count = obj->countA;
        D_002AF2C0.count = count;
        obj->active = 0;
    } else {
        for (i = 0; i < obj->countA; i++) {
            func_0011AD18(&D_002AF2C0, i, i);
            func_001FABE8((char *)D_002AF2C0.x50130[i] + 0x78D8, &obj->a[i]);
        }
        for (i = 0; i < obj->countB; i++) {
            func_0011AD80(&D_002AF2C0, i, i + obj->countA);
            func_001FABE8((char *)D_002AF2C0.x50140[i] + 0x78D8, &obj->b[i]);
        }
        D_002AF2C0.count = obj->countA + obj->countB;
        obj->active = obj->countB;
    }
}

int func_001B89C8(Race *o)
{
    int result = 0;

    if (o->x10C == 14 || o->x10C == 5 || o->x10C == 13 || o->x10C == 17 || o->x10C == 18 || o->x10C == 11) return 0;
    if (o->x288 == 1) {
        if (func_00182290(o->x2A0) == 2) result = o->x10C == 10 ? 8 : 9;
        else if (func_00182290(o->x2A0) == 1) result = 7;
        else if (func_00182290(o->x2A0) == 0) result = 5;
        else result = o->x2A0 == 23 ? 5 : 0;
    } else if (o->x288 == 2) {
        if (func_00182290(o->x2A0) == 2) result = 6;
        else if (func_00182290(o->x2A0) == 1) result = 5;
        else if (func_00182290(o->x2A0) == 0) result = 4;
        else result = o->x2A0 == 23 ? 4 : 0;
    }
    return D_004B25A0.xB174 == 0 ? result : 0;
}

void func_001B8B10(Race *race)
{
    char *manager;
    Global *other;

    for (ListIter it = D_004D0318.begin(); it != D_004D0318.end(); ++it) {
        func_002112D0(*it);
    }
    manager = D_00398C40;
    func_00223B00(D_00663B08);
    other = &D_004B25A0;
    func_00238A68(60);
    func_00218038();
    func_0020E860();
    func_00220CC0();
    func_0022F8C0();
    func_0023D6C8();
    func_0016B748(manager);
    func_0016B8C8(manager);
    func_001B9220(other, D_004BD4DC, D_0029D43C, 0);
    func_00158680(&D_0029DD68, D_0029D9D4, 0);
    func_001B87F8(other);
    race->x36C = 0;
    func_0016BC98(manager);
    func_00220550(1);
    D_0043EEC0.x4C = 1;
    race->x350 = 1;
    if (race->x10C == 15) {
        D_004B1F40[0] = 0x1C20;
    }
}

}
extern "C" {
typedef struct Obj { char pad0[0x2A4]; int x2A4; } Obj;
extern int D_0026F210[];
int func_001B8C48(Obj *obj) {
    int value = D_0026F210[obj->x2A4];
    obj->x2A4++;
    if (obj->x2A4 >= 24) obj->x2A4 = 0;
    return value;
}
}
/* func_001B8C80 is a C++ constructor (returns this; constants are loaded
   before the stores the way gcc emits constructor bodies). Classes are named
   after their constructors' functions so the mangled names
   (__13func_001B8C80, __13func_001C5A00, __13func_001C4E58) link to the
   splat symbols. */

struct func_001C5A00 {
    char pad0[0x4F0 - 0x374];
    func_001C5A00(void);
};

struct func_001C4E58 {
    char pad0[0x9F8 - 0x4F0];
    func_001C4E58(void);
};

struct func_001B8C80 {
    char pad0[0x35C];
    unsigned char x35C;
    char pad35D[0x374 - 0x35D];
    func_001C5A00 x374;
    func_001C4E58 x4F0;
    int x9F8;
    int x9FC;
    int xA00;
    unsigned char xA04;
    func_001B8C80();
};

func_001B8C80::func_001B8C80()
{
    x35C = 1;
    x9F8 = 0x3C3;
    x9FC = 0x200;
    xA00 = 0x399;
    xA04 = 0;
}
extern "C" {
extern void func_00243430(int, int, int, short);
extern void func_00243308(void);
void func_001B8CD8(void *obj, short value) {
    func_00243430(0, 1, 2, value);
    func_00243308();
}

extern char D_0029D468[];
extern char D_0029D470[];
extern char D_0029D478[];
extern void func_0020D078(LoadContext *, int);
extern void *func_00230EC0(void *, int);
extern void func_00230F80(void *, void *, void *, void *, int, int);

extern void *func_001B94B8(LoadObj *, int, int);
extern void func_001BA6F0(LoadObj *, int, LoadRec *, void *, void *, int);
extern int func_0020D170(LoadContext *, int);
extern int func_00218E88(int, int);
extern void func_00218D98(int);

extern const LoadValues D_0028EED0 = { { 1, 2, 3, 0 } };

void func_001B8D10(LoadObj *obj, void *arg1)
{
    LoadValues values = D_0028EED0;
    char a[0x150];
    char b[0x150];
    int i;
    int j;
    int count;
    int handle;
    int *old;
    int p;
    void *entry;

    old = D_004D0368.x8;
    p = ((int)D_004D0368.x4 + 3) & ~3;
    D_004D0368.x8 = (int *)p;
    D_004D0368.x4 = (int *)(p + 4);
    *(int *)p = (int)old;

    func_0020D078(&D_004D30B0, 1);
    func_0020D078(&D_004D1A18, 1);
    D_004D30B0.x1684 = &D_004D0368;
    D_004D1A18.x1684 = &D_004D0368;

    func_00230EC0(a, 1);
    func_00230F80(a, D_0029D430, arg1, D_0029D468, D_0029D43C, 0);

    for (i = 0; i < obj->x288; i++) {
        entry = func_001B94B8(obj, 1, i);
        func_00230F80(a, D_0029D470, entry, D_0029D468, 1, (unsigned char)i);
        count = (obj->x11C[i].x0 >= 35 && obj->x11C[i].x0 <= 37) ? 1 : 4;
        if (count != 0) {
            int *vp = values.x;
            int n = count;
            do {
                func_001BA6F0(obj, *vp, &obj->x11C[i], a, D_0029D468, i);
                n--;
                vp++;
            } while (n != 0);
        }
    }

    for (i = 0; i < obj->x28C; i++) {
        entry = func_001B94B8(obj, 0, i);
        func_00230F80(a, D_0029D470, entry, D_0029D468, 1, (unsigned char)i);
        count = (obj->x14C[i].x0 >= 35 && obj->x14C[i].x0 <= 37) ? 1 : 4;
        if (count != 0) {
            int *vp = values.x;
            int n = count;
            do {
                func_001BA6F0(obj, *vp, &obj->x14C[i], a, D_0029D468, i);
                n--;
                vp++;
            } while (n != 0);
        }
    }

    ProcessLoadQueueLegacy(a);

    D_004D0368.x4 = D_004D0368.x8;
    D_004D0368.x8 = (int *)*D_004D0368.x8;

    handle = func_001BA6B8(obj);
    func_00218E88(handle, func_0020D170(&D_004D30B0, 0));

    func_00230EC0(b, 2);
    func_00230F80(b, D_0029D430, arg1, D_0029D478, D_0029D43C, 0);

    for (i = 0; i < obj->x288; i++) {
        entry = func_001B94B8(obj, 1, i);
        func_00230F80(b, D_0029D470, entry, D_0029D478, 1, (unsigned char)i);
        count = (obj->x11C[i].x0 >= 35 && obj->x11C[i].x0 <= 37) ? 1 : 4;
        if (count != 0) {
            int *vp = values.x;
            int n = count;
            do {
                func_001BA6F0(obj, *vp, &obj->x11C[i], b, D_0029D478, i);
                vp++;
                n--;
            } while (n != 0);
        }
    }

    for (i = 0; i < obj->x28C; i++) {
        entry = func_001B94B8(obj, 0, i);
        func_00230F80(b, D_0029D470, entry, D_0029D478, 1, (unsigned char)i);
        count = (obj->x14C[i].x0 >= 35 && obj->x14C[i].x0 <= 37) ? 1 : 4;
        if (count != 0) {
            int *vp = values.x;
            int n = count;
            do {
                func_001BA6F0(obj, *vp, &obj->x14C[i], b, D_0029D478, i);
                vp++;
                n--;
            } while (n != 0);
        }
    }

    ProcessLoadQueueLegacy(b);
    func_0020D078(&D_004D30B0, 0);
    func_0020D078(&D_004D1A18, 0);
    func_00218D98(func_001BA6B8(obj));
}
}
extern "C" {
extern char D_0029D480[];
/* NGP pass of the LoadObj writer (sibling func_001B8D10 does RTX+TEX).
   Second set offsets by obj->x288; entry type 35-37 uses 1 id, else 4. */
void func_001B9220(void *raw, void *arg1, int arg2, int enable) {
    LoadObj *obj = (LoadObj *)raw;
    LoadValues ids = D_0028EED0;
    char buf[0x150];
    int i, count, remaining;
    int *p;
    if (enable) {
        func_0020D078((LoadContext *)D_004D0380, 2);
        ((LoadContext *)D_004D0380)->x1684 = &D_004D0368;
        func_00230EC0(buf, 0);
        func_00230F80(buf, D_0029D430, arg1, D_0029D480, arg2, 0);
        for (i = 0; i < obj->x288; i++) {
            func_00230F80(buf, D_0029D470, func_001B94B8(obj, 1, i), D_0029D480, 1, (unsigned char)i);
            count = 4;
            if ((unsigned)(obj->x11C[i].x0 - 35) <= 2) count = 1;
            if (count) {
                remaining = count;
                p = ids.x;
                do {
                    func_001BA6F0(obj, *p, &obj->x11C[i], buf, D_0029D480, i);
                    remaining--;
                    p++;
                } while (remaining);
            }
        }
        for (i = 0; i < obj->x28C; i++) {
            func_00230F80(buf, D_0029D470, func_001B94B8(obj, 0, i), D_0029D480, 1, (unsigned char)(obj->x288 + i));
            count = 4;
            if ((unsigned)(obj->x14C[i].x0 - 35) <= 2) count = 1;
            if (count) {
                remaining = count;
                p = ids.x;
                do {
                    int id = *p;
                    func_001BA6F0(obj, id, &obj->x14C[i], buf, D_0029D480, i + obj->x288);
                    p++;
                    remaining--;
                } while (remaining);
            }
        }
        ProcessLoadQueueLegacy(buf);
        func_0020D078((LoadContext *)D_004D0380, 0);
    }
}
}

/* func_001B94B8: C++ method from src/unsorted/001B94B8.cpp (class renamed from
   Obj to Obj94B8: this unit already defines Obj for func_001B8C48; the link
   aliases any func_001B94B8__* mangling to the plain splat symbol). */
struct Elem {
    char pad0[0xC];
};

extern void *func_001FA3E8(Elem *e);

struct Obj94B8 {
    char pad0[0x1B8];
    Elem x1B8[4];
    Elem x1E8[4];
    void *func_001B94B8(int flag, int index);
};

void *Obj94B8::func_001B94B8(int flag, int index) {
    if (flag) {
        return func_001FA3E8(&x1B8[index]);
    }
    return func_001FA3E8(&x1E8[index]);
}

extern "C" {
extern char D_0029D488[];
extern void func_00230EF8(void *, void (*)(void));
extern void func_0020B228(void);

/* func_001B94F8 reuses LoadValues/LoadRec/LoadObj above; Arena keeps x4 as int
   (LoadArena's x4 is int *, which would emit pointer arithmetic here). */
typedef struct Arena { int x0; int x4; int *x8; } Arena;
typedef struct SaveGlobal { char p0[0x1684]; Arena *x1684; } SaveGlobal;
typedef struct Obj94F8 {
    char p0[0x11c];
    LoadRec x11C[4], x14C[4];
    char p17C[0x288 - 0x17c];
    int x288, x28C;
    char p290[0xaf3c - 0x290];
    int xAF3C;
} Obj94F8;

void func_001B94F8(void *arg) {
    Obj94F8 *obj = (Obj94F8 *)arg;
    LoadValues values = D_0028EED0;
    char stream[0x150];
    int i, n, serial = 1;
    int *value;
    unsigned int address;
    int *old;
    func_00230EC0(stream, 3);
    func_00230EF8(stream, func_0020B228);
    old = ((Arena *)D_00663B08)->x8;
    address = (((Arena *)D_00663B08)->x4 + 3) & ~3;
    ((Arena *)D_00663B08)->x8 = (int *)address;
    ((Arena *)D_00663B08)->x4 = address + 4;
    *(int *)address = (int)old;
    ((SaveGlobal *)D_004D4748)->x1684 = (Arena *)D_00663B08;
    func_0020D078((LoadContext *)D_004D4748, 1);
    func_00230F80(stream, D_0029D430, &obj->xAF3C, D_0029D488, D_0029D43C, 0);
    for (i = 0; i < obj->x288; i++) {
        func_00230F80(stream, D_0029D470, func_001B94B8((LoadObj *)obj, 1, i), D_0029D488, 1, (unsigned char)serial++);
        {
            int iterations = ((unsigned)(obj->x11C[i].x0 - 35) <= 2) ? 1 : 4;
            if (iterations) {
                n = iterations;
                value = values.x;
                do {
                    func_001BA6F0((LoadObj *)obj, *value, &obj->x11C[i], stream, D_0029D488, serial++);
                    value++;
                } while (--n);
            }
        }
    }
    for (i = 0; i < obj->x28C; i++) {
        func_00230F80(stream, D_0029D470, func_001B94B8((LoadObj *)obj, 0, i), D_0029D488, 1, (unsigned char)serial++);
        {
            int iterations = ((unsigned)(obj->x14C[i].x0 - 35) <= 2) ? 1 : 4;
            if (iterations) {
                n = iterations;
                value = values.x;
                do {
                    func_001BA6F0((LoadObj *)obj, *value, &obj->x14C[i], stream, D_0029D488, serial++);
                    value++;
                } while (--n);
            }
        }
    }
    ProcessLoadQueueLegacy(stream);
    func_0020D078((LoadContext *)D_004D4748, 0);
    ((Arena *)D_00663B08)->x4 = (int)((Arena *)D_00663B08)->x8;
    ((Arena *)D_00663B08)->x8 = (int *)*((Arena *)D_00663B08)->x8;
}
}
