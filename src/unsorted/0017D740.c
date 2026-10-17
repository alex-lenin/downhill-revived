/* UNIT 0x0017D740 0x0017D7C8 */
typedef struct Obj { int x0; char pad4[0x258]; int x25C[25], x2C0[25], x324[25], x388[25], x3EC; unsigned char x3F0[25], x409[25], x422[25], x43B,x43C,x43D; } Obj;
extern void func_0017DC78(Obj *);
extern void func_0017DCC8(Obj *);
extern void func_0017D7C8(Obj *);
void func_0017D740(Obj *o, int value) {
int count;
char *p;
char *q;
o->x0=value;
func_0017DC78(o);
func_0017DCC8(o);
q=(char *)o->x422;
p=(char *)o->x388;
count=24;
do {
*(int *)(p-0x12C)=0;*(int *)(p-0xC8)=30;*(int *)(p-0x64)=0;*(int *)p=0;
*(q-50)=0;*(q-25)=0;*q=0;
p+=4;q++;
} while (--count >= 0);
func_0017D7C8(o);
o->x43D=1;o->x3EC=0;o->x43B=0;o->x43C=0;
}
