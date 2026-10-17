/* UNIT 0x00195B08 0x00195B68 */
typedef struct Samples { char pad[4]; short x4[150]; short x130[150]; short x25C[150]; unsigned char x388[150]; } Samples;
void func_00195B08(Samples *obj, int index, float *out, unsigned char *flag) {
    out[0] = obj->x4[index];
    out[1] = obj->x130[index];
    out[2] = obj->x25C[index];
    *flag = obj->x388[index];
}
