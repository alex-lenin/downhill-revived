/* UNIT 0x0023AFA8 0x0023AFF8 */
typedef struct Entry { signed char x0,x1,x2,x3; float x4; } Entry;
typedef struct Obj { char pad[0x665]; signed char count; char pad2[0xB70-0x666]; Entry entries[6]; } Obj;
signed char func_0023AFA8(Obj *obj, int a, int b, int c, int d, float value) {
 Entry *entry; unsigned char count; int result;
 if (obj->count < 6) {
 entry = &obj->entries[obj->count];
 entry->x0 = a; entry->x1 = c; entry->x2 = b; entry->x3 = d; entry->x4 = value;
 count = obj->count; obj->count=count+1; result = (signed char)count;
 } else result = -1;
 return result;
}
