/* UNIT 0x001580B0 0x00158120 */
struct Item {char pad[0x18];float x18;};
struct Obj {struct Item *items[100];float values[100];int count;};
void func_001580B0(struct Obj *p) {
 int i=1;float *v=p->values;struct Item *a,*b;float t;
 for(;i<p->count;i++,v++) {
 b=*(struct Item **)((char *)v-0x18C);a=*(struct Item **)((char *)v-0x190);
 if(a->x18<b->x18) {
 t=v[1];*(struct Item **)((char *)v-0x18C)=a;v[1]=v[0];*(struct Item **)((char *)v-0x190)=b;v[0]=t;
 }
 }
}
