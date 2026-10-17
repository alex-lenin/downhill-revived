/* UNIT 0x002116E8 0x00211780 */
typedef struct Item { char p0[0x1c]; float x1c; char p20[0x10]; } Item;
typedef struct Group { char p0[0x20]; Item *items; int count; } Group;
Item *func_002116E8(Group *groups,int count,Item *after) {
Item *best=0; Item *p; int n;
while(count) {
n=groups->count; count--; p=groups->items;
while(n) {
if((!best || best->x1c > p->x1c) && (!after || after->x1c < p->x1c || (after->x1c==p->x1c && (unsigned int)after<(unsigned int)p))) best=p;
n--;p++;
}
groups++;
}
return best;
}
