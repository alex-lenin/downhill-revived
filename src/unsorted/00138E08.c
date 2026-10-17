/* UNIT 0x00138E08 0x00138F70 .sdata=0x29C3C4:0x29C3C8 */
typedef struct Buffer { int x0; int end; } Buffer;
typedef struct Obj { char pad[0x2bf10]; int x2bf10; char pad2bf14[0x316e0-0x2bf14]; Buffer *x316e0; Buffer *x316e4; int x316e8; int x316ec; int x316f0; int x316f4; int x316f8; char pad316fc[0x31714-0x316fc]; int x31714; char pad31718[0x31760-0x31718]; int x31760; } Obj;
typedef struct Other { char pad[0x7a5c]; int x7a5c; } Other;
typedef struct Global { char pad[0x10c]; int x10c; char pad110[0x288-0x110]; int x288; } Global;
extern Global D_004B25A0;
extern int D_0029C3B8;
int D_0029C3C4 = 8;
extern int func_00238F60(void);
extern void func_00138F70(Obj *);
extern void func_00139308(Obj *);
extern void func_00138868(Obj *,float);
void func_00138E08(Obj *obj,Other *other) {
    if (D_004B25A0.x10c == 18) {
        if (obj->x31760 < 24) {
            int state = other->x7a5c;
            if (state == 1 && D_004B25A0.x288 == state) {
                int index = obj->x316f0;
                Buffer *buffer = obj->x316e0;
                if ((int)((char *)buffer + (index*4+32)) < buffer->end &&
                    (!obj->x316e4 || (int)((char *)obj->x316e4 + (obj->x316f8*4+32)) < obj->x316e4->end)) {
                    int count = obj->x2bf10;
                    if (index < count && obj->x316f8 < count && obj->x31714 && D_0029C3B8) {
                        obj->x316ec += func_00238F60();
                        if (obj->x316ec >= D_0029C3C4) {
                            obj->x316ec = 0;
                            func_00138F70(obj);
                            if (obj->x316e4) {
                                func_00139308(obj);
                                goto update;
                            }
                        } else {
update:
                            if (obj->x316e4) func_00138868(obj,(float)obj->x316ec/(float)D_0029C3C4);
                        }
                    }
                }
            }
        }
    }
}
