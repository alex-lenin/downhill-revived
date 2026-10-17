/* UNIT 0x00192990 0x00192A50 */
typedef struct Item { unsigned long flags; char pad8[4]; float xC; char pad10[0xAC]; int xBC; } Item;
typedef struct Obj { int x0; Item *x4; Item *x8; char padC[0x228]; struct { char pad[0x14]; } x234; int x248; } Obj;
extern Item *func_00208FB8(void);
extern void func_00218278(Item *, unsigned int *);
extern void func_001BCC50(void *, int, Obj *);
extern void func_002182A8(Item *, int);
void func_00192990(Obj *obj, unsigned int *data) {
 obj->x248=(*data >> 7)&0x7FF;
 obj->x4=func_00208FB8();
 func_00218278(obj->x4,data);
 func_001BCC50(&obj->x234,8,obj);
 obj->x8=func_00208FB8();
 obj->x8->flags &= 0xFDFFFFFFFFFFFFFFUL;
 obj->x4->xBC=2;
 func_002182A8(obj->x8,0);
 obj->x4->xC=300.0f;
}
