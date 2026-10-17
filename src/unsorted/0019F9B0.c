/* UNIT 0x0019F9B0 0x0019FE28 */
#define F(p, t, o) (*(t *)((char *)(p) + (o)))

typedef struct {
    char pad0[0x44];
    int x44;
    int x48;
    char pad4C[0x58 - 0x4C];
    unsigned char x58[0x14];
    unsigned char x6C;
} Obj;

struct State { char pad[0x10c]; int x10c; int x110; char pad2[0x174]; int x288; };
typedef struct { char pad[0x18]; } Entry;

extern struct State D_004B25A0;
extern unsigned char D_003985B0[];
extern char D_00398C40[];
typedef struct { char pad[0x150]; char x150[0x1380 - 0x150]; } Rider;
extern Rider D_003B2270[];
extern char D_004B1F40[];
extern char D_004C3740[];
extern char D_004225A0[];
extern char D_0038DB60[];
extern unsigned char D_0043FA34[];
extern Entry D_00381CF0[];

extern void func_001E5328(void *);
extern int func_00168F70(void *);
extern void func_00169808(void *);
extern void func_001B45F8(void *, int, int);
extern void func_001E1D50(void *);
extern void func_00193E48(void *);
extern void func_00194898(void *);
extern void func_0014E460(Entry *);
extern void func_001C17B8(void *, int, int);
extern void func_00175658(void *, int);
extern void func_001614D8(void *, int);

static __inline__ struct State *GS(void) { return &D_004B25A0; }
static __inline__ void set7A68(char *ctx, int v) { F(ctx, unsigned char, 0x7A68) = v; }
static __inline__ void riderN(Rider *base, unsigned int i) { if (i < 4) func_00175658((char *)base + (i * 0x1380 + 0x150), 0xAA); }
static __inline__ void notify(void *p, int value, int flag) { func_001C17B8((char *)p + 0x5b74, value, flag); }
static __inline__ void rider(Rider *r) { func_00175658(r->x150, 0xAA); }

void func_0019F9B0(Obj *obj, char *ctx, int idx)
{
    int mode = GS()->x288;
    int time = obj->x6C ? 15 : 8;
    int count;

    if (obj->x58[idx] != 0) {
        return;
    }
    obj->x58[idx] = 1;
    set7A68(ctx, 1);
    obj->x48++;
    if ((count = F(ctx, int, 0x7A5C)) != 1) {
        return;
    }
    obj->x44++;
    if (GS()->x10c != 5) {
        func_001E5328(ctx + 0x6610);
    }
    if (mode == count) {
        if (GS()->x10c == 5) {
            func_00168F70(D_003985B0);
            func_00168F70(D_003985B0);
            if (D_003985B0[0x556] != 0) {
                return;
            }
            func_001E5328(ctx + 0x6610);
            func_00169808(D_003985B0);
            func_001B45F8(D_004B1F40, 1, time * 60);
            D_003985B0[0x556] = mode;
            return;
        }
        if (GS()->x10c == 0x12) {
            func_001B45F8(D_004B1F40, 1, time * 60);
            func_001E1D50(D_004C3740);
        } else {
            func_001B45F8(D_004B1F40, 1, time * 60);
            if (!D_0043FA34[0] && GS()->x110 == mode) {
                func_00193E48(D_004225A0);
                func_0014E460(&D_00381CF0[D_004B25A0.x10c]);
            } else if (GS()->x10c == 13 || GS()->x10c == 12) {
                notify(D_00398C40, F(ctx, int, 0x798C), 0);
            }
        }
        rider(&D_003B2270[0]);
        return;
    } else if (mode == 2) {
        if (GS()->x10c == 13) {
            func_001B45F8(D_004B1F40, 1, time * 60);
        } else {
            if (GS()->x10c != 16) {
                riderN(D_003B2270, F(ctx, unsigned int, 0x7988));
            }
            if (obj->x44 >= mode) {
                if (GS()->x10c == 16) {
                    rider(&D_003B2270[0]);
                    rider(&D_003B2270[1]);
                }
                func_001B45F8(D_004B1F40, 1, time * 60);
                if (GS()->x110 == 1) {
                    func_00194898(D_004225A0);
                    func_0014E460(&D_00381CF0[GS()->x10c]);
                }
            }
        }
        if (GS()->x10c == 16) {
            func_001614D8(D_0038DB60, F(ctx, int, 0x7978));
        }
        if (GS()->x10c == 13 || GS()->x10c == 12 || GS()->x10c == 16 || GS()->x10c == 4 ||
            GS()->x10c == 6 || GS()->x10c == 7 || GS()->x10c == 8 || GS()->x10c == 9) {
            notify(D_00398C40, F(ctx, int, 0x798C), 0);
        }
    } else if (mode >= 3) {
        if (GS()->x10c == 13) {
            func_001B45F8(D_004B1F40, 1, time * 60);
        } else if (obj->x44 >= mode) {
            func_001B45F8(D_004B1F40, 1, time * 60);
        }
        if (GS()->x10c == 13 || GS()->x10c == 12 || GS()->x10c == 4 ||
            GS()->x10c == 6 || GS()->x10c == 7 || GS()->x10c == 8 || GS()->x10c == 9) {
            notify(D_00398C40, F(ctx, int, 0x798C), 0);
        }
    }
}
