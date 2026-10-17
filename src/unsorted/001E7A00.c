/* UNIT 0x001E7A00 0x001E7B78 */
typedef struct Obj { char pad[0x796c]; int x796C; } Obj;
int func_001E7A00(Obj **arg0) {
 int result = 0;
 switch ((*arg0)->x796C) {
 case 0x20: result = 0x1D; break;
 case 0x40: result = 0x1E; break;
 case 0x60: result = 0x1F; break;
 case 0x80: result = 0x20; break;
 case 0xA0: result = 0x21; break;
 case 0xC0: result = 0x22; break;
 case 0xE0: result = 0x23; break;
 case 0x100: result = 0x24; break;
 case 0x120: result = 0x25; break;
 case 0x140: result = 0x26; break;
 case 0x160: result = 0x27; break;
 case 0x180: result = 0x28; break;
 case 0x1A0: result = 0x29; break;
 case 0x200: result = 0x2A; break;
 }
 return result;
}
