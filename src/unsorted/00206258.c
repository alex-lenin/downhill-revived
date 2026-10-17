/* UNIT 0x00206258 0x00206320 */
typedef struct { char p[0x10]; unsigned short x10; } Link;
typedef struct Node { int x0; Link *x4; char p[0x10]; struct Node *x18,*x1C; char q[56]; } Node;
typedef struct { int x0; Node *x4; int x8; unsigned short xC; } Arg;
typedef struct { Node nodes[128]; unsigned char free[128]; unsigned char count; } Pool;
extern Pool D_004CD480; extern void func_00203CB8(Arg*);
void func_00206258(Node *o,int a) { Arg arg; int count; Pool *pool; if(o&&a) { switch(o->x0&63) { case 33: arg.x0=a; arg.x8=0; arg.x4=o; arg.xC=o->x4->x10; func_00203CB8(&arg); break; case 34: func_00206258(o->x18,a); func_00206258(o->x1C,a); pool=&D_004CD480; count=pool->count-1; pool->count=count; pool->free[(unsigned char)count]=o-pool->nodes; break; } } }
