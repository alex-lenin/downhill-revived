/* UNIT 0x00110C98 0x00110D38 */
typedef struct Obj { char *x0; } Obj;
extern void func_001E5300(void *,int **,int *,int *,int *);
extern int func_001109B0();
int func_00110C98(Obj *obj) {
 struct { int *x0; int x4,x8,xC; } values;
 int *p, count, sum, result, divisor;
 func_001E5300(obj->x0+0x6610,&values.x0,&values.x4,&values.x8,&values.xC);
 p=values.x0; divisor=28; sum=0; count=29;
 do { sum+=*p++; count--; } while(count);
 result=func_001109B0(obj,sum/divisor+1);
 if(!result) result=func_001109B0(obj,99999);
 return result;
}
