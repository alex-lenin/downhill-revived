/* UNIT 0x001A9EA8 0x001AA0F0 */
struct Rider { char pad0[0x4098]; unsigned long flags; char pad40A0[0x3B30]; };
struct World { char pad0[0x50140]; struct Rider *riders[26]; int count; };
struct Camera { char pad0[0x2B4]; struct Rider *target; };
struct Game { char pad0[0x488]; int x488; };
struct Obj { char pad0[0x30]; int switchTimer; int x34; char pad38[0x98C]; int timeLeft; };
struct State { char pad[0x374]; char x374[1]; };

extern struct World D_002AF2C0;
extern struct Camera D_0037EA60;
extern struct Rider *D_0037ED14[];
extern struct Rider *D_003B3524[];
extern struct Game D_0043EEC0;
extern struct State D_004B25A0;
extern char D_004B1F40[];
extern int D_004B28F8[];
extern int func_00222B08(int, int);
extern void func_001487D8(struct Camera *, struct Rider *);
extern void func_0012C2E8(struct Rider *);
extern void func_0012C2C0(struct Rider *);
extern int func_001A0590(struct Game *);
extern int func_002201B0(int);
extern void func_001C5B80(void *);
extern void func_001B45F8(void *, int, int);
extern void func_001C6628(void *);
extern void func_001A95E8(struct Obj *, int);

void func_001A9EA8(struct Obj *obj) {
    struct Rider *rider, *current;
    int timer, old;

    if ((unsigned)(D_0043EEC0.x488 - 2) < 3) {
        if (obj->timeLeft == 0x1C20) {
            rider = (struct Rider *)((char *)&D_002AF2C0
                + func_00222B08(2, D_002AF2C0.count - 1) * 0x7BD0);
            if (rider) {
                func_001487D8(&D_0037EA60, rider);
                D_003B3524[0] = rider;
                obj->switchTimer = 180;
                func_0012C2E8(rider);
            }
        }
        /* The two identical call blocks are merged by cross-jumping after
           sched1; written once, $a2 is set only once in the function and
           the scheduler moves `$a2 = 1` ahead of `$a1 = 0`. */
        if (--obj->timeLeft <= 0) {
            func_001C5B80(D_004B25A0.x374);
            func_001B45F8(D_004B1F40, 0, 1);
            func_001C6628(D_004B25A0.x374);
        } else if (func_001A0590(&D_0043EEC0) || func_002201B0(0) || func_002201B0(1)) {
            func_001C5B80(D_004B25A0.x374);
            func_001B45F8(D_004B1F40, 0, 1);
            func_001C6628(D_004B25A0.x374);
        } else {
            old = obj->switchTimer;
            if (old <= 0) {
                if (func_00222B08(0, 1)) {
                    if (func_00222B08(0, 4) < 2) {
                        timer = func_00222B08(0, D_004B28F8[0] - 1);
                        rider = D_002AF2C0.riders[timer];
                        current = D_0037EA60.target;
                        if (!rider) goto reset;
                        if ((unsigned char)(rider->flags >> 2) & 1) { timer = 60; goto store; }
                        if ((unsigned char)(current->flags >> 2) & 1) { timer = 60; goto store; }
                        func_0012C2C0(current);
                        func_001487D8(&D_0037EA60, rider);
                        D_003B3524[0] = rider;
                        func_0012C2E8(rider);
                    } else {
                        if (!obj->x34) goto set;
                        if (!D_0037ED14[0]) goto set;
                        if ((unsigned char)(D_0037ED14[0]->flags >> 2) & 1) goto set;
                        func_001A95E8(obj, 0);
                        goto reset;
                    set:
                        obj->x34 = 1;
                    }
                }
            reset:
                timer = 60;
            } else {
                timer = old - 1;
            }
        store:
            obj->switchTimer = timer;
        }
    }
}
