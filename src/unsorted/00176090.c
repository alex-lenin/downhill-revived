/* UNIT 0x00176090 0x00176138 */
/* Node: object pointed to by Group.parts[]; the field at +8 is compared
   against 7 (parts[0]) and 6 (parts[1..3]). */
typedef struct Node {
    char pad0[0x8];
    int type;               /* 0x8 */
} Node;

/* Group: event/group object. +8 is a kind; +0x20..+0x2C are four node
   pointers, parts[0] is checked separately and parts[1..3] in a loop. */
typedef struct Group {
    char pad0[0x8];
    unsigned short kind;    /* 0x8 */
    char padA[0x16];
    Node *parts[4];         /* 0x20 */
} Group;

typedef struct Link {
    Group *group;           /* 0x0 */
    char x4;                /* 0x4 */
} Link;

typedef struct Obj {
    char pad0[0xF84];
    Group *group;           /* 0xF84 */
    char padF88[0x1F4];
    Link *link;             /* 0x117C */
} Obj;

extern Link *func_00209108(void);

void func_00176090(Obj *obj, Group *group)
{
    int ok;
    int i;

    ok = 1;
    if (group->kind == 4) {
        /* One variable holds the node pointer and then its type: the compiler
           keeps both loads in one register this way (retail does too). */
        int type = (int)group->parts[0];
        type = *(int *)((char *)type + 8);
        if (type == 7) {
            for (i = 1; i < 4; i++) {
                int part = (int)group->parts[i];
                part = *(int *)((char *)part + 8);
                if (part != 6) ok = 0;
            }
        } else {
            ok = 0;
        }
    } else {
        ok = 0;
    }
    if (ok != 0) {
        Link *link = func_00209108();
        obj->link = link;
        link->x4 = 0;
        obj->link->group = group;
        obj->group = group;
    }
}
