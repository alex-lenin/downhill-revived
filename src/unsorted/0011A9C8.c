/* UNIT 0x0011A9C8 0x0011AD00 .rodata=0x288B10:0x288B70 */
extern int D_0029DD68,D_0029DD6C;
extern char D_0036B338[];
extern struct { char pad[0x288]; int x288; int x28C; int x290[1]; } D_004B25A0;
extern void func_001404B8(void *,void *),func_001B65E8(void *,void *),func_001FACD8(void *,void *),func_0012DE00(void *,void *),func_00218278(void *,void *),func_0012BEF0(void *,void *),func_0012C110(void *,void *);
extern int func_00139BF0(void);
extern void *func_00209108(void),*func_00208FB8(void);
#define FIELD(p,n) (*(int *)((char *)(p)+(n)))
void func_0011A9C8(char *base,unsigned int *node) {
    char *obj=base+D_0029DD68*0x7BD0;
    unsigned int flags,kind;
    int subtype,group;
    func_001404B8(D_0036B338,node);
    flags=*node; kind=flags&0xFFFC0000;
    if(kind==0x780000) { FIELD(obj,0x7944)=(int)node; FIELD(obj,0x78E0)=(int)node; }
    else if(kind==0x7C0000) { FIELD(obj,0x7948)=(int)node; FIELD(obj,0x78E4)=(int)node; }
    else if(kind==0xE140000) FIELD(obj,0x78D8)=(int)node;
    else if(kind==0xE180000) FIELD(obj,0x78DC)=(int)node;
    else if(kind==0xE1C0000) { FIELD(obj,0x7AFC)=D_0029DD6C; FIELD(obj,0x78E8)=(int)node; }
    else if(kind==0xE200000) { FIELD(obj,0x7AF8)=D_0029DD6C; FIELD(obj,0x78EC)=(int)node; }
    else if(kind==0xE240000) FIELD(obj,0x78F0)=(int)node;
    else {
        kind=flags>>18;
        if(kind>=900) {
            void *item;
            FIELD(obj,0x797C)=D_0029DD68; FIELD(obj,0x7AF4)=D_0029DD6C;
            if(D_0029DD68<D_004B25A0.x288 && D_004B25A0.x290[D_0029DD68]==0x200) item=func_00209108();
            else item=func_00208FB8();
            func_00218278(item,node); func_0012BEF0(obj,item); FIELD(obj,0x7970)=*node>>18;
        } else {
            subtype=kind&31; group=kind-subtype;
            if((unsigned int)(subtype-2)<22) switch(subtype) {
            case 8: if(!func_00139BF0()) func_001B65E8(base+D_0029DD68*0x7BD0,node); break;
            case 2: func_001FACD8(base+D_0029DD68*0x7BD0+0x78D8,node); break;
            case 16: {
                void *item;
                char *p;
                if(func_00139BF0()) break;
                if((*node&0xFFFC0000)==0x8400000) item=func_00209108(); else item=func_00208FB8();
                func_00218278(item,node); p=base+D_0029DD68*0x7BD0;
                FIELD(p,0x796C)=group; func_0012C110(p,item);
                FIELD(p,0x3FA4)=D_0029DD6C; FIELD(p,0x7974)=0; FIELD(base,0x501A8)++; break;
            }
            case 10: { char *p=base+D_0029DD68*0x7BD0; if(p) FIELD(p,0x4774)=(int)node; break; }
            case 9: { char *p=base+D_0029DD68*0x7BD0; if(p) FIELD(p,0x4778)=(int)node; break; }
            case 22: case 23: { char *p=base+D_0029DD68*0x7BD0; if(p) func_0012DE00(p,node); break; }
            default: break;
            }
        }
    }
}
