/* UNIT 0x0014F300 0x0014F868 .rodata=0x289DB0:0x289F08 */
struct Obj {
    char pad[0x500];
    int timer;    /* x500 */
    int alpha;    /* x504 */
    int fade;     /* x508 */
    int x50C;
    unsigned int sponsors; /* x510: bit mask */
    unsigned int bikes;    /* x514: bit mask */
};
struct BikeEntry { int name; int pad[2]; };
struct BikeSet { char pad[0x30]; struct BikeEntry entries[7]; int count; char pad2[0x18]; };
struct Other { char pad[0x796C]; int x796C; };
struct Global { char pad[0x50130]; struct Other *x50130; };

extern struct Global D_002AF2C0;
extern char D_003F52D0[];
extern char D_003B2270[];
extern char D_00398C40[];
extern char D_004225A0[];
extern int D_004B25A0[];
extern struct BikeSet D_004C8508[];
extern char D_00277EC0[][16];
extern char D_00276C78[][14];

extern void func_0023D650(int, int *, int *);
extern void func_0017A678(void *, int);
extern int func_00194E30(void *, int);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_001BC4F0(void *);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern void func_0018DC20(struct Obj *);

static inline void set_font(char *base) { func_0017A678(base + 0x150, -2); }
static inline void finish(char *base) { func_001BC4F0(base + 0x5350); }
static inline char *track_info(void) { return D_004225A0 + ((D_002AF2C0.x50130->x796C >> 5) - 1) * 0x694; }
static inline int get_x20(char *base) { return *(int *)(base + 0x20); }
static inline void set_flag(char *base) { base[0x12D] = 1; }
static inline struct BikeSet *bike_set(struct BikeSet *sets, int idx) { return &sets[idx]; }

void func_0014F300(struct Obj *p)
{
    int pos[2];
    int rank;
    int key;
    int y;
    int x;
    int col;
    int i;
    int n;

    func_0023D650(get_x20(D_003F52D0), &pos[0], &pos[1]);
    pos[0] = pos[0] / 2;
    set_font(D_003B2270);
    key = D_004B25A0[0x43];
    rank = func_00194E30(track_info(), key);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, p->alpha);
    func_0020FB80(0, 0x180, 0x120, -0x9B, -0x20);
    if (p->x50C) {
        finish(D_00398C40);
    }
    if (rank == 1) {
        if (p->x50C) {
            func_0020F9F0(0, 0, pos[0], 0x48, "Your rank is upgraded to");
            func_0020F9F0(0, 0, pos[0], 0x54, "Sport Racer");
            func_0020F9F0(0, 0, pos[0], 0x64, "You now earn 1.5X cash for");
            func_0020F9F0(0, 0, pos[0], 0x70, "Race Placement and");
            func_0020F9F0(0, 0, pos[0], 0x7C, "Checkpoint Bonus");
        }
    } else if (rank == 2) {
        if (p->x50C) {
            func_0020F9F0(0, 0, pos[0], 0x48, "Great Job, Your rank is upgraded");
            func_0020F9F0(0, 0, pos[0], 0x54, "to Expert Racer");
            func_0020F9F0(0, 0, pos[0], 0x64, "You now earn 2X cash for");
            func_0020F9F0(0, 0, pos[0], 0x70, "Race Placement and");
            func_0020F9F0(0, 0, pos[0], 0x7C, "Checkpoint Bonus");
        }
    } else if (rank == 3) {
        y = 0x14;
        if (p->x50C) {
            func_0020F9F0(0, 0, pos[0], 0x14, "Right On, Your rank is upgraded");
            y = 0x60;
            func_0020F9F0(0, 0, pos[0], 0x20, "to Pro Racer");
            func_0020F9F0(0, 0, pos[0], 0x34, "You now earn 3X cash for");
            func_0020F9F0(0, 0, pos[0], 0x40, "Race Placement and");
            func_0020F9F0(0, 0, pos[0], 0x4C, "Checkpoint Bonus");
        }
        col = 0;
        x = 0x1E;
        if (p->sponsors) {
            func_0020F9F0(0, 0, pos[0], y, "Pro Sponsors Unlocked");
            y += 0xC;
            func_0020FB80(0, 0x120, 0x100, -0x75, -0x20);
            func_0020F518(0, 0xDD, 0xDB, 0xA, p->alpha);
            for (i = 0; i < 18; i++) {
                if (p->sponsors & (1 << i)) {
                    func_0020F9A8(0, 0, x, y, D_00277EC0[i]);
                    x += 0x8C;
                    if (++col == 3) {
                        y += 10;
                        x = 0x1E;
                    }
                }
            }
        }
        if (p->bikes) {
            n = bike_set(D_004C8508, D_004B25A0[0x43])->count;
            func_0020F518(0, 0x6E, 0x6E, 0xC8, p->alpha);
            func_0020FB80(0, 0x180, 0x120, -0x9B, -0x20);
            y += 0x10;
            col = 0;
            x = 0x1E;
            func_0020F9F0(0, 0, pos[0], y, "Pro Bikes Unlocked");
            y += 0xC;
            func_0020FB80(0, 0x120, 0x100, -0x75, -0x20);
            func_0020F518(0, 0xDD, 0xDB, 0xA, p->alpha);
            for (i = 0; i < n; i++) {
                if (p->bikes & (1 << i)) {
                    func_0020F9A8(0, 0, x, y, D_00276C78[bike_set(D_004C8508, D_004B25A0[0x43])->entries[i].name]);
                    x += 0x8C;
                    if (++col == 3) {
                        y += 10;
                        x = 0x1E;
                    }
                }
            }
        }
    }
    p->timer--;
    p->alpha += p->fade;
    if (p->alpha > 0xAA) {
        p->alpha = 0xAA;
    } else if (p->alpha < 0) {
        p->alpha = 0;
    }
    if (p->timer < 0x23) {
        p->fade = -5;
    }
    if (p->timer <= 0) {
        set_flag(D_003F52D0);
    }
    func_0018DC20(p);
}
