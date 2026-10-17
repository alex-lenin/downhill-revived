/* UNIT 0x0017F6A0 0x0017F750 */
struct Node { int x0; char pad[12]; struct Node *x10; };
struct Obj { char pad[0x55D0]; int x55D0[92]; int x5740; };
extern int func_00208FB8(void);
extern void func_002182A8(int,int);
extern void func_00218278(int,struct Node *);
void func_0017F6A0(struct Obj *obj,struct Node *node) {
 struct Node *child; int slots, address, address2, value;
 if((node->x0 & 63)==6) {
  child=node->x10;
  if((child->x0 & 63)==1) {
   slots=(int)obj->x55D0;
   value=func_00208FB8();
   address=slots; address+=obj->x5740*4; *(int *)address=value;
   address2=slots; address2+=obj->x5740*4;
   func_002182A8(*(int *)address2,0);
   slots += obj->x5740 * 4;
   func_00218278(*(int *)slots,child);
   obj->x5740++;
  }
 }
}
