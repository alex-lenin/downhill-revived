/* UNIT 0x00222170 0x00222330 */
/* Builds the projection matrix in out (second * first) and the two
 * perspective matrices a and b. */
typedef struct Matrix {float m[16];} __attribute__((aligned(16))) Matrix;
extern void func_00221F20(Matrix*);
extern void func_00221930(Matrix*,Matrix*,Matrix*);
void func_00222170(Matrix *out,Matrix *a,Matrix *b,float scale,float x,float y,float tx,float ty,float z0,float z1,float nearZ,float farZ,float width,float height,float aspect) {
 Matrix second; Matrix first; struct { float x; } ratios;
 float depth=z1-z0;
 float offset=-z1;
 float product=farZ*nearZ;
 float depth2,offset2,denom;
 float scaledDepth;
 float one=1.0f;
 z0*=farZ;
 offset*=nearZ;
 scaledDepth=product*depth;
 offset+=z0;
 denom=farZ-nearZ;
 scaledDepth/=denom;
 offset/=denom;
 denom=offset;
 func_00221F20(&first);
 first.m[0]=scale;
 first.m[5]=scale;
 first.m[14]=one;
 first.m[11]=one;
 first.m[10]=0.0f;
 first.m[15]=0.0f;
 func_00221F20(&second);
 second.m[0]=x;
 second.m[5]=y;
 second.m[10]=scaledDepth;
 second.m[3]=tx;
 second.m[7]=ty;
 second.m[11]=denom;
 func_00221930(out,&second,(Matrix*)&first);
 func_00221F20(a);
 a->m[14]=one;
 a->m[15]=0.0f;
 aspect=scale/aspect;
 depth2=(farZ+nearZ)/(farZ-nearZ);
 offset2=(product*-2.0f)/(farZ-nearZ);
 a->m[0]=aspect;
 a->m[10]=depth2;
 a->m[11]=offset2;
 a->m[5]=aspect;
 func_00221F20(b);
 {float rw=scale/width;
 ratios.x=scale/height;
 b->m[10]=depth2;
 b->m[11]=offset2;
 b->m[14]=one;
 b->m[15]=0.0f;
 b->m[5]=ratios.x;
 b->m[0]=rw;}
}
