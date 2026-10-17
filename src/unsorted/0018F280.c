/* UNIT 0x0018F280 0x0018F2F0 */
typedef unsigned int U128 __attribute__((mode(TI)));
struct Obj { char pad[0x490]; U128 x490; char pad2[12]; int x4AC; char pad3[0x50]; void (*x500)(int); int x504; unsigned char x508,x509; };
extern char D_003F52D0[];
extern void func_001904B8(void *,int);
void func_0018F280(struct Obj *obj,int arg) {
 if(obj->x500) obj->x500(arg);
 if(obj->x4AC && obj->x508) func_001904B8(D_003F52D0,obj->x4AC);
 if(!obj->x509) obj->x490=0;
}
