/* UNIT 0x0018EF50 0x0018F000 */
struct Child { char pad[0x4F8]; virtual void first(); virtual void second(); };
struct Obj { char pad[0x4B0]; int x4B0; char pad2[0x4C]; Child *x500[12]; int x530,x534; };
extern char D_003F52D0[];
extern void func_001904B8(void *,int);
void func_0018EF50(Obj *obj) {
 int slots, address;
 if(obj->x534 >= obj->x530-1) { if(obj->x4B0) func_001904B8(D_003F52D0,obj->x4B0); }
 else { slots=(int)obj->x500; address=slots; address+=obj->x534*4; (*(Child **)address)->second(); obj->x534++; slots+=obj->x534*4; (*(Child **)slots)->first(); }
}
