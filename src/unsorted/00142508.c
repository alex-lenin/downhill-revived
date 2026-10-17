/* UNIT 0x00142508 0x001425B0 .rodata=0x289C00:0x289C20 */
struct Obj { char pad[0x50]; int state; int x54; int timer; char pad5c[0x94]; int active; };
extern void func_00142940(void *);
extern void func_00142A28(void *);
extern void func_00142D50(void *);
extern void func_001425E0(void *);
void func_00142508(struct Obj *obj) {
 if (!obj->active) return;
 if (obj->timer) obj->timer--;
 switch(obj->state) {
 default: case 0: break;
 case 1: if (!obj->timer) obj->state=2; break;
 case 2: func_00142940(obj); break;
 case 3: case 4: case 5:
 func_00142A28(obj); func_00142D50(obj); func_001425E0(obj); break;
 }
}
