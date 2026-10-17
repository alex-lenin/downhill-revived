/* UNIT 0x00222460 0x002224E0 */
extern void func_002225A8(void *, void *);
extern void func_00221F50(void *, void *);
extern void func_00221888(void *, void *, void *);
extern float func_00248D90(float, float);
extern void func_002087C0(float *);
void func_00222460(char *obj, float *out) {
 float matrix[16] __attribute__((aligned(16)));
 float vec[4] __attribute__((aligned(16)));
 func_002225A8(out, obj+0x10);
 func_00221F50(matrix,out);
 func_00221888(matrix,obj,vec);
 out[1]=func_00248D90(vec[0],vec[2])-1.5707964f;
 func_002087C0(out+1);
}
