/* UNIT 0x001816B0 0x00181710 */
typedef struct Item { void *x0; char pad[0x21c]; } Item;
typedef struct State { char p0[8]; int x8; char pC[12]; int x18; char p1C[12]; int x28; char p2C[4]; Item items[40]; } State;
extern State D_003B7E00;
extern char D_00285CE8[];
void func_001816B0(int a, int b) {
 int i; Item *p;
 if(b==65535 && a) {
 D_003B7E00.x8=0; D_003B7E00.x18=0; D_003B7E00.x28=0;
 p=D_003B7E00.items;
 for(i=39;i!=-1;i--,p++) p->x0=D_00285CE8;
 }
}
