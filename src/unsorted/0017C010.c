/* UNIT 0x0017C010 0x0017C130 */
typedef struct Node {
    char pad0[0x4C];
    int x4C;
} Node;

typedef struct Group {
    char pad0[0x8];
    int count;
    char padC[0x4];
    int x10;
    char pad14[0x8];
    Node *nodes[1];
} Group;

typedef struct Track {
    char pad0[0x8];
    unsigned short count;
    char padA[0x16];
    Group *groups[1];
} Track;

typedef struct Flag {
    char pad0[0x4];
    char x4;
} Flag;

typedef struct Game {
    char pad0[0x10C];
    int x10C;
    char pad110[0x178];
    int x288;
} Game;

typedef struct Obj {
    char pad0[0xF84];
    Track *track;
    char padF88[0x1F4];
    Flag *x117C;
    char pad1180[0x70];
    float x11F0;
    char pad11F4[0x94];
    float x1288;
    char pad128C[0x84];
    int x1310;
    int x1314;
    int x1318;
    int x131C;
    char pad1320[0x36];
    char x1356;
} Obj;

extern Game D_004B25A0;
extern void func_0017C2A0(Obj *obj);

void func_0017C010(Obj *obj, int arg1)
{
    int i;
    int j;
    Group *group;

    if (obj->track == 0 || D_004B25A0.x10C == 5) {
        return;
    }
    obj->x131C = arg1;
    func_0017C2A0(obj);
    obj->x1310 = 300;
    obj->x1314 = 10;
    obj->x1318 = 0;
    if (D_004B25A0.x288 == 1) {
        obj->x1288 = 0.90000004f;
    } else {
        obj->x1288 = 0.5f;
    }
    obj->x11F0 = -22.0f;
    obj->x117C->x4 = 1;
    obj->x1356 = 0;
    for (i = 0; i < obj->track->count; i++) {
        group = obj->track->groups[i];
        group->x10 = 0;
        for (j = 1; j < group->count; j++) {
            group->nodes[j]->x4C = 0;
        }
    }
}
