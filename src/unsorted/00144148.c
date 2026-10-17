/* UNIT 0x00144148 0x001441C0 */
typedef struct Item { char pad[0xC]; float xC; } Item;
typedef struct Obj { char pad[0x770]; Item *x770[12]; int x7A0; } Obj;
extern Item *func_001591D8(int);
void func_00144148(Obj *p,int arg) {
 if(p->x7A0<7) {
 p->x770[p->x7A0]=func_001591D8(arg);
 p->x770[p->x7A0]->xC=2000.0f;
 p->x7A0++;
 }
}
