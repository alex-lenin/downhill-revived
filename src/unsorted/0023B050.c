/* UNIT 0x0023B050 0x0023B0B0 .sdata=0x29DC9C:0x29DCA0 */
int D_0029DC9C = 0;
typedef struct Entry { short x0,x2; int x4; } Entry;
typedef struct Obj { char pad[0x668]; signed char count; char pad2[0xBAC-0x669]; Entry entries[50]; } Obj;
int func_0023B050(Obj *obj) {
 Entry *entry;
 if(obj->count >= 50) return -1;
 entry = &obj->entries[obj->count];
 entry->x0 = -1; entry->x4 = 0; entry->x2 = 0;
 if(D_0029DC9C < obj->count) D_0029DC9C = obj->count;
 return obj->count++;
}
