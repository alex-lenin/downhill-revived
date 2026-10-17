/* UNIT 0x0012FA70 0x0012FE38 */
extern int func_00238F48(void);
extern float func_00138580(void *);
extern float D_0029C890;
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define B(p,o) (*(unsigned char *)((char *)(p)+(o)))
#define H(p,o) (*(short *)((char *)(p)+(o)))
static __inline__ int down(void *p,int k) { return B(p,0x13D0+k) ? I(p,0x123C+k*4) : I(p,0x1174+k*4); }
static __inline__ int pressed(void *p, int k) {
    void *prev=(void *)((long)p+0xFE0);
    int v=0;
    if(down(p,k)) {
        if (!(B(prev,0x3F0+k) ? I(prev,0x25C+k*4) : I(prev,0x1F8+k*4))) v=1;
    }
    return v & 0xFF;
}
void func_0012FA70(void *p) {
    int held=0; int edge;
    if (down(p,16) || down(p,15) || down(p,17)) held=1;
    edge=0; if (pressed(p,16) || pressed(p,15) || pressed(p,17)) edge=1;
    if (!I(p,0x7A6C) && !B(p,0x7A6A)) {
        if (edge) {
            if (down(p,8)) I(p,0x7A6C)=-1;
            else if (down(p,7)) I(p,0x7A6C)=1;
        } else if (held) {
            if (pressed(p,8)) I(p,0x7A6C)=-1;
            else if (pressed(p,7)) I(p,0x7A6C)=1;
        }
    }
    if (I(p,0x7A6C)==-1) {
        if (held && down(p,8)) {
            if(!B(p,0x7A6A)) I(p,0x7A70)=func_00238F48();
            B(p,0x7A6A)=1;
        } else B(p,0x7A6A)=0;
    }
    if (I(p,0x7A6C)==1) {
        if (held && down(p,7)) {
            if(!B(p,0x7A6A)) I(p,0x7A70)=func_00238F48();
            B(p,0x7A6A)=1;
        } else B(p,0x7A6A)=0;
    }
    if (!held && !down(p,8) && !down(p,7)) { I(p,0x7A6C)=0; B(p,0x7A6A)=0; }
    if (I(p,0x7A6C)==-1 && !held && down(p,7)) { I(p,0x7A6C)=0; B(p,0x7A6A)=0; }
    if (I(p,0x7A6C)==1 && !held && down(p,8)) { I(p,0x7A6C)=0; B(p,0x7A6A)=0; }
    if (B(p,0x7A6A) && func_00138580((char *)p+0x40C0)<D_0029C890*1.4666667f) B(p,0x7A6A)=0;
}
