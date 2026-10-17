/* UNIT 0x00158058 0x001580B0 */
extern void func_00254204(void *, int, int);
typedef struct { int x0[100]; int x190[100]; int x320; } Obj;
void func_00158058(Obj *obj) {
 int count; int *ptr;
 func_00254204(obj,0,0x324);
 ptr=obj->x0; count=99;
 do { ptr[0]=0; count--; ptr[100]=0; ptr++; } while(count>=0);
 obj->x320=0;
}
