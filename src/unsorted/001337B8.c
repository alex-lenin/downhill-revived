/* UNIT 0x001337B8 0x00133940 */
typedef struct Obj { char pad0[0x110]; float x110,x114,x118,x11C,x120,x124; int x128,x12C,x130; char pad134[12]; float x140[4],x150[4],x160[4],x170[4],x180[4],x190[4],x1A0[4],x1B0[4]; } Obj;
static inline void set3(float *p,float x,float y,float z) { p[0]=0.0f; p[1]=x; p[2]=y; p[3]=z; }
static inline void set2(float *p,float x,float y,float z) { p[0]=x; p[1]=y; p[2]=z; }
void func_001337B8(Obj *obj) {
 obj->x110=100.0f; obj->x114=0.2f; obj->x118=0.28f; obj->x11C=0.987f; obj->x120=0.975f; obj->x124=3.0f;
 obj->x128=180; obj->x12C=4; obj->x130=4;
 set2(obj->x140,0.0f,0.0f,-0.85f);
 set3(obj->x150,2.5f,-2.6f,0.75f);
 set3(obj->x160,-1.4f,-2.8f,0.75f);
 set3(obj->x170,1.5f,0.5f,0.75f);
 set3(obj->x180,-0.75f,0.0f,0.75f);
 set3(obj->x190,2.0f,0.0f,0.0f);
 set3(obj->x1A0,2.0f,2.0f,0.0f);
 set3(obj->x1B0,-2.0f,0.0f,0.0f);
 set3(obj->x1B0,-2.0f,2.0f,0.0f);
}
