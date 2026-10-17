/* UNIT 0x0010AF20 0x0010AFB0 */
struct State { char pad[0x384]; int x384; };
struct Rider { char pad[0x78d4]; struct State *x78D4; };
struct Sub { char pad[0xb0]; int xB0; };
struct Obj { struct Rider *x0; char pad[0x2d0]; struct Sub sub; char pad2[0xb8]; int x440; };
struct Global { char pad[0x50140]; struct Rider *riders[26]; int count; };
extern struct Global D_002AF2C0;
extern int D_004B2828[];

void func_0010AF20(struct Obj *obj) {
    struct Global *global;
    struct Rider **riders;
    struct Rider *rider;
    struct Sub *sub;
    int total, count;

    obj->x440 = 1;
    global = &D_002AF2C0;
    total = global->count;
    count = total - D_004B2828[0];
    if (count > 0) {
        sub = &obj->sub;
        riders = global->riders;
        do {
            rider = *riders;
            if (rider != 0 && obj->x0 != rider && rider->x78D4->x384 < sub->xB0) {
                obj->x440++;
            }
            count--;
            riders++;
        } while (count != 0);
    }
}
