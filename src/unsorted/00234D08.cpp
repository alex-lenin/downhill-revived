/* UNIT 0x00234D08 0x00234EC8 */
struct Item { int index; unsigned int time; };
struct Table { char pad[12]; unsigned char **names; };
extern "C" int func_002352E8(void *,Item *);
class Obj {
 void *heap;Item **lists[32];int counts[32];char pad[0x1d8-0x104];Table *table;char pad2[0x5fc-0x1dc];
public:
 virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual unsigned int Value();
 int func_00234D08(int,int);
};
int Obj::func_00234D08(int kind,int index) {
 int i=0,j;unsigned int time=Value();Item **list=lists[kind];
 for(i=0;i<counts[kind];i++) {
  if(list[i]->time<time) {
   func_002352E8(heap,list[i]);
   for(j=i+1;j<counts[kind];j++) list[j-1]=list[j];
   list[counts[kind]-1]=0;counts[kind]--;return 1;
  }
 }
 if(table->names[index][0]>=table->names[list[0]->index][0]) {
  func_002352E8(heap,list[0]);
  for(j=1;j<counts[kind];j++) list[j-1]=list[j];
  list[counts[kind]-1]=0;counts[kind]--;return 1;
 }
 return 0;
}
