/* UNIT 0x001A0590 0x001A05E0 */
struct Global { char pad[0x501a8]; int count; };
extern struct Global D_002AF2C0;
struct Obj { char pad[0x58]; unsigned char flags[1]; };
int func_001A0590(struct Obj *o) {
 int i; int count=D_002AF2C0.count;
 for(i=0;i<count;i++) { if(o->flags[i]) return 1; }
 return 0;
}
