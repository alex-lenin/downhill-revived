/* UNIT 0x001109B0 0x00110BF8 */
struct Obj { char *x0; char pad4[0x2bc-4]; float x2BC; char pad2c0[0x2dc-0x2c0]; int x2DC[10]; int x304[10]; char pad32c[0x344-0x32c]; float x344; char pad348[12]; int x354[10]; char pad37c[0x428-0x37c]; int x428; };
struct State { char pad0[0x5bb4]; int x5BB4; char pad5bb8[0x24]; int x5BDC; };
extern struct State D_00398C40;
extern void func_001E5300(void *,int **,int **,int **,int **);
extern int func_00110BF8(struct Obj *);
extern int func_00110D38(struct Obj *,int);
extern int func_00222BD8(int,int);
static inline void clamp_count(int *value,int lower,int upper) {
 if(*value<=lower) *value=lower;
 if(*value>upper-1) *value=upper;
}
int func_001109B0(struct Obj *obj,int limit) {
 int *values,*p4,*p8,*pC;
 int *weights;
 int *weight;
 int result=0;
 int found,count,amount,choice;
 int n;
 int *first,*second;
 func_001E5300(obj->x0+0x6610,&values,&p4,&p8,&pC);
 if(obj->x344<obj->x2BC) {
 first=obj->x2DC; second=obj->x304; weights=obj->x354; found=0;
 if(D_00398C40.x5BB4==0) amount=D_00398C40.x5BDC/2;
 else if(D_00398C40.x5BB4==2) amount=D_00398C40.x5BDC/2+50;
 else amount=D_00398C40.x5BDC/2+25;
 n=amount/10;
 clamp_count(&n,1,10);
 count=0;
 if(n>0) {
loop:
 /* Retain the loop-entry alignment emitted for a do-once scope. */
 do { } while(0);
 weight=(int *)((obj->x428<<2)+(unsigned int)weights);
 if(func_00222BD8(0,100)<*weight) {
 choice=first[obj->x428];
 if(choice==0) {found=1;result=func_00110BF8(obj);}
 else if((func_00110D38(obj,choice) && (choice=first[obj->x428],values[choice]<limit)) || (func_00110D38(obj,second[obj->x428]) && (choice=second[obj->x428],values[choice]<limit))) {
 result=choice;found=1;
 }
 }
 obj->x428++;
 if(obj->x428>=10) obj->x428=0;
 ++count;
 if(count<n) { if(!found) goto loop; }
 }
 }
 return result;
}
