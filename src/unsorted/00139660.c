/* UNIT 0x00139660 0x001396B8 */
struct Global { char pad0[0x10c]; int x10c; char pad110[0x178]; int x288; };
struct Obj { char pad0[0x31700]; int x31700; int x31704; char pad31708[0x58]; int x31760; };
struct Other { char pad0[0x7a5c]; int x7a5c; };
extern struct Global D_004B25A0;
void func_00139660(struct Obj *arg0, struct Other *arg1) {
    int state;
    if (D_004B25A0.x10c == 0x12 && arg0->x31760 < 0x18) {
        state = arg1->x7a5c;
        if (state == 1 && D_004B25A0.x288 == state) {
            arg0->x31700 = 0;
            arg0->x31704 = 0;
        }
    }
}

