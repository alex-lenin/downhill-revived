/* UNIT 0x00193068 0x001930C8 */
typedef struct Bits { unsigned int x0:7; unsigned int x7:11; unsigned int x18:14; } Bits;
typedef struct Obj { char p0[4]; Bits **x4; char p8[0x240]; unsigned int x248; int x24C; } Obj;
extern int D_0029E4A0;
extern Obj D_003F5690[];
void func_00193068(void) {
 Obj *p; int n;
 if(D_0029E4A0>0) {
 p=D_003F5690; n=D_0029E4A0;
 do { if(p->x4) { Bits *v=*p->x4; v->x7=p->x248; } n--; p++; } while(n!=0);
 }
}
