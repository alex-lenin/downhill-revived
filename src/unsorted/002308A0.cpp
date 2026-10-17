/* UNIT 0x002308A0 0x00230928 */
struct Obj {
    char pad[0x8088];
    int x8088;
    char pad808C[8];
    float x8094;
    virtual int serialize(char *out);
    void func_002308A0(char *out);
};
void Obj::func_002308A0(char *out) {
    *(float *)(out+0) = 1.2630964e9f;
    *(float *)(out+4) = 2.0f;
    *(float *)(out+8) = (float)x8088;
    x8094 = 2.0f;
    out += serialize(out+12) + 12;
    *(int *)(out+0) = 1;
    *(int *)(out+4) = 8;
}
