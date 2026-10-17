/* UNIT 0x00230728 0x002308A0 */
typedef struct { int x0,x4; int x8,xC,x10,x14,x18,x1c,x20,x24,x28,x2c,x30,x34,x38; } Packet;
extern void func_0023D5C0(void *,void *,void *,void *);
extern void func_0023D840(void *,void *,void *);
extern void func_0020EC78(void *,void *,void *,void *,void *,void *,void *);
extern void func_00230190(int,void *,void *,void *,void *,void *);
int func_00230728(void *unused, Packet *base) {
    int offset,i=0;
    Packet *p,*pB,*pC,*end;
    int *p1,*p2,*p3,*p4,*p5;
    base->x4=24; base->x0=2;
    func_0023D5C0(&base->x8,&base->xC,&base->x10,&base->x14);
    offset=base->x4; p=(Packet *)((char *)base+offset);
    p->x4=24; p->x0=3;
    func_0023D840(&p->x8,&p->xC,&p->x10);
    offset+=p->x4; pB=(Packet *)((char *)base+offset);
    pB->x0=4; pB->x4=64;
    func_0020EC78(&pB->x8,&pB->xC,&pB->x10,&pB->x14,&pB->x18,&pB->x1c,&pB->x20);
    offset+=pB->x4; pC=(Packet *)((char *)base+offset);
    pC->x0=5; pC->x4=132;
    p5=&pC->x38; p4=&pC->x2c; p3=&pC->x20; p2=&pC->x14; p1=&pC->x8;
    do { func_00230190(i,p1,p2,p3,p4,p5); i++; p5++; p4++; p3++; p2++; p1++; } while(i<3);
    offset+=pC->x4; end=(Packet *)((char *)base+offset);
    end->x0=7; end->x4=0xa14;
    return offset+0xa14;
}
