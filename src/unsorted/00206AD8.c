/* UNIT 0x00206AD8 0x00206C38 */
struct Count { unsigned short x0,x2; };
struct Node { unsigned int flags; char pad[0x24]; unsigned char x28; char pad2[0x27]; struct Node *next; };
struct Data { int x0; float *values; void *bits; char pad[0x10]; int flags; struct Count *count; };
struct Obj { int x0; struct Data *data; struct Node *node; float *values; struct Node *list; };
typedef struct { unsigned long value; } __attribute__((packed)) Bits;
extern void func_00204C40(struct Node *,struct Data *);
static __inline__ void sync_values(struct Obj *p,float *dst) {
 struct Data *d; char *bits; float *src; int blocks,count; long mask;

 d=p->data; src=d->values; blocks=(d->count->x2>>6)+1; bits=(char *)d->bits;
 if(blocks) do {
 mask=((Bits *)bits)->value;
 if(blocks<2) count=p->data->count->x2&63; else count=64;
 while(count) {
 if((mask&1L)==1L) *dst=*src; else *src=*dst;
 count--; mask=(unsigned long)mask>>1; src++; dst++;
 }
 bits+=8;
 } while(--blocks);
}
void func_00206AD8(struct Obj *p) {
 struct Data *d=p->data; int flags=d->flags; struct Node *node;
 if(!(flags&2)) {
 { struct Node *n=p->node;
 if(n && !(flags&4) && ((n->flags&0x3f)!=0x21 || n->x28)) func_00204C40(n,d); }
 if(p->list) {
 { float *v=p->values; if(v) sync_values(p,v); }
 if(!(p->data->flags&8)) {
 node=p->list;
 if(node) do { func_00204C40(node,p->data); node=node->next; } while(node);
 }
 }
 }
}
