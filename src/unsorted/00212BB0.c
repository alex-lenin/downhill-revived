/* UNIT 0x00212BB0 0x00212D90 */
typedef struct Item { long flags; char rest[16]; } Item;
typedef struct Node { int x0; Item *x4; } Node;
typedef struct Iter { char pad[16]; Item *x10; Node *x14; } Iter;
typedef struct Obj { char pad0[16]; char x10[16]; float x20,x24,x28; char pad2c[20]; int x40,x44; char pad48[8]; Iter x50; int pad68; int x6c; void *x70; Item *x74; int x78; char x7c[16]; } Obj;
extern void func_00211DF0(Iter*,int,int),func_00211EC8(Iter*),func_00212748(Item*,void*,void*),func_00215A18(Item*,int);
extern int func_002145F8(Obj*,void*,float*,float*,void*,void*,float),func_00215140(Item*,void*,int),func_00215448(void*,int);

int func_00212BB0(Obj *p)
{
    Item *entry;
    Item *actor;
    Iter *it;
    char a[16], b[16];
    float first, second, third, fourth;
    struct { unsigned char v; } empty, done;
    int res, flag, isDirect;
    {
        Iter *tmp=&p->x50;
        empty.v=0;
        if(tmp->x10==0) empty.v=(tmp->x14==0);
        if(empty.v & 0xFF) func_00211DF0(tmp,p->x40,p->x44);
        else func_00211EC8(tmp);
    }
    it=&p->x50;
    goto check;
    while(1) {
    check:
        done.v=0;
        if(it->x10==0) done.v=(it->x14==0);
        if(done.v & 0xFF) break;
        isDirect=(it->x10!=0);
        if(isDirect) {
            p->x74=0;
            p->x70=p->x7c;
            first=p->x24;
            second=p->x28;
            entry=it->x10;
            if(!entry) entry=it->x14->x4;
            res=func_002145F8(p,p->x10,&first,&second,entry,(char*)entry+12,p->x20);
            if(res && func_00215140(entry,&p->x78,(p->x6c>>17)&1)) return 1;
        } else {
            actor=it->x10 ? it->x10 : it->x14->x4;
            third=p->x24;
            fourth=p->x28;
            func_00212748(actor,a,b);
            if(func_002145F8(p,p->x10,&third,&fourth,a,b,p->x20)) {
                if(actor->flags & 0x400000000000000UL) {
                    p->x74=actor;
                    p->x70=0;
                } else {
                    p->x74=0;
                    p->x70=p->x7c;
                }
                {
                    int flags=p->x6c;
                    int b18=(flags>>18)&1;
                    int b17=(flags>>17)&1;
                    func_00215A18(actor,b18);
                    if(func_00215448(&p->x78,b17)) return 1;
                }
            }
        }
        func_00211EC8(it);
    }
    return 0;
}
