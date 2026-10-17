/* UNIT 0x00115D78 0x00115F20 */
typedef struct Rider { char pad0[0x796c]; int x796C; } Rider;
typedef struct Obj { char pad0[0x58]; Rider *x58; char pad5c[0x54]; int xB0; char padb4[0x18]; int xCC; } Obj;
void func_00115D78(Obj *obj) {
    int key;
    obj->xB0=0;
    key=obj->x58->x796C;
    if(key==0x100) goto cat100;
    if(key<0x101) {
        if(key==0x80) goto cat80;
        if(key<0x81) {
            if(key==0x40) goto cat40;
            if(key<0x41) {
                if(key==0x20) goto cat20;
                obj->xB0=36; goto done;
            } else {
                if(key==0x60) goto cat60;
                obj->xB0=36; goto done;
            }
        } else {
            if(key==0xc0) goto catc0;
            if(key<0xc1) {
                if(key==0xa0) goto cata0;
                obj->xB0=36; goto done;
            } else {
                if(key==0xe0) goto cate0;
                obj->xB0=36; goto done;
            }
        }
    } else {
        if(key==0x180) goto cat180;
        if(key<0x181) {
            if(key==0x140) goto cat140;
            if(key<0x141) {
                if(key==0x120) goto cat120;
                obj->xB0=36; goto done;
            } else {
                if(key==0x160) goto cat160;
                obj->xB0=36; goto done;
            }
        } else {
            if(key==0x1c0) goto catdefault;
            if(key>=0x1c1) goto catdefault;
            if(key==0x1a0) goto cat1a0;
            obj->xB0=36; goto done;
        }
    }
cata0: obj->xB0=16; goto done;
catc0: obj->xB0=4; goto done;
cat40: obj->xB0=3; goto done;
cat60: obj->xB0=10; goto done;
cat80: obj->xB0=6; goto done;
cat20: obj->xB0=9; goto done;
cat120: obj->xB0=12; goto done;
cat160: obj->xB0=18; goto done;
cate0: obj->xB0=1; goto done;
cat100: obj->xB0=3; goto done;
cat140: obj->xB0=1; goto done;
cat180: obj->xB0=3; goto done;
cat1a0: obj->xB0=4; goto done;
catdefault: obj->xB0=36;
done:
    if(((obj->xCC>>11)&1) && obj->xB0>=2) obj->xB0=1;
}
