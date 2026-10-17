/* UNIT 0x0018F090 0x0018F128 */
class Actor {
public:
char pad[0x4b9]; unsigned char x4b9; char p4ba[0x4f8-0x4ba];
virtual void Begin(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void Update();
};
struct Obj {char pad[0x500]; Actor *x500[13]; int x534;};
void func_0018F090(Obj *o){
if(!o->x500[o->x534]->x4b9){o->x500[o->x534]->Begin();o->x500[o->x534]->x4b9=1;}
o->x500[o->x534]->Update();
}
