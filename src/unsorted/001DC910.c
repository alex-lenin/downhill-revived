/* UNIT 0x001DC910 0x001DC988 */
typedef struct Node { int x0; char pad[0x18]; float x1C; struct Node *x20; } Node;
typedef struct Link { char pad[0x1C]; Node *x1C; } Link;
typedef struct { char pad[0x34]; Link *x34; float x38,x3C; int x40,x44,x48,x4C,x50,x54; } Obj;
void func_001DC910(Obj *o,int a) { Node *n; o->x44=a; if(o->x34) { __asm__ volatile("sqc2 $vf0,0(%0)
	sqc2 $vf0,0x10(%0)
	sqc2 $vf0,0x20(%0)"::"r"(o):"memory"); o->x38=500.f; n=o->x34->x1C; if((n->x0&63)==1) { n=n->x20; if(!(n->x0&63)) o->x3C=n->x1C; } o->x48=0; o->x4C=0; o->x54=0; } }
