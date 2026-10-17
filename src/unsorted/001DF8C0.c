/* UNIT 0x001DF8C0 0x001DF920 */
struct Obj { char pad[0x276c]; unsigned short x276C; };
int func_001DF8C0(struct Obj *obj) {
 switch(obj->x276C) {
 case 20: case 46: return 1;
 case 19: case 48: return 2;
 default: return 0;
 }
}
