/* UNIT 0x00111F98 0x00112158 */
typedef struct Point { float x,y,z; } Point;
typedef struct Rider { char pad[0x792C]; struct Body *x792C; } Rider;
typedef struct Body { char pad[0x18]; float x18; } Body;
typedef struct Obj { Rider *x0; char pad4[0x6C]; Point *x70,*x74; char pad78[0x240]; float x2B8; } Obj;
extern char D_0029EA20[];
extern int func_00119150(void *,Point *,int);
extern int func_00119120(void *,Point *,int);
extern void func_0010B290(Obj *,int *);
static inline float maxf(float a,float b) { float result; __asm__ volatile("max.s %0,%1,%2" : "=&f"(result) : "f"(a),"f"(b)); return result; }
void func_00111F98(Obj *obj,int *out) {
    float height,denom;
    int difference,a,b,limit;
    height=obj->x70->z-obj->x0->x792C->x18;
    height=maxf(0.0f,height);
    difference=func_00119150(D_0029EA20,obj->x70,1);
    limit=-1;
    difference-=func_00119120(D_0029EA20,obj->x70,1);
    difference=difference>limit?difference:0;
    a=func_00119150(D_0029EA20,obj->x70,1)-(int)((float)difference*obj->x2B8);
    difference=func_00119150(D_0029EA20,obj->x74,1);
    difference-=func_00119120(D_0029EA20,obj->x74,1);
    difference=difference>limit?difference:0;
    b=func_00119150(D_0029EA20,obj->x74,1)-(int)((float)difference*obj->x2B8);
    if(a<0) a=0;
    if(b<0) b=0;
    func_0010B290(obj,&a); func_0010B290(obj,&b);
    if(a==b) *out=a;
    else {
        int step=b-a;
        denom=maxf(height,obj->x70->z-obj->x74->z);
        *out=a+(int)((float)step*height/denom);
    }
}
