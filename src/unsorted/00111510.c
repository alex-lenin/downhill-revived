/* UNIT 0x00111510 0x001118A8 */
struct Limit { char pad[0x18]; float x18; };
struct Ratio { float x0,x4,x8; };
struct Rider {
    char pad0[0x11EC]; int x11EC;
    char pad11F0[0x60]; int x1250;
    char pad1254[0x181]; unsigned char x13D5;
    char pad13D6[0x1ABE]; struct Ratio x2E94;
    char pad2EA0[0x202C]; void *x4ECC;
    char pad4ED0[0x2A5C]; struct Limit *x792C;
};
struct Obj {
    struct Rider *x0; char pad4[0x24C]; float x250;
    char pad254[0x38]; float x28C;
    char pad290[0xA8]; float x338;
    char pad33C[0x48]; int x384;
    char pad388[0x80]; int x408;
    char pad40C[0x24]; int x430;
    char pad434[8]; int x43C;
    long x440;
};
extern int D_0039E7F4[],D_0043EEC8[];
extern int func_001118A8(struct Obj *);
extern int func_00111A98(struct Obj *);
extern float func_001385A8(void *);
#define ACTIVE (1L<<35)
#define LATCH (1L<<34)
void func_00111510(struct Obj *obj)
{
    int time,threshold,status;
    struct Rider *rider;
    float ratio;
    struct Ratio *ratioObj;
    if(D_0039E7F4[0]==0 && obj->x430<1800) obj->x440 &= ~ACTIVE;
    else {
        time=obj->x430;
        if(time<300) {
            if(time<obj->x384) obj->x440 &= ~ACTIVE;
            else {
                if(time<(int)obj->x440*15) obj->x440 &= ~ACTIVE;
                else obj->x440 |= ACTIVE;
            }
        } else if(!(obj->x440 & (1L<<40))) {
            if(func_001118A8(obj)) {
                struct Rider *owner=obj->x0;
                if(owner->x4ECC==(char *)owner+0x5E90) obj->x440 &= ~ACTIVE;
                else if(func_00111A98(obj)>=6 && obj->x0->x792C->x18<obj->x28C) obj->x440 &= ~ACTIVE;
                else if(func_00111A98(obj)>=4 && obj->x0->x792C->x18<obj->x28C && obj->x408>=-299) obj->x440 &= ~ACTIVE;
                else obj->x440 |= ACTIVE;
            } else {
                obj->x440 &= ~ACTIVE;
                if(obj->x28C<obj->x0->x792C->x18) {
                    threshold=240;
                    if(obj->x43C) threshold=240-D_0043EEC8[0]*240/obj->x43C;
                    if(obj->x408<threshold) obj->x440 |= ACTIVE;
                }
            }
        }
    }
    if(obj->x440 & ACTIVE) {
        obj->x440 |= LATCH;
        rider=obj->x0;
        ratioObj=&rider->x2E94;
        ratio=ratioObj->x4/ratioObj->x8;
        status=rider->x13D5 ? rider->x1250 : rider->x11EC;
        if(status) {
            if(ratio<0.05f) obj->x440 &= ~ACTIVE;
        } else {
            if(ratio<obj->x338 || obj->x250>0.17453292f) obj->x440 &= ~ACTIVE;
        }
    } else if((obj->x440 & LATCH) && func_001385A8((char *)obj->x0+0x40C0)<0.15f) obj->x440 |= ACTIVE;
}
