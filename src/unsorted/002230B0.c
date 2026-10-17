/* UNIT 0x002230B0 0x00223168 */
void func_002230B0(float *out, float *in) {
 float x=in[0], y=in[1], z=in[2], w=in[3];
 float length=x*x+y*y+z*z+w*w;
 float root, scale;
 if (length >= 1.000001f || length <= 0.99999905f) {
  __asm__ volatile ("sqrt.s %0,%1" : "=f"(root) : "f"(length));
  scale=1.0f/root;
  out[0]=x*scale; out[1]=in[1]*scale; out[2]=in[2]*scale; out[3]=in[3]*scale;
 }
}
