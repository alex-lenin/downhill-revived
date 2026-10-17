/* UNIT 0x001B6F80 0x001B70F0 */
typedef struct { char pad[0x317c]; int x317c; } Tail;
typedef struct { char pad[0x7c8c]; Tail tail; } Other;
extern Other D_004B2914;
typedef struct { char pad[0x5350]; int x5350; } Global;
extern Global D_00398C40;
extern void func_0020DF40(void);
extern void func_0021F400(void);
extern void func_00201180(int);
extern void func_001C5A30(void *);
extern int func_001BC290(void *);
extern void func_00254D08(void *);
extern void func_001C71D0(void *);
extern void func_001FA590(void *);
extern void func_00193BB8(void *);
extern void func_0014DD90(void *);
extern void func_0019BD28(void *);
extern void func_0016B5C0(void *);
extern char D_0028EAC0[], D_004C8508[], D_004225A0[], D_00381CF0[], D_0043DCC0[];
extern volatile char D_0029DE7C;
void func_001B6F80(char *base) {
    int *p0,*p1,*p2,*p3,*p4,*p5,*p6;
    int i;
    char *other;
    Tail *tail;
    Other *root;
    Global *g;
    func_0020DF40(); func_0021F400(); func_00201180(1);
    func_001C5A30(base+0x374);
    g=&D_00398C40;
    if(!func_001BC290(&g->x5350)) func_00254D08(D_0028EAC0);
    root=&D_004B2914;
    func_001C71D0(root);
    tail=&root->tail;
    root=(Other *)((char *)root-0x374);
    other=(char *)root;
    *(volatile int *)(other+0x10c)=12;
    *(volatile int *)(other+0x2a4)=0;
    tail->x317c=0;
    D_0029DE7C=0;
    i=0;
    p6=(int *)(base+0x1c0); p5=(int *)(base+0x1bc); *(volatile int *)(other+0x110)=0; p4=(int *)(base+0x1b8);
    p3=(int *)(base+0x254); p2=(int *)(base+0x124); p1=(int *)(base+0x120); p0=(int *)(base+0x11c);

    do { *p0=17; *p1=4; p0+=3; *p2=0; p1+=3; *p3=0; p2+=3; *p4=i; p3++; *p5=0; i++; *p6=0; p5+=3; p6+=3; p4+=3; } while(i<4);
    func_001FA590(D_004C8508); func_00193BB8(D_004225A0); func_0014DD90(D_00381CF0); func_0019BD28(D_0043DCC0); func_0016B5C0(&D_00398C40);
    *(int *)(base+0x368)=32; *(int *)(base+0x360)=0; *(int *)(base+0x364)=0; base[0x350]=0; *(int *)(base+0xb178)=0;
}
