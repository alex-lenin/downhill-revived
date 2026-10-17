/* UNIT 0x001C2EF8 0x001C3080 */
typedef struct Point {float x0,x4,x8,xC,x10;} Point;
typedef struct Obj {char pad[0xD0]; int xD0,xD4; char pD8[12]; int *xE4; int xE8,xEC,xF0,xF4; Point *xF8;} Obj;
typedef struct Camera {char pad[0x10]; float pos[3]; char p1C[0x24]; float matrix[16];} Camera;
extern void func_0023D650(int,int*,int*);
extern Camera *func_0023D040(int);
extern void func_001C2AE8(Obj*);
extern void func_00221F50(float*,float*);
extern void func_0023D1F0(float*,int);
int func_001C2EF8(Obj *obj) {
 float rot[4]; int width,height; int result; Camera *cam; Point *point;
 func_0023D650(0,&width,&height);
 result=0;
 cam=func_0023D040(0);
 func_001C2AE8(obj);
 point=(Point *)(obj->xD4*20+(int)obj->xF8);
 rot[0]=point->x10*0.017453292f;
 rot[2]=point->xC*0.017453292f; rot[1]=0; rot[3]=0;
 cam->pos[0]=point->x0;
 cam->pos[1]=obj->xF8[obj->xD4].x4;
 cam->pos[2]=obj->xF8[obj->xD4].x8;
 func_00221F50(cam->matrix,rot);
 func_0023D1F0(cam->pos,0);
 obj->xD0--;
 *obj->xE4+=obj->xE8;
 if(*obj->xE4<0)*obj->xE4=0;
 if(*obj->xE4>128)*obj->xE4=128;
 if(obj->xD0<32)obj->xE8=4;
 if(obj->xD0<0){obj->xD0=120; obj->xD4++; if(obj->xD4>=8){result=1;obj->xF0=1;} obj->xE8=-8;}
 return result;
}
