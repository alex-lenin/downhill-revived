/* UNIT 0x001906A8 0x00190740 */
typedef struct Entry { short delta,unused; void (*call)(); } Entry;
typedef struct Actor { char pad[0x464]; int x464; char p468[0x4b9-0x468]; unsigned char x4b9; char p4ba[0x4f8-0x4ba]; Entry *vt; } Actor;
typedef struct Node {int x0,x4;} Node;
typedef struct {char pad[0x154]; Node *x154[12]; int x184;} Obj;
extern Node *func_0018D718(int,Actor *,int);
void func_001906A8(Obj *o,Actor *a){int previous;Entry *v;
a->x464=-1;v=a->vt;v[14].call((char *)a+v[14].delta);
previous=0;
if(o->x184>0)previous=o->x154[o->x184-1]->x4;
o->x154[o->x184]=func_0018D718(previous,a,0);
a->x4b9=0;o->x184++;
}
