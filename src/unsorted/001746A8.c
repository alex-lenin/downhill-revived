/* UNIT 0x001746A8 0x00174730 .sdata=0x29C8F8:0x29C900 */
typedef struct Obj { char pad[0x5C]; int x5C; } Obj;
typedef struct Entry { char pad[0x14]; unsigned short x14; } Entry;
extern Entry *func_00163D40(Obj *, int);
int D_0029C8F8 = -1;
int func_001746A8(Obj *o) {
int count=o->x5C;
D_0029C8F8++;
while (count && !(func_00163D40(o, D_0029C8F8)->x14 & 1)) {
D_0029C8F8++;
if (D_0029C8F8 >= o->x5C) D_0029C8F8=0;
count--;
}
return func_00163D40(o,D_0029C8F8);
}
