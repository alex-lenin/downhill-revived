/* UNIT 0x0012E028 0x0012E070 */
extern float func_00174920(void *);
float func_0012E028(char *obj) {
    float scale = 1.0f;
    if (*(unsigned char *)(obj + 0x7BA8)) scale = 5.0f;
    return func_00174920(obj + 0x5E40) * scale;
}
