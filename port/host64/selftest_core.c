/* Synthetic module only: no game code or data. */
typedef unsigned U;
#define IMPORT(n) __attribute__((import_module("env"),import_name(n)))
IMPORT("yield") extern void yield(U id);
IMPORT("probe") extern void probe(U id,U address,U depth);
IMPORT("host_fp_mode") extern void fp_mode(U mode);
#define EXPORT(n) __attribute__((export_name(n)))
static volatile U result[3];
static U plus(U x) { return x+17; }
static U times(U x) { return x*3; }
static U (*volatile callbacks[2])(U)={plus,times};
EXPORT("table_call") U table_call(U i,U x) { return callbacks[i](x); }
EXPORT("out_of_bounds") U out_of_bounds(U address,U store) {
    volatile U *p=(U *)address;
    if (store) { *p=123; return 0; }
    return *p;
}
EXPORT("memory_test") U memory_test(void) {
    volatile U *a=(U *)0x08000100,*b=(U *)0x28000100,*c=(U *)0x38000100;
    *a=11; if (*b!=11 || *c!=11) return 1;
    *b=22; if (*a!=22 || *c!=22) return 2;
    *c=33; if (*a!=33 || *b!=33) return 3;
    /* Pointer VALUES survive translation, including uncached bits. */
    *a=(U)b; if (*c!=0x28000100u) return 5;
    *a=33;
    /* Unaligned scalar crossing either edge of either alias window. */
    volatile unsigned char *edge=(unsigned char *)0x29ffffff;
    edge[0]=0x12; edge[1]=0x34;
    if (*(volatile unsigned short *)edge!=0x3412) return 6;
    edge=(unsigned char *)0x37ffffff;
    edge[0]=0x56; edge[1]=0x78;
    if (*(volatile unsigned short *)edge!=0x7856) return 7;
    /* Bulk operations: folding must respect the complete byte interval. */
    __builtin_memset((void *)0x39fffff0,0x5a,32);
    if (*(volatile U *)0x09fffff0!=0x5a5a5a5au ||
        *(volatile U *)0x3a000000!=0x5a5a5a5au ||
        *(volatile U *)0x0a000000!=0) return 8;
    __builtin_memcpy((void *)0x28000200,(void *)0x38000100,4);
    if (*(volatile U *)0x08000200!=33) return 9;
    /* Disjoint aliases in address space, overlapping canonical storage. */
    __builtin_memset((void *)0x08000300,0x45,64);
    __builtin_memmove((void *)0x28000304,(void *)0x38000300,60);
    if (*(volatile U *)0x0800033c!=0x45454545u) return 10;
    for (U i=0;i<32;i++) ((volatile unsigned char *)0x29fffff0)[i]=(unsigned char)i;
    /* First half of source aliases the second half of destination. */
    __builtin_memmove((void *)0x09ffffe0,(void *)0x29fffff0,32);
    for (U i=0;i<32;i++)
        if (((volatile unsigned char *)0x09ffffe0)[i]!=(unsigned char)i) return 11;
    volatile U *s=(U *)0x70003ffc,*v=(U *)0x1100fffc;
    volatile U *r=(U *)0x1000fffc,*g=(U *)0x12001ffc;
    *s=44; *v=55; *r=66; *g=77;
    return *s==44 && *v==55 && *r==66 && *g==77 ? 0 : 4;
}
__attribute__((noinline)) static U deep(U id,U depth) {
    volatile U words[32];
    for (U i=0;i<32;i++) words[i]=id*100000+depth*32+i;
    probe(id,(U)&words[0],depth);
    U sum;
    if (depth) sum=deep(id,depth-1);
    else { yield((id+1)%3); sum=0; }
    for (U i=0;i<32;i++) sum+=words[i];
    return sum;
}
EXPORT("thread") void thread(U id) {
    U sum=0;
    fp_mode(id==1?0:1);
    for (U r=0;r<20;r++) sum+=deep(id,180);
    result[id]=sum;
    yield(id==2?3:id+1);
}
EXPORT("result") U get_result(U id) { return result[id]; }
EXPORT("float_test") U float_test(U mode,U operation,U abits,U bbits) {
    union { U u; float f; } a={abits},b={bbits},c;
    fp_mode(mode);
    c.f=operation ? a.f*b.f : a.f/b.f;
    return c.u;
}
