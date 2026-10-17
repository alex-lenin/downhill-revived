/* UNIT 0x00195480 0x00195520 */
typedef struct Entry { int x0; short x4,x6; char pad8[3]; char xB; char padC[12]; } Entry;
typedef struct Obj { Entry entries[3]; long x48; char pad50[8]; int x58; Entry list[5]; } Obj;
void func_00195480(Obj *obj) {
 int p; int mask=0x0FFFFFFF; int invalid=-1; int end=(int)obj+0x48;
 p=(int)obj;
 do {
  Obj *e=(Obj *)p;
  e->list[0].x0=mask; e->list[0].x4=invalid; e->list[0].x6=invalid; e->list[0].xB=0;
  p+=0x18;
 } while((int)p<(int)end);
 obj->x58=0;
 obj->list[3].x0=0x0FFFFFFF;
 obj->list[3].x4=-1; obj->list[3].x6=-1; obj->list[3].xB=0;
 obj->list[4].x0=0x0FFFFFFF;
 obj->list[4].x4=-1; obj->list[4].x6=-1; obj->list[4].xB=0;
 obj->entries[0].x0=0; obj->entries[0].x4=-1; obj->entries[0].x6=-1; obj->entries[0].xB=0;
 obj->entries[1].x0=0; obj->entries[1].x4=-1; obj->entries[1].x6=-1; obj->entries[1].xB=0;
 obj->entries[2].x0=0; obj->entries[2].x4=-1; obj->entries[2].xB=0;
 *((long *)((char *)obj+0x48))=0;
 obj->entries[2].x6=-1;
}
