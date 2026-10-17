/* UNIT 0x0015BEA0 0x0015BF10 */
extern float *func_00261028(void *);
extern float *func_00261030(void *);
extern float *func_00261038(void *);
void func_0015BEA0(void *obj, float value) {
 float scaled; void *p;
 scaled=value*60.0f;
 p=(char *)obj+0x140;
 *(float *)((char *)obj+0x1B0)=value;
 *func_00261028(p)=scaled;
 *func_00261038(p)=scaled;
 *func_00261030(p)=scaled;
 *(float *)((char *)obj+0x134)=scaled;
}
