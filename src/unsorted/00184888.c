/* UNIT 0x00184888 0x001848E0 */
typedef struct { int x0,x4; short x8; char xa,xb; int xc,x10,x14,x18,x1c; char x20; char pad[0xc4-0x21]; char items[20]; } Obj;
void func_00184888(Obj *o,int a){ int i; char *p;
o->x1c=-1;o->x4=3;p=(char *)o+0xd0;o->x18=a;i=0;
o->x10=0;o->x20=0;o->x0=0;o->xa=0;o->xc=0;
do {p[-12]=0;i--;*(int *)(p-8)=0;*(int *)(p-4)=0;*(int *)p=0;p+=20;}while(i>=0);
}
