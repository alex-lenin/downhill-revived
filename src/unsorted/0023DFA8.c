/* UNIT 0x0023DFA8 0x0023DFF8 */
typedef struct Obj { int x0; int x4; int x8,xC,x10; } Obj;
int func_0023DFA8(volatile Obj *obj,int arg1,char *arg2,int count) {
 int offset;
 obj->xC=0; obj->x0=arg1; obj->x4=(int)arg2; obj->x10=count; obj->x8=0;
 if(count>0) { offset=0; do { count--; *(int *)(offset+obj->x4)=0; offset+=0x85740; } while(count!=0); }
 return 1;
}
