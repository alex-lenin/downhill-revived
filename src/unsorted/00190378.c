/* UNIT 0x00190378 0x00190410 */
struct Owner { char pad[0x4AC]; void *x4AC; };
struct Entry { int x0; struct Owner *x4; };
struct State { char pad[0x154]; struct Entry *entries[12]; int x184,x188; };
extern void func_0018D7B0(void *);
extern void func_0018E4C0(struct Owner *,int);
void func_00190378(struct State *self) {
 struct Entry **p=self->entries; int count=11;
 do {
  if(*p) {
   func_0018D7B0(*p);
   if((*p)->x4) {
    if((*p)->x4->x4AC) {
     func_0018D7B0((*p)->x4->x4AC);
     func_0018E4C0((*p)->x4,0);
    }
   }
   *p=0;
  }
  --count; ++p;
 } while(count>=0);
 self->x184=0; self->x188=0;
}
