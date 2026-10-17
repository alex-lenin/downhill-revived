/* UNIT 0x0017DCC8 0x0017DD70 */
typedef struct { int words[25]; } Block;
typedef struct { char pad[0x194]; Block x194; Block x1f8; } Obj;
void func_0017DCC8(Obj *obj) { obj->x1f8 = obj->x194; }
