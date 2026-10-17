/* UNIT 0x001589A8 0x001591D8 .sdata=0x29C6D0:0x29C6D8 */
#include "object_registry.h"
/* Dispatch on (flags>>18): scene-graph callback from func_00209618
   (arg0 = stack Node**, arg1 = pos, arg2 = matrix). */
typedef struct Node {
    unsigned int x0;
    char pad4[2];
    unsigned short x6;
    char pad8[0x18];
    void *x20;
} Node;

/* Bitfield view for the 0x1E6F case: (int)flags&7 must emit
   lhu/dsll32/dsra32/andi (same idiom as func_0021B500). */
typedef struct Hdr6 {
    unsigned long pad : 48;
    unsigned long flags : 16;
} Hdr6;

typedef struct Global {
    char pad0[0x10C];
    int x10C;
} Global;

/* $gp-accessed outside delay slots, so defined here; retail range is
   8 bytes (second word padding). */
void *D_0029C6D0 = 0;
int pad_0029C6D4 = 0;

extern int D_0029DD68;
extern int D_0029DD70;
extern void *D_0029E150;
extern void *D_0029E158;
extern char D_002AF2C0[];
extern char D_0032FC60[];
extern char D_0036B870[];
extern char D_00372650[];
extern char D_00373630[];
extern char D_00380770[];
extern char D_0038DB60[];
extern void *D_0039E838[];
extern char D_003B2270[];
extern char D_003B71D0[];
extern char D_003C6370[];
extern char D_003C6930[];
extern char D_003F52D0[];
extern char D_004348F0[];
extern char D_0043EEC0[];
extern Global D_004B25A0;
extern int D_004B2828[];
extern char D_004BD740[];
extern char D_004BF950[];
extern char D_004C5C50[];

extern void func_0023D060(void *);
extern void *func_001824A8(void *);
extern void func_0023D0B0(void *, int);
extern void func_001DCD60(void *, void *);
extern void func_00210250(void *);
extern void func_0018FF80(void *, void *);
extern void func_0011A9C8(void *, void *);
extern void func_001AC350(void *, void *);
extern void func_00143C50(void *, void *);
extern void func_00163708(void *, void *);
extern void func_001DFF98(void *, int);
extern void func_00182990(void *, void *, int);
extern void func_00192F68(void *);
extern void func_001962D0(void *, int, void *);
extern void *func_002105B8(void);
extern void func_002105E0(void *, void *, void *);
extern void func_001EAC98(void *, void *, void *);
extern void func_00227F10(int, void *);
extern void func_001E50E8(void *);
extern void func_00147018(void *, void *, void *, void *);
extern void func_001DC2D8(void *, void *, void *);
extern void func_0017C9B8(void *, void *, void *, void *);
extern void *func_00147AA0(void);
extern void func_00147B60(void *, void *, void *, void *);
extern void func_001B1150(void *, int, void *);
extern void func_00147688(void *, void *, void *, void *);
extern void func_0017D4C8(void *);
extern void *func_001AA238(void *, void *, void *, void *, int);

extern void *func_0020B600(void);
extern int func_002046F8(int, void *);
extern void func_00150DF8(void *, void *);
extern void func_00151FA8(void *, void *, void *);
extern void func_00138998(void *, void *);
extern void func_001FB560(void *, void *);
extern void func_001FB508(void *, void *);
extern void func_00151DF0(void *, void *, void *);
extern void func_00153AA0(void *);
extern void func_00147128(void *, void *);
extern void func_0019FEA8(void *, void *);
extern void func_0019FF58(void *, void *);
extern void func_00175A78(void *, void *);
extern void func_0010A208(void *);
extern void func_001BA080(void *, int, void *);
extern void func_0013B0A8(void *);
extern void func_001C2800(void *, void *);
extern void func_00161418(void *, int);
extern void func_001835E0(void *, void *);
extern void func_00150DD0(void *, void *);
extern void func_00150E08(void *, void *);
extern void func_00150E00(void *, void *);

void func_001589A8(Node **arg0, void *arg1, void *arg2)
{
    Node *node;
    int flags;
    int id;

    node = arg0[0];
    flags = node->x0;
    id = (unsigned)flags >> 18;
    if (id < 0x20) {
        if (id == 1) {
            func_0023D060(node);
            D_0039E838[0] = func_001824A8(node);
        } else if (id == 2) {
            func_0023D0B0(node, 0);
            if (D_004B2828[0] == 2) {
                func_0023D0B0(node, 1);
            } else if (D_004B2828[0] == 3) {
                func_0023D0B0(node, 1);
                func_0023D0B0(node, 2);
            } else if (D_004B2828[0] == 4) {
                func_0023D0B0(node, 1);
                func_0023D0B0(node, 2);
                func_0023D0B0(node, 3);
            }
        } else if (id == 4) {
            func_001DCD60(arg1, arg2);
        } else if (id == 0x12) {
            func_00210250(node);
        } else if (id == 0x13) {
            node->x0 = (flags & -0x40) | 9;
        } else if (id == 0x14) {
            func_0018FF80(D_003F52D0, node);
        } else if (id == 0x1D) {
            func_0011A9C8(D_002AF2C0, node);
        } else if (id == 0x1E) {
            func_0011A9C8(D_002AF2C0, node);
        } else if (id == 0x1F) {
            func_0011A9C8(D_002AF2C0, node);
        }
    } else if (id < 0x3E9) {
        func_0011A9C8(D_002AF2C0, node);
        func_0013B0A8(node);
    } else if (id < 0x4B0) {
        {
            int dd;
            int prod;
            char *p;

            dd = D_0029DD68;
            if (dd >= 0) {
                prod = dd * 0x7BD0;
                p = D_002AF2C0 + prod;
                p += 0x1E40;
                func_001AC350(p, node);
            }
        }
    } else if (id < 0x5DC) {
        if ((unsigned)(id - 0x4D8) < 0xBU) {
            func_00143C50(D_0036B870, node);
        } else {
            func_00163708(&D_0029DD70, node);
        }
    } else if (id < 0x63C) {
    } else if (id < 0x76C) {
    } else if ((unsigned)(id - 0x820) < 0x191U) {
    } else if ((unsigned)(id - 0xAF0) < 0xC8U) {
        func_001DFF98(arg1, id < 0xB54);
    } else if ((unsigned)(id - 0xC1C) < 3U) {
        func_00182990(D_003C6370, node, id);
    } else if ((unsigned)(id - 0xFA0) < 0x1F4U) {
        func_00192F68(node);
    } else if ((unsigned)(id - 0x1388) < 0x65U) {
        func_001962D0(D_004348F0, id - 0x1389, node);
    } else if ((unsigned)(id - 0x1450) < 0x64U) {
        func_002105E0(func_002105B8(), node, arg1);
    } else if ((unsigned)(id - 0x1770) < 3U) {
        func_001EAC98(D_004C5C50, node, arg1);
    } else if ((unsigned)(id - 0x1B58) < 0x65U) {
        func_00227F10(id - 0x1B58, node);
    } else if (id == 0x1C20) {
        func_001E50E8(node);
    } else if ((unsigned)(id - 0x1CE8) < 0x64U) {
        func_00147018(D_00372650, node, arg1, arg2);
    } else if ((unsigned)(id - 0x1D4C) < 2U) {
        func_001DC2D8(D_004BF950, node, arg1);
    } else if (id == 0x1D51) {
        func_0017C9B8(D_003B71D0, node, arg0[1], arg1);
    } else if (id == 0x1DB1) {
        D_0029C6D0 = func_00147AA0();
    } else if (id == 0x1DB2) {
        func_00147B60(D_0029C6D0, node, arg1, arg2);
    } else if ((unsigned)(id - 0x2EE0) < 0x1F4U) {
        {
            int dd;
            int prod;
            char *p;

            dd = D_0029DD68;
            prod = dd * 0x7BD0;
            p = D_002AF2C0 + prod;
            p += 0x3AF0;
            func_001B1150(p, (node->x0 >> 18) - 0x2EE0, node);
        }
    } else if ((unsigned)(id - 0x1E14) < 0x32U) {
        if (id == 0x1E44) {
            func_00147688(D_00373630, node, arg1, arg2);
        } else {
            func_0017D4C8(node);
        }
    } else if ((unsigned)(id - 0x1E46) < 0x29U) {
        {
            unsigned int *p;
            unsigned int t;
            void *q;
            void *r;
            int v;

            p = (unsigned int *)arg0[1];
            t = node->x0;
            if ((t & 0x3F) == 1) {
                q = *(void **)((char *)node + 0x20);
                if (*(short *)((char *)q + 8) == (t & 0x3F)) {
                    r = func_001AA238(p, q, arg1, arg2, t >> 18);
                    v = RegisterObjectRetail((void *)(r), (unsigned int)(p));
                    *p = (*p & 0xFFFC007F) | ((v & 0x7FF) << 7);
                }
            }
        }
    } else if (id == 0x1E6F) {
        {
            Node *b;
            char *p;
            void *tbl;
            int off;

            off = ((int)((Hdr6 *)node)->flags & 7) - 1;
            b = (Node *)((char *)node + off * 16);
            p = (char *)b + 0x60;
            tbl = func_0020B600();
            *(short *)(p + 2) = func_002046F8(0x3C, *(void **)(*(char **)((char *)tbl + 0x20) + 0xC));
            *(short *)((char *)b + 0x60) = func_002046F8(0x3D, *(void **)(*(char **)((char *)tbl + 0x20) + 0xC));
            *(long *)node |= (long)0x8000 << 28;
        }
    } else if (id == 0x1E70) {
        func_00150DF8(D_00380770, node);
    } else if (id == 0x1E71) {
        func_00151FA8(node, arg2, arg1);
    } else if (id == 0x1E72) {
        func_00138998(D_0032FC60, node);
    } else if (id == 0x1E73) {
        func_001FB560(&D_0029E158, node);
    } else if (id == 0x1E74) {
        func_001FB508(&D_0029E150, node);
    } else if (id == 0x1E75) {
        func_00151DF0(node, arg2, arg1);
    } else if (id == 0x1E76) {
        func_00153AA0(node);
    } else if (id == 0x1E77) {
        func_00147128(D_00372650, node);
    } else if ((unsigned)(id - 0x1F40) < 0x1F4U) {
        if ((unsigned)(id - 0x1F72) < 2U) {
            func_0019FEA8(D_0043EEC0, node);
        } else if ((unsigned)(id - 0x1F7C) < 9U) {
            func_0019FF58(D_0043EEC0, node);
        } else {
            func_00175A78(D_003B2270, node);
        }
    } else if (id < 0x2328) {
        if ((unsigned)(id - 0x2260) < 0xC8U) {
            func_0010A208(node);
        }
    } else if ((unsigned)(id - 0x2328) < 0x3E8U) {
        if (D_004B25A0.x10C != 0x14) {
            func_001BA080(&D_004B25A0, id - 0x2328, node);
            func_0013B0A8(node);
        }
    } else if (id == 0x32FA) {
        func_001C2800(D_004BD740, node);
    } else if ((unsigned)(id - 0x332C) < 0xC8U) {
        func_00161418(D_0038DB60, id);
    } else if ((unsigned)(id - 0x33F4) < 0x33U) {
        func_001835E0(D_003C6930, node);
    } else if (id == 0x343A) {
        func_00150DD0(D_00380770, node);
    } else if (id == 0x343B) {
        func_00150E08(D_00380770, node);
    } else if (id == 0x343C) {
        func_00150E00(D_00380770, node);
    }
}
