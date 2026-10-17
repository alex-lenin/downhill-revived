/* UNIT 0x00242998 0x00242C80 */
typedef float Vector[4] __attribute__((aligned(16)));
typedef struct Node { struct Node *x0,*x4,*x8; unsigned short xC,xE; float x10[4]; } Node;
typedef struct Result { Vector x0; float x10,x14,x18,x1C,x20,x24; short x28,x2A; unsigned short x2C,x2E; } Result;
typedef struct Vec { float x[4]; } Vec;
static inline void set_leaf(Result *out,short id) { out->x28=0; out->x2A=id; out->x2E=id; out->x2C=id; }
static inline bool invert(bool b) { return !b; }
static inline bool is_empty(unsigned short value) { return value==512; }
extern Result *D_0029E498;
extern void func_00242D08(float *,float *,float *,float *);
extern unsigned short func_00242C80(Node *,void *);
extern void func_00242998(Node *,float *,float *);
static inline void visit(Node *child,short id,float *a,float *b,Result *out) {
    if(!child) set_leaf(out,id); else func_00242998(child,a,b);
}
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, %1\n\tsq $2, %0" : "=m"(dst) : "m"(*(Vec *)(src)) : "$2")
void func_00242998(Node *node,float *start,float *end) {
    float distance[2]; Vector point; Result *out; Node *child;
    unsigned short id,previous; bool empty; float factor,x,z,y;
    out=D_0029E498;
    func_00242D08(node->x10,start,end,distance);
    if(distance[0]<0.0009765625f && distance[0]>-0.0009765625f) distance[0]=distance[1];
    if(distance[1]<0.0009765625f && distance[1]>-0.0009765625f) distance[1]=distance[0];
    if(distance[0]>=0.0f) { id=node->xC; child=node->x0; } else { id=node->xE; child=node->x4; }
    if(distance[0]*distance[1]>=0.0f) {
        visit(child,(short)id,start,end,out);
    } else {
        factor=distance[0]/(distance[0]-distance[1]);
        x=start[0]; y=start[1]; z=start[2];
        point[0]=x+factor*(end[0]-x); point[1]=y+factor*(end[1]-y); point[2]=z+factor*(end[2]-z);
        visit(child,(short)id,start,point,out);
        if(out->x28==0) {
            if(distance[0]>=0.0f) { id=node->xE; child=node->x4; } else { id=node->xC; child=node->x0; }
            previous=out->x2E;
            visit(child,(short)id,point,end,out);
            empty=is_empty(previous);
            if(out->x2C==512) { if(invert(empty)) goto hit_boundary; } else if(empty) goto hit_boundary;
            return;
hit_boundary:;
            out->x28=1;
            COPY_VECTOR(out->x0,point);
            out->x10=node->x10[0]; out->x14=node->x10[1]; out->x18=node->x10[2]; out->x20=node->x10[3];
            out->x24=distance[1]; out->x2C=previous;
            if(node->x8) out->x2E=func_00242C80(node->x8,point); else out->x2E=node->xE;
            if(distance[0]<0.0f) { id=out->x2E; out->x2E=previous; out->x2C=id; }
        }
    }
}
