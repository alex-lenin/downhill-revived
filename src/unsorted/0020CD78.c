/* UNIT 0x0020CD78 0x0020CDF0 */
typedef struct {
    char pad[0x290];
    float x290,x294,x298,x29c;
    char pad2[0x14];
    short x2b4;
    short x2b6;
    int x2b8,x2bc;
} Obj;
extern Obj D_004D9720[];
void func_0020CD78(int i,float a,float b) {
    Obj *p;
    float f;
    a*=60.0f;
    b*=60.0f;
    p=&D_004D9720[i];
    p->x2b4=(int)a;
    f=p->x29c;
    p->x290=f;
    p->x2b8=p->x2b4>0?4:5;
    if(b>0.5f) f/=b;
    p->x298=f;
}
