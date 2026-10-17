/* UNIT 0x00195420 0x00195480 */
typedef struct Entry { int x0,x4,x8; } Entry;
typedef struct Obj { Entry entries[99]; int x4A4; } Obj;
int func_00195420(Obj *o, Entry *key) {
 int i;
 for(i=0;i<o->x4A4;i++) {
 Entry *p=&o->entries[i];
 if(key->x4==p->x4 && key->x0==p->x0 && key->x8==p->x8) return 1;
 }
 return 0;
}
