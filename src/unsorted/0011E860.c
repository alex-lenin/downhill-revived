/* UNIT 0x0011E860 0x0011E9F0 */
typedef struct { int x0, x4, x8, xC; } Entry;
typedef struct { char pad[0x3C]; Entry entries[7]; } Obj;
extern void func_00203CB8(Entry *);
void func_0011E860(Obj *obj) {
 Entry temp;
 if (obj->entries[0].x0) { temp = obj->entries[0]; func_00203CB8(&temp); }
 if (obj->entries[1].x0) { temp = obj->entries[1]; func_00203CB8(&temp); }
 if (obj->entries[2].x0) { temp = obj->entries[2]; func_00203CB8(&temp); }
 if (obj->entries[3].x0) { temp = obj->entries[3]; func_00203CB8(&temp); }
 if (obj->entries[4].x0) { temp = obj->entries[4]; func_00203CB8(&temp); }
 if (obj->entries[5].x0) { temp = obj->entries[5]; func_00203CB8(&temp); }
 if (obj->entries[6].x0) { temp = obj->entries[6]; func_00203CB8(&temp); }
}
