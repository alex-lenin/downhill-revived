/* UNIT 0x00175A78 0x00176090 .rodata=0x28B950:0x28BBD8 */
typedef struct Msg {
    unsigned int x0;
    unsigned char x4;
    char pad5[3];
    unsigned short x8;
    char padA[0x16];
    struct Tag *x20;
    struct Tag *x24;
} Msg;

typedef struct Tag {
    unsigned int x0;
    char pad4[0x20];
    struct Tag *x24;
} Tag;

typedef struct Node {
    struct Msg *x0;
    unsigned char x4;
    char pad5[3];
} Node;

typedef struct Player {
    char pad0[0xF60];
    void *xF60;
    void *xF64;
    char padF68[0xF70 - 0xF68];
    void *xF70;
    char padF74[0xF78 - 0xF74];
    void *xF78;
    void *xF7C;
    void *xF80;
    char padF84[0xF8C - 0xF84];
    void *xF8C;
    void *xF90;
    void *xF94;
    void *xF98;
    void *xF9C;
    void *xFA0;
    char padFA4[0xFA8 - 0xFA4];
    void *xFA8;
    void *xFAC;
    void *xFB0;
    char padFB4[0xFC0 - 0xFB4];
    void *xFC0;
    char padFC4[0x1168 - 0xFC4];
    void *x1168;
    void *x116C;
    void *x1170;
    char pad1174[0x1178 - 0x1174];
    void *x1178;
    char pad117C[0x1180 - 0x117C];
    void *x1180;
    char pad1184[0x1188 - 0x1184];
    void *x1188;
    char pad118C[0x126C - 0x118C];
    void *x126C;
    void *x1270;
    void *x1274;
    char pad1278[0x1380 - 0x1278];
} Player;

typedef struct Obj {
    char pad0[0x150];
    Player players[4];
} Obj;

typedef struct Config {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
    char pad28C[0x2A0 - 0x28C];
    int x2A0;
    char pad2A4[0x358 - 0x2A4];
    int x358;
} Config;

typedef struct Entry {
    char pad0[0x770C];
    int x770C;
} Entry;

extern int D_004B2828[];
extern Config D_004B25A0;
extern int D_004C3778[];
extern char D_004225A0[];

extern Node *func_00209108(void);
extern void func_00218278(Node *, Msg *);
extern void func_00176468(Player *, Msg *);
extern void func_0017BA78(Player *, Msg *);
extern void func_0017BB90(Player *, Msg *);
extern void func_0017BC20(Player *, Msg *);
extern void func_0017BCB0(Player *, Msg *);
extern void func_0017BD60(Player *, Msg *);
extern void func_00176090(Player *, Msg *);
extern void func_00176138(Player *, Msg *);
extern void func_001761D8(Player *, Msg *);
extern void func_00218498(void *, int);

void func_00175A78(Obj *obj, Msg *msg)
{
    static const void *table[161] __attribute__((section(".rdata"), aligned(8))) = {
        &&case_1F41, &&case_1F42, &&case_1F43, &&case_1F44, &&case_1F45, &&end,
        &&end, &&end, &&case_1F49, &&case_1F4A, &&case_1F4B, &&case_1F4C,
        &&case_1F4D, &&case_1F4E, &&case_1F4F, &&case_1F50, &&case_1F51, &&case_1F52,
        &&case_1F53, &&case_1F54, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&case_1F5D, &&case_1F5E,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&case_1FA4, &&case_1FA5, &&case_1FA6,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&case_1FAE, &&case_1FAF, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&case_1FBA, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&case_1FC4,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&case_1FCE, &&end, &&end,
        &&end, &&end, &&end, &&end, &&end, &&case_1FD6,
        &&end, &&end, &&end, &&end, &&end, &&end,
        &&end, &&end, &&end, &&end, &&case_1FE1
    };
    unsigned int index;
    Node *p;
    Tag *tagp;
    Tag *first;
    int tag;
    int count;
    unsigned int sel;

    index = msg->x0 >> 7;
    index &= 0x7FF;
    sel = (msg->x0 >> 18) - 0x1F41;
    if (sel >= 161)
        goto end;
    goto *table[sel];

    case_1F41:
        if (D_004B2828[0] < 3) {
            p = func_00209108();
            func_00218278(p, msg);
            p->x4 = 1;
            if (index < 4)
                obj->players[index].x1168 = p;
        }
        goto end;
    case_1F43:
        if (D_004B2828[0] < 3)
            goto end;
        if (D_004B2828[0] == 3 && index == D_004B2828[0])
            goto end;
        p = func_00209108();
        func_00218278(p, msg);
        p->x4 = 1;
        obj->players[index].x1188 = p;
        goto end;
    case_1F44:
        if (D_004B2828[0] >= 3)
            func_00176468(&obj->players[index], msg);
        goto end;
    case_1F45:
        if (D_004B2828[0] >= 3)
            func_0017BD60(&obj->players[index], msg);
        goto end;
    case_1F42:
        if (D_004B2828[0] < 3)
            func_00176468(&obj->players[index], msg);
        goto end;
    case_1F49:
        func_0017BA78(&obj->players[index], msg);
        goto end;
    case_1F4A:
        func_0017BB90(&obj->players[index], msg);
        goto end;
    case_1F4B:
        func_0017BC20(&obj->players[index], msg);
        goto end;
    case_1F4C:
        obj->players[index].xF9C = msg;
        goto end;
    case_1F4D:
        obj->players[index].xFAC = msg;
        func_0017BCB0(&obj->players[index], msg);
        goto end;
    case_1F4E:
        if (D_004B2828[0] < 3)
            func_0017BD60(&obj->players[index], msg);
        goto end;
    case_1F4F:
        if (D_004B25A0.x288 < 3) {
            first = msg->x20;
            tag = first->x0 & 0x3F;
            if (tag == 1) {
                obj->players[index].xF60 = first->x24;
            } else {
                tagp = first;
                if (tag == 6) {
                    obj->players[index].xF60 = tagp;
                    if (msg->x8 >= 2) {
                        tagp = msg->x24;
                        if ((tagp->x0 & 0x3F) == tag) {
                            count = D_004B25A0.x288 + D_004B25A0.x358;
                            if (D_004B25A0.x10C == 18 && D_004C3778[0] != 2) {
                                if (D_004C3778[0] == 1)
                                    count = 2;
                                else
                                    count = (((Entry *)(D_004225A0 + D_004B25A0.x2A0 * 216))->x770C == 0x0FFFFFFF) ? 1 : 2;
                            }
                            func_00218498(tagp, count - 1);
                        }
                    }
                }
            }
        }
        goto end;
    case_1F50: {
        unsigned int off;
        off = index;
        off *= 0x1380;
        *(void **)((char *)obj + off + 0x10B4) = msg;
        p = func_00209108();
        p->x4 = 1;
        if (index < 4) {
            off = index;
            off *= 0x1380;
            *(Node **)((char *)obj + off + 0x12BC) = p;
        }
        off = index;
        off *= 0x1380;
        *(void **)((char *)obj + off + 0x10F8) = msg;
        goto end;
    }
    case_1F51: {
        unsigned int off;
        if (D_004B2828[0] < 3) {
            off = index;
            off *= 0x1380;
            *(void **)((char *)obj + off + 0x10C0) = msg;
            p = func_00209108();
            p->x0 = msg;
            p->x4 = 1;
            off = index;
            off *= 0x1380;
            *(Node **)((char *)obj + off + 0x12C0) = p;
        }
        goto end;
    }
    case_1F54:
        obj->players[index].x126C = msg;
        goto end;
    case_1F52:
        obj->players[index].x1270 = msg;
        goto end;
    case_1F53:
        obj->players[index].x1274 = msg;
        goto end;
    case_1FAE: {
        if (D_004B2828[0] < 3) {
            unsigned int off;
            off = index;
            off *= 0x1380;
            *(void **)((char *)obj + off + 0x10C8) = msg;
            p = func_00209108();
            p->x0 = msg;
            p->x4 = 1;
            off = index;
            off *= 0x1380;
            *(Node **)((char *)obj + off + 0x12C8) = p;
            off = index;
            off *= 0x1380;
            *(void **)((char *)obj + off + 0x1100) = msg;
        }
        goto end;
    }
    case_1FAF:
        if (D_004B2828[0] < 3) {
            if (index == 0) {
                Player *pl = obj->players;
                pl->xF7C = msg;
                p = func_00209108();
                p->x0 = msg;
                p->x4 = 1;
                pl->x1180 = p;
            } else if (index == 1) {
                obj->players[0].xF80 = msg;
            }
        }
        goto end;
    case_1FBA:
        if (D_004B2828[0] == 1)
            obj->players[0].xF8C = msg;
        goto end;
    case_1FC4:
        if (D_004B2828[0] >= 2)
            obj->players[0].xF8C = msg;
        goto end;
    case_1FCE:
        if (D_004B2828[0] >= 2)
            obj->players[0].xF90 = msg;
        goto end;
    case_1FA4:
        func_00176090(&obj->players[index], msg);
        goto end;
    case_1FA5:
        func_00176138(&obj->players[index], msg);
        if (D_004B2828[0] >= 2)
            func_00176138(&obj->players[1], msg);
        goto end;
    case_1FA6:
        func_001761D8(&obj->players[index], msg);
        if (D_004B2828[0] >= 2)
            func_001761D8(&obj->players[1], msg);
        goto end;
    case_1F5D:
        if ((msg->x0 & 0x3F) == 1) {
            obj->players[0].xF94 = msg;
            obj->players[1].xF94 = msg;
        }
        goto end;
    case_1F5E:
        if ((msg->x0 & 0x3F) == 1)
            obj->players[index].xF98 = msg;
        goto end;
    case_1FD6:
        obj->players[index].xFC0 = msg;
        goto end;
    case_1FE1:
        obj->players[index].xFA0 = msg;
        goto end;
end:;
}
